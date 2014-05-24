/*first ticktacktoe draft...
 
 
 created 2014
 by Daniel, Christian & Lisa

This code is in the public domain.
*/

// constants won't change. Used here to 
// set pin numbers:
const int RedledPin   =  2;      // the number of the red LED pin
const int GreenledPin =  3;      // the number of the green LED pin
const int sensorPin1  =  A0;     // the number of the sensor (analogue)
const int threshold  =  200;    // trigger value to switch LED on
// game states
const int Reset       = 0;
const int RedTurn     = 1;       
const int GreenTurn   = 2;
const int RedWon      = 3;
const int GreenWon    = 4;
// field states
const int FieldFree   = 0;
const int FieldRed    = 1;
const int FieldGreen  = 2;


// Variables will change:
int sensorValue1;        // variable to store the value coming from the sensor, possible Range 0 - 1023
int RedledState;         // ledState used to set the red LED
int GreenledState;       // ledState used to set the green LED
int gameState = Reset;
int count     = 0;
int fieldState[9];


void setup() {
  // set the digital pin as output:
  pinMode(RedledPin, OUTPUT);      
  pinMode(GreenledPin, OUTPUT);      
  pinMode(sensorPin1, INPUT);      
}

int checkTriplet (int a, int b, int c) {
  return fieldState[a] != FieldFree && 
         fieldState[a] == fieldState[b] && 
         fieldState[a] == fieldState[c];
}

// switch on red or green light
int switchLamp (int index, int newFieldState) {
  
}

int checkFields(int newFieldState) {
    int fieldValue = checkSensor();
    if (fieldValue >= 0 && fieldState[fieldValue] == FieldFree) {
      fieldState[fieldValue] = newFieldState;
      switchLamp(fieldValue, newFieldState);
    }
}

int checkWinner() {
  return checkTriplet(0, 1, 2) ||
         checkTriplet(3, 4, 5) ||
         checkTriplet(6, 7, 8) ||
         checkTriplet(0, 3, 6) ||
         checkTriplet(1, 4, 7) ||
         checkTriplet(2, 5, 8) ||  
         checkTriplet(0, 4, 8) ||
         checkTriplet(2, 4, 6);
}

int checkSensor()
{
  sensorValue1 = analogRead(sensorPin1);    
  if (sensorValue1 > threshold) 
    count++;
  else 
    count = 0;
    
  if (count > 15000) {
    count = 0;
    return 1;
  }
  else 
    return 0; 
}

  


void loop()
{
  if (gameState == Reset) {
    for (int i = 0; i < 9; i++)
      fieldState[i] = FieldFree;
    gameState = RedTurn;

  } else if (gameState == RedTurn) {
    RedledState   = HIGH;
    GreenledState = LOW;
 
    checkFields(FieldRed);   
    
    if (checkWinner())
      gameState = RedWon;
    else
      gameState = GreenTurn;

  } else if (gameState == GreenTurn) {
    GreenledState = HIGH;
    RedledState   = LOW;
    
    checkFields(FieldGreen);   

    if (checkWinner())
      gameState = GreenWon;
    else
      gameState = RedTurn;
    
  } else if (gameState == RedWon) {
    gameState = Reset;
  } else if (gameState == GreenWon) {
    gameState = Reset;
  } 
  
  
   // set the LED with the ledState of the variable:
   digitalWrite(GreenledPin, GreenledState);
   digitalWrite(RedledPin, RedledState);
  
  } 


  





