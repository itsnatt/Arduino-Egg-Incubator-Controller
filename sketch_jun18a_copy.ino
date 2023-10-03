//inisiasi Liblary
// 1 lcd
#include <LiquidCrystal.h>
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//2 Rotary encoder
#define CLK 9
#define DT 8
#define SW 7

int counter2 = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

//3 dht 11
#include "DHT.h"
#define DHTPIN 6   
#define DHTTYPE DHT11   
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float t;
float h;
float f;
float hic;
float hif;
//

//4 rtc
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//5 relay
int kipas = A0;
int lamp  = A1;
int hum   = A2; 

//6 servo
#include <Servo.h>
Servo myservo;
int pos;

//var
int max_hum = 0;
int max_temp = 0;
int min_hum = 0;
int min_temp = 0;
int day = 0;
int day1 = 12;
int xx = 1;




void setup() 
{
  //servo
  
  myservo.attach(10);
  //serial
  Serial.begin(9600);
  // LCD
  lcd.begin(16, 2);

  // Set encoder pins as inputs
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);
 //relay
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  //
  //dht
  dht.begin();

  //rtc
 while (!Serial); // for Leonardo/Micro/Zero
 if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
 }
 if (! rtc.isrunning()) {
   Serial.println("RTC is NOT running!");
   // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // This line sets the RTC with an explicit date & time, for example to set
   // January 21, 2014 at 3am you would call:
   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 }




}

void loop() 
{
   DateTime now = rtc.now();
 /*  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" day =    ");
  Serial.print(day);
  Serial.println();
  //delay(5000);*/
if ((now.second() == day1 ) && xx == 1 )
  {
   day ++;
   xx = 0;
   if(pos == 0)
   {
      for (pos = 0; pos <= 180; pos += 1) 
       { 
         myservo.write(pos);        
          delay(15);                       
        }
   }
    else
    {
      for (pos = 180; pos >= 0; pos -= 1)
      { 
      myservo.write(pos);              
      delay(15);                       
      }

    }
     delay(1000);
  }
 else
 {
   xx = 1;
 }
  
if (day > 4)
  {
    digitalWrite(A2, HIGH);
  }
else
  { 
    digitalWrite(A2, LOW);
  }

  dht1();
  button1();
  var1();
switch (counter2) {
  case 1:
    menu1();

    break;
  case 2:
    lcd.setCursor(13, 0);
    lcd.print("    ");
    lcd.setCursor(13, 1);
    lcd.print("    ");
    menu2();
    break;
  case 3:
     menu3();
    break;
  case 4:
    menu4();
    break;
  case 5:
    menu5();
    break;
  case 6:
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("SAVING");
    lcd.setCursor(0, 0);
    lcd.print("SAVING.");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING..");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING...");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING....");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING.....");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING......");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING.......");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING........");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING.........");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("SAVING..........");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("                ");
   counter2 = 1;
    break;

  default:
    // if nothing else matches, do the default
    counter2 = 1;
    break;
}
}
/////

void var1()
{
  
   
// temp 
if(t > max_temp)  
  { 
     digitalWrite(A0, LOW);
  } 
if(t < min_temp)  
  {
     digitalWrite(A0, HIGH);
  } 
if(h > max_hum)  
  { 
     digitalWrite(A1, LOW);
  } 
if(h < min_hum)  
  {
     digitalWrite(A1, HIGH);
  }

/*else if((t / 10) < min_temp )
  {
    digitalWrite(A0, HIGH);
  }
else if((h / 10) > max_hum )  
  {
    digitalWrite(A1, LOW);
  }
else if((h / 10) < min_hum )
  {
    digitalWrite(A1, HIGH);
  }
*/
//
//day






}
void button1()
{
  int lastStateSW;
	lastStateSW = digitalRead(SW);
  if (lastStateSW == LOW) // light the LED
  {
  counter2 ++;
  delay(600);
  }
}

void dht1()
{
  //dht
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
   return;
  }
  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(t, h, false);
  // dht
}

void menu1()
{
  
  lcd.setCursor(0, 0);
    lcd.print("Temp = ");
     lcd.setCursor(6, 0);
    lcd.print(t);

    lcd.setCursor(0, 1);
    lcd.print("hum  = ");
     lcd.setCursor(6, 1);
    lcd.print(h);

    lcd.setCursor(13, 0);
    lcd.print("Day");
     lcd.setCursor(14, 1);
    lcd.print(day);
  Serial.print(F("Humidity: "));
  Serial.println(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
}

void menu2()
{
   lcd.setCursor(0, 0);
    lcd.print("Max Temp = ");
    lcd.setCursor(11, 0);
    lcd.print(	max_temp);
  	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);
	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			max_temp --;
		} else {
			// Encoder is rotating CW so increment
			max_temp ++;
		}
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;
}

void menu3()
{
  	lcd.setCursor(0, 1);
    lcd.print("Min Temp = ");
    lcd.setCursor(11, 1);
    lcd.print(	min_temp);
	currentStateCLK = digitalRead(CLK);
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
			min_temp --;
		} else {
			min_temp ++;
		}
	}
	lastStateCLK = currentStateCLK;
}

void menu4()
{
   lcd.setCursor(0, 0);
    lcd.print("Max Hum = ");
    lcd.setCursor(11, 0);
    lcd.print(max_hum);
	currentStateCLK = digitalRead(CLK);
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
			max_hum --;
		} else {
			max_hum ++;
		}
	}
	lastStateCLK = currentStateCLK;
}

void menu5()
{
    lcd.setCursor(0, 1);
    lcd.print("Min hum = ");
    lcd.setCursor(11, 1);
    lcd.print(	min_hum);
	currentStateCLK = digitalRead(CLK);
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
			min_hum --;
		} else {
			min_hum ++;
		}
	}
	lastStateCLK = currentStateCLK;
}


