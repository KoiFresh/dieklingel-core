import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import com.dieklingel 1.0

import "signs" as Signs

Window {
    visible: true
    title: "dieKlingel Kiosk"
    color: "#1c1f1e"
    minimumWidth: 480
    minimumHeight: 820
    //visibility: Window.FullScreen

    Connections {
        target: App
        function onInactivity() {
            carousel.currentIndex = 0;
        }
    }

    SwipeView {
        id: carousel
        anchors {
            fill: parent
        }

        Signs.Sign {}
    }
}
