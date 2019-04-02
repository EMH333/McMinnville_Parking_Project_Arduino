#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>


#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Must match RX's freq!
#define RF95_FREQ 915.0

#define SERVER_ID 1

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, SERVER_ID);
void setup() {
  // put your setup code here, to run once:
  initRadios();
  //confirm rasp pi up
}
void initRadios(){
   pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  while (!Serial) {
    delay(1);
  }

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
void loop() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  uint16_t timeout = 2000;//will change
  uint8_t to = 0;
  uint8_t from = 0;
  if(manager.recvfromAckTimeout(buf,&len,timeout,&from, &to,NULL, NULL)){//message id and flags we don't care about at this point
      Serial.print("got request from node:");
      Serial.print(from, HEX);
      Serial.print(". Message:");
      for(int i = 0; i < len; i++){
        Serial.print(buf[i]);
        Serial.print("-");
      }
      Serial.println();
  }
  //listen for radio
  //if car, send to pi
  //send to pi
}
