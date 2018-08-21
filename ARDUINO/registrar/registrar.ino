/*************************************************** 
  REGISTRAR NUEVOS USUARIOS, QUITAR EL CABLE DE ALIMENTACION DEL BLUETOOTH Y CARGAR ESTE PROGRAMA
  */

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 4);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

uint8_t pinRele = 12;


void setup()  
{
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  pinMode(pinRele, OUTPUT);
  delay(50);
  digitalWrite(pinRele, LOW); // activamos el rele
  delay(400);
  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor de Huella Encontrado!");
  } else {
    Serial.println("No se encontró un sensor de huella :(");
    while (1) { delay(10); }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  Serial.println("Sistema listo para registrar una huella!");
  Serial.println("Favor ingrese el ID # (de 1 a 256) en el cual desea guardar esta huella...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Registrando ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Esperando por un dedo valido para registrat como #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Muestra tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error de muestra");
      break;
    default:
      Serial.println("Error desconocido");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Muestra convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Muestra poco clara");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se pudo detectar las caracteristicas de sensor de huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se pudo detectar las caracteristicas de sensor de huella");
      return p;
    default:
      Serial.println("Error desconocido");
      return p;
  }
  
  Serial.println("Aleje el dedo");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Vuelva a colocar el mismo dedo");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Muestra tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error en el muestreo");
      break;
    default:
      Serial.println("Error desconocido");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Muestra convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Muestra podo clara");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se pudo detectar las caracteristicas de sensor de huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se pudo detectar las caracteristicas de sensor de huella");
      return p;
    default:
      Serial.println("Error desconocido");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Huellas coincidieron!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Dedos no coincidieron");
    return p;
  } else {
    Serial.println("Error desconocido");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Guardado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de comunicacion");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("No se pudo guardar en esa localizacion");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error al escribir a la memoria flash");
    return p;
  } else {
    Serial.println("Error desconocido");
    return p;
  }   
}
