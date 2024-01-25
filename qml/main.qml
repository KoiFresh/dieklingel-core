import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import com.dieklingel 1.0

Window {
    visible: true
    title: "dieKlingel Kiosk"
    color: "#1c1f1e"

    Column {
        spacing: 20
        anchors.centerIn: parent
        
        Image {
            source: "bell.png"
            width: parent.width
            fillMode: Image.PreserveAspectFit
        }
        Button {
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            font.pointSize: 24
            padding: 10
            text: "Fam. Mustermann"
            onClicked: App.ring("koifresh@sip.linphone.org")
        }
    }
}
