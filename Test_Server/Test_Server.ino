#include <SPI.h>
#include <RH_RF95.h>
// Singleton instance of the radio driver
//RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

int led = 13;
// Must match RX's freq!
#define RF95_FREQ 915.0
void setup() 
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
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
}

long previousTime = 0;
void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      //digitalWrite(led, HIGH);
//      Serial.print("got request: ");
      Serial.print(",");
      Serial.print(buf[0]);
      Serial.print(buf[1]);
      Serial.print(",");
      Serial.print(buf[2]);
      Serial.print(buf[3]);
      Serial.print(",");
      Serial.println(millis()-previousTime);
      previousTime = millis();
//      Serial.print("RSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);
      
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
