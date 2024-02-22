import QtQml 2.15
import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
	id: root
	signal pressed()
	signal released()
	property url icon: ""
	property color color: "white"
	property alias active: btn.pressed

	width: 100
	height: 100

	Button {
		id: btn
		width: root.width
		height: root.height
		background: Rectangle {
			color:  parent.pressed ? "#1EFFFFFF" : "transparent"
			border.width: 1
			border.color: root.color
			radius: 15
		}

		Image {
			source: root.icon
			width: 50
			height: 50
			anchors {
				centerIn: parent
			}
			fillMode: Image.PreserveAspectFit
		}

		onPressed: {
			root.pressed();
		}

		onReleased: {
			root.released();
		}
	}
}
