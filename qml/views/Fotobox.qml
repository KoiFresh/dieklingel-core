import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import com.dieklingel 1.0

Item {
	Column {
		Button {
			anchors {
				horizontalCenter: parent.horizontalCenter
			}
			text: "snapshot"

			onReleased: {
				App.snapshot();
			}
		}

		Image {
			id: img
		}

		Connections {
			target: App

			function onSnapshotTaken(snapshot) {
				img.source = snapshot;
			}
		}
	}
}