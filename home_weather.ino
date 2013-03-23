#include <Wire.h>
#include <LiquidCrystal_SR3W.h>
#include <DHT22.h>

#define   APP_VERSION    "0.1" 

#define   CONTRAST_PIN   5
#define   DHT22_PIN      6
#define   LDR_PIN        A0 
#define   STATUS_LED_PIN 13
#define   BEEP_PIN       11

#define   CONTRAST       120
#define   MEASUREMENT_DELAY 5000
 
DHT22 myDHT22(DHT22_PIN);
LiquidCrystal_SR3W lcd(2, 3, 4);
int LDRReading;
DHT22_ERROR_t errorCode;

void welcomeScreen()
{
  lcd.blink();
  lcd.setCursor(0, 0);
  lcd.print("Home Weather");

  lcd.setCursor(0, 1);
  lcd.print("Version ");
  lcd.print(APP_VERSION);
  beep(50);
  beep(50);
  beep(50);
 
  // Wait for DHT initilization
  delay(2000);
  lcd.noBlink();
}

void beep(int duration)
{
  analogWrite(BEEP_PIN, 10); 
  delay(duration);
  analogWrite(BEEP_PIN, 0); 
  delay(duration);
}

void setup(void)
{
   Serial.begin ( 9600 );
   pinMode(STATUS_LED_PIN, OUTPUT); 
   pinMode(BEEP_PIN, OUTPUT);
   pinMode(CONTRAST_PIN, OUTPUT);
   analogWrite (CONTRAST_PIN, CONTRAST);   
   lcd.begin ( 16, 2 );
   welcomeScreen();
}

void printMeasurements(void)
{ 
  // Serial output
  Serial.println(LDRReading);   
  Serial.print(myDHT22.getTemperatureC());
  Serial.print("C\t");
  Serial.print(myDHT22.getHumidity());
  Serial.println("%");
  
  // LCD output
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(myDHT22.getTemperatureC());
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print(myDHT22.getHumidity());
  lcd.print(" %");
  
  // Status update
  if(myDHT22.getHumidity() < 45.0){
   digitalWrite(STATUS_LED_PIN, HIGH);        
  }else{       
    digitalWrite(STATUS_LED_PIN, LOW); 
  }  
}

void doMeasurements(void)
{
  errorCode = myDHT22.readData();
  LDRReading = analogRead(LDR_PIN);
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      printMeasurements(); 
      break;
    
    case DHT_ERROR_CHECKSUM:
      lcd.setCursor(0, 0);
      lcd.print("check sum error ");
      break;
    case DHT_BUS_HUNG:
      lcd.setCursor(0, 0);
      lcd.print("BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      lcd.setCursor(0, 0);
      lcd.print("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      lcd.setCursor(0, 0);
      lcd.print("ACK time out ");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      lcd.setCursor(0, 0);
      lcd.print("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      lcd.setCursor(0, 0);
      lcd.print("Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      lcd.setCursor(0, 0);
      lcd.print("Polled to quick ");
      break;
  }
}


void loop(void)
{   
  doMeasurements(); 
  delay(MEASUREMENT_DELAY);
}


