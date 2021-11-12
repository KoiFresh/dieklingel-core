class core {
    constructor(domain) {
        this.domain = domain;
        this.websocket = new WebSocket(domain);
        this.websocket.onopen = () => {
            console.log("Socket connected");
        };
        this.websocket.onmessage = (message) => {
            var json = JSON.parse(message.data);
            //this.dispatchEvent(new Event("unlock"));
    
            $(this).trigger("unlock");
            console.log(this.domain);
            console.log(json.body.context + " should triggerd");
            /*try {
                
                console.log("message", json["body"]["context"]);
            } catch(e) {
                console.log("received message has the wrong format! or ", e.message);
            } */
        }
        this.websocket.onerror = () => {
            this.websocket.close();
        }
        this.websocket.onclose = () => {
            this.CreateEndpoint(this.domain);
        }
    }

    send(message) {
        if(this.websocket !== null && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(message);
        } 
    }
}
    
var Context = {
    Response: 'response',
    DeviceUpdate: 'device-update',
    ConnectionState: 'connection-state',
    Movement: 'movement',
    DisplayState: 'display-state',
    Log: 'log',
    Unlock: 'unlock',
    EnterPasscode: 'enter-passcode',
    Call: 'call',
    Register: 'register',
    Script: 'script',
    Unknown: 'unknown'
}

export default core;