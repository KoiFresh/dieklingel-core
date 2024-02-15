import QtQuick 2.12
import QtQuick.Controls 2.12
import com.dieklingel 1.0

Item {
	Component {
		id: tumblerDelegate

		Label {
			text: Tumbler.tumbler.moving ? modelData : "•"
			opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			font.pointSize: 24
			color: "white"
		}
	}

	function randomize() {
		for(let i = 0; i < passcode.children.length; i++) {
			const tumbler = passcode.children[i];
			tumbler.currentIndex = Math.floor(Math.random() * tumbler.model.length);
		}
	}

	function getPasscode() {
		let pin = "";
		for(let i = 0; i < passcode.children.length; i++) {
			const tumbler = passcode.children[i];
			const digit = tumbler.model[tumbler.currentIndex];
			pin += digit;
		}
		return pin;
	}

	Component.onCompleted: {
		randomize();
	}

	Item {
		anchors {
			fill: parent
		}
		
		Row {
			id: passcode
			anchors {
				centerIn: parent
			}

			Tumbler {
				model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"]
				height: parent.parent.height * 2/5
				width: parent.parent.width / 6
				delegate: tumblerDelegate
			}
			Tumbler {
				model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"]
				height: parent.parent.height * 2/5
				width: parent.parent.width / 6
				delegate: tumblerDelegate
			}
			Tumbler {
				model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"]
				height: parent.parent.height * 2/5
				width: parent.parent.width / 6
				delegate: tumblerDelegate
			}
			Tumbler {
				model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"]
				height: parent.parent.height * 2/5
				width: parent.parent.width / 6
				delegate: tumblerDelegate
			}
			Tumbler {
				model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"]
				height: parent.parent.height * 2/5
				width: parent.parent.width / 6
				delegate: tumblerDelegate
			}
			Tumbler {
				model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"]
				height: parent.parent.height * 2/5
				width: parent.parent.width / 6
				delegate: tumblerDelegate
			}
		}

		Row {
			height: 120
			spacing: 100
			anchors {
				bottom: parent.bottom
				horizontalCenter: parent.horizontalCenter
			}

			LightButton {
				anchors {
					top: parent.top
				}
			}

			Button {
				width: 100
				height: 100

				background: Rectangle {
					color:  parent.pressed ? "#1EFFFFFF" : "transparent"
					border.width: 1
					border.color: "white"
					radius: 15
				}

				onReleased: {
					let pin = getPasscode();
					randomize();
					if(pin === App.env("QML_PASSCODE")) {
						console.log("TODO: unlock the door");
					}
				}

				Image {
					source: "key.png"
					width: 60
					height: 60
					anchors {
						centerIn: parent
					}
					fillMode: Image.PreserveAspectFit
				}
			}
		}
	}
}
