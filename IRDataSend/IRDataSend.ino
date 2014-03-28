/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                   2013                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*        Module:     serialArduino.c                                          */
/*        Author:     James Pearman                                            */
/*        Created:    5 August 2013                                            */
/*                                                                             */
/*        Revisions:  V0.1                                                     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Description:                                                             */
/*                                                                             */
/*    Demonstration code for serial comms transmition from Arduino to          */
/*    VEX cortex.                                                              */
/*                                                                             */
/*    Licensed under the Apache License, Version 2.0 (the "License");          */
/*    you may not use this file except in compliance with the License.         */
/*    You may obtain a copy of the License at                                  */
/*                                                                             */
/*    http://www.apache.org/licenses/LICENSE-2.0                               */
/*                                                                             */
/*    Unless required by applicable law or agreed to in writing, software      */
/*    distributed under the License is distributed on an "AS IS" BASIS,        */
/*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/*    See the License for the specific language governing permissions and      */
/*    limitations under the License.                                           */
/*                                                                          */
/*-----------------------------------------------------------------------------*/

#include <SoftwareSerial.h>
#include <Wire.h>
struct InfraredResult
{
  byte Direction;
  byte Strength;
};

class InfraredSeeker
{
  public:
    static void Initialize();
    static boolean Test();
    static void ReadACRaw(byte* buffer);
    static void ReadDCRaw(byte* buffer);
    static InfraredResult ReadAC();
    static InfraredResult ReadDC();
    static int DirectionAngle(byte Direction);
  private:
    static InfraredResult PopulateValues(byte* buffer);
    static void ReadValues(byte OffsetAddress, byte* buffer);
    static const int Address = 0x10 / 2; //Divide by two as 8bit-I2C address is provided
};

void InfraredSeeker::Initialize()
{
  Wire.begin();
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x00);
  Wire.endTransmission();
  while(Wire.available() > 0)
    Wire.read();
}

