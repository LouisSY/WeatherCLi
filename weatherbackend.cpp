#include "weatherbackend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

WeatherBackend::WeatherBackend(QObject *parent)
    : QObject(parent) {
    m_apiKey = loadApiKey();
    if (m_apiKey.isEmpty()) {
        setErrorMessage(QStringLiteral("Failed to load API key from config.json"));
    }
}

void WeatherBackend::fetchWeather(const QString &city) {
    const QString trimmedCity = city.trimmed();
    if (trimmedCity.isEmpty()) {
        clearWeather();
        setErrorMessage(QStringLiteral("Please enter a city name."));
        return;
    }

    setLoading(true);
    setErrorMessage(QString());

    // First, get coordinates from geocoding API (no auth needed)
    QString geoUrlString = QStringLiteral("https://geocoding-api.open-meteo.com/v1/search?name=%1&count=1&language=en&format=json")
        .arg(trimmedCity);
    QUrl geoUrl(geoUrlString);

    QNetworkRequest request(geoUrl);
    request.setRawHeader("User-Agent", "WeatherCLi/1.0");
    QNetworkReply *reply = m_networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto cleanUp = [this, reply]() {
            reply->deleteLater();
        };

        if (reply->error() != QNetworkReply::NoError) {
            clearWeather();
            setErrorMessage(QStringLiteral("Network error: %1").arg(reply->errorString()));
            setLoading(false);
            cleanUp();
            return;
        }

        const QByteArray payload = reply->readAll();
        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);

        if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
            clearWeather();
            setErrorMessage(QStringLiteral("Invalid response from weather service."));
            setLoading(false);
            cleanUp();
            return;
        }

        const QJsonObject rootObject = document.object();
        const QJsonArray resultsArray = rootObject.value(QStringLiteral("results")).toArray();

        if (resultsArray.isEmpty()) {
            clearWeather();
            setErrorMessage(QStringLiteral("City not found."));
            setLoading(false);
            cleanUp();
            return;
        }

        const QJsonObject locationObject = resultsArray.first().toObject();
        const double latitude = locationObject.value(QStringLiteral("latitude")).toDouble();
        const double longitude = locationObject.value(QStringLiteral("longitude")).toDouble();

        // Now fetch weather data using coordinates
        QString weatherUrlString = QStringLiteral("https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&current=temperature_2m,relative_humidity_2m,weather_code,wind_speed_10m&timezone=auto")
            .arg(latitude).arg(longitude);
        QUrl weatherUrl(weatherUrlString);

        QNetworkRequest weatherRequest(weatherUrl);
        weatherRequest.setRawHeader("User-Agent", "WeatherCLi/1.0");
        QNetworkReply *weatherReply = m_networkManager.get(weatherRequest);

        connect(weatherReply, &QNetworkReply::finished, this, [this, weatherReply]() {
            const auto cleanUp2 = [this, weatherReply]() {
                setLoading(false);
                weatherReply->deleteLater();
            };

            if (weatherReply->error() != QNetworkReply::NoError) {
                clearWeather();
                setErrorMessage(QStringLiteral("Network error: %1").arg(weatherReply->errorString()));
                cleanUp2();
                return;
            }

            const QByteArray payload2 = weatherReply->readAll();
            QJsonParseError parseError2;
            const QJsonDocument document2 = QJsonDocument::fromJson(payload2, &parseError2);

            if (parseError2.error != QJsonParseError::NoError || !document2.isObject()) {
                clearWeather();
                setErrorMessage(QStringLiteral("Invalid response from weather service."));
                cleanUp2();
                return;
            }

            const QJsonObject rootObject2 = document2.object();
            const QJsonObject currentObject = rootObject2.value(QStringLiteral("current")).toObject();

            m_temperature = QString::number(currentObject.value(QStringLiteral("temperature_2m")).toDouble(), 'f', 1) +
                            QStringLiteral(" C");
            m_humidity = QString::number(currentObject.value(QStringLiteral("relative_humidity_2m")).toInt()) +
                         QStringLiteral(" %");
            m_windSpeed = QString::number(currentObject.value(QStringLiteral("wind_speed_10m")).toDouble(), 'f', 1) +
                          QStringLiteral(" m/s");

            const int weatherCode = currentObject.value(QStringLiteral("weather_code")).toInt();
            m_description = getWeatherDescription(weatherCode);

            emit weatherChanged();

            cleanUp2();
        });

        cleanUp();
    });
}

