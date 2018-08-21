
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
        bluetoothSerial.subscribe("\n", app.onMessage, app.subscribeFailed); 
        setTimeout(activateAllSystems, 2000);    
        // activateAllSystems();
    }, 
    onDisconnect: function() {
        alert("Bluetooth Desconectado, revise la conexion e inicie nuevamente");
        resetApp();
    },
    onMessage: function(data) {
        // counter.innerHTML = data; 
        processIncomingData(data);      
    },
    subscribeFailed: function() {
        alert("Fallo la suscripción al Bluetooth Arduino");
    },
    
    sendData: function(data){
        let trueData = data + "\n";
        bluetoothSerial.write(trueData);
    },
    // From here JAD FUNCTIONS SPECIFIC FOT THIS PROJECT
    setListeners: function() {
        let activarBtn = document.querySelector("#activarBtn");
        let desactivarBtn = document.querySelector("#desactivarBtn");
        let pass = document.getElementById("pass"); // password input
        let statusLabel = document.getElementById("statusLabel");
        let cardImg = document.getElementById("carImg");
        let abrirBtn = document.getElementById("abrirBtn");
        let labSelection = document.getElementById("labSelection");
        let devolverBtn = document.getElementById("devolverBtn");
        let testBtn = document.getElementById("testBtn");

        activarBtn.addEventListener('click', () => {
            checkSystem(pass.value);
        } , false);

        desactivarBtn.addEventListener('click', function(){
            app.sendData(">DIS_FINGER<");
            bluetoothSerial.disconnect(resetApp, problemDisconnecting);
        }, false );

        // testBtn.addEventListener('click', function(){
        //     app.sendData(">HolaCHoco<");
        // }, false);

        abrirBtn.addEventListener('click', function(){
            let sel = labSelection.value;  ///string
            let servo = ">SERVO" + sel + "<";
            app.sendData(servo);
        }, false);

        devolverBtn.addEventListener('click', function(){
            app.sendData(">SERVODEVOL<");
        }, false);
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
    app.sendData(">EN_FINGER<");
    setStatusLabel("Pase su huella", "lime");
    setcardImage("finger.jpg");
    pass.value = '';
    enableDisableBtns(activarBtn, false);
    enableDisableBtns(desactivarBtn, true);
    enableDisableBtns(testBtn, true);
}

function resetApp(){
    // restarts the app ewhithn the app
    setStatusLabel("");
    pass.value = '';
    enableDisableBtns(activarBtn, true);
    enableDisableBtns(desactivarBtn, false);

    enableDisableBtns(abrirBtn, false);
    enableDisableBtns(devolverBtn, false);
    removeUserData();
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

function processIncomingData(data){
    /* VALID INSTRUCTIONS 
        user#  -- eg user1, user2, user3
    */
   let divTESTEST = document.getElementById("tete");
   divTESTEST.innerText = data;
    if (data.indexOf("user") != -1){
        console.log("La instruccion es Correcta!");
        let userID = data.substring(4);
        checkValidID(userID);
    }
    else if ("ERROR!"){
        alert("ERROR DETECTADO FAVOR REINICIE TOTO EL SISTEMA");
    }
    else{
       console.log("INCORRECTA!!!!!");
    }
} // end processincomingdata


 function checkValidID(id){
    let len = usuarios.length;
    console.log("Longitud array usuarios: " + len);
    for (let i = 0 ; i < len; i++){
        if (usuarios[i].id == Number(id)){
            retreiveUserData(i, id);
            break;
        }
    }
 }


function retreiveUserData(index, ID){
    enableDisableBtns(abrirBtn, true);
    enableDisableBtns(devolverBtn, true);

    nombreSpan.innerText = usuarios[index].nombre;
    carreraSpan.innerText = usuarios[index].carrera;
    registroSpan.innerText = usuarios[index].registro;
    setcardImage(usuarios[index].foto);
}

function removeUserData(){
    nombreSpan.innerText = "..........................";
    carreraSpan.innerText = "..........................";
    registroSpan.innerText = "..........................";
    setcardImage("uagrm.jpg");
}