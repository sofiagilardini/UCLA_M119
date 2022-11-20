#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define BLE_UUID_ACCELEROMETER_SERVICE "1101"
#define BLE_UUID_ACCELEROMETER_X "2101"
#define BLE_UUID_ACCELEROMETER_Y "2102"
#define BLE_UUID_ACCELEROMETER_Z "2103"

#define BLE_DEVICE_NAME "Nano_Sofia"
#define BLE_LOCAL_NAME "Nano_Sofia"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);

BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);

float x, y, z;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");

  // initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

   // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 IoT");
  BLE.setAdvertisedService(accelerometerService);

  BLEDescriptor AccelXDescriptor("2901", "AccelX");
  accelerometerCharacteristicX.addDescriptor(AccelXDescriptor);

  BLEDescriptor AccelYDescriptor("2901", "AccelY");
  accelerometerCharacteristicY.addDescriptor(AccelYDescriptor);
  
  BLEDescriptor AccelZDescriptor("2901", "AccelZ");
  accelerometerCharacteristicZ.addDescriptor(AccelZDescriptor);


  // add the characteristic to the service
  accelerometerService.addCharacteristic(accelerometerCharacteristicX);
  accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  accelerometerService.addCharacteristic(accelerometerCharacteristicZ);

  // add service
  BLE.addService(accelerometerService);

  // set the initial value for the characteristic:
  accelerometerCharacteristicX.writeValue(0);
  accelerometerCharacteristicY.writeValue(0);
  accelerometerCharacteristicZ.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE Accelerometer Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    float aX, aY, aZ;
    const char * spacer = ", ";
    float aXi, aYi, aZi;

    // reading systematic sensor bias
    if (
        IMU.accelerationAvailable() 
      ) {
      }

    if (
      IMU.accelerationAvailable() 
    ) {      
      IMU.readAcceleration(aX, aY, aZ);
      
      // while the central is still connected to peripheral:
      while (central.connected()) {
        IMU.readAcceleration(aX, aY, aZ);
        accelerometerCharacteristicX.writeValue(aX);
        accelerometerCharacteristicY.writeValue(aY);
        accelerometerCharacteristicZ.writeValue(aZ);
        delay(100);
        Serial.println("sending data");
      }
    }
  }
}

