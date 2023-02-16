/*********************************************************************************
 * Sketch name: CB_Final001
 * Description: ESP8266-01 Access Point L293D with two motos GPIO 2 fwd GPIO 3 bak
 * steering servo attached pin GPIO 0
 * Created on:  October 2022
 * Author:      Bud Johnson
 * from 
 * Date
 * No app or internet access needed.
 * Two motors in unison for forward and back
 * one servo for steering
 ********************************************************************************/

#include <Arduino.h>
#include <Servo.h>              //include servo library
#include <ESP8266WebServer.h>   //include web server library

//*********************************************
ESP8266WebServer server;      //instantiate webserver
//*********************************************

//*********************************************
Servo steer;                  //instantiate steer servo
//*********************************************

//*********************************************
const int sPin = 0;           //ESP01 pin 0 is steering servo
const int fwd = 3;            //ESP01 pin 3 high is forward
const int bak = 2;            //ESP01 pin 2 high is reverse
//*********************************************

//*********************************************
char ssidAP[] = "CB_Robot";   //WLAN SSID
char passwordAP[] = "1234";   //WLAN SSID passwor
//*********************************************

//*********************************************
IPAddress local_ip(192,168,2,1);        	// pre-defined IP address values
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);
//*********************************************

//*********************************************
//userinterface HTML code
const char mainPage[] PROGMEM = R"***(
<!DOCTYPE html>
<head><title></title></head>
</body>
	<br>
	<br>
	<br>
	<h1>WiFi Robot Controller</h1>
	<br>
	<br>
	<p><a href=/forward>FORWARD</a></p>
	<p><a href=/still>STOP</a></p>
	<p><a href=/back>REVERSE</a></p>
	<br>
	<br>
	<br>
	<br>
	<p><a href=/turnLeft>LEFT</a></p>
	<p><a href=/turnStraight>STRAIGHT</a></p>
	<p><a href=/turnRight>RIGHT</a></p>
</body>
</html>
)***";
//********FUNCTIONS****************************
void handleRoot()
{
  server.send(200, "text/html", mainPage);
}

void forward()
{
	digitalWrite(fwd, HIGH);
	digitalWrite(bak, LOW);
	server.send(200, "text/html", mainPage);
}
//--------------------------------------

void back() 
{
	digitalWrite(fwd, LOW);
	digitalWrite(bak, HIGH);
	server.send(200, "text/html", mainPage);
}
//--------------------------------------

void still() 
{
	digitalWrite(fwd, LOW);
	digitalWrite(bak, LOW);
	server.send(200, "text/html", mainPage);
}
//--------------------------------------

void turnLeft() 
{
	steer.write(180);
	server.send(200, "text/html", mainPage);
}
//--------------------------------------

void turnStraight() 
{
	steer.write(90);
	server.send(200, "text/html", mainPage);
}
//--------------------------------------

void turnRight() 
{
	steer.write(0);
	server.send(200, "text/html", mainPage);
}
//--------------------------------------

//********SETUP********************************

void setup()
{
//--------------------------------------
steer.attach(sPin);							// attach steering servo to GPIO 0
pinMode(fwd, OUTPUT);						// pin 2 fwd is OUTPUT
pinMode(bak, OUTPUT);						// pin 3 bak is OUTPUT
//--------------------------------------
  WiFi.mode(WIFI_AP);               		// Wi-Fi AP mode
  delay(1000);                    			// setup AP mode
//--------------------------------------
  
  WiFi.softAPConfig(local_ip, gateway, subnet);   	//  predefined IP address
  WiFi.softAP(ssidAP, passwordAP);      			// initialise Wi-Fi
//--------------------------------------

  server.begin();                 			// initialise server
  server.on("/", handleRoot);     			// send this as default client request
  server.on("/forward", forward);
  server.on("/still", still);
  server.on("/back", back);
  server.on("/turnLeft", turnLeft);
  server.on("/turnStraight", turnStraight);
  server.on("/turnRight", turnRight);
//--------------------------------------

 
  
 
  
}
//********LOOP*********************************

void loop()
{
  server.handleClient();            		// manage HTTP requests
}
