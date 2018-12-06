/*
   Group 5: Nick Gelman, Caleb George, Aly El-Mansy
*/

int In1 = 4; // digital pin 7 connected to motor controllers IN1
int In2 = 2;
int ENA = 3;
const int startMotor = HIGH;
const int stopMotor = LOW;

int openSwitch = 6;
int closeSwitch = 7;

int lightSensor = A0;
int darkDetected ; //change later  // analogRead reads a value between 0 to 1023

int lockPin = 5;
const int UNLOCK = LOW;
const int LOCK = HIGH;

unsigned long delayTime = 10000; // time inbetween sensor checkings
unsigned long checkTime = 0;

const int openCloseButton = 8; // pin to which open/close button is connected to
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int reading;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);

  pinMode(In1, OUTPUT); // tells arduino that In1 pin is set to give the output
  pinMode(In2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(openSwitch, INPUT);
  pinMode(closeSwitch, INPUT);
  pinMode(openCloseButton, INPUT);
  pinMode(lockPin, OUTPUT);

  // Sets motor at opening (clockwise)
  digitalWrite(In1, HIGH); // to change direction of motor, change from HIGH to LOW here and change In2 to high
  digitalWrite(In2, LOW);

  while  (digitalRead(openSwitch) != LOW) //make sure door is open
  {
    digitalWrite(lockPin, UNLOCK);
    digitalWrite(ENA, startMotor); // open door
    Serial.println("door is opening");

  }
  digitalWrite(ENA,  stopMotor); // stop motor
  digitalWrite(lockPin, LOCK);
  Serial.println("door is open");
  delay(1000);
  while (digitalRead(openCloseButton)!=LOW)
  {
    Serial.println("waiting for set button");
  }
  delay (1000);
  darkDetected = analogRead(lightSensor);
  Serial.println(analogRead(lightSensor));
  
}

void loop() {
  Serial.println(analogRead(lightSensor));
if (analogRead(lightSensor) > darkDetected) //it is dark outside
  {
    Serial.println("its dark");
    delay(1000);

    // Sets motor to (counterclockwise)
    digitalWrite(In1, LOW); // change direction of motor
    digitalWrite(In2, HIGH);
    //close door
    while  (digitalRead(closeSwitch) != LOW)//make sure door is closed
    {
      Serial.println("door is closing");

      digitalWrite(lockPin, UNLOCK);
      digitalWrite(ENA, startMotor); // close  door
    }
    delay(1000);

    digitalWrite(ENA,  stopMotor); // stop motor
    digitalWrite(lockPin, LOCK);
    Serial.println("door is closed");
    delay(1000);

  }
  else //if it is light outside
  {
    Serial.println("it is light outside");
    digitalWrite(In1, HIGH); // to change direction of motor, change from HIGH to LOW here and change In2 to high
    digitalWrite(In2, LOW);
delay(1000);
    while  (digitalRead(openSwitch) != LOW) //make sure door is open
    {
      Serial.println("door is opening");

      digitalWrite(lockPin, UNLOCK);
      digitalWrite(ENA, startMotor); // open door
    }
    digitalWrite(ENA,  stopMotor); // stop motor
    digitalWrite(lockPin, LOCK);

    Serial.println("door is open");
  }
  delay(1000);
  checkTime = millis();
  while ( millis() - checkTime < delayTime) {
    Serial.println("checking buttons");
    
    // read the state of the switch into a local variable:
    reading = digitalRead(openCloseButton);

    // check to see if you just pressed the button (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();

    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonState)
      {
        buttonState = reading;

        if (buttonState == LOW)
        {
          Serial.println("button is pressed");
          delay (1000);
          if (digitalRead(openSwitch) != LOW)
          {
            Serial.println("while door is not open");
           delay (1000);
            digitalWrite(In1, HIGH); // to change direction of motor
            digitalWrite(In2, LOW);

            while  (digitalRead(openSwitch) != LOW) //make sure door is open
            {
              Serial.println("door is opening");
              digitalWrite(lockPin, UNLOCK);
              digitalWrite(ENA, startMotor); // open door
            }
            Serial.println("door is open");
            delay (1000);
            digitalWrite(ENA,  stopMotor); // stop motor
            digitalWrite(lockPin, LOCK);
            checkTime = millis();//reset timer

          }
          else
          {
            Serial.println("while door is open");
            delay (1000);
            digitalWrite(In1, LOW); // to change direction of motor
            digitalWrite(In2, HIGH);

            while  (digitalRead(closeSwitch) != LOW) //make sure door is closed
            {
              Serial.println("door is closing");
              digitalWrite(lockPin, UNLOCK);
              digitalWrite(ENA, startMotor); // close door
            }
            digitalWrite(ENA,  stopMotor); // stop motor
            digitalWrite(lockPin, LOCK);
            Serial.println("door is closed");
            checkTime = millis(); //reset timer
          }
          delay(1000);
        }

      }

    }

    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;
  }
  Serial.print("leaaving loop\n");

}
