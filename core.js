configure('core.kiosk', (kiosk) => {
	kiosk.entry('qml/main.qml');
	kiosk.platform('wayland');

	kiosk.qml = {
		global: {
			clip: {
				left: 0, 
				top: 0,
				right: 0, 
				bottom: 0 
			}
		},
		sign: {
			street: {
				number: "1",
				name: "Bienenweg"
			},
			family: {
				name: "Fam. Schoch"
			}
		},
		debug: {
			fotobox: false,
			fullscreen: false,
		}
	}

	kiosk.light = reactive({
		isOn: false,
		toogle: () => {
			kiosk.light.isOn = !kiosk.light.isOn
		}
	})

	kiosk.unlock = (pin) => {
		return pin === "ABCDEF"
	}
});

configure(!"core.mqtt", (mqtt) => {
	mqtt.auth("username", "password");
	mqtt.broker("broker.hivemq.com:1883");
});

configure(!"core.web", (web) => {
	web.auth("username", "password");
	web.port(80);
});

configure(!"core.sip", (sip) => {
	sip.auth("username", "password");
	sip.proxy("sip.linphone.org");
	sip.transport(["tls"]);
});

configure(!"audio", (audio) => {
	audio.playback("PulseAudio Unknown: Echo-Cancel Sink");
	audio.capture("PulseAudio Unknown: Echo-Cancel Source");
});

/* TODO
configure("gpios", (gpios) => {
	gpios.input(17, (state) => {
	})
});
*/

/*
 * Some experimental utility functions, not yet supported by the Setup yet.
 */
function reactive(obj) {
	const props = Object.keys(obj).map((key) => `property var ${key}`).join('\n');
	const object = Qt.createQmlObject(
		`
	   import QtQuick 2.0
	   
	   QtObject {
		   ${props}
	   }`,
		Qt.application, 'reactive');
	for (const [key, value] of Object.entries(obj)) {
		object[key] = value;
	}
	return object;
}
