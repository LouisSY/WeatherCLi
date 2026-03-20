import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 480
    height: 360
    visible: true
    title: "WeatherCLI"

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#2c3e50" }
            GradientStop { position: 1.0; color: "#4ca1af" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 14

        Label {
            text: "Current Weather"
            font.pixelSize: 28
            font.bold: true
            color: "white"
            Layout.alignment: Qt.AlignHCenter
        }

        TextField {
            id: cityInput
            placeholderText: "Enter city name"
            font.pixelSize: 16
            Layout.fillWidth: true
            enabled: !weatherBackend.loading
            onAccepted: fetchButton.clicked()
        }

        Button {
            id: fetchButton
            text: weatherBackend.loading ? "Loading..." : "Get Weather"
            font.pixelSize: 15
            Layout.fillWidth: true
            enabled: !weatherBackend.loading
            onClicked: weatherBackend.fetchWeather(cityInput.text)
        }

        BusyIndicator {
            running: weatherBackend.loading
            visible: weatherBackend.loading
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 10
            color: "#ffffff"
            opacity: weatherBackend.weatherInfo.length > 0 ? 1.0 : 0.9

            Behavior on opacity {
                NumberAnimation { duration: 250 }
            }

            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 8

                Label {
                    text: weatherBackend.weatherInfo.length > 0 ? weatherBackend.weatherInfo : "Enter a city and click Get Weather"
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    color: "#222222"
                }

                Label {
                    text: weatherBackend.errorMessage
                    visible: weatherBackend.errorMessage.length > 0
                    color: "#c0392b"
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}