boolean InfraredSeeker::Test()
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 16);
  char Manufacturer_Model[16];
  while(Wire.available() < 16);
  for(byte i=0; i < 16; i++)
  {
    Manufacturer_Model[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
  return strncmp(Manufacturer_Model, "HiTechncNewIRDir", 16)==0;
}

void InfraredSeeker::ReadValues(byte OffsetAddress, byte* buffer)
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(OffsetAddress);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 6);
  while(Wire.available() < 6);
  for(byte i = 0; i < 6; i++)
  {
    buffer[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
}

void InfraredSeeker::ReadACRaw(byte* buffer)
{
  ReadValues(0x49, buffer);
}

void InfraredSeeker::ReadDCRaw(byte* buffer)
{
  ReadValues(0x42, buffer);
}

InfraredResult InfraredSeeker::PopulateValues(byte* buffer)
{
  InfraredResult Data;
  Data.Direction = buffer[0];
  if(buffer[0] != 0)
  {
    if(buffer[0] % 2 == 0)
    {
      Data.Strength = (buffer[buffer[0] / 2] + buffer[buffer[0] / 2 + 1]) / 2;
    }
    else
    {
      Data.Strength = buffer[buffer[0] / 2 + 1];
    }
  }
  else
  {
    Data.Strength = 0;
  }
  return Data;
}

InfraredResult InfraredSeeker::ReadAC()
{
  byte buffer[6];
  ReadACRaw(buffer);
  return PopulateValues(buffer);
}

InfraredResult InfraredSeeker::ReadDC()
{
  byte buffer[6];
  ReadDCRaw(buffer);
  return PopulateValues(buffer);
}

int DirectionAngle(byte Direction)
{
  return Direction * 30 - 150;
}
// Storage for the vex communications data
typedef struct irData {
      unsigned char  header_aa;
      unsigned char  header_55;
      unsigned char  message_type;
      unsigned char  datalen;
      unsigned short irDirection;
      unsigned short irDirectionAngle;
      unsigned short irStrength;
      unsigned short var_4;
      unsigned short var_5;
      unsigned short var_6;
      unsigned short var_7;
      unsigned short var_8;
      unsigned char  id;
      unsigned char  checksum;
      } irData;

#define  VEXDATAOFFSET         4
#define  VEX_DATA_BUFFER_SIZE  sizeof(irData)
     
typedef union _vexirdata { 
      irData ir;  
      unsigned char    buffer[VEX_DATA_BUFFER_SIZE];  
      } vexirdata;

vexirdata  MyVexIrDataTx;

/*-----------------------------------------------------------------------------*/
/*  Initialize the software serial port                                        */
/*-----------------------------------------------------------------------------*/

 SoftwareSerial myserial(6,7);// RX, TX

void
serialInit( long baud )
{
    myserial.begin( baud );
}

/*-----------------------------------------------------------------------------*/
/*  Transmit one character                                                     */
/*-----------------------------------------------------------------------------*/

void
serialTxChar(unsigned char c)
{
    // software send
     myserial.write( c );
    
    // hardware send
    //Serial.write( c );  
}

/*-----------------------------------------------------------------------------*/
/*  Arduino setup function                                                     */
/*-----------------------------------------------------------------------------*/

void setup()
{
    InfraredSeeker::Initialize();
    
    // initialize the digital pin 13 as an output.
    pinMode(13, OUTPUT);  
    
    // Initialize the VEX data structures
    VexDataInit( &MyVexIrDataTx );
 
    // Open the software serial port, luckily it defaults to 8bit, 1 stop, no parity
    // as this is not easily changed
    serialInit(115200);    
        
    // console
    Serial.begin( 115200 );  

    // Do not use any debug print statements as we are using the serial
    // port for the VEX communications

    Serial.print(F("\r\nSerial demo code"));
}


/*-----------------------------------------------------------------------------*/
/*  Arduino main loop                                                          */
/*-----------------------------------------------------------------------------*/

void loop()
{
    static  uint32_t  timer = 0;
    InfraredResult InfraredBall = InfraredSeeker::ReadAC();
    // Run every 20mS (50x per second)
    if(millis() - timer < 20)
      return;     
    timer =  millis();
        
    // dummy data - the real variables would be set here
    MyVexIrDataTx.ir.irDirection = InfraredBall.Direction;
    MyVexIrDataTx.ir.irDirectionAngle = DirectionAngle(InfraredBall.Direction);
    MyVexIrDataTx.ir.irStrength = InfraredBall.Strength; 

    // next id
    MyVexIrDataTx.ir.id++;
    
    // Calculate checksum   
    VexDataChecksum( &MyVexIrDataTx );

    // Transmit data
    VexDataTransmit( &MyVexIrDataTx );
}

/*-----------------------------------------------------------------------------*/
/*  Initialize the VEX data                                                    */
/*-----------------------------------------------------------------------------*/

void
VexDataInit( union _vexirdata *v )
{
    int  i;
    
    // clear all
    for(i=0;i<VEX_DATA_BUFFER_SIZE;i++)
      v->buffer[i] = 0;
    
    // Initialize packet  
    v->ir.header_aa    = 0xAA;
    v->ir.header_55    = 0x55;
    v->ir.message_type = 0x01; // Only one type of message
    v->ir.datalen      = 0x10; // 16 bytes of data
    v->ir.id           = 0x00;
}

/*-----------------------------------------------------------------------------*/
/*  Calculate the checksum for the VEX data                                    */
/*-----------------------------------------------------------------------------*/

void
VexDataChecksum( union _vexirdata  *v )
{
    int  i;
    int  cs = 0;
   
    for(i=0;i<(VEX_DATA_BUFFER_SIZE-1);i++)
      cs += v->buffer[i];
    
    v->ir.checksum = 0x100 - (cs & 0xFF);
}

/*-----------------------------------------------------------------------------*/
/*  send the VEX data to the serial port                                       */
/*-----------------------------------------------------------------------------*/

void
VexDataTransmit( union _vexirdata  *v )
{
    int  i;
    
    for(i=0;i<VEX_DATA_BUFFER_SIZE;i++)
      serialTxChar( v->buffer[i] );
}

