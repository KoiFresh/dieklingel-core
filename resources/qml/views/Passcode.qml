import QtQuick 2.12
import QtQuick.Controls 2.12
import com.dieklingel 1.0
import "../components"

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
			let index = tumbler.currentIndex;
			for(;;) {
				let next = Math.floor(Math.random() * tumbler.model.length);
				if(next !== index) {
					tumbler.currentIndex = next;
					break;
				}
			}
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

			CButton {
				id: light
				property bool isOn: Boolean(App.core && App.core.light && App.core.light.state)

				color: isOn ? "yellow" : "white"
				icon: isOn ? "lightbulb" : "light_off"
				
				onPressed: {
					if(App.core && App.core.light && App.core.light.toogle) {
						App.core.light.toogle();
					}
				}
			}

			CButton {
				color: "white"
				icon: "key"

				onReleased: {
					let pin = getPasscode();
					randomize();

					if(App.core && App.core.unlock && App.core.unlock(pin)) {
						console.log("TODO: handle unlock");
					}
				}
			}
		}
	}
}
