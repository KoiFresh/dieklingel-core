import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import com.dieklingel 1.0

import "signs" as Signs
import "views" as Views

Window {
    visible: true
    title: "dieKlingel Kiosk"
    color: "black"
    minimumWidth: 480
    minimumHeight: 820
    visibility: App.env("qml.debug.fullscreen") === "false" ? Window.AutomaticVisibility : Window.FullScreen;

    Rectangle {
        clip: true
        anchors { 
            fill: parent
            leftMargin: App.env("qml.global.clip.left")
            topMargin: App.env("qml.global.clip.top")
            rightMargin: App.env("qml.global.clip.right")
            bottomMargin: App.env("qml.global.clip.bottom")
        }
        color: "#1c1f1e"

        Component {
            id: debug

            SwipeView {
                id: carousel
                anchors {
                    fill: parent
                }

                Signs.Sign {}
                Views.Passcode {}
                Views.Fotobox {}

                Connections {
                    target: App
                    function onInactivity() {
                        carousel.currentIndex = 0;
                    }
                }
            }
        }

        Component {
            id: release

            SwipeView {
                id: carousel
                anchors {
                    fill: parent
                }

                Signs.Sign {}
                Views.Passcode {}

                Connections {
                    target: App
                    function onInactivity() {
                        carousel.currentIndex = 0;
                    }
                }
            }
        }

        Loader {
            anchors {
                fill: parent
            }
            sourceComponent: App.env("qml.debug.fotobox") === "true" ? debug : release
        }
    }
}
