#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
 

// Constants
const int PULSE_SENSOR_PIN = 0;  // Analog PIN where the PulseSensor is connected
const int LED_PIN = 13;          // On-board LED PIN
const int THRESHOLD = 550;       // Threshold for detecting a heartbeat

SoftwareSerial ss(4,3);
// Create PulseSensorPlayground object
PulseSensorPlayground pulseSensor;
 
int digitalPin = 7;   // KY-037 digital interface
int analogPin = A1;   // KY-037 analog interface
int ledPin = 13;      // Arduino LED pin
int digitalVal;       // digital readings
int analogVal; 

int push_btn_pin = 11;
int pushBTN_state;


void setup()
{
  // Initialize Serial Monitor
  Serial.begin(9600);
  ss.begin(9600);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  pinMode(digitalPin,INPUT); 
  pinMode(analogPin, INPUT);
  pinMode(ledPin,OUTPUT);
 
  // Configure PulseSensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);

  pinMode(push_btn_pin, INPUT);
 
  // Check if PulseSensor is initialized
  if (pulseSensor.begin())
  {
    Serial.println("PulseSensor object created successfully!");
  }
}
 
void loop()
{
  while (ss.available() > 0){
  // get the byte data from the GPS
    byte gpsData = ss.read();
    Serial.println(gpsData);
  }

  digitalVal = digitalRead(digitalPin); 
  pushBTN_state = digitalRead(push_btn_pin);
  Serial.println(pushBTN_state);
  
  if(digitalVal == HIGH) 
  {
    digitalWrite(ledPin, HIGH); // Turn ON Arduino's LED
  }
  else
  {
    digitalWrite(ledPin, LOW);  // Turn OFF Arduino's LED
  }
  

  // Read analog interface
  analogVal = analogRead(analogPin);
  // Print analog value to serial
  //Serial.println(analogVal);
  //delay(100);

  // Get the current Beats Per Minute (BPM)
  int currentBPM = pulseSensor.getBeatsPerMinute();
 
  // Check if a heartbeat is detected
  if (pulseSensor.sawStartOfBeat())
  {
    Serial.println("A HeartBeat Happened!");
    Serial.print("BPM: ");
    Serial.println(currentBPM);

    
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("BPM: ");
      lcd.print(currentBPM);
      
  }

  if(analogVal>=200 && currentBPM>=100)
    {
      lcd.setCursor(0,0);
      lcd.print("R u in danger?");
      if (pushBTN_state == 1)
      {
        lcd.clear();
      }
      else{
        delay(15000);
        lcd.clear();
        lcd.print("Calling a police");
      }
      
    }
 
  // Add a small delay to reduce CPU usage
  delay(20);
}