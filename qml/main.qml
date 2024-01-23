import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import com.dieklingel 1.0

Window {
    visible: true
    title: "dieKlingel Kiosk"

    Button {
        text: "ring"
        onClicked: App.ring()
    }
}
