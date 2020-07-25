#include <OneWire.h>
#include <DS18B20.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
const int plus = 8, minus = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define ONE_WIRE_BUS 2
#define RELAYPIN 10

OneWire oneWire(ONE_WIRE_BUS);
DS18B20 sensor(&oneWire);

uint32_t start, stop;

float targetTemp = 56;
const float err = 0.2;

void setup()
{
  lcd.begin(16, 2);
  sensor.begin();
  sensor.setResolution(12);
  sensor.requestTemperatures();

  start = millis();
  while (!sensor.isConversionComplete());
  stop = millis();

  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);


pinMode(plus, INPUT_PULLUP);
pinMode(minus, INPUT_PULLUP);
}

float temperature = 0;

bool checkButtons(){
       
  int sensorVal = digitalRead(plus);
  //print out the value of the pushbutton

  if (sensorVal == 0){
     targetTemp+=0.1;
     lcd.setCursor(0,1);
     lcd.print("Target: " + String(targetTemp, 1) + "C"); 
     delay(100);
     return true;
  } else if  (( sensorVal = digitalRead(minus)) == 0){
     targetTemp-=0.1;
     lcd.setCursor(0,1);
     lcd.print("Target: " + String(targetTemp, 1) + "C"); 
     delay(100);
     return true;
  } 

  return false;
}


void loop()
{  
  
  lcd.clear();
    
  lcd.setCursor(0,0);
  lcd.print("Temp:   " + String(temperature, 1) + "C");
  lcd.setCursor(0,1);
  lcd.print("Target: " + String(targetTemp, 1) + "C");      

  if(temperature > targetTemp){
    digitalWrite(RELAYPIN, HIGH);
    lcd.setCursor(15,0);
    lcd.print("0");
  }
    
  else if (temperature < targetTemp - err){
    digitalWrite(RELAYPIN, LOW);
    lcd.setCursor(15,0);
    lcd.print("1");
  }
  
  if(checkButtons())
    return;

  for (int res = 9; res < 13; res++)
  {
    sensor.setResolution(res);

    start = millis();
    sensor.requestTemperatures();
    while (!sensor.isConversionComplete())
      if(checkButtons())
        return;
    temperature = sensor.getTempC();
    stop = millis();

    //Serial.print(res);
    //Serial.print("\t");
    //Serial.print(stop - start);
    ///Serial.print("\t");
    //Serial.println(temperature, 1); // 1 decimal makes perfect sense
  }
}
