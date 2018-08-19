
let macAddress = "20:16:11:29:61:79";  // BT

let app = {
    initialize: function() {
        this.bindEvents();
        console.log("dentro de initialize");
        // this.iniciaBotones();
    },
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    onDeviceReady: function() {
        bluetoothSerial.connect(macAddress, app.onConnect, app.onDisconnect);
    },
    onConnect: function() {
        bluetoothSerial.subscribe("\n", app.onMessage, app.subscribeFailed);
        statusDiv.innerHTML="Connected to " + macAddress + ".";        
    }, 
    onDisconnect: function() {
        alert("Disconnected");
        statusDiv.innerHTML="Disconnected.";
    },
    onMessage: function(data) {
        counter.innerHTML = data;        
    },
    subscribeFailed: function() {
        alert("subscribe failed");
    }, 
    // From here JAD FUNCTIONS SPECIFIC FOT THIS PROJECT
    setListeners: () => {
        
    }
}; // END of APP

document.addEventListener('DOMContentLoaded', function (){
    FastClick.attach(document.body); // smoother clicks on mobile dev
    app.initialize();
}, false);