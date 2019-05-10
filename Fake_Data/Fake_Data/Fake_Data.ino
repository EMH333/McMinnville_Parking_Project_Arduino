#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

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



//PID number 1 for sending car data
bool sendCarOne(){
  const uint8_t PACKET_LENGTH = 7;
  uint8_t packet[PACKET_LENGTH] = {1, 0, 0, 0, 0,1,NODE_ID};//Create packet with PID of 1, length of 7

  manager.sendtoWait((uint8_t *)packet, PACKET_LENGTH,SERVER_ID);
}

bool sendCarTwo(){
  const uint8_t PACKET_LENGTH = 7;
  uint8_t packet[PACKET_LENGTH] = {1, 0, 0, 0, 0,2,NODE_ID};//Create packet with PID of 1, length of 7

  manager.sendtoWait((uint8_t *)packet, PACKET_LENGTH,SERVER_ID);
}


void sendHeartbeat(uint8_t one, uint8_t two){
  const int leng = 5;
  uint8_t packet[leng] = {3,one/10,one%10, two/10,two%10};//PID of 3, then send distances
  manager.sendtoWait((uint8_t *)packet, leng ,SERVER_ID);
}








const int IN_PIN = 15;
const int OUT_PIN = 16;
const int HEART_PIN = 17;
bool outPrevious = false;
bool inPrevious = false;
bool heartPrevious = false;
void setup() {
  initRadios();
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, INPUT);
  pinMode(HEART_PIN, INPUT);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13, LOW);
  if(digitalRead(IN_PIN)==HIGH){
    digitalWrite(13, HIGH);
    if(inPrevious == false){
      inPrevious = true;
      //send car going in
      sendCarOne();
      Serial.println("CAR ONE");
    }
  }else{
    inPrevious = false;
  }

  if(digitalRead(OUT_PIN)==HIGH){
    digitalWrite(13, HIGH);
    if(outPrevious == false){
      outPrevious = true;
      //send car going out
      sendCarTwo();
      Serial.println("CAR TWO");
    }
  }else{
    outPrevious = false;
  }

  if(digitalRead(HEART_PIN)==HIGH){
    digitalWrite(13, HIGH);
    if(heartPrevious == false){
      heartPrevious = true;
      //send heartbeat
      sendHeartbeat(random(100,300),random(100,300));
      Serial.println("HEART");
    }
  }else{
    heartPrevious = false;
  }

  delay(250);//lets me not debounce
}

