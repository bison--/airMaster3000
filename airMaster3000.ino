#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//-----Hardware Adressing-----
//in case of an not-working LCD switch commenting those 2 lines
//for a different default I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);  // default I2C address of an LCD
//LiquidCrystal_I2C lcd(0x3F,16,2);  // the other default I2C address of an LCD

#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

bool statusLed = false;

// the backslash character is not in the standard char set
// of the LCD so let's redefine it
uint8_t char_backslash_bytes[8] = {
    0b00000,
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00001,
    0b00000,
    0b00000
};

// fire animation frame 1
uint8_t char_fire1_bytes[8] = {
    0b10101,
    0b00100,
    0b01111,
    0b01111,
    0b11010,
    0b10010,
    0b11010,
    0b11010
};

// fire animation frame 2
uint8_t char_fire2_bytes[8] = {
    0b10100,
    0b00100,
    0b11110,
    0b11110,
    0b01011,
    0b01001,
    0b01011,
    0b01011
};

char fireAnimation[] = {1, 2};
int fireAnimationIndex = 0;

char activityAnimation[] = {'/', '-', 0, '|'};
int activityAnimationIndex = 0;

int led_pin_r = 11;
int led_pin_g = 10;
int led_pin_b = 9;

int backlightButton = 12;
bool backlightOn = true;  // not needed, in hardware!

bool warmUpComplete = false;
bool hasShownWelcomeMessage = false;

int line2ValueSwitch = 0;

int temperatureOffset = -20; // default: -25.0

void setup()
{
  // initialize digital pin 13/LED_BUILTIN as an output.
  // the build-in LED will be used to blink and indicate that the sensor is active
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(led_pin_r, OUTPUT);
  pinMode(led_pin_g, OUTPUT);
  pinMode(led_pin_b, OUTPUT);

  //pinMode(backlightButton, INPUT);
  
  Serial.begin(9600);
  Serial.println("starting airMaster 3000");

  // upload the self-defined chars to the LCD
  lcd.init();
  lcd.createChar(0, char_backslash_bytes);
  lcd.createChar(1, char_fire1_bytes);
  lcd.createChar(2, char_fire2_bytes);
  lcd.clear();
  
  if(!ccs.begin())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Failed to start sensor! Please check your wiring.");

    doBacklight();
    lcd.setCursor(0, 0);
    lcd.print("sensor failed!");
    lcd.setCursor(0, 1);
    lcd.print("check wiring");
    
    while(1){delay(100);};
  }
  
  //calibrate temperature sensor
  Serial.println("calibrate temperature sensor");
  
  doBacklight();
  
  lcd.setCursor(0, 0);
  lcd.print("calibrate temp.");
  lcd.setCursor(0, 1);
  lcd.print("sensor");
  
  while(!ccs.available()) {
    blinkStatus();
  }
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp + temperatureOffset);
  ccs.setDriveMode(CCS811_DRIVE_MODE_1SEC);
}

void blinkStatus() {
  // 1. blink the build-in LED to indicate it's still alive
  // 2. animate a spinning thingi to indicate to the end user it's still alive
  // 3. animate a fire to indicate to the end user that the sensor is still warming up
  
  if (statusLed) {
    statusLed = false;
  } else {
    statusLed = true;
  }
  digitalWrite(LED_BUILTIN, statusLed);
  lcd.setCursor(15, 1);

  activityAnimationIndex++;
  if (activityAnimationIndex > 3) {
    activityAnimationIndex = 0;
  }
  
  lcd.print(activityAnimation[activityAnimationIndex]);
  
  // set a bool that the warmup was completed since micros will overflow in 72 hours
  if (!warmUpComplete) {
    // 20 min warmup time
    if (micros() < 1200000000) {
      lcd.setCursor(15, 0);

      fireAnimationIndex++;
      if (fireAnimationIndex > 1) {
        fireAnimationIndex = 0;
      }
      
      lcd.print(fireAnimation[fireAnimationIndex]);
    } else {
      warmUpComplete = true;
    }
  }
}

void clearLine(int rowIndex) {
  lcd.setCursor(0, rowIndex);
  lcd.print("                ");
}

void printLcdLine(String msg, int rowIndex) {
  lcd.setCursor(0, rowIndex);
  lcd.print(msg);
}

/*
 * the scrollDisplay does NOT work as I thought it would
void writeSentenceDelayed(String text, int rowIndex, int delayTime) {
  clearLine(rowIndex);
  lcd.setCursor(0, rowIndex);
  //lcd.autoscroll();
  for (int i = 0; i < text.length(); i++) {
    lcd.print(text[i]);
    lcd.scrollDisplayLeft();
    if (isSpace(text[i])) {
      delay(delayTime);
    }
  }
  //lcd.noAutoscroll();
}
*/

