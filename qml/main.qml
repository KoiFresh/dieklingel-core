import QtQuick.Controls 2.12
import QtQuick.Window 2.12

import "signs" as Signs

Window {
    visible: true
    title: "dieKlingel Kiosk"
    color: "#1c1f1e"
    minimumWidth: 480
    minimumHeight: 820

    SwipeView {
        anchors {
            fill: parent
        }

        Signs.Sign {}
    }
}
