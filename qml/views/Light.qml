import QtQuick 2.12
import QtQuick.Controls 2.12
import com.dieklingel 1.0

Item {
	Row {
		anchors {
			centerIn: parent
		}
		spacing: 10

		Button {
			id: light
			property bool isOn: false
			width: 100
			height: 100
			background: Rectangle {
				color:  parent.pressed ? "#1EFFFFFF" : "transparent"
				border.width: 1
				border.color: light.isOn ? "yellow" : "grey"
				radius: 15
			}

			Image {
				source: light.isOn ? "light.on.png" : "light.off.png"
				width: 50
				height: 50
				anchors {
					centerIn: parent
				}
				fillMode: Image.PreserveAspectFit

				Connections {
        			target: App
        
					function onMessageReceived(topic, message) {
						if(topic === "home/light/main/door") {
							light.isOn = (message === "on") ? true : false;
						}
					}
				}
			}

			onPressed: {
				App.publish("home/light/main/door", light.isOn ? "off" : "on")
			}
		}
	}
}
