const int NUM_SLIDERS = 3;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2};//, A3, A4};
const int digitalInputs[NUM_SLIDERS] = {12,-1,-1};
const int ledPin[NUM_SLIDERS] = {LED_BUILTIN,-1,-1};
bool areMutedStates[NUM_SLIDERS] = {false, false, false};

int analogSliderValues[NUM_SLIDERS];
bool state;

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
// button pins  
for(int i = 0; i < NUM_SLIDERS; i++){
  if(digitalInputs[i] != -1){
  pinMode(digitalInputs[i], INPUT_PULLUP);
  }
}
// led pins
for(int i = 0; i < NUM_SLIDERS; i++){
  if(digitalInputs[i] != -1){
    pinMode(ledPin[i], OUTPUT);
  }
}

  Serial.begin(9600);
}

void loop() {
  initialiseMuteButtonAndLEDPins();
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void checkMuteLEDS(String volumeValue, int ledIndex){
  if(ledPin[ledIndex] != -1){
     if(volumeValue == "0"){
      digitalWrite(ledPin[ledIndex], HIGH); 
     } else{
      digitalWrite(ledPin[ledIndex], LOW);
     }
  }
}

void initialiseMuteButtonAndLEDPins(){
   for(int i = 0; i < NUM_SLIDERS; i++){
      if(digitalInputs[i] != -1){
        checkMutedChannels(digitalInputs[i],i);
      }      
    }    
}

void checkMutedChannels(int pinNumber,int pinIndex){
 if(digitalRead(pinNumber)==LOW){
    areMutedStates[pinIndex] = !areMutedStates[pinIndex];
      while(digitalRead(pinNumber)==LOW){ 
      }
    }
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    String resultString = areMutedStates[i] ? "0" : String((int)analogSliderValues[i]);
    checkMuteLEDS(resultString,i);
    builtString += resultString;

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
