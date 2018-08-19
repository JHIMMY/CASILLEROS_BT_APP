
if ('addEventListener' in document){
    document.addEventListener('DOMContentLoaded', function (){
        FastClick.attach(document.body);
        app.initialize();
    }, false);
}

let macAddress = "20:16:11:29:61:79";

let app = {
    initialize: function() {
        this.bindEvents();
        status.innerHTML = "INICIANDO";
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

    // iniciaBotones: () => {
    //     let connectButton = document.getElementById("btn1");

    //     connectButton.addEventListener('click', this.contectarModuloBT, false);
    // },

    // contectarModuloBT: () => {
    //     console.log("Intrentando Conectar ....");
    //     bluetoothSerial.connect(macAddress_or_uuid, connectSuccess, connectFailure);
    // }
};