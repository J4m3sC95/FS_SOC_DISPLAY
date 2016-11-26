// sketch to display out put based on incoming serial data
//    - serial data encoded to lower case letters (+97)

#define TIMEOUT_THRESHOLD 5000
#define LED_PAUSE 100

unsigned char led_pins[11] = {2,3,4,5,6,7,8,9,14,15};

int SOC = 255;
unsigned long last_serial = 0;
unsigned long timeout = 0;

void setup() {
  Serial.begin(9600);
  for(int n = 0; n<10; n++){
    pinMode(led_pins[n], OUTPUT);
    digitalWrite(led_pins[n], LOW);
  }
}

void loop() {
  timeout = millis() - last_serial;
  // if SOC hasn't been initialised do inverted knightrider
  if(SOC == 255){
    knightrider(1);
  }
  // if SOC is 0 do knightrider
  else if(SOC == 0){
    knightrider(0);
  }
  // if havent received any data in TIMEOUT_THRESHOLD then flash to show data not up to date
  else if(timeout > TIMEOUT_THRESHOLD){
    setOutput(SOC);
    delay(2*LED_PAUSE);
    for(int n = 0; n<10; n++){
      digitalWrite(led_pins[n], LOW);
    }
    delay(2*LED_PAUSE);
  }
  // if ok display SOC normally
  else{
    setOutput(SOC);
  }
}

void setOutput(int level){
  for(int n = 0; n<= level - 1; n++){
    digitalWrite(led_pins[n], HIGH);
  }
  for(int n = level; n<10; n++){
    digitalWrite(led_pins[n], LOW);
  }
}

void knightrider(int invert){
  int state1, state2;
  if(invert){
    state1 = LOW;
    state2 = HIGH;
  }
  else{
    state1 = HIGH;
    state2 = LOW;
  }
  for(int n = 0; n< 10; n++){
    digitalWrite(led_pins[n], state1);
    delay(LED_PAUSE);
    digitalWrite(led_pins[n], state2);
  }
  for(int n = 8; n>0; n--){
    digitalWrite(led_pins[n], state1);
    delay(LED_PAUSE);
    digitalWrite(led_pins[n], state2);
  }
}

void serialEvent(){
  int SOC_temp;
  while(Serial.available()){
    SOC_temp = Serial.read() - 97;
  }
  if((SOC_temp < 0)|(SOC_temp>10)){
    Serial.println("Error!! - SOC out of range");
  }
  else{
    SOC = SOC_temp;
    last_serial = millis(); 
  }
  Serial.println(SOC);
}

