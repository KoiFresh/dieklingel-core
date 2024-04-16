type CAMERA = {
	device: (device: string) => void;
	takeB64Snapshot: (callback: (image: string) => void) => void
}

type AUDIO = {
	playback: (device: string) => void;
	capture: (device: string) => void;
	play: (file: URL) => void;
}

type MQTT = {
	/**
	 * The username and the password used to connect to the mqtt broker.
	 * 
	 * @param username the username to use
	 * @param password the password to use
	 * 
	 * @example
	 * 	mqtt.auth("username", "password")
	 */
	auth: (username: string, password: string) => void;

	/**
	 * The mqtt broker to connect to, and an optional port
	 * 
	 * @param uri the uri to connect to <domain>[:port]
	 * 
	 * @example
	 * 	mqtt.broker("broker.hivemq.com:1883")
	 */
	broker: (uri: string) => void;
}

type KIOSK = {
	/**
	 * The entry qml file for the Kiosk to load as soon as the configuration is completed.
	 * 
	 * @param file the .qml file to use
	 * 
	 * @example
	 * 	kiosk.entry("qml/main.qml")
	 */
	entry: (file: string) => void;

	/**
	 * The environment variable wich should be used for the Qt platform abstraction.
	 * 
	 * @param platform the value to use as env var for QT_QPA_PLATFORM
	 * 
	 * @see https://wiki.qt.io/Qt_Platform_Abstraction
	 * @example
	 * 	kiosk.platform("wayland")
	 */
	platform: (platform: string) => void;
}

type WEB = {}

type SIP = {
	auth: (username: string, password: string) => void;
	proxy: (proxy: string) => void;
	transport: (transports: ("udp" | "tcp" | "tls" | "dtls")[]) => void;
}

type GPIO = {
	input: (pin: number, callback: ((state: boolean) => void)) => void;
	output: () => {
		high: () => void;
		low: () => void
	};
}

type OPTIONS = {
	"camera": CAMERA;
	"audio": AUDIO;
	"core.mqtt": MQTT;
	"core.kioks": KIOSK;
	"core.web": WEB;
	"gpio": GPIO;
}

declare function configure<KEY extends keyof OPTIONS>(v: KEY, c: (e: OPTIONS[KEY]) => void): void;

declare function use<KEY extends keyof OPTIONS>(v: KEY): OPTIONS[KEY];
