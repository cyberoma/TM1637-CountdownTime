// DEFINES
#define numberOfSeconds(_time_) ((_time_ / 1000) % 60)  
#define numberOfMinutes(_time_) ((_time_ / 1000) / 60) 
#define outputA 6
#define outputB 7
#define startButton 5
#define resetButton 8
#define buzzer 9
// INCLUDES
// https://github.com/avishorp/TM1637
#include <TM1637Display.h>

// CONSTANTS
const int onDuration=1000;
const int periodDuration=6000;
const uint8_t OFF[] = {0, 0, 0, 0};
// In this library, the byte order is .GFEDCBA
const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};
// GLOBALS
// Create a display object, specifying parameters (Clock pin, Data pin)
TM1637Display display(2, 3);

// 1000ms in 1sec, 60secs in 1min, 60mins in 1hr. So, 1000x60x60 = 3600000ms = 1hr
unsigned long timeLimit;
unsigned long lastPeriodStart;
int stop;
int start;
//Variables for Rotary Encoder
int aState;
int aLastState;

void setup(){
  Serial.begin(9600);
  // Set brightness
  display.setBrightness(0x0c);
  // Clear the display
  display.setSegments(OFF);
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode(startButton, INPUT_PULLUP); 
  pinMode(resetButton, INPUT_PULLUP); 
  pinMode(buzzer, OUTPUT);
  aLastState = digitalRead(outputA);
    
  stop = 0;
  start = 0;
 
}
// -----  MENU ------
void menu() {
   unsigned long runTime;
   //Clear the display
   display.setSegments(OFF);
   timeLimit = 1;
  
   while(start == 0){
    display.showNumberDecEx(timeLimit, 0, true, 2, 2);
    aState = digitalRead(outputA);
   //Change Timer with Rotary Encoder
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
      timeLimit++;
     } 
     else {
      //timeLimit--;
     }
    
   } 
   // Updates the previous state of the outputA with the current state
   aLastState = aState; 
  //Check Start button - If Button pressed countdown starts..
  int sensorVal = digitalRead(startButton);
  if(sensorVal == HIGH){}
  else {
    start = 1;
  }
  
   }
  //Add the runtime until timer starts to timeLimit
  runTime = millis();
  timeLimit = timeLimit*1000*60 + runTime;
}
void countdown() {
  // Calculate the time remaining 
 
  unsigned long timeRemaining = timeLimit - millis();
   Serial.println("Countdown started..: ");
   
   
     Serial.println(timeRemaining);
    // To display the countdown in mm:ss format, separate the parts
    int seconds = numberOfSeconds(timeRemaining);
    int minutes = numberOfMinutes(timeRemaining); 
  
    // This displays the seconds in the last two places
    display.showNumberDecEx(seconds, 0, true, 2, 2);
    // Display the minutes in the first two places, with colon
    display.showNumberDecEx(minutes, 0x80>>3, true, 2, 0);

    // Update the time remaining
    timeRemaining = timeLimit - millis();
    if(timeRemaining < 50){
      Serial.println("BREAK!!");
      stop = 1;
      start = 0;

    }
  }
void displayText() {
  display.setSegments(SEG_DONE);
  //delay(2000);
 
 if (millis()-lastPeriodStart>=periodDuration)
  {
    lastPeriodStart+=periodDuration;
    tone(buzzer,550, onDuration); // play 550 Hz tone in background for 'onDuration'
  }
}
void loop(){


 if(start == 0 && stop == 0) {
   menu();
 }
  if(start == 1 && stop == 0) {
    countdown();
  }
  if(stop == 1 && start == 0){
    displayText();
  }
  //Go back to menu
  int resetVal = digitalRead(resetButton);
  if(resetVal == HIGH){}
  else{stop = 0; start = 0;}

  
  
}
