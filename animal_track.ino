//----------------------------------------------------------------------------------------------
//BSP : Seeed nRF52 Borads 1.0.0 (no mbed)
//Board : Seeed nRF52 Borads / Seeed XIAO nRF52840 Sense
//2023/02/25
//----------------------------------------------------------------------------------------------
//central : PC

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>   //Serial.print()
#include <bluefruit.h>
#include "LSM6DS3.h"
#include "Wire.h"

#define DATANUM 28           // sample payload
union unionData {           // for data conversion
  uint32_t  data32[DATANUM/4];
  int16_t   data16[DATANUM/2];
  uint8_t   data8[DATANUM];
};
union unionData ud;
unsigned long timestamp;    // for loop timer

//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A


// A:    C1:73:C6:56:D9:C6, name: RatTrackA, uuid: ['845c8a00-0000-4fcb-a990-64f508f3781e']
// B:    D4:17:D7:95:52:ED, name: RatTrackB, uuid: ['845c8a01-0000-4fcb-a990-64f508f3781e']
// C:    C7:C5:1F:11:2C:D6, name: RatTrackC, uuid: ['845c8a02-0000-4fcb-a990-64f508f3781e']
// D:    DE:43:87:EF:AF:B4, name: RatTrackD, uuid: ['845c8a03-0000-4fcb-a990-64f508f3781e']
// E:    EE:A8:01:48:78:B1, name: RatTrackE, uuid: ['845c8a04-0000-4fcb-a990-64f508f3781e']
// F:    D8:36:A1:8C:37:92, name: RatTrackF, uuid: ['845c8a05-0000-4fcb-a990-64f508f3781e']
// G:    D5:29:E7:9B:56:FA, name: RatTrackG, uuid: ['845c8a06-0000-4fcb-a990-64f508f3781e']
// H:    DD:B4:45:64:C6:26, name: RatTrackH, uuid: ['845c8a07-0000-4fcb-a990-64f508f3781e']
// Test: FD:4B:26:0E:30:0D, name: Test, uuid: ['845c8a88-0000-4fcb-a990-64f508f3781e']
BLEService        data1Service          ("845c8a07-0000-4fcb-a990-64f508f3781e");
BLECharacteristic data1Characteristic   ("845c8a07-0010-4fcb-a990-64f508f3781e");

void setup() {
  //Serial.begin(115200);
//  while (!Serial);

  myIMU.begin();
  // if (myIMU.begin() != 0) {
  //   Serial.println("Device error");
  // } else {
  //   Serial.println("Device OK!");
  // }

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, HIGH);    //LED OFF
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.configUuid128Count(15);

  Bluefruit.begin();
  Bluefruit.setName("RatTrackH"); //Name change for each device ---------------------
  Bluefruit.setTxPower(4);
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  data1Service.begin();

  data1Characteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  data1Characteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  data1Characteristic.setFixedLen(DATANUM);
  data1Characteristic.begin();

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(data1Service);
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setIntervalMS(20, 153);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0);
}

void loop() 
{
    if ( Bluefruit.connected() )
    {
      digitalWrite(LED_GREEN, LOW);
      
      // pay load
      ud.data32[0] = millis();
      ud.data32[1] = myIMU.readFloatAccelX();
      ud.data32[2] = myIMU.readFloatAccelY();
      ud.data32[3] = myIMU.readFloatAccelZ();
      ud.data32[4] = myIMU.readFloatGyroX();
      ud.data32[5] = myIMU.readFloatGyroY();
      ud.data32[6] = myIMU.readFloatGyroZ();
    
      digitalWrite(LED_RED, LOW);
      

      data1Characteristic.notify(ud.data8, DATANUM);              
      
      delay(10);  // for LED
      digitalWrite(LED_RED, HIGH);
      
      while(millis() - timestamp <= 500); // loop time
      timestamp = millis();

    }
    digitalWrite(LED_GREEN, HIGH);
}

void connect_callback(uint16_t conn_handle)
{
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  // Serial.print("Connected to ");
  // Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  // Serial.println();
  // Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
