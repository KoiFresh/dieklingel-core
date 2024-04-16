import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import com.dieklingel 1.0
import "../components"

Item {
	Image {
		id: img
		anchors {
			centerIn: parent
		}
		source: (App.fotobox && App.fotobox.image) || ""
		width: Math.min(640, parent.width)
		height: 480
		fillMode: Image.PreserveAspectFit
	}

	CButton {
		anchors {
			bottom: parent.bottom
			bottomMargin: 20
			horizontalCenter: parent.horizontalCenter
		}
		icon: "camera"

		onReleased: {
			if(App.snapshot) {
				App.snapshot();
			}
		}
	}

	/*Connections {
		target: App

		function onSnapshotTaken(snapshot) {
			img.source = snapshot;
		}
	}*/
}