// Author: Emma Halferty
// Net ID: emmahalferty
// Date: 3/18/2024
// Assignment: Lab 4
// lab partners: Maria Arvizu Velazquez, Alireza Shariati, Jerry Myers

/*1. Two state machines are used for debouncing the switch and for 
implementing the display of the 8x8 LED smiley or frowny face. 
2. If the accelerometer movement reaches a defined threshold value, a 
piezo alarm will trigger and produce a chirping sound. Your group will 
determine the threshold value to trigger the piezo through 
experimentation.
3. Once the piezo alarm is triggered it will remain on until it is 
silenced by pressing a button switch.
4. The x,y and z data from the accelerometer will be read out in the 
while loop in main()
- Read the datasheet and obtain necessary register numbers (hex)
- Initialize I2C process
- sei() <- make sure this is in there so that Serial.println works
- Start the I2C transmission to the SLA
- Write to power management -> write wakeup command (all zeros)
- Inside the while(1)
• Read from X high register -> Read from X low register
• Read from Y high register -> Read from Y low register
• Read from Z high register -> Read from Z low register
• Combine registers and serial print information
• Stop the I2C trans*/

#include <avr/io.h>
#include <Arduino.h>
#include "i2c.h"
#include "pwm.h"
#include "spi.h"
#include "timer.h"
#include "switch.h"
#include <avr/interrupt.h>

#define SLA               0x68  // MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT          0x6B
#define WAKEUP            0x00
#define ACCEL_CONFIG      0x1C
#define AFS_SEL_2         0x00
#define SL_MEMA_XAX_HIGH  0x3B
#define SL_MEMA_XAX_LOW   0x3C
#define SL_MEMA_YAX_HIGH  0x3D
#define SL_MEMA_YAX_LOW   0x3E
#define SL_MEMA_ZAX_HIGH  0x3F
#define SL_MEMA_ZAX_LOW   0x40
#define SL_TEMP_HIGH      0x41
#define SL_TEMP_LOW       0x42
#define SL_WHO_AM_I       0X75

int minVal=265;
int maxVal=402;

/*
 * Defining a set of states that can be used in the state machine using an enum.
 */
// creating 4 states for switch debounce
typedef enum stateType_enum{
  wait_press, debounce_press, wait_release, debounce_release
} Button_State;

// alarm states
typedef enum {
  alarm_OFF, alarm_ON
} Alarm_State;

// alarm states continued - for when button is pressed
typedef enum {
  alarm_DISABLED, alarm_ENABLED
} Alarm_Enable;

// state for sound of buzzer
typedef enum {
  sound_ON, sound_OFF
} Sound_State;

// set push button state to wait_press state initially
// volatile type so it can be chaned in ISR
volatile Button_State pbstate = wait_press;
volatile Alarm_State alarm = alarm_OFF;
volatile Sound_State buzzer = sound_OFF;
volatile Alarm_Enable alarm_enable = alarm_ENABLED;

int main(){

  Serial.begin(9600);
  // Enable global interrupts
  sei();

  int16_t xRot;
  int16_t yRot;
  int16_t zRot;

  // Initialize components
  initI2C();
  StartI2C_Trans(SLA);
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP); // send data to Wake up from sleep mode
  StopI2C_Trans();

  initSwitchPB3();
  initPWMTimer3();

  // initialize 8 x 8 LED array (info from MAX7219 datasheet)
  initMatrix();
  ledSimleyFace(); // start w/ smiley

  while(1) {
    // alarm logic
    switch(alarm) {
      case alarm_OFF:
        soundOff();
        break;
      case alarm_ON:
      // when alarm is on but button is not pressed - turn on the buzzer
        if(alarm_enable == alarm_ENABLED) {
          soundOn();
          if(buzzer == sound_ON){
            //change pitch for chirp
            // randomly step changing pitch for testing purposes to produce chirp
            for(int i = 0; i < 10; i++){
              pitch(1000-i*200);
              delayms(10);
            }
            buzzer = sound_OFF; // change state of sound after
          }
          else{ // if button pressed (alarm disabled) -> keep sound off as interrupt 
            soundOff();
            buzzer = sound_ON; //reset state of buzzer
          }
        } else { // else the alarm is disabled and we have to keep sound off while button is pressed
          soundOff();
          break;
        }
        break;
      default:
        break;
    }

    // Debounce button logic
        switch(pbstate){
        case wait_press: // waiting for button to be pressed
          break;
        case debounce_press: // when button is pressed - debounce
          delayms(100);
          alarm = alarm_OFF;
          alarm_enable = alarm_DISABLED;
          pbstate = wait_release;
          break;
        case wait_release: // wait for button release
          break;
        case debounce_release: // debounce button release action
          delayms(100);
          pbstate = wait_press;
          break;
    }
    
    // reading rotation values from accelerometer module
    xRot = readXRot();
    yRot = readYRot();
    zRot = readZRot();

// for converting to the angle of the rotation
    int xAng = map(xRot,minVal,maxVal,-90,90);
    int yAng = map(yRot,minVal,maxVal,-90,90);
    int zAng = map(zRot,minVal,maxVal,-90,90);

    double x;
    double y;
    double z;

    x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
    y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  // printing to serial monitor
    Serial.print("XRot: ");
    Serial.println(xRot);

    Serial.print("YRot: ");
    Serial.println(yRot);

    Serial.print("ZRot: ");
    Serial.println(zRot);
    Serial.println("\n\n");

    // logic for displaying smiley or frowny face based on rotation
    if((z < 45) | (y < 45)){
      ledSadFace();
      alarm = alarm_ON;
    } else {
      alarm_enable = alarm_ENABLED;
      ledSimleyFace();
    }
    delayms(30); 
  }  
  
  return 0;
}

ISR(PCINT0_vect){
  // If it is in a waiting state, change it to the debounce state when the button is pressed
  cli(); // stop interrupts 
  
  if (pbstate == wait_press) {
   // Serial.println("in button wait_press"); -- for debugging purposes
    pbstate = debounce_press;
  }

// else if interrupt triggered during wait_release state, then debounce the release action
// have to check led speed first and change it then set to debounce_release state
  else if (pbstate == wait_release) {
    pbstate = debounce_release; // update state after button release
  }
  sei(); // re-enable interrupts
}