/*
    Hack AMP - Copyright 2013 David L Norris
        Hardware:
            2 x Toshiba TDA2003 10W Audio Amp with Crossover Network
            DS3930 Hex Digital Potentiometer - Pair of 3 Wipers with common ends
            LCD 16x2 with Backlight
            Rotary Encoder with Pushbutton
            FTDI USB Serial Controller
            ATMEGA328P Microcontroller
 */

// LCD: Backlight (PWM), D4-D7, Register, Enable
#define LCD_BK 5
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 10
#define LCD_D7 9
#define LCD_RS 12
#define LCD_EN 11

// Rotary Encoder: Encoder, Pushbutton
#define ENC_R 3
#define ENC_L 2
#define ENC_B A0

#define UBRRH

#include "HackAmp.h"
#include <LiquidCrystal.h>
#include <Button.h>
#include <TicksPerSecond.h>
#include <RotaryEncoderAcceleration.h>

// Init LCD display pins
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// Init encoder pins
RotaryEncoderAcceleration enc(ENC_L, ENC_R);

// Init encoder position
long position = 1;

// setup the hardware so nothing fries or scares people
void setup() {
  // initialize the serial port; ignore Eclipse errors.
  Serial.begin(9600);

  // initialize the encoder button pin
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, 0);

  // initialize the rotary encoder pins, limit range from 1 to 256, set position (
  // @TODO: read from TWI eeprom
  enc.setMinMax(1, 256);
  enc.setPosition(16);

  // set up the LCD's number of columns and rows, disable text, do boot routine
  lcd.begin(16, 2);
  lcd.noDisplay();
  lcdBoot();
}

// main execution loop
void loop() {
  // read the encoder
  encoderRead();

  // light the led if the button is pressed
  if( buttonRead() == 1 ){
    digitalWrite(13,255);
  }
  else {
    digitalWrite(13,0);
  }
}


// Brightness: 0 = off, 4 = 50%, 8 = on
void lcdBright(int bright){
  // Set boundaries
  if( bright > 8 ) bright = 8;
  if( bright < 0 ) bright = 0;
  // invert the brightness level so higher is brighter
  analogWrite(LCD_BK, ( 255 - ( ( bright * 32 ) - 1 ) ) );
}

// Write to the entire screen with one function
void lcdWrite(const char* line1, const char* line2){
   // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// bring up the lights dramatically
void lcdAnimateBacklight(void){
  for( int i = 0; i < 9; i++ ){
      lcdBright(i);
      delay(125);
  }
}

void lcdBoot(void){
  // Bring up the lights
  lcdAnimateBacklight();

  // Write Boot Message
  lcdWrite(" HackAMP   1.0   ", "8888888888888888");
  lcd.display();
}

void mixerSet(int channel, long level){
  char line1[16];
  char line2[16];

  sprintf(line1, "Volume: %#7lu ", ((level - (level % 16) ) / 16) );
  lcdWrite(line1,"");
}

// is the button pressed?
bool buttonRead(void){
   if(analogRead(A0) > 512){
       return(true);
   }
   else {
       return(false);
   }
}

void encoderRead(void){
  enc.update();
  long newPos = enc.getPosition();

  // save new position if the encoder moved
  if (newPos != position) {
    position = newPos;
    mixerSet(1, position);
  }
}

