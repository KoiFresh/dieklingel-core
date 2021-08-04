class dieklingel {
    CreateEndpoint(domain) {
        this.domain = domain;
        this.websocket = new WebSocket(domain);
        this.websocket.onopen = () => {
            console.log("Socket connected");
        }
        this.websocket.onmessage = (message) => {
            try {
                var json = JSON.parse(message.data);
                $(this).trigger(json.body.context, json);
            } catch(e) {
                console.log("received message has the wrong format! or ", e.message);
            }
        }
        this.websocket.onerror = () => {
            this.websocket.close();
        }
        this.websocket.onclose = () => {
            this.CreateEndpoint(this.domain);
        }
    }
    DeleteEndpoint() {

    }   
    Send(message) {
        if(this.websocket !== null && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(message);
        } 
    }
}

// Last Context Update - 20201-03-11

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
    Register: '',
    Unknown: 'unknown'
}