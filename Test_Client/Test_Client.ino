// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95
#include <SPI.h>
#include <RH_RF95.h>
// Singleton instance of the radio driver
//RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

// Must match RX's freq!
#define RF95_FREQ 915.0
#define SENSOR_ONE_PIN 14//TBD
#define SENSOR_TWO_PIN 15//TBD


void setup() 
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
  Serial.begin(9600);
  //while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);


  pinMode(SENSOR_ONE_PIN, INPUT);//Init
  pinMode(SENSOR_TWO_PIN, INPUT);
}


unsigned int readDistanceOne() {
  /*unsigned long duration = pulseIn(SENSOR_ONE_PIN, PULSE_TYPE, PULSE_TIMEOUT);
    unsigned int distance = duration / MICRO_PER_CM;
    if(distance < 20){//less than 20 cm then return 0
    return 0;
    }else{
    return distance;
    }*/
  int raw = analogRead(SENSOR_ONE_PIN);//3.2mv per cm
  int voltage = map(raw, 0, 1023, 0, 3300);
  return (unsigned int) voltage / 3.2;
}

unsigned int readDistanceTwo() {
  int raw = analogRead(SENSOR_TWO_PIN);//3.2mv per cm
  int voltage = map(raw, 0, 1023, 0, 3300);
  return (unsigned int) voltage / 3.2;
}

void loop()
{
  //Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  //uint8_t data[] = "Hello World!";
  int dOne = readDistanceOne();
  int dTwo = readDistanceTwo();
  const int leng = 4;
  uint8_t packet[leng] = {dOne/10,dOne%10, dTwo/10,dTwo%10};
  rf95.send(packet, leng);
  rf95.waitPacketSent();

}

