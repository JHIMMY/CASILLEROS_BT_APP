/*******************************************************************************
 * PROYECTO: DISPENSADOR DE LLAVES LABORATORIO
 * AUTOR: 
 * COLABORACIONES: JADSA
 * DESCRIPCION: 
 * VERSION: 1.0.0
 * 2018
  *******************************************************************************/
 //Librerias
#include <Adafruit_Fingerprint.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// SoftwareSerial Bluetooth(7, 8); // RX, TX

/* GLOBAL VARIABLES */
String inputString = "";      
String CMD = "";

bool ACCESS_GRANTED = false;
int ID = -1;

const byte relay_1 = 12; // power fingerprint sensor
const byte relay_2 = 13; // power NC Devolucion servo, NO lab 6

const uint8_t servo1Pin = 3;
const uint8_t servo2Pin = 5;
const uint8_t servo3Pin = 6;
const uint8_t servo4Pin = 9;
const uint8_t servo5Pin = 10;
const uint8_t servo6Pin = 11;


Servo servo1; // DEV / lab1
Servo servo2; // lab2
Servo servo3; // lab3
Servo servo4; // lab4
Servo servo5; // lab 5
Servo servo6; // lab 6
const int servosNumber = 6;
Servo myServos[servosNumber] = {servo1, servo2, servo3, servo4, servo5, servo6};

SoftwareSerial mySerial(2, 4); // Rx, TX -> fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
/* END gLOBAL VARIABLES */


void setup(){
    finger.begin(57600);
    Serial.begin(9600);
    // Bluetooth.begin(9600);


    pinMode(relay_1, OUTPUT);
    pinMode(relay_2, OUTPUT);
    //Apagar todos los reles por defecto
    relayLockerControl(1, false); // fingerprint sensor
    relayLockerControl(2, false); // could be to use servo 1 and devol servo trick

    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    servo3.attach(servo3Pin);
    servo4.attach(servo4Pin);
    servo5.attach(servo5Pin);
    servo6.attach(servo6Pin);
    
    // reset servos to 0 degrees
    controlServo(1, 0); // reset servo 1 for DELV servo
    controlServo(2, 0);
    controlServo(3, 0);
    controlServo(4, 0);
    controlServo(5, 0);
    controlServo(6, 0);
    executeCMD(">SERVO1<"); //reset servo1 for LAB1
} // fin setup()

void loop(){
    serialEventINcheck();
    if (CMD != ""){
      //execute the CMD
      executeCMD(CMD);
      CMD = ""; // 
    }
    delay(200);

    if (ACCESS_GRANTED){
       ID = getFingerprintID();
        if (ID != -1 && ID > 0){
          String userID = "user" + String(ID);
          Serial.println(userID);
          ID = -1;
          ACCESS_GRANTED = false;
        }
    }
}

//This function handles the reception of the CMD instructions from HMI
void serialEventINcheck() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n'){
    //   Serial.print("ARDUINO: recibido de APP =>  ");
    //   Serial.println(inputString);
      CMD = inputString;
      inputString = "";
    }
    else {
      // add it to the inputString:
    inputString += inChar;
    }  
  }
} // EOF serialEventManual()

void executeCMD(String comando){
  //exe
  if (comando == ">EN_FINGER<"){
      fingerprintCheckAndAction(true);
      ACCESS_GRANTED = true;
      CMD = "";
  }
  else if(comando == ">DIS_FINGER<"){
      fingerprintCheckAndAction(false);
      ACCESS_GRANTED = false;
      ID = -1;
      CMD = "";
  }
  else if(comando == ">SERVO1<"){
      relayLockerControl(2, true); // enable lab 1 servo
      delay(450);
      Serial.println("ABRIR1");
      controlServo(1, 180);
      delay(1800);
      controlServo(1, 0);
      delay(320);
      relayLockerControl(2, false);
  }
  else if(comando == ">SERVO2<"){
      Serial.println("ABRIR2");
      controlServo(2, 180);
      delay(2000);
      controlServo(2, 0);
  }
  else if(comando == ">SERVO3<"){
      Serial.println("ABRIR3");
      controlServo(3, 180);
      delay(2000);
      controlServo(3, 0);
  }
  else if(comando == ">SERVO4<"){
      Serial.println("ABRIR4");
      controlServo(4, 180);
      delay(2000);
      controlServo(4, 0);
  }
  else if(comando == ">SERVO5<"){
      Serial.println("ABRIR5");
      controlServo(5, 180);
      delay(2000);
      controlServo(5, 0);
  }
  else if(comando == ">SERVO6<"){
      Serial.println("ABRIR6");
      controlServo(6, 180);
      delay(2000);
      controlServo(6, 0);
  }
  else if(comando == ">SERVODEVOL<"){
      Serial.println("ABRIR-DEVOL");
      controlServo(1, 180);
      delay(6000);
      controlServo(1, 0);
  }
} // fin executeCMD()

void relayLockerControl(int number, boolean action){
    // number -> numero de pin de rele a controlar
    // action -> true = activado / false = desactivado
    int relay; 
    switch (number)
    {
        case 1:
            relay = relay_1;
            break;
        case 2:
            relay = relay_2;
            break;
        default:
            systemError("Numero de rele invalido!");
    } 

    if (action){
        digitalWrite(relay, LOW); // rele activado

    }
    else{
        digitalWrite(relay, HIGH);  
    }
} // fin releLockerControl()

void systemError(String msg){
    // resetear el arduino para continuar
    //Serial.println(msg);
    Serial.println("ERROR!"); // error fatal reinicie
    while (1) { delay(10); }
}

void controlServo(int servoNumber, int rotation ){
    if (servoNumber >= 1 && servoNumber <= servosNumber){ // 7 servos
        int index = servoNumber - 1;
        myServos[index].write(rotation);
    }
    else{
        systemError("Servo Invalido");
    }
 
} // FIN controlServo()

void fingerprintCheckAndAction(bool action){
  //if action true - enable its rele and checkss if founf, action false disables its rele (turn off fingerprint)
    if (action){
        relayLockerControl(1, true);
        delay(600);
        // detectar sensor huella
        if (finger.verifyPassword()) {
            //Serial.println("Found fingerprint sensor!");
            // finger.getTemplateCount();
            // Serial.print("El sensor contiene "); 
            // Serial.print(finger.templateCount); Serial.println(" Huellas registradas");
            // Serial.println("Waiting for valid finger...");
        } 
        else {
             systemError("No se pudo encontrar al sensor :(\nFAVOR REINICIE!");
        }   
    }
    else{
        relayLockerControl(1, false);
    }
}

// devuelve -1 si falla, de lo contrario devuelve el ID #
int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  return finger.fingerID; 
} // fin getFingerprintIDez()