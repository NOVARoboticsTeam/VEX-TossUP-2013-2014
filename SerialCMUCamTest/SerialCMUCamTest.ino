/***************************************************************************//**
* @file
* Color Tracking Template Code w/ Poll Mode
*
* @version @n 1.0
* @date @n 8/14/2012
*
* @authors @n Kwabena W. Agyeman
* @copyright @n (c) 2012 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Initial Release - 8/14/2012
*******************************************************************************/

#include <CMUcam4.h>
#include <CMUcom4.h>

#define RED_MIN 187
#define RED_MAX 217
#define GREEN_MIN 118
#define GREEN_MAX 183
#define BLUE_MIN 106
#define BLUE_MAX 125
// Red Buckyball Tracking Values

#define RED_MIN1 78 
#define RED_MAX1 113
#define GREEN_MIN1 101
#define GREEN_MAX1 167
#define BLUE_MIN1 162
#define BLUE_MAX1 201
//Blue Buckyball Tracking Values

#define RED_MIN2 134
#define RED_MAX2 183
#define GREEN_MIN2 79
#define GREEN_MAX2 93
#define BLUE_MIN2 84
#define BLUE_MAX2 108
//Red Large ball Tracking Values


#define RED_MIN3 63
#define RED_MAX3 88
#define GREEN_MIN3 91
#define GREEN_MAX3 119
#define BLUE_MIN3 109
#define BLUE_MAX3 168
//Blue Large Ball Tracking Values



#define LED_BLINK 5 // 5 Hz
#define WAIT_TIME 5000 // 5 seconds
#define YUV_MODE true 

CMUcam4 cam(CMUCOM4_SERIAL1);



typedef struct _vdata {
      unsigned char  header_aa;
      unsigned char  header_55;
      unsigned char  message_type;
      unsigned char  datalen;
      unsigned short mx;
      unsigned short my;
      unsigned short x1;
      unsigned short y1;
      unsigned short x2;
      unsigned short y2;
      unsigned short pixels;
      unsigned short confidence;
      unsigned short poll; 
      unsigned char  id;
      unsigned char  checksum;
      } vdata;

#define  VEXDATAOFFSET         4
#define  VEX_DATA_BUFFER_SIZE  sizeof(vdata)
     
typedef union _vexdata { 
      vdata data;  
      unsigned char    buffer[VEX_DATA_BUFFER_SIZE];  
      } vexdata;

vexdata  MyVexDataTx;

/*-----------------------------------------------------------------------------*/
/*  Initialize the software serial port                                        */
/*-----------------------------------------------------------------------------*/

//SoftwareSerial mySerial(6, 7); // RX, TX

void
serialInit( long baud )
{
    Serial3.begin( baud );
}

/*-----------------------------------------------------------------------------*/
/*  Transmit one character                                                     */
/*-----------------------------------------------------------------------------*/

void
serialTxChar(unsigned char c)
{
    // software send
     Serial3.write( c );
    
    // hardware send
    //Serial.write( c );  
}
void setup()
{
  cam.begin();
   VexDataInit( &MyVexDataTx );
  // Wait for auto gain and auto white balance to run.

  cam.LEDOn(LED_BLINK);
  delay(WAIT_TIME);
      serialInit(115200); 
  // Turn auto gain and auto white balance off.

  cam.autoGainControl(false);
  cam.autoWhiteBalance(false);

  cam.LEDOn(CMUCAM4_LED_ON);
}

void loop()
{
  CMUcam4_tracking_data_t t_data;

  cam.pollMode(true);

  for(;;)
  {
    cam.trackColor(RED_MIN, RED_MAX, GREEN_MIN, GREEN_MAX, BLUE_MIN, BLUE_MAX);
    cam.getTypeTDataPacket(&t_data); // Get a tracking packet.

    // Process the packet data safely here.
    MyVexDataTx.data.mx =  (short)&t_data.mx; 
    MyVexDataTx.data.my = (short)&t_data.my; 
    MyVexDataTx.data.x1 = (short)&t_data.x1; 
    MyVexDataTx.data.y1 = (short)&t_data.y1; 
    MyVexDataTx.data.x2 = (short)&t_data.x2; 
    MyVexDataTx.data.y2 = (short)&t_data.y2; 
    MyVexDataTx.data.pixels = (short)&t_data.pixels;
    MyVexDataTx.data.confidence = (short)&t_data.confidence; 
    
    MyVexDataTx.data.id++;
    
    // Calculate checksum   
    VexDataChecksum( &MyVexDataTx );

    // Transmit data
    VexDataTransmit( &MyVexDataTx );
  }

  

    
}

void
VexDataInit( union _vexdata *v )
{
    int  i;
    
    // clear all
    for(i=0;i<VEX_DATA_BUFFER_SIZE;i++)
      v->buffer[i] = 0;
    
    // Initialize packet  
    v->data.header_aa    = 0xAA;
    v->data.header_55    = 0x55;
    v->data.message_type = 0x01; // Only one type of message
    v->data.datalen      = 0x10; // 16 bytes of data
    v->data.id           = 0x00;
}

/*-----------------------------------------------------------------------------*/
/*  Calculate the checksum for the VEX data                                    */
/*-----------------------------------------------------------------------------*/

void
VexDataChecksum( union _vexdata  *v )
{
    int  i;
    int  cs = 0;
   
    for(i=0;i<(VEX_DATA_BUFFER_SIZE-1);i++)
      cs += v->buffer[i];
    
    v->data.checksum = 0x100 - (cs & 0xFF);
}

/*-----------------------------------------------------------------------------*/
/*  send the VEX data to the serial port                                       */
/*-----------------------------------------------------------------------------*/

void
VexDataTransmit( union _vexdata  *v )
{
    int  i;
    
    for(i=0;i<VEX_DATA_BUFFER_SIZE;i++)
      serialTxChar( v->buffer[i] );
}


/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* @n
* @n The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* @n
* @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
