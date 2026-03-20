#ifndef WEATHERBACKEND_H
#define WEATHERBACKEND_H

#include <QObject>
#include <QNetworkAccessManager>

class WeatherBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY weatherChanged)
    Q_PROPERTY(QString description READ description NOTIFY weatherChanged)
    Q_PROPERTY(QString humidity READ humidity NOTIFY weatherChanged)
    Q_PROPERTY(QString windSpeed READ windSpeed NOTIFY weatherChanged)
    Q_PROPERTY(QString weatherInfo READ weatherInfo NOTIFY weatherChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit WeatherBackend(QObject *parent = nullptr);

    Q_INVOKABLE void fetchWeather(const QString &city);

    QString temperature() const;

    QString description() const;

    QString humidity() const;

    QString windSpeed() const;

    QString weatherInfo() const;

    QString errorMessage() const;

    bool loading() const;

signals:
    void weatherChanged();

    void errorMessageChanged();

    void loadingChanged();

private:
    void setErrorMessage(const QString &message);

    void setLoading(bool loading);

    void clearWeather();

    QString loadApiKey();

    QString getWeatherDescription(int weatherCode);

    QNetworkAccessManager m_networkManager;
    QString m_apiKey;
    QString m_temperature;
    QString m_description;
    QString m_humidity;
    QString m_windSpeed;
    QString m_errorMessage;
    bool m_loading = false;
};

#endif // WEATHERBACKEND_H