QString WeatherBackend::temperature() const {
    return m_temperature;
}

QString WeatherBackend::description() const {
    return m_description;
}

QString WeatherBackend::humidity() const {
    return m_humidity;
}

QString WeatherBackend::windSpeed() const {
    return m_windSpeed;
}

QString WeatherBackend::weatherInfo() const {
    if (m_temperature.isEmpty()) {
        return QString();
    }

    return QStringLiteral("Temperature: %1\nDescription: %2\nHumidity: %3\nWind Speed: %4")
            .arg(m_temperature, m_description, m_humidity, m_windSpeed);
}

QString WeatherBackend::errorMessage() const {
    return m_errorMessage;
}

bool WeatherBackend::loading() const {
    return m_loading;
}

void WeatherBackend::setErrorMessage(const QString &message) {
    if (m_errorMessage == message) {
        return;
    }

    m_errorMessage = message;
    emit errorMessageChanged();
}

void WeatherBackend::setLoading(bool loading) {
    if (m_loading == loading) {
        return;
    }

    m_loading = loading;
    emit loadingChanged();
}

void WeatherBackend::clearWeather() {
    const bool hasData = !m_temperature.isEmpty() || !m_description.isEmpty() || !m_humidity.isEmpty() || !m_windSpeed.
                         isEmpty();
    if (!hasData) {
        return;
    }

    m_temperature.clear();
    m_description.clear();
    m_humidity.clear();
    m_windSpeed.clear();
    emit weatherChanged();
}

QString WeatherBackend::loadApiKey() {
    // Try multiple possible paths for config.json
    QStringList possiblePaths;

    // 1. Same directory as executable
    possiblePaths << QCoreApplication::applicationDirPath() + QStringLiteral("/config.json");

    // 2. Project root (for development)
    possiblePaths << QCoreApplication::applicationDirPath() + QStringLiteral("/../../../config.json");
    possiblePaths << QStringLiteral("./config.json");
    possiblePaths << QStringLiteral("../config.json");
    possiblePaths << QStringLiteral("../../config.json");
    possiblePaths << QStringLiteral("../../../config.json");

    // Try each path
    for (const QString &path : possiblePaths) {
        QFile configFile(path);
        if (configFile.open(QIODevice::ReadOnly)) {
            const QByteArray data = configFile.readAll();
            configFile.close();

            QJsonParseError parseError;
            const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
                const QJsonObject obj = doc.object();
                const QString apiKey = obj.value(QStringLiteral("openweathermap_api_key")).toString();
                if (!apiKey.isEmpty()) {
                    qWarning() << "Loaded API key from:" << path;
                    return apiKey;
                }
            }
        }
    }

    qWarning() << "Could not find config.json in any of these paths:";
    for (const QString &path : possiblePaths) {
        qWarning() << "  -" << path;
    }
    return QString();
}

QString WeatherBackend::getWeatherDescription(int weatherCode) {
    // WMO Weather interpretation codes
    if (weatherCode == 0) return QStringLiteral("Clear sky");
    if (weatherCode == 1 || weatherCode == 2) return QStringLiteral("Mostly clear");
    if (weatherCode == 3) return QStringLiteral("Overcast");
    if (weatherCode == 45 || weatherCode == 48) return QStringLiteral("Foggy");
    if (weatherCode == 51 || weatherCode == 53 || weatherCode == 55) return QStringLiteral("Drizzle");
    if (weatherCode == 61 || weatherCode == 63 || weatherCode == 65) return QStringLiteral("Rain");
    if (weatherCode == 71 || weatherCode == 73 || weatherCode == 75) return QStringLiteral("Snow");
    if (weatherCode == 77) return QStringLiteral("Snow grains");
    if (weatherCode == 80 || weatherCode == 81 || weatherCode == 82) return QStringLiteral("Rain showers");
    if (weatherCode == 85 || weatherCode == 86) return QStringLiteral("Snow showers");
    if (weatherCode == 95 || weatherCode == 96 || weatherCode == 99) return QStringLiteral("Thunderstorm");
    return QStringLiteral("Unknown");
}


