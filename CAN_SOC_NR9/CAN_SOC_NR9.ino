/*
 * Elithion bms manual CAN specs:
 * http://lithiumate.elithion.com/php/controller_can_specs.php
 * 
 * This finally works in NR9 with the following parameters:
 * BUS speed = 1000kbps
 * MCP clock = 8MHz
 * can high wire = Green
 * can low = yellow/white
 */
#include <mcp_can.h>
#include <SPI.h>

#define SOC_CAN_MESSAGE 0x626

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

unsigned char led_pins[11] = {2,3,4,5,6,7,8,9,14,15};

int SOC = 0;

// start CAN class with chip select pin = 10
MCP_CAN CAN0(10);

void setup()
{
  Serial.begin(115200);
  // initialise CAN bus at 1000kbps for 8MHz can module clock speed
  CAN0.begin(CAN_1000KBPS, MCP_8MHz); 
  // setup interrupt pin as Analog 2 or pin 16
  pinMode(16, INPUT);

  // setup LED pins
  for(int n = 0; n<10; n++){
    pinMode(led_pins[n], OUTPUT);
    digitalWrite(led_pins[n], LOW);
  }
}

void loop(){
  // if interrupt pin is low then data available
  if(!digitalRead(16)){
    //Read data from CAN module
    CAN0.readMsgBuf(&len, rxBuf);
    // get message ID
    rxId = CAN0.getCanId();
    if(rxId == SOC_CAN_MESSAGE){
      SOC = rxBuf[0];
      Serial.print("SOC = ");
      Serial.println(SOC);
      // divide by 10 to get to a displayable number (this will always round down) 
      SOC /= 10;
      setOutput(SOC);
    }
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
