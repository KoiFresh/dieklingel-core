import QtQuick 2.12
import QtMultimedia 5.15
import com.dieklingel 1.0

Item {    
	SoundEffect {
        id: bell
        source: "bell.wav"
    }

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
                bell.play();
                App.ring("sip:koifresh@sip.linphone.org")
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
                    id: img
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
                        verticalCenter: parent.verticalCenter
                    }
                    x: ((img.width * 0.75) + parent.width - width) / 2
                    horizontalAlignment: Text.AlignHCenter
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