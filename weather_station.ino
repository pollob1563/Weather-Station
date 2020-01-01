//A Project of Google Code-in 2019 For fedora Project created by pollob1563
/*
 * 
 * |||||||||||||||||Components||||||||||||||||||||
 * ||                                           ||
 * ||1.ESP8266(NODEMCU);                        ||
 * ||2.Temperature & Humidity Sensors(DHT11);   ||
 * ||3.Breadboard;                              ||
 * ||4.Some Jumper Wires;                       ||              
 * ||5.LED/Buzzer;                              ||
 * ||6. Resistor(between 100 ohm to 200 ohm);   ||
 * |||||||||||||||||||||||||||||||||||||||||||||||
 *    
 * 
 * |||||||||||||||||||||||Pinout|||||||||||||||||||||
 * ||                                              ||
 * ||DHT                     TO         NODEMCU    ||
 * ||                                              ||
 * ||DHT(+)                  =           VIN;      ||
 * ||DHT(-)                  =           GND;      ||
 * ||DHT(DATA)               =           D0;       ||      
 * ||LED(+)/Buzzer(+)        =           D4;       ||
 * ||LED(-)/Buzzer(-)        =           GND;      ||
 * ||Use Resistor for Restrain overload current;   ||                                 
 * ||                                              ||
 * ||||||||||||||||||||||||||||||||||||||||||||||||||
 * 
 * 
 * 
 * Github Repo Link: https://github.com/pollob1563/Weather-Station     
 * 
 * 
 * Thingspeak channel link: https://thingspeak.com/channels/947635/
 *
 * 
 *N.B: If DHT11 corrupt then LED light/Buzzer will turn on.
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

int cautionLight = D4; //decleared a variable for cautionLight and connect to D4 pin of nodemcu to LED(+)

float totalAttempt,totalTemperature,totalHumidity,averageTemperature,averageHumidity;  //For Calculation

int totalTime, Time = 2000, avoidTime = 1000,totalAvoidTime = 0 ;                    //For Calculation and time value is 2 second


void setup() {
  delay(1000);

  WiFi.begin(ssid, password);                               //Connecting to WiFi
  
  Serial.begin(115200);                                   // Baud rate of Serial Monitor
  
  Serial.print("WiFi: ");                               //Print WiFi Name at Serial Monitor
  Serial.println(ssid);

  delay(1000);
  
  Serial.println("WiFi is Connecting.....");    //Print WiFi Status at Serial Monitor
 
  delay(8000);                                //Waitng For Connecting
 
  while (WiFi.status() != WL_CONNECTED) {
  
    Serial.println("WiFi NOT CONNECTED"); //Print WiFi Status at Serial Monitor(Confarmation Messege)
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

  pinMode(cautionLight,OUTPUT); //Declear the pin for cautionLight

  digitalWrite(cautionLight,HIGH); // Testing (Light On) 
  delay(500);
  digitalWrite(cautionLight,LOW);// Testing (Light off) 
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
  

  byte temperature = 0;                      //Decleared a variable for Temperature
  
  byte humidity = 0;                       //Decleared a variable for Humidity
  
  delay(2000);
  
  int error = SimpleDHTErrSuccess; //Decleared a variable for DHT error message

  digitalWrite(cautionLight,LOW);                                               //If DHT11 work properly then cautionLight off
    
  if ((error = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {

    digitalWrite(cautionLight,HIGH);                                                //cautionLight on
    
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
    
    for(totalTime = 0,totalAttempt = 0,totalAvoidTime = 0, totalTemperature = 0,totalHumidity = 0,averageTemperature = 0,averageHumidity = 0;  ;  ){

      
      if ((error = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess){   // Avoiding send Wrong Sensor Data
        
        Serial.println("Please Check Your DHT11 Sensor Connection or change it :)"); //If DHT11 Doesn't response then the instruction print at Serial Monitor 
        digitalWrite(cautionLight,HIGH);                                            //cautionLight on
        delay(avoidTime);
        
        totalAvoidTime = totalAvoidTime + avoidTime;                             //Calculating total time of avoiding send Wrong Sensor Data
        
        continue;
      }

      digitalWrite(cautionLight,LOW);                                               //If DHT11 work properly then cautionLight off

      totalTemperature = totalTemperature + temperature;                          //Storing Total Temperature value
      
      totalHumidity = totalHumidity + humidity;                                 //Storing Total Humidity value

      totalAttempt++;                                                         //Storing Total attempt

      Serial.println(".");

      delay(Time);                                                          //Delay 2 Second

      totalTime = totalTime + Time + totalAvoidTime;                      //Storing Total Total

      totalAvoidTime = 0;                                                //Calculation 
      
      if(totalTime>=3539000){
        
        averageTemperature = totalTemperature/totalAttempt;           // Calculating average temperature value
        delay(1000);
        Serial.print("Average temperature: ");
        delay(1000);
        Serial.print(averageTemperature);                          //Print average temperature value at Serial Monitor
        delay(1000);
        Serial.println(" ^C");

        averageHumidity = totalHumidity/totalAttempt;          // Calculating average humidity value
        delay(1000);
        Serial.print("Average Humidity: ");
        delay(1000);
        Serial.print(averageHumidity);                     //Print average humidity value at Serial Monitor
        delay(1000);
        Serial.println(" %");

        ThingSpeak.setField(1,averageTemperature);           //Ready For sent average temperature value to ThingSpeak server

        ThingSpeak.setField(2,averageHumidity);            //Ready For sent average humidity value to ThingSpeak server
        
        ThingSpeak.writeFields(myChannelNo, API);        //Send all of value to ThingSpeak server

        delay(1000);

        Serial.println("Successfully sent average temperature and humidity of this hour :) "); //Print Confirmed sent data at Serial Monitor

        delay(1000);

        Serial.println("</></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></></>");//endline sign
        
        break;
      }
      
      
    }

    
  }

}
