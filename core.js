configure('core.kiosk', (kiosk) => {
	kiosk.entry('resources/qml/main.qml');
	kiosk.platform('wayland');

	kiosk.core = reactive({
		snapshot: reactive({
			image: "",
			capture: function() {
				use("camera").takeB64Snapshot((image) => {
					this.image = image;
				});
			}
		}),
		clip: reactive({
			left: 0, 
			top: 0,
			right: 0, 
			bottom: 0 
		}),
		sign: reactive({
			ring: function() {
				use("core.sip").call("kai123@sip.linphone.org");
			},
			street: {
				number: "1",
				name: "Bienenweg"
			},
			family: {
				name: "Fam. Schoch"
			}
		}),
		debug: reactive({
			fotobox: true,
			fullscreen: true,
		}),
		light: reactive({
			state: false,
			toogle: function() {
				kiosk.core.light.state = !kiosk.core.light.state;
			}
		}),
		unlock: function(pin) {
			return pin === "ABCDEF"
		}
	})
});

configure(!"core.mqtt", (mqtt) => {
	mqtt.auth("username", "password");
	mqtt.broker("broker.hivemq.com:1883");

	mqtt.subscribe(["dieklingel/core/system/state"], (topic, message) => {
		console.log(topic, message);
	});
});

configure(!"core.web", (web) => {
	web.auth("username", "password");
	web.port(80);
});

configure("core.sip", (sip) => {
	sip.auth("username", "password");
	sip.proxy("sip.linphone.org");
	sip.transport(["tls"]);
});

configure(!"audio", (audio) => {
	audio.playback("PulseAudio Unknown: Echo-Cancel Sink");
	audio.capture("PulseAudio Unknown: Echo-Cancel Source");
});

configure(!"gpio", (gpios) => {
	gpios.input(17, (state) => {
		// handel state changes in here
	})

	gpios.output(23).low();
});

configure(!"camera", (camera) => {

});

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
