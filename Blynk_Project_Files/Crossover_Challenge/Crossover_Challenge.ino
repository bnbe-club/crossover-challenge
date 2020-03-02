/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "RrJYNEOThzmJ8lHwjt3LOAMCigYrmFxz";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Network";
char pass[] = "Password";

BlynkTimer timer;

void SendSerial()
{
     // Sent serial data to Blynk terminal - Unlimited string readed
   String content = "";  //null string constant ( an empty string )
   char character;
   while(Serial.available()) 
   {
        character = Serial.read();
        content.concat(character);
        //Serial.println();  
   }
   if (content != "") 
   {
        Blynk.virtualWrite (V1, content);
   }  
}

void setup()
{
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  digitalWrite(D7, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D5, LOW);
    
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5, SendSerial);
// You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  timer.run();
}

//**********Translate the Joystick Position to a Rover Direction**********
//
//This function translates the joystick movement to a Rover direction.
//Blynk Joysick is centered at y=128, x=128 with a range of 0-255. Thresholds set the Joystick 
//sensitivity. These are my adjustments for my touch, you may need something different. Making
//the range too tight will make the rover hard to control. Note: Y values greater than 128 will
//drive the motors FOWARD. Y values less than 128 will drive the motorS in REVERSE. The Rover will
//turn in the direction of the "slow" or unpowered (RELEASE) wheels.
//
//  Joystick Movement along x, y Axis
// (Inside the * is the Threshold Area)
//            y=255--(y_position=255, x_position=128; y_direction=+1, x_direction=0)
//           * | *
//           * | *
//           * | *
//   ********* | *********
//x=0---------------------x=255--(y_position=128, x_position=255; y_direction=0, x_direction=0)
//   ********* | *********
//           * | *
//           * | * (Inside the * is the Threshold Area)
//           * | *
//            y=0--(y_position=0, x_position=128; y_direction=-1, x_direction=0)

BLYNK_WRITE(V0) 
{
  const int X_THRESHOLD_LOW = 108; //X: 128 - 20
  const int X_THRESHOLD_HIGH = 148; //X: 128 + 20   

  const int Y_THRESHOLD_LOW = 108;
  const int Y_THRESHOLD_HIGH = 148;
      
  int x_position = param[0].asInt();  //Read the Blynk Joystick x Position 0-255
  int y_position = param[1].asInt();  //Read the Blynk Joystick y Position 0-255

  int x_direction;  //Variable for Direction of Joystick Movement: x= -1, 0, 1
  int y_direction;  //Variable for Direction of Joystick Movement: y= -1, 0, 1
 
//Determine the direction of the Joystick Movement

  x_direction = 0;
  y_direction = 0;

  if (x_position > X_THRESHOLD_HIGH) 
  {
    x_direction = 1;
  } 
  else if (x_position < X_THRESHOLD_LOW) 
  {
    x_direction = -1;
  }
  if (y_position > Y_THRESHOLD_HIGH) 
  {
    y_direction = 1;
  } 
  else if (y_position < Y_THRESHOLD_LOW) 
  {
    y_direction = -1;
  }
//if x and y are within the threshold their values then x_direction = 0 and y_direction = 0

  if((x_direction == 0) && (y_direction == 0))
  {
    //000
    digitalWrite(D7, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
  }
  else if((x_direction == 0) && (y_direction == 1))
  {
    //011
    digitalWrite(D7, LOW);
    digitalWrite(D6, HIGH);
    digitalWrite(D5, HIGH);
  }
  else if((x_direction == 0) && (y_direction == -1))
  {
    //100
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
  }
  else if((x_direction == 1) && (y_direction == -1))
  {
    //100
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
  }
  else if((x_direction == -1) && (y_direction == -1))
  {
    //100
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
  }
  else if((x_direction == 1) && (y_direction == 1))
  {
    //001
    digitalWrite(D7, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D5, HIGH);    
  }
  else if((x_direction == -1) && (y_direction == 1))
  {
    //010
    digitalWrite(D7, LOW);
    digitalWrite(D6, HIGH);
    digitalWrite(D5, LOW);
  }
  else if((x_direction == 1) && (y_direction == 0))
  {
    //101
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D5, HIGH);
  }
  else if((x_direction == -1) && (y_direction == 0))
  {
    //110
    digitalWrite(D7, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D5, LOW);
  } 
 }
 //**********END Translate the Joystick Position to a Rover Direction**********
