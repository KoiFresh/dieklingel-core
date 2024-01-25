import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import com.dieklingel 1.0

Window {
    visible: true
    title: "dieKlingel Kiosk"
    color: "#1c1f1e"
    minimumWidth: 480
    minimumHeight: 820

    Column {
        anchors.centerIn: parent
        
        Text {
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            text: "1"
            color: "wheat"
            font {
                pointSize: 200
                family: "Helvetica"
            }
        }

        Text {
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            text: "Regenbogen Str."
            color: "wheat"
            font {
                pointSize: 35
                family: "Helvetica"
            }
        }

        Item {
            width: 1
            height: 100
        }

        MouseArea {
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            width: 460
            height: 120
            onPressed: {
                App.ring("koifresh")
            }

            Rectangle {
                anchors {
                    fill: parent
                }
                color: parent.pressed ? "grey" : "transparent"
                border {
                    width: 5
                    color: "green"
                }
                radius: height / 2

                Image {
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    source: "bell.png"
                    width: 120
                    fillMode: Image.PreserveAspectFit
                } 

                Text {
                    anchors  {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    padding: 0,0,0,20
                    text: "Fam. Mustermann"
                    color: "white"
                    font {
                        pointSize: 28
                        family: "Helvetica"
                    }
                }
            }
        }
    }
}
