#include <SoftwareSerial.h>                    //Library for serial connection with ESP
SoftwareSerial ESP(2,3);                       //ESP is connected to 12 and 13 pin of Arduino

String sendData = "GET /channels/ 1029312/fields/1/last.json?api_key=IMGAX00CXYE7QAQK&results=2";
String output = "";                            //Initialize a null string variable 
#define Relay2 7

void setup() 
{
    pinMode(Relay2,OUTPUT);
    delay(2000);

    Serial.begin (9600);
    delay(100);
    ESP.begin(9600);
  
  
    ESP_talk("AT",1000);
    ESP_talk("AT+RST",1000);
    ESP_talk("AT+CWMODE=1",5000);
    Serial.print("connecting WiFi..");          
    ESP_talk("AT+CWJAP=\"D-Link_DIR-615\",\"*mm1323*\"",10000);
    Serial.println("Wifi Connected"); 
    delay(1000);
}

void loop() 
{
    String cmd = "AT+CIPSTART=\"TCP\",\"";        // Establishing TCP connection// AT+CIPSTART=4,"TCP","google.com",80
    cmd += "184.106.153.149";                      //  api.thingspeak.com
    cmd += "\",80";                                // port 80
    ESP_talk(cmd,1000);
    delay(100); 
    ESP_talk("AT+CIPSEND=76",1000);  
    delay(100);
    output = "";
    ESP_talk(sendData,1000);
    delay(100);

    char incoming_value = output[126];
    Serial.print("incoming_value is : ");   
    Serial.println(incoming_value);
    Serial.print("Listning....");               //Intro Message line 1          

    if (incoming_value == '1')                  //light is turned on
    {
      Serial.print("Light is ON : "); 
      digitalWrite(Relay2,HIGH);
    }
    if (incoming_value == '0')                  //light is turned off
    {
      Serial.print("Light is OFF : "); 
      digitalWrite(Relay2,LOW);
    }
}

void ESP_talk(String AT_cmd, const int timeout)
{
  
  Serial.print("Sent: ");
  Serial.print(AT_cmd);
  ESP.println(AT_cmd);                          //print to ESP through software serial 
  Serial.println("");                           //Move to next line 
  
  
  long int time = millis();

  output="";                                    //clear the string
  
  while ( (time + timeout) > millis())
  {
    while (ESP.available())
    {
      char i = ESP.read();                      //read one char 
      output += i;                              //Combine char to string 
    }
  }
  
  Serial.println("Received: ");
  Serial.print(output);
}
