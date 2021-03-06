// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// AIS328DQTR
// This code is designed to work with the AIS328DQTR_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=AIS328DQTR_I2CS#tabs-0-product_tabset-2

#include<Wire.h>

// AIS328DQTR I2C address is 0x18(24) 
#define Addr 0x18

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register1
  Wire.write(0x20);
  // Select 50Hz, power on mode, X, Y, Z-Axis enabled  
  Wire.write(0x27);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register4
  Wire.write(0x23);
  // Select continuous update, full-scale range +/- 8g
  Wire.write(0x30);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[6];
  for(int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((40+i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr,1);

    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if(Wire.available()==1)
    {
      data[i] = Wire.read();
    }
    delay(300);
  }
  
  // Convert the data
  int xAccl = ((data[1] * 256) + data[0]);
  int yAccl = ((data[3] * 256) + data[2]);
  int zAccl = ((data[5] * 256) + data[4]);
  
  // Output data to serial monitor
  Serial.print("Acceleration in X-Axis : ");
  Serial.println(xAccl);
  Serial.print("Acceleration in Y-Axis : ");
  Serial.println(yAccl);
  Serial.print("Acceleration in Z-Axis : ");
  Serial.println(zAccl);
}
