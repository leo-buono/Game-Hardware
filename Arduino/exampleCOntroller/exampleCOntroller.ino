/*
 * This example turns the ESP32 into a Bluetooth LE gamepad that presses buttons and moves axis
 *
 * At the moment we are using the default settings, but they can be canged using a BleGamepadConfig instance as parameter for the begin function.
 *
 * Possible buttons are:
 * BUTTON_1 through to BUTTON_16
 * (16 buttons by default. Library can be configured to use up to 128)
 *
 * Possible DPAD/HAT switch position values are:
 * DPAD_CENTERED, DPAD_UP, DPAD_UP_RIGHT, DPAD_RIGHT, DPAD_DOWN_RIGHT, DPAD_DOWN, DPAD_DOWN_LEFT, DPAD_LEFT, DPAD_UP_LEFT
 * (or HAT_CENTERED, HAT_UP etc)
 *
 * bleGamepad.setAxes sets all axes at once. There are a few:
 * (x axis, y axis, z axis, rx axis, ry axis, rz axis, slider 1, slider 2)
 *
 * Library can also be configured to support up to 5 simulation controls
 * (rudder, throttle, accelerator, brake, steering), but they are not enabled by default.
 *
 * Library can also be configured to support different function buttons
 * (start, select, menu, home, back, volume increase, volume decrease, volume mute)
 * start and select are enabled by default
 */

#include <Arduino.h>
#include <BleGamepad.h>

#define LED 12

//4 face button pins (Or I guess back buttons in this case)
#define A_Button 13
#define B_Button 16 
#define X_Button 14
#define Y_Button 27

//Start and Select
#define Start_Button 33
#define Select_Button  32

//Shift Button
#define Shift_Button 04

//LB and RB
#define Left_Bumper 25
#define Right_Bumper 26

//Analog Stick
#define X_Stick 34//34 //INPUT ONLY GPIO PINS!!
#define Y_Stick 35//35 //INPUT ONLY GPIO PINS!!

#define Left_Trigger 36 //INPUT ONLY GPIO PINS!!
#define Right_Trigger 39 //INPUT ONLY GPIO PINS!!

#define ANALOG_FACTOR 8


const uint16_t faceButtons[] = {A_Button, B_Button, X_Button, Y_Button};
//A,B,X,Y,UP,DOWN,LEFT,RIGHT
const uint16_t allButtons[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, HAT_DOWN, HAT_RIGHT, HAT_LEFT, HAT_UP};

BleGamepad bleGamepad("SoftWHERE Controller");

BleGamepadConfiguration bleGamepadConfig;

void setup()
{
    Serial.begin(115200);
    
    //Face (Back of device) Buttons
    pinMode(A_Button, INPUT_PULLUP);
    pinMode(B_Button,  INPUT_PULLUP);
    pinMode(X_Button,  INPUT_PULLUP);
    pinMode(Y_Button,  INPUT_PULLUP);

    // //Start/Select
    pinMode(Start_Button,  INPUT_PULLUP);
    pinMode(Select_Button,  INPUT_PULLUP);

    // //Shift button
    pinMode(Shift_Button,  INPUT_PULLUP);
    
    // //Bumpers
    pinMode(Left_Bumper,  INPUT);
    pinMode(Right_Bumper,  INPUT);    

    pinMode(X_Stick, INPUT);
    pinMode(Y_Stick, INPUT);
    pinMode(Left_Trigger, INPUT);
    pinMode(Right_Trigger, INPUT); 
    pinMode(LED, OUTPUT);   
  
    bleGamepadConfig.setAxesMin(0x8001); // -32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal  
    bleGamepadConfig.setAxesMax(0x7FFF); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);    
    bleGamepadConfig.setHatSwitchCount(1);  


    Serial.println("Starting BLE work!");
    bleGamepad.begin(&bleGamepadConfig);
    // The default bleGamepad.begin() above enables 16 buttons, all axes, one hat, and no simulation controls or special buttons
}
void loop()
{
  //LOW MEANS BUTTON PRESSED, HIGH MEANS BUTTON RELEASED

    if (bleGamepad.isConnected())
    {      
      //float stick[4] = {}; 
      float stick[4] = {analogRead(X_Stick), analogRead(Y_Stick), analogRead(Left_Trigger), analogRead(Right_Trigger)};
      uint8_t shift = digitalRead(Shift_Button); 
      //uint8_t shift = 0; 
      ////ANALOG STICK     
      //bleGamepad.setAxes(stick[0] * !shift * ANALOG_FACTOR, stick[1] * !shift * ANALOG_FACTOR, 0, stick[0] * shift * ANALOG_FACTOR, stick[1] * shift * ANALOG_FACTOR, 0, stick[2] * ANALOG_FACTOR, stick[3] * ANALOG_FACTOR);
      //bleGamepad.setAxes(stick[0] * !shift, stick[1] * !shift, 0, stick[0] * shift, stick[1] * shift, 0, 0, 0);
      bleGamepad.setAxes(2048, 2048, 2048, 2048, stick[2] * ANALOG_FACTOR, stick[3] * ANALOG_FACTOR, 0,0);    

      //Face Buttons and Dpad
      if(shift == HIGH)
      {
        digitalWrite(LED, LOW);                   
       for(int i = 0; i < 4; i++)
       {
          if(digitalRead(faceButtons[i]) == LOW) 
          {
            bleGamepad.press(allButtons[i]);
          }
          else
          {
            bleGamepad.release(allButtons[i]);
          }
       }
      }
      else
      {
        digitalWrite(LED, 255);
        for(int i = 0; i < 4; i++)
        { 
          if(digitalRead(faceButtons[i]) == LOW) 
          {
            bleGamepad.setHat1(allButtons[i + 4]);
          }
          else
          {
            bleGamepad.setHat1(HAT_CENTERED);
          }
        }
      }  
      


      // //Start and Select
      if(digitalRead(Start_Button) == HIGH)
      {
        bleGamepad.press(BUTTON_9);
      }  
      else
      {
        bleGamepad.release(BUTTON_9);        
      }
      if(digitalRead(Select_Button) == HIGH)
      {
        bleGamepad.press(BUTTON_10);
      }  
      else
      {
        bleGamepad.release(BUTTON_10);        
      } 





      if(digitalRead(Right_Bumper) == HIGH)
      {
        bleGamepad.press(BUTTON_5);
      }  
      else
      {
        bleGamepad.release(BUTTON_5);        
      }
      if(digitalRead(Left_Bumper) == HIGH)
      {
        bleGamepad.press(BUTTON_6);
      }  
      else
      {
        bleGamepad.release(BUTTON_6);        
      }   


        //  Serial.println("Press buttons 5, 16 and start. Move all enabled axes to max. Set DPAD (hat 1) to down right.");
        //  bleGamepad.press(BUTTON_5);
        //  bleGamepad.press(BUTTON_16);
        //  bleGamepad.pressStart();
        //  bleGamepad.setAxes(32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767);
        //  bleGamepad.setHat1(HAT_DOWN_RIGHT); 
        //  // All axes, sliders, hats etc can also be set independently. See the IndividualAxes.ino example
        //  delay(500);

        //  Serial.println("Release button 5 and start. Move all axes to min. Set DPAD (hat 1) to centred.");
        //  bleGamepad.release(BUTTON_5);
        //  bleGamepad.releaseStart();
        //  bleGamepad.setHat1(HAT_CENTERED);
        //  bleGamepad.setAxes(0, 0, 0, 0, 0, 0, 0, 0);
        //  delay(500);      
    }
}