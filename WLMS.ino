#include <LiquidCrystal_I2C.h> 
//#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
//Initialize the LCD display 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
char auth[] = "nQFpCPuEP0hk7Ntc1dX6h-oxXsZlyY2q";//Enter your Auth token 
char ssid[] = "sreelakam_bhilai";//Enter your WIFI name 
char pass[] = "Kerala*1749";//Enter your WIFI password 
BlynkTimer timer; 
// Define the component pins 
#define trig D7 
#define echo D8 
#define relay 3 
#define SOUND_SPEED 0.034 
//Enter your tank max value(CM) 
int Tank_height = 45 ;  //Distance(cm) when tank is empty 
int upper_limit =  2 ;  //Distance(cm) when tank is full  
void setup() { 
Serial.begin(9600);//initialize a serial communicator at a baud rate of 
9600..so that we can print the data in serial monitor 
lcd.init();//initialize the lcd display 
lcd.backlight();//initialize the backlight of lcd 
pinMode(trig, OUTPUT);//sets trig pin of sensor as o/p 
pinMode(echo, INPUT);//sets echo of sensor as i/p 
pinMode(relay, OUTPUT);//set relay that is 3 as o/p 
digitalWrite(relay, HIGH); //make the relay high  
Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);//configur blynk  that is 
connecting to the internet,set auth token 
//Call the functions 
timer.setInterval(100L, ultrasonic); //calls ultrasonic function every 0.1 
second 
} 
//Get the ultrasonic sensor values 
void ultrasonic() { 
digitalWrite(trig, LOW); //trigger is made low 
delayMicroseconds(4); 
digitalWrite(trig, HIGH); 
delayMicroseconds(10); 
digitalWrite(trig, LOW); 
long t = pulseIn(echo, HIGH);//function to get the sound wave travel time: 
int distance = t * SOUND_SPEED/2; 
int   
waterLevelPer; 
int blynkDistance = (distance - Tank_height) * -1; 
waterLevelPer = map((int)distance ,Tank_height,upper_limit, 0, 100);//for 
gauge value 
if (distance <= Tank_height) { 
    Blynk.virtualWrite(V0, waterLevelPer); //virtual pin V0 denotes the water 
level 
  Blynk.virtualWrite(V2, String(distance) + " cm"); 
  } else { 
    Blynk.virtualWrite(V0, 0); 
    Blynk.virtualWrite(V2, String(distance) + " cm"); 
  }                                    //L1=7.5 L2=6.5 L3=5.5 L4=4.5 L5=3.5 
} 
//Get the button value 
BLYNK_WRITE(V1) { //pass the virtual pin for water pump 
  bool Relay = param.asInt();//relay will have the value if the virtual pin v1 
of water pump 
  if (Relay == 1) { 
    digitalWrite(relay, LOW); 
    lcd.setCursor(0, 1);  //sets the cursor at column 0 row1 
    lcd.print("Motor is ON "); 
  } else { 
    digitalWrite(relay, HIGH); 
    lcd.setCursor(0, 1); 
    lcd.print("Motor is OFF"); 
  } 
} 
void loop() { 
  Blynk.run();//Run the Blynk library 
  timer.run();//Run the Blynk timer 
} 
