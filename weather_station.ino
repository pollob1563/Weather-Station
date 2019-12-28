//A Project of Google Code-in 2019 For fedora Project created by pollob1563
/*
 * 
 * ||||||||||||||||||||Components||||||||||||||||
 * ||                                          ||
 * ||1.ESP8266(NODEMCU)                        ||
 * ||2.Temperature & Humidity Sensors(DHT11)   ||
 * ||3.Breadboard                              ||
 * ||4.Some Jumper Wires                       ||
 * ||                                          ||
 * ||||||||||||||||||||||||||||||||||||||||||||||
 *    
 * 
 * ||||||||||||||Pinout|||||||||||||||||||||
 * ||                                     ||
 * ||DHT           TO         NODEMCU     ||
 * ||                                     ||
 * ||DHT(+)        =           VIN;       ||
 * ||DHT(-)        =           GND;       ||
 * ||DHT(DATA)     =           D0;        ||
 * ||                                     ||
 * |||||||||||||||||||||||||||||||||||||||||
 * 
 * 
 * 
 * Github Repo Link:       
 * 
 * 
 * Thingspeak channel link: https://thingspeak.com/channels/947635/
 *
 *
 * Schematic Circuit Diagram Link: https://easyeda.com/prosenjit.pollob/thingspeak-with-dht-nodemcu 
 * 
 *
 */

// include the library code:
#include <ESP8266WiFi.h>    // initialize the library by associating any needed ESP8266(NODEMCU) interface pin
#include "ThingSpeak.h"    // initialize the library by associating any needed ThingSpeak Server
#include <SimpleDHT.h>    // initialize the library by associating any needed DHT interface pin



      /**//**//**//**//**//**//**//**//**//**//*Fill Up Your Information/**//**//**//**//**//**//**//**//**//**//***//**//***/
     /**/                                                                                                                /**/
    /**/               const char* ssid = "TP-Link_5EEC";        // Your WiFi SSID name                                 /**/
   /**/                const char* password = "76278860" ;      // Your WiFi password                                  /**/
  /**/                 const char*  API = "3YQULS8CQS01EJ4N";  // Your ThingSpeak api key                             /**/
 /**/                                                                                                                /**/
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

WiFiClient client;    //Declear WiFiClient

int myChannelNo = 0; //Declear Just a number You can place any digit

SimpleDHT11 dht(D0);//decleared a variable for DHT and connect to D0 pin of nodemcu to DHT data pin

int ti,n,t,h,averageTemperature,averageHumidity;  //For Calculation

int Time = 2000;                                 //For Calculation 



void setup() {
  delay(1000);

  WiFi.begin(ssid, password);                               //Connecting to WiFi
  
  Serial.begin(115200);                                   // Baud rate of Serial Monitor
  
  Serial.print("WiFi: ");                               //Print WiFi Name at Serial Monitor
  Serial.println(ssid);

  delay(1000);
  
  Serial.println("WiFi is Connecting.....");    //Print WiFi Status at Serial Monitor
 
  delay(8000);                                 //Waitng For Connecting
 
  while (WiFi.status() != WL_CONNECTED) {
  
    Serial.println("WiFi NOT CONNECTED");    //Print WiFi Status at Serial Monitor(Confarmation Messege)
    Serial.println();

    delay(500);

    Serial.println("Wait a few Seconds or check your WiFi SSID and Password"); //If WiFi Not CONNECTED then the instruction print at Serial Monitor 
    
    delay(1000);
    
  }

  delay(100);
  
  Serial.println("WiFi Connected!");    //Print WiFi Status at Serial Monitor(Confarmation Messege)
  
  delay(1000);
  Serial.print("Default Gateway: "); //Gateway IP
  delay(100);
  Serial.println(WiFi.localIP() ); //Print Gateway IP at Serial Monitor

  ThingSpeak.begin(client);      //Connecting To ThingSpeak Server

  delay(500);

  Serial.println();

  delay(1000);


  Serial.println("=========================================================================================================================================================================="); //ENDING SIGN OF VOID SETUP

  delay(400);

  Serial.println();
  
  delay(100);
  
}


void loop() {

  delay(1000);

  Serial.println("DHT11 is cultributing....."); // Ready To Sensor Reading
  

  byte temperature = 0;                       //Decleared a variable for Temperature
  
  byte humidity = 0;                         //Decleared a variable for Humidity
  
  delay(2000);
  
  int error = SimpleDHTErrSuccess; //Decleared a variable for DHT error message
    
  if ((error = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    delay(500);
    Serial.println("DHT11 can't read,Check your DHT11 Sensor or Change this. :)");//Print DHT11 Sensor Status at Serial Monitor(Confarmation Messege)
    delay(500);
    
    Serial.print("ERROR= ");      //Print DHT11 Sensor error code at Serial Monitor
    Serial.println(error);
    
    delay(500);
    
    Serial.println("Try again. :)"); 
    delay(500);

    Serial.println();
    
    delay(500);
    
    Serial.println("</></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></>");//endline sign
    delay(500);

    Serial.println();
    delay(2000);
    return ;
  }

  else{
    
    for(ti = 0,n = 0,t = 0,h = 0,averageTemperature = 0,averageHumidity=0;  ; ){
      
      if ((error = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) // Avoiding Wrong Sensor Data
        continue;

      t = t + temperature; //Storing Total Temperature value
      
      h = h + humidity;   //Storing Total Humidity value

      n++;               //Stoing Total attempt

      delay(Time);      //Delay 2 Second

      ti = ti + Time;  //Stoing Total Total

      if(ti>=3582000){
        
        averageTemperature = t/n;                                         // Calculating average temperature value
        delay(1000);
        Serial.print("Average temperature: ");
        delay(1000);
        Serial.print(averageTemperature);                             //Print average temperature value at Serial Monitor
        delay(1000);
        Serial.println(" C");

        averageHumidity = h/n;                                    // Calculating average humidity value
        delay(1000);
        Serial.print("Average Humidity: ");
        delay(1000);
        Serial.print(averageHumidity);                         //Print average humidity value at Serial Monitor
        delay(1000);
        Serial.println(" %");

        ThingSpeak.setField(1,averageTemperature);          //Ready For sent average temperature value to ThingSpeak server

        ThingSpeak.setField(2,averageHumidity);            //Ready For sent average humidity value to ThingSpeak server
        
        ThingSpeak.writeFields(myChannelNo, API);        //Sent all of value to ThingSpeak server

        delay(1000);

        Serial.println("Successfully sent average temperature and humidity of this hour :) ");

        delay(1000);

        Serial.println("</></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></>");//endline sign
        
        break;
      }
      
      
    }

    
  }

}