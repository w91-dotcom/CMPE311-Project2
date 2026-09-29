#include <Arduino.h>


int selectedLED = -1;
int inputValue = 0;
bool inputStarted = false;


unsigned long blinkRateOne = 0;
unsigned long blinkRateTwo = 0;


unsigned long blinkTrackerOne = 0;
unsigned long blinkTrackerTwo = 0;




// Task prototypes
void taskLED1();
void taskLED2();
void taskSerial();




// Function pointer table
void (*tasks[])() = {
  taskLED1,
  taskLED2,
  taskSerial
};




void setup() {
  Serial.begin(9600);


  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);


  Serial.println("What LED? (1 or 2):");
}




void loop() {


  // Round-robin cyclic executive
  for (int i = 0; i < 3; i++) {
    tasks[i]();
  }
}




void taskLED1() {


  unsigned long currentTime = millis();


  if (blinkRateOne &&
      (currentTime - blinkTrackerOne) >= (blinkRateOne / 2)) {


    digitalWrite(2, !digitalRead(2));
    blinkTrackerOne = currentTime;
  }
}




void taskLED2() {


  unsigned long currentTime = millis();


  if (blinkRateTwo &&
      (currentTime - blinkTrackerTwo) >= (blinkRateTwo / 2)) {


    digitalWrite(3, !digitalRead(3));
    blinkTrackerTwo = currentTime;
  }
}


void taskSerial() {


  if (Serial.available() > 0) {


    char input = Serial.read();


    // If input is a number
    if (input >= '0' && input <= '9') {


      inputValue = inputValue * 10 + (input - '0');
      inputStarted = true;
    }

    // If user presses Enter
    else if ((input == '\n' || input == '\r') && inputStarted) {


      // Getting LED number
      if (selectedLED == -1) {

        selectedLED = inputValue;
        Serial.println("What interval (in msec):");
      }


      // Getting blink interval
      else {


        if (selectedLED == 1) {
          blinkRateOne = inputValue;
        }


        else if (selectedLED == 2) {
          blinkRateTwo = inputValue;
        }


        selectedLED = -1;
        Serial.println("What LED? (1 or 2):");
      }


      inputValue = 0;
      inputStarted = false;
    }
  }
}
