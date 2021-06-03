#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#define sensor 8
#define greenLed 13
#define redLed 12
#define buz 9
#define MAX 0.7

LiquidCrystal_I2C lcd(0x27,20,4);
// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  5, 10, 10, 5, 5, 5, 5, 5
};
float adcValue=0;
float v = adcValue * (5.0/1024.0);
float mgL = 0.67 * v;
float airValue = 0;

ISR(TIMER1_COMPA_vect) {
  adcValue = analogRead(sensor);
  v = adcValue * (5.0/1024.0);
  mgL = 0.67 * v;
}

void playSong() {

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(9, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(9);

  }
}
void setup() 
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Etilotest pornit ");
  pinMode(sensor, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  lcd.clear();
  lcd.print("Se calibreaza... ");
  delay(10000);
  lcd.clear();
  
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  
  OCR1A = 31249;            // compare match register 16MHz/256/2Hz-1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  
  sei();

  
}

void printState() {
    lcd.setCursor(0,0);
    lcd.print("Conc: ");
    lcd.print(mgL,4);
    lcd.print(" mg/L        ");
    lcd.setCursor(0,1);
}

void loop() 
{
    if(mgL > MAX)
    { 
      printState();
      lcd.print("Prea mult   ");
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      playSong();
    }
    else
    {
      printState();
      lcd.print("Normal   ");
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    }
}
