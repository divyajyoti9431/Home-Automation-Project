#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "SoftwareSerial.h"
#define relay1 A0
#define relay2 A1
#define relay3 A2
#define fan A3
#define echoPin1 6 //connect echo pin of ultrasonic sensor to D12 of Arduino
#define trigPin1 7 //connect trigger pin of ultrasonic sensor to D10 of Arduino
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16,2);

long duration1;  // declare variables to hold duration and distance
int distance1;

float Temperature;
float Humidity;

SoftwareSerial mySerial(10, 11); // RX, TX
void setup() {
   lcd.init();
  lcd.backlight();
dht.begin(); 
  pinMode(trigPin1,OUTPUT); //set trigPin as output pin of Arduino
 pinMode(echoPin1,INPUT);  //set echoPin as output pin of Arduino
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(relay1, HIGH); // relay module work with active low
  digitalWrite(relay2, HIGH); // so u need to make high logic
  digitalWrite(relay3, HIGH); // before start.
  digitalWrite(fan, HIGH);
  lcd.setCursor(0,0);
  lcd.print("Home  Automation");
  lcd.setCursor(4,1);
  lcd.print("System");
  delay(1000);
  lcd.clear();
  Serial.begin(115200);
}

void dht_sen() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
Humidity = dht.readHumidity(); // Gets the values of the humidity

lcd.setCursor(0,1);
lcd.print("T :");
lcd.setCursor(3,1);
lcd.print(Temperature);
lcd.setCursor(7,1);
lcd.print("C");  
lcd.setCursor(9,1);
lcd.print("H :");
lcd.setCursor(12,1);
lcd.print(Humidity);  
        
}
void loop() { // run over and over
  
  digitalWrite(trigPin1,LOW); //generate square wave at trigger pin
 delayMicroseconds(2);
 digitalWrite(trigPin1,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin1,LOW);
 duration1=pulseIn(echoPin1,HIGH);//calculation of distance of obstacle
 distance1=(duration1*0.034/2);
  if (distance1<10)
    {digitalWrite(fan, LOW);
    lcd.setCursor(12,0);
    lcd.print("R4ON");
    dht_sen();
    }
    
  if (mySerial.available()) {
    char inChar = mySerial.read();
    Serial.print(inChar);
    
    Serial.println(distance1);
    
    if (inChar == 'a') {
      digitalWrite(relay1, LOW); //relay1 on
      lcd.setCursor(0,0);
      lcd.print("R1-N");
      dht_sen();
    }
    else if (inChar == 'b') {
      digitalWrite(relay1, HIGH); //relay1 off
      lcd.setCursor(0,0);
      lcd.print("R1-F");
      dht_sen();
    }
    else if (inChar == 'c') {
      digitalWrite(relay2, LOW); //relay2 on
      lcd.setCursor(4,0);
      lcd.print("R2-N");
      dht_sen();
    }
    else if (inChar == 'd') {
      digitalWrite(relay2, HIGH); //relay2 off
      lcd.setCursor(4,0);
      lcd.print("R2-F");
      dht_sen();
    }
    else if (inChar == 'e') {
      digitalWrite(relay3, LOW); //relay3 on
      lcd.setCursor(8,0);
      lcd.print("R3-N");
      dht_sen();
    }
    else if (inChar == 'f') {
      digitalWrite(relay3, HIGH); //relay3 off
      lcd.setCursor(8,0);
      lcd.print("R3-F");
      dht_sen();
    }
    else if (inChar == 'g' || distance1<10) {
      digitalWrite(fan, LOW); //fan on
      lcd.setCursor(12,0);
      lcd.print("R4-N");
      dht_sen();
    }
    else if (inChar == 'h') {
      digitalWrite(fan, HIGH); //fan off
      lcd.setCursor(12,0);
      lcd.print("R4-F");
      dht_sen();
    }
    /*if (distance1<10)
    {digitalWrite(fan, LOW);
    lcd.setCursor(12,0);
    lcd.print("R4ON");
    dht_sen();
    }*/
  }
  delay(500);
}
