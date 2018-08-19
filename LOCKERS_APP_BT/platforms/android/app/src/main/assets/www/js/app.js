
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
        app.setListeners();
    },
    onConnect: function() {
        activateAllSystems();
        bluetoothSerial.subscribe("\n", app.onMessage, app.subscribeFailed);     
    }, 
    onDisconnect: function() {
        alert("Bluetooth Desconectado, revise la conexion e inicie nuevamente");
        resetApp();
    },
    onMessage: function(data) {
        // counter.innerHTML = data;        
    },
    subscribeFailed: function() {
        alert("Fallo la suscripción al Bluetooth Arduino");
    }, 
    // From here JAD FUNCTIONS SPECIFIC FOT THIS PROJECT
    setListeners: function() {
        let activarBtn = document.querySelector("#activarBtn");
        let desactivarBtn = document.querySelector("#desactivarBtn");
        let pass = document.getElementById("pass"); // password input
        let statusLabel = document.getElementById("statusLabel");
        let cardImg = document.getElementById("carImg");

        activarBtn.addEventListener('click', () => {
            checkSystem(pass.value);
        } , false);

        desactivarBtn.addEventListener('click', function(){
            bluetoothSerial.disconnect(resetApp, problemDisconnecting);
        }, false );
    }, 
 
}; // END of APP

document.addEventListener('DOMContentLoaded', function (){
    FastClick.attach(document.body); // smoother clicks on mobile dev
    app.initialize();
}, false);



/* HELPER FUNCTIONS */
function checkSystem(password){
    //checks the password and connection to BT
    console.log(`The password you've entered is: ${password}`);
    if (password.toLowerCase() === 'julio'){
        console.log("password correct");
        bluetoothSerial.connect(macAddress, app.onConnect, app.onDisconnect);
    }
    else{
        setStatusLabel("Contraseña Incorrecta", "yellow");
        pass.value = '';
    }
}

function activateAllSystems(params) {
    setStatusLabel("Pase su huella", "lime");
    setcardImage("finger.jpg");
    pass.value = '';
    enableDisableBtns(activarBtn, false);
    enableDisableBtns(desactivarBtn, true);
    
}

function resetApp(){
    // restarts the app ewhithn the app
    setStatusLabel("");
    setcardImage("uagrm.jpg");
    pass.value = '';
    enableDisableBtns(activarBtn, true);
    enableDisableBtns(desactivarBtn, false);
}

function problemDisconnecting(){
    alert("Hay un problema al desconectar, favor reinicie la aplicacion");
    setStatusLabel("Falló la desconexión segura");
}

function setStatusLabel(msg, color = "whitesmoke"){
    statusLabel.innerText = msg;
    statusLabel.style.color = color;
}

function setcardImage(imageName){
    // default path www/img/
    cardImg.src = `img/${imageName}`; 
}

function enableDisableBtns(btn, cmd = false){
    // if cmd is true enable btn || if false disable btn
    if (cmd){
        btn.disabled = false;
    }
    else{
        btn.disabled = true;
    }
}