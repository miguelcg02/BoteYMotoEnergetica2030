#include <SoftwareSerial.h>
SoftwareSerial gsm(9,8); // RX: 9, TX:8
#include <ArduinoJson.h> //Version 5
StaticJsonBuffer<200> jsonBuffer; //Buffer del Json
String respuesta = "";

void setup() {
    Serial.begin(9600); //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
    Serial.println("Initializing..........");
    gsm.begin(9600);         //Begin serial communication with Arduino and SIM800L
    DynamicJsonBuffer jsonBuffer;
}

void loop() {
    Serial.println("Initializing...");
    gsm.println("AT");
    updateSerial();
    delay(2000);
    
    gsm.println("AT+CMEE=1");
    delay(200);
    updateSerial();
    
    gsm.println("AT+CSQ");
    delay(200);
    updateSerial();
    
    gsm.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    delay(2000);
    updateSerial();
    
    gsm.println("AT+SAPBR=3,1,\"APN\",\"internet.movistar.com.co\"");//APN
    delay(5000);
    updateSerial();
    
    gsm.println(F("AT+CREG?"));
    delay(200);
    updateSerial();
    
    gsm.println("AT+CGREG?");
    delay(200);
    updateSerial();
    
    gsm.println("AT+SAPBR=1,1");
    delay(200);
    updateSerial();
    
    gsm.println("AT+SAPBR=2,1");
    delay(200);
    updateSerial();
    
    gsm.println("AT+HTTPINIT");
    delay(700);
    updateSerial();
    
    gsm.println("AT+HTTPPARA=\"CID\",1");
    delay(500);
    updateSerial();

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& object = jsonBuffer.createObject();

    object.set("deviceID","-----");
    object.printTo(Serial);
    Serial.println(F(" "));
    String sendtoserver;
    object.prettyPrintTo(sendtoserver);
    delay(200);

    gsm.println("AT+HTTPPARA=\"URL\",\"http://servidor-monitoria.herokuapp.com/\""); //Server address
    delay(7000);
    updateSerial();

    gsm.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
    delay(7000);
    updateSerial();

    gsm.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
    delay(7000);
    Serial.println(sendtoserver);
    updateSerial();

    gsm.println(sendtoserver);
    delay(200);
    updateSerial();

    gsm.println("AT+HTTPACTION=1");
    delay(200);
    updateSerial();

    gsm.println("AT+HTTPREAD");
    delay(200);
    updateSerial();

    gsm.println("AT+HTTPTERM");
    delay(200);
    updateSerial();
}

String updateSerial()
{
  respuesta = "";
  delay(500);
  Serial.print("entra Update");
  while (Serial.available())
  {
    Serial.print("aaaa");
    gsm.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (gsm.available())
  {
    Serial.print("entra a crear respuesta");
    respuesta = respuesta + gsm.readString();
  }
  Serial.print(respuesta);//Forward what Software Serial received to Serial Port
  if ((respuesta.indexOf("OK") >= 0) || (respuesta.indexOf(">") >= 0) || (respuesta.indexOf(".") >= 0)) {
    Serial.print("OK 1");
  }
  else {
    delay(10000);
  }
  return respuesta;
}