void welcomeMessage() {
  //String msg0 = "                "; // display length sample
    String msg1 = "Hello User      "; // future me: don't forget to change to JujiBla when updating
    String msg2 = "the sensor needs";
    String msg3 = "20m to warm up  ";
    String msg4 = "indicated by:   ";
    String msg5 = "enjoy the       ";
    String msg6 = "airMaster 3000  ";
    String msg7 = "v1.1.1          "; // software version, increment if changed
    
    printLcdLine(msg1, 0);
    delay(3000);
    
    printLcdLine(msg2, 0);
    printLcdLine(msg3, 1);
    delay(6000);
    
    printLcdLine(msg3, 0);
    printLcdLine(msg4, 1);

    lcd.setCursor(14, 1);
    lcd.print(fireAnimation[0]);
    delay(6000);

    printLcdLine(msg5, 0);
    printLcdLine(msg6, 1);
    delay(6000);

    printLcdLine(msg6, 0);
    printLcdLine(msg7, 1);
    delay(6000);
}

void setStatusLedCo2(int co2ppm)
{
  /*
   * the CO2 LED indicators from my bought CO2 sensor
   CO2
      : green
   800: yellow/orange
  1200: red
  */
  
  analogWrite(led_pin_r, 0);
  analogWrite(led_pin_g, 0);
  analogWrite(led_pin_b, 0);
  
  if (co2ppm <= 800) {
    analogWrite(led_pin_g, 10);
  } else if (co2ppm <= 1800) {
    // set to 1800 because my sensor is often a bit higher than the bought one
    // since the LED I used is... not that good I can't mix colors very well, so I use blue instead of yellow/orange
    analogWrite(led_pin_b, 20);
  } else {
    analogWrite(led_pin_r, 20);
  }
}

void setStatusLedTvoc(float tvocppb)
{
  analogWrite(led_pin_r, 0);
  analogWrite(led_pin_g, 0);
  analogWrite(led_pin_b, 0);
  
  if (tvocppb <= 25) {
    analogWrite(led_pin_g, 10);
  } else if (tvocppb <= 50) {
    analogWrite(led_pin_g, 20);
    analogWrite(led_pin_b, 20);
  } else if (tvocppb <= 100) {
    analogWrite(led_pin_b, 20);
  } else if (tvocppb <= 200) {
    analogWrite(led_pin_r, 20);
  } else {
    analogWrite(led_pin_r, 50);
  }
}

void doBacklight() {
  // i trigger the backlight always when writing to the display, otherwise it sometimes went off
  if (backlightOn) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
}

void loop()
{
  // read up: https://learn.sparkfun.com/tutorials/ccs811-air-quality-breakout-hookup-guide/all
  //backlightOn = digitalRead(backlightButton);
  if (ccs.available())
  {
    if (!hasShownWelcomeMessage) {
      hasShownWelcomeMessage = true;
      
      doBacklight();
      lcd.clear();

      welcomeMessage();
    }
    
    float temp = ccs.calculateTemperature();
    if(!ccs.readData())
    {
      line2ValueSwitch++;

      if (line2ValueSwitch >= 9) {
        line2ValueSwitch = 0;
      }
      
      int co2 = ccs.geteCO2();
      float tvoc = ccs.getTVOC();
      Serial.print("CO2: ");
      Serial.print(co2);
      Serial.print("ppm, TVOC: ");
      Serial.print(tvoc);
      Serial.print("ppb   Temp:");
      Serial.println(temp);

      doBacklight();
      lcd.clear();
      
      lcd.setCursor(0, 0);
      lcd.print("CO2: " + String(co2) + " ppm");

      
      // permanent TVOC display
      lcd.setCursor(0, 1);
      lcd.print("TVOC:" + String(int(tvoc)) + " ppb");
      setStatusLedCo2(co2);
      
      /*
      // debug & calibrating with temp
      if (line2ValueSwitch <= 4) {
        lcd.setCursor(0, 1);
        lcd.print("Temp: " + String(temp));
        setStatusLedCo2(co2);       
      } else if (line2ValueSwitch <= 8) {
        lcd.setCursor(0, 1);
        lcd.print("TVOC:" + String(tvoc) + " ppb");
        setStatusLedTvoc(tvoc);
      }
      */
      
      blinkStatus();
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("ERROR!");
      doBacklight();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR! RESTART! ");
      lcd.setCursor(0, 1);
      lcd.print("HALP ME HUMAN!  ");
      while(1){delay(100);};
    }
  }
  delay(1000);
}
