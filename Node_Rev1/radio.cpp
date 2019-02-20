#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include "radio.h"
#include "UltrasonicDetection.h"

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Must match RX's freq!
#define RF95_FREQ 915.0

#define SERVER_ID 1
#define NODE_ID 2//MUST CHANGE PER NODE, server is "1"

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, NODE_ID);

void initRadios() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  
  /*while (!Serial) {
    delay(1);
  }*/

  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  if (!manager.init())
  {
    printf( "Manager init failed\n" );
  }

  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}


int16_t packetnum = 0;  // packet counter, we increment per xmission
void oldLoop() {
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server

  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket + 13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;

  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Waiting for packet to complete...");
  delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(1000))
  {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len))
    {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }

}


bool sendCarData(CarInformation car){
  const uint8_t PACKET_LENGTH = 7;
  uint8_t packet[PACKET_LENGTH] = {1};//Create packet with PID of 1, length of 7
  //we will fill in time later as this isn't a huge priority
 // packet[5] = car.getDirection(); //not sure why this line isn't working
  packet[6] = NODE_ID;

  manager.sendtoWait((uint8_t *)packet, PACKET_LENGTH,SERVER_ID);
}

void sendRawData(uint8_t one, uint8_t two){
  const int leng = 3;
  uint8_t packet[leng] = {one, 9,two};
  manager.sendtoWait((uint8_t *)packet, leng ,SERVER_ID);
}
