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
			width: 100
			height: 100
			
			background: Rectangle {
				color:  parent.pressed ? "#1EFFFFFF" : "transparent"
				border.width: 1
				border.color: "grey"
				radius: 15
			}

			Image {
				source: "light.png"
				width: 50
				height: 50
				anchors {
					centerIn: parent
				}
				fillMode: Image.PreserveAspectFit
			}

			onPressed: {
				App.publish("dieklingel/core/io/light", "on")
			}
		}
	}
}
