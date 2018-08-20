/*******************************************************************************
 * PROYECTO: DISPENSADOR DE LLAVES LABORATORIO
 * AUTOR: 
 * COLABORACIONES: JADSA
 * DESCRIPCION: 
 * VERSION: 1.0.0
 * 2018
  *******************************************************************************/

#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(7, 8); // RX, TX


/* GLOBAL VARIABLES */
String inputString = "";      
String CMD = "";
/* END gLOBAL VARIABLES */


void setup(){
    Bluetooth.begin(9600);
    Serial.begin(115200);
}

void loop(){
    serialEventINcheck();
    if (CMD != ""){
      //execute the CMD
      executeCMD(CMD);
      CMD = ""; // 
    }

    delay(200);
}

//This function handles the reception of the CMD instructions from HMI
void serialEventINcheck() {
  while (Bluetooth.available()) {
    // get the new byte:
    char inChar = (char)Bluetooth.read();

    if (inChar == '\n'){
      Serial.print("ARDUINO: recibido de APP =>  ");
      Serial.println(inputString);
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

}