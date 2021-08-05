#include <HID-Project.h>

/**
 * Pins:
 * 2 - Encoder 12
 * 3 - Encoder 13
 * 4 - Button Encoder 11
 * 
 * 5 - Button 5
 * 6 - Button 9
 * 7 - Button 6
 * 8 - Button 1
 * 9 - Button 2
 * 10 - Button 3
 * 16 - Button 4
 * 14 - Button 7
 * 15 - Button 10
 * A0 - Button 8
 * 
 * A1 - Button Encoder 14
 * A2 - Encoder 15
 * A3 - Encoder 16
 */
 
//Arrays for Buttons and Encoders
long Encoder1[3] = {-1, -1, -1};
long Encoder2[3] = {-1, -1, -1};

long Button4[2] = {-1, -1};
long Button5[2] = {-1, -1};
long Button6[2] = {-1, -1};
long Button7[2] = {-1, -1};
long Button8[2] = {-1, -1};
long Button9[2] = {-1, -1};
long Button10[2] = {-1, -1};
long Button16[2] = {-1, -1};
long Button14[2] = {-1, -1};
long Button15[2] = {-1, -1};
long ButtonA0[2] = {-1, -1};
long ButtonA1[2] = {-1, -1};

//Functions
void InitEncoder(int, int, int, int, long [3]);
void InitButton(int, int, long [2]);
//Debounce in ms
const int Debounce = 100;

void setup() {
  //PinMode Setup
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  //Debugging
  Serial.begin(9600);
  
  //Act as HID Device
  Gamepad.begin();
}

void loop() {
  //Button and Encoder
  InitEncoder(12, 13, 3, 2, Encoder1);
  InitEncoder(15, 16, A2, A3, Encoder2);
  
  InitButton(11, 4, Button4);
  InitButton(5, 5, Button5);
  InitButton(9, 6, Button6);
  InitButton(6, 7, Button7);
  InitButton(1, 8, Button8);
  InitButton(2, 9, Button9);
  InitButton(3, 10, Button10);
  InitButton(4, 16, Button16);
  InitButton(7, 14, Button14);
  InitButton(10, 15, Button15);
  InitButton(8, A0, ButtonA0);
  InitButton(14, A1, ButtonA1);
  
  //Save Changes
  Gamepad.write();
}

void InitEncoder(int Button1, int Button2, int Pin1, int Pin2, long Arr[3]) {
  int ButtonStatus1 = digitalRead(Pin1);
  int ButtonStatus2 = digitalRead(Pin2);
  int OldButtonStatus1 = Arr[0];
  int OldButtonStatus2 = Arr[1];
  long LastDebounce = millis() - Arr[2];
  Serial.print(ButtonStatus1);
  Serial.print(" ");
  Serial.println(ButtonStatus2);
  Serial.print(OldButtonStatus1);
  Serial.print(" ");
  Serial.println(OldButtonStatus2);

  if(LastDebounce > Debounce) {
    Gamepad.release(Button1);
    Gamepad.release(Button2);
  }

  if((ButtonStatus1 != OldButtonStatus1 || ButtonStatus2 != OldButtonStatus2) && LastDebounce > Debounce) {
    Arr[0] = ButtonStatus1;
    Arr[1] = ButtonStatus2;
    Arr[2] = millis();

    if(OldButtonStatus1 == 0 && OldButtonStatus2 == 0) {
      if(ButtonStatus1 == 0 && ButtonStatus2 == 1) {
        Gamepad.release(Button1);
        Gamepad.press(Button2);
      }

      if(ButtonStatus1 == 1 && ButtonStatus2 == 0) {
        Gamepad.press(Button1);
        Gamepad.release(Button2);
      }
    }

    if(OldButtonStatus1 == 0 && OldButtonStatus2 == 1) {
      if(ButtonStatus1 == 1 && ButtonStatus2 == 1) {
        Gamepad.release(Button1);
        Gamepad.press(Button2);
      }

      if(ButtonStatus1 == 0 && ButtonStatus2 == 0) {
        Gamepad.press(Button1);
        Gamepad.release(Button2);
      }
    }

    if(OldButtonStatus1 == 1 && OldButtonStatus2 == 1) {
      if(ButtonStatus1 == 1 && ButtonStatus2 == 0) {
        Gamepad.release(Button1);
        Gamepad.press(Button2);
      }

      if(ButtonStatus1 == 0 && ButtonStatus2 == 1) {
        Gamepad.press(Button1);
        Gamepad.release(Button2);
      }
    }

    if(OldButtonStatus1 == 1 && OldButtonStatus2 == 0) {
      if(ButtonStatus1 == 0 && ButtonStatus2 == 0) {
        Gamepad.release(Button1);
        Gamepad.press(Button2);
      }

      if(ButtonStatus1 == 1 && ButtonStatus2 == 1) {
        Gamepad.press(Button1);
        Gamepad.release(Button2);
      }
    }
  }
}

void InitButton(int Button, int Pin, long Arr[2]) {
  /*
   * 0 = Gedrückt = LOW
   * 1 = Nichte Gedrückt = HIGH
   */
   
  int ButtonStatus = digitalRead(Pin);
  int OldButtonStatus = Arr[0];
  long LastDebounce = millis() - Arr[1];

  if(ButtonStatus != OldButtonStatus && LastDebounce > Debounce) {
    Arr[0] = ButtonStatus;
    
    if(ButtonStatus == LOW)
      Arr[1] = millis();
  }

  switch(Arr[0]) {
    case -1: {
      Gamepad.release(Button);
    } break;
    case 0: {
      Gamepad.press(Button);
    } break;
    case 1: {
      Gamepad.release(Button);
    } break;
  }
}
