#include <ArduinoBLE.h>
#include <Wire.h>

BLEService kneeAngleService("13012F00-F8C3-4F4A-A8F4-16CD926DA146"); // BLE Service

// Order must be matched on the Python side. Total size is 6 bytes (unsigned long=4 + 1*(ints=2)=2)
typedef struct  __attribute__ ((packed)) {
  unsigned long timeread;
  int flex_value;
} sensordata;

BLECharacteristic structDataChar("13012F08-F8C3-4F4A-A8F4-15CD926DA146", BLERead | BLENotify, sizeof(sensordata));


ADS myFlexSensorTwoAxis; //Create instance of the Angular Displacement Sensor (ADS) class.The default I2C address is 0x13.

void setup() {
  // put your setup code here, to run once
  Wire.begin(); // Need for i2c communication

  if (myFlexSensorTwoAxis.begin(19) == false)
  {
    //Serial.println(F("No flex sensor two axis detected... Check wiring or 12C address."));
    while (1);
  }

  BLE.setLocalName("Arduino Nano 33 BLE");
  BLE.setDeviceName("Arduino Nano 33 BLE"); 
  BLE.setAdvertisedService(kneeAngleService);  // Set service UUID

  // Add Descriptors to Improve readability on BLE Client Side
  BLEDescriptor structDataDescriptor("2901", "Timestamped Flex values ReadNotify");
  structDataChar.addDescriptor(structDataDescriptor);
  
  kneeAngleService.addCharacteristic(structDataChar);

  BLE.addService(kneeAngleService);
  BLE.setAdvertisingInterval(1000);  // 1 second
  BLE.advertise();

}

void loop() {
  // put your main code here, to run repeatedly
  millis_value = millis();
  
  if ((millis_value - last_ms) >= PERIOD_MS){
        if (myFlexSensorTwoAxis.available() == true){
      float flex_x_value = myFlexSensorTwoAxis.getX();  
      flex_angle = round(flex_x_value) * -1;  // Make it an int value. Also the position of the flex means negative numbers (so minus 1)
      flex_angle_y_axis = round(flex_y_value);
    }
  }

}
