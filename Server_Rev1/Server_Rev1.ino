#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Must match RX's freq!
#define RF95_FREQ 915.0
#define SERVER_ID 1

uint8_t toTransmit[25][6];//The data left to be transmited to the raspi, in the form of [x][0] = direction, [x][1] = node ID/location, [x][2-5] = time if needed
int carsToTransmit = 0;

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, SERVER_ID);
void setup() {
  // put your setup code here, to run once:
  initRadios();
  //confirm rasp pi up
}
void initRadios() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  Serial1.begin(9600);//start serial to ras pi

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
  
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  rf95.setTxPower(23, false);
}


void loop() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  uint16_t timeout = 1000;//will change
  uint8_t to = 0;
  uint8_t from = 0;
  while (manager.recvfromAckTimeout(buf, &len, timeout, &from, &to, NULL, NULL)) { //message id and flags we don't care about at this point
    Serial.print("got request from node:");
    Serial.print(from, HEX);
    Serial.print(". Message:");
    for (int i = 0; i < len; i++) {
      Serial.print(buf[i]);
      Serial.print("-");
    }
    Serial.println();

    //car packet id to be treated accordingly
    if (buf[0] == 1) {
      toTransmit[carsToTransmit][0] = buf[5];//direction
      toTransmit[carsToTransmit][1] = buf[6];//nodeID/location

      //time
      toTransmit[carsToTransmit][2] = buf[1];
      toTransmit[carsToTransmit][3] = buf[2];
      toTransmit[carsToTransmit][4] = buf[3];
      toTransmit[carsToTransmit][5] = buf[4];

      carsToTransmit++;
    }else if (buf[0] == 3){//Heartbeat
      Serial.print(buf[1]);
      Serial.print(buf[2]);
      Serial.print(",");
      Serial.print(buf[3]);
      Serial.println(buf[4]);
    }
  }


  int retries = 0;
  //if cars to transmit and we havn't tried too many times, then send cars
  while (carsToTransmit > 0 && retries < 4) {    
    Serial1.println("DATA");//DATA to ras

    //expect OK from ras, this function has a 1 second timeout 
    uint8_t buf[4];
    if(Serial1.readBytes(buf, 2) == 2 && buf[0] == 'O' && buf[1] == 'K'){
        carsToTransmit--;
        buf[0] = 'z';//insure buffer is clear of commands
        //trasmit data with - inbetween to ras
        for (int i = 0; i < 6; i++) {
          Serial.print(toTransmit[carsToTransmit][i]);
          Serial.print("-");
        }
    }else{
      retries++;
    }
  }

}
