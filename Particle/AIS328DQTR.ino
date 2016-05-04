// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// AIS328DQTR
// This code is designed to work with the AIS328DQTR_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=AIS328DQTR_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// AIS328DQTR I2C address is 0x18(24)
#define Addr 0x18

int xAccl = 0, yAccl =  0, zAccl = 0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "AIS328DQTR");
  Particle.variable("xAccl", xAccl);
  Particle.variable("yAccl", yAccl);
  Particle.variable("zAccl", zAccl);

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register1
  Wire.write(0x20);
  // Select 50Hz, normal power mode
  Wire.write(0x27);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register4
  Wire.write(0x23);
  // Select full-scale range
  Wire.write(0x30);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[6];
  for (int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((40 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
    delay(300);
  }

  // Convert the data
  xAccl = (((data[1] & 0xFF) * 256) + (data[0] & 0xFF));
  if (xAccl > 32767)
  {
    xAccl -= 65536;
  }

  yAccl = (((data[3] & 0xFF) * 256) + (data[2] & 0xFF));
  if (yAccl > 32767)
  {
    yAccl -= 65536;
  }

  zAccl = (((data[5] & 0xFF) * 256) + (data[4] & 0xFF));
  if (zAccl > 32767)
  {
    zAccl -= 65536;
  }

  // Output data to dashboard
  Particle.publish("Acceleration in X-Axis is :", String(xAccl));
  Particle.publish("Acceleration in Y-Axis is :", String(yAccl));
  Particle.publish("Acceleration in Z-Axis is :", String(zAccl));
}
