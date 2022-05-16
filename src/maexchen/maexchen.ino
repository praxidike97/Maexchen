int latchPin = 5;  // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6; // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;  // Data pin of 74HC595 is connected to Digital pin 4

int rollPin = 8;

byte leds01 = 0;    // Variable to hold the pattern of which LEDs are currently turned on or off
byte leds02 = 0;

int debounceDelay = 200;
unsigned long lastButtonPress = 0;


/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup() 
{
  Serial.begin(9600);
  // Set all the pins of 74HC595 as OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  pinMode(rollPin, INPUT_PULLUP);
}

/*
 * loop() - this function runs over and over again
 */
void loop(){

  unsigned long currentTime = millis();
  
  if (digitalRead(rollPin) == LOW && (currentTime - lastButtonPress) >= debounceDelay){
    lastButtonPress = currentTime;
    leds01 = byte(random(0, 256));
    leds02 = byte(random(0, 256));

    updateShiftRegister();
  }
  

}

void ledRun(){
  leds01 = 0; // Initially turns all the LEDs off, by giving the variable 'leds' the value 0
  leds02 = 0;
  updateShiftRegister();
  delay(500);
  for (int i = 0; i < 16; i++) // Turn all the LEDs ON one by one.
  {
    if (i < 8){
      bitSet(leds01, i);
      leds02 = 0;
    }else{
      bitSet(leds02, i%8);
    }

    Serial.println(leds02);
    
    updateShiftRegister();
    delay(500);
  }
}

/*
 * updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
 */
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds02);
   shiftOut(dataPin, clockPin, LSBFIRST, leds01);
   digitalWrite(latchPin, HIGH);
}
