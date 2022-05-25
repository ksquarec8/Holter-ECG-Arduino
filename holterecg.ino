#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include<time.h>
File myFile;
RTC_DS3231 rtc;
int hours=0,minutes=0,months=1,days=1;
SoftwareSerial BT(0, 1); 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int selected = 0;
int entered = -1;
void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  BT.begin(9600);
  rtc.begin();
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7, INPUT); // Setup for leads off detection LO +
pinMode(8, INPUT); // Setup for leads off detection LO -
  Serial.begin(9600);
SD.begin(10);
  }
void displaymenu(void) {
int up = digitalRead(2);
int enter = digitalRead(3);
int back = digitalRead(4);
 if (up == LOW) {if(selected>2){
    selected=0;
  }else
    selected = selected + 1;
    delay(200);
  };
 
  if (enter == LOW) {
    entered = selected;
  };
  if (back == LOW) {
    entered = -1;
  };
  const char *options[4] = {
    "1.Real Time ECG",
    "2.Holter Mode",
    "3.Edit Time",
    "4.Edit Date ",};
   

  if (entered == -1) {
    DateTime now = rtc.now();
  char buffer [16];
 sprintf (buffer, "D:%02d/%02d-T=%02d:%02d", now.month(),now.day(),now.hour(),now.minute());
    
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(buffer);
        lcd.setCursor(0,1);
        lcd.print(options[selected]);
       delay(500);
        
    
  

   
  } else if (entered == 0) {
 RealTime();
  } else if (entered == 1) {
    Holter();
  }
  else if (entered == 2) {
    EditTime();
  } else if (entered == 3) {
    EditDate();
  }
}



void loop()
{
  displaymenu();
}
void Holter(){
    lcd.clear();
     lcd.setCursor(0,1);
     lcd.print("Storing your Data");
  DateTime now = rtc.now();
  char buffer [16],times[16];
 sprintf (buffer, "%02d-%02d.txt", now.month(),now.day());
 myFile = SD.open(buffer, FILE_WRITE);
if (myFile) {    
  myFile.print("time");
  myFile.print(",");
  myFile.print("data");
  myFile.println("");
   myFile.close();
}
 while(1){
myFile = SD.open(buffer, FILE_WRITE);
if (myFile) {    
  
 myFile.print(now.timestamp(DateTime::TIMESTAMP_TIME));
    myFile.print(",");  
if((digitalRead(7) == 1)||(digitalRead(8) == 1)){
  myFile.print('0');}else{
   myFile.print(analogRead(A0)); 
   }
   myFile.println("");
   myFile.close();
   
   }
   if(digitalRead(4)==0){
    break;
   }
    delay(1);// close the file
    }}
void RealTime(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("connecting to");
   lcd.setCursor(0,1);
  lcd.print(" your PC");
  while(1){
if((digitalRead(7) == 1)||(digitalRead(8) == 1)){
BT.println('0');
Serial.println('0');
}
else{
BT.println(analogRead(A0));
Serial.println(analogRead(A0));
}

delay(1);
if(digitalRead(4)==LOW){
  break;
}
}}
void EditTime(){
 int clkAdd= digitalRead(6);//twobuttons for clock setting
  int clkShift=digitalRead(5);//two buttons for clock setting
DateTime now = rtc.now();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DateTime Edit");
 if(clkAdd==LOW){
  if(hours>=23){
    hours=0;
    delay(200);
  }else{
    hours++;
    delay(200);
  }
 };
 if(clkShift==LOW){
  if(minutes>=59){
    minutes=0;
    delay(200);
    }else{
      minutes++;
      delay(200);
  }
 }
  char buffer [10];  
  sprintf (buffer, "T=%d:%d", hours, minutes);
 lcd.setCursor(0,1);
 lcd.print(buffer);
 delay(100);
 
 rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, minutes, now.second()));
}
void EditDate(){
  int clkAdd= digitalRead(6);//twobuttons for clock setting
  int clkShift=digitalRead(5);//two buttons for clock setting
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Edit Date");
delay(300);
 DateTime now = rtc.now();
  
 
 if(clkAdd==LOW){
  if(months>=12){
    months=1;
    delay(200);
  }else{
    months++;
    delay(200);
  }
 };
 if(clkShift==LOW){
  if(days>=32){
    days=1;
    delay(200);
    }else{
      days++;
      delay(200);
  }
 }
  char buffer [16];  
  sprintf (buffer, "Date=%d/%d", months, days);
 lcd.setCursor(0,1);
 lcd.print(buffer);
 delay(300);
  if(digitalRead(3)==LOW);
  rtc.adjust(DateTime(now.year(),months, days, now.hour(), now.minute(),now.second()));
  }
