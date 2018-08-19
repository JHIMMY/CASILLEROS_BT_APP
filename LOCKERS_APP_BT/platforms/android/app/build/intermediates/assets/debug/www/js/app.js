
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
        // bluetoothSerial.connect(macAddress, app.onConnect, app.onDisconnect);
        app.setListeners();
    },
    onConnect: function() {
        activateAllSystems();
        bluetoothSerial.subscribe("\n", app.onMessage, app.subscribeFailed);
        // statusDiv.innerHTML="Connected to " + macAddress + ".";        
    }, 
    onDisconnect: function() {
        alert("Bluetooth Desconectado");
        // statusDiv.innerHTML="Disconnected.";
    },
    onMessage: function(data) {
        // counter.innerHTML = data;        
    },
    subscribeFailed: function() {
        alert("Fallo la conexión al Bluetooth Arduino");
    }, 
    // From here JAD FUNCTIONS SPECIFIC FOT THIS PROJECT
    setListeners: function() {
        let activarBtn = document.querySelector("#activarBtn");
        let desactivarBtn = document.querySelector("#desactivarBtn");
        let pass = document.getElementById("pass"); // password input
        let statusLabel = document.getElementById("statusLabel");

        activarBtn.addEventListener('click', () => {
            checkSystem(pass.value);
        } , false);
    }, 
 
}; // END of APP

document.addEventListener('DOMContentLoaded', function (){
    FastClick.attach(document.body); // smoother clicks on mobile dev
    app.initialize();
}, false);

function checkSystem(password){
    //checks the password and connection to BT
    console.log(`The password you've entered is: ${password}`);
    if (password.toLowerCase() === 'julio'){
        console.log("password correct");
        bluetoothSerial.connect(macAddress, app.onConnect, app.onDisconnect);
    }
}

function activateAllSystems(params) {
    setStatusLabel("Sistema Conectado", "red");
}

function setStatusLabel(msg, color){
    statusLabel.innerText = msg;
    statusLabel.style.color = color;
}