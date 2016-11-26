/*
 * This sketch will output all CAN messages on the bus its connected to
 */

#include <Canbus.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include <global.h>
#include <defaults.h>

// storage for current message
tCAN message;

void setup() {
  // setup serial comms for debugging
  Serial.begin(9600);
  // setup CANbus
  if(mcp2515_init(CANSPEED_1000)){
    Serial.println("MCP2515 Setup!!");
  }
  else{
    Serial.println("Error!! - MCP2515 Setup Failed!!");
  }
  //setup led pin
  //pinMode(13, OUTPUT);
}

void loop() {
  // if message available
  if(mcp2515_check_message()){
    // get message
    if(mcp2515_get_message(&message)){
      Serial.print("CAN id: ");
      Serial.print(message.id, HEX);
      Serial.print("\tData:\t");
      for(int n = 0; n < 8; n++){
        Serial.print("\t");
        Serial.print(message.data[n], HEX);
      }
      Serial.println("");
    }
    else{
      //Serial.println("Error!! - Can't get message!!");
    }
  }
  else{
    //Serial.println("Error!! - No messages!!");
    //ledflash();
  }
}

void ledflash(void){
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
}

