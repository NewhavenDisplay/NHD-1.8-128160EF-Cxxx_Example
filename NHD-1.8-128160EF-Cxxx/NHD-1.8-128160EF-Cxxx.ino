/*****************************************************************************
* NHD_1_8_128160EF_Parallel.ino
* 
* Program for writing to Newhaven Display 1.8" TFTs with ILI9163 controller.
* This code is written for the Arduino Uno, and uses 8-bit Parallel Interface
* Replace Arduino Uno 5V regulator to a 3.3V.
* This code is applicable for the following Newhaven Display models: 
* 
* NHD-1.8-128160EF-CTXI#
* NHD-1.8-128160EF-CTXI#-F
* NHD-1.8-128160EF-CTXI#-T 
* NHD-1.8-128160EF-CTXI#-FT 
* NHD-1.8-128160EF-CSXN#
* NHD-1.8-128160EF-CSXN#-F 

* Newhaven Display invests time and resources providing this open source code,
* Please support Newhaven Display by purchasing products from Newhaven Display!

* Copyright: Alee Shah (c) 2021, Newhaven Display International

* This code is provided as an example only and without any warranty by Newhaven Display. 
* Newhaven Display accepts no responsibility for any issues resulting from its use. 
* The developer of the final application incorporating any parts of this 
* sample code is responsible for ensuring its safe and correct operation
* and for any consequences resulting from its use.
* See the GNU General Public License for more details. 
* 
*****************************************************************************/

/****************************************************
*         PINOUT: Arduino Uno -> 1.8" TFT           *
*****************************************************/

// The 8 bit data bus is connected to PORTD of the Arduino Uno

////////////////          // /CS signal tied to ground
#define   DC    8         // D/C signal connected to Arduino digital pin 8
#define   WR    9         // /WR signal connected to Arduino digital pin 9
#define   RD    10        // /RD signal connected to Arduino digital pin 10
#define   RST   11        // /RST signal connected to Arduino digital pin 11

/****************************************************
*                 Function Commands                  *
*****************************************************/

void command(unsigned char c)
{
  digitalWrite(DC, LOW);
  PORTD = c;
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}

void data(unsigned char d)
{
  digitalWrite(DC, HIGH);
  PORTD = d;
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}

/****************************************************
*                Advanced Functions                 *
*****************************************************/

void window_set(unsigned s_x, unsigned e_x, unsigned s_y, unsigned e_y)
{
  command(0x2a);      //SET column address
  digitalWrite(RST, HIGH);
  data((s_x)>>8);     //SET start column address
  data(s_x);
  data((e_x)>>8);     //SET end column address
  data(e_x);
  
  command(0x2b);      //SET page address
  digitalWrite(RST, HIGH);
  data((s_y)>>8);     //SET start page address
  data(s_y);
  data((e_y)>>8);     //SET end page address
  data(e_y);
}
//--------------------------------------------------------------------------


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Border + Fill Interior (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void disp2()
{
  unsigned int i,j;
  window_set(0,127,0,0);      //Border on Bottom 
  command(0x2C);              //command to begin writing to frame memory
  digitalWrite(RST, HIGH);
  for(i=0;i<128;i++)                
  {
    for (j=0;j<1;j++)                
    {
    data(0xFF);
    data(0xFF);
    data(0xFF);
    }
  }
  delay(100);
  window_set(0,0,0,159);      //Border on Left side
  command(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<1;i++)                 
  {
    for (j=0;j<160;j++)                
    {
    data(0xFF);
    data(0xFF);
    data(0xFF);
    }
  }
  delay(100);
  window_set(0,127,159,159);  //Border on Top
  command(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<128;i++)               
  {
    for (j=0;j<1;j++)                
    {
    data(0xFF);
    data(0xFF);
    data(0xFF);
    }
  }
  delay(100);
  window_set(127,127,0,159);  //Border on Right side
  command(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<1;i++)
  {
    for (j=0;j<160;j++)                
    {
    data(0xFF);
    data(0xFF);
    data(0xFF);
    }
  }
  delay(100);
  window_set(1,126,1,158);    //Fill Interior Section of Display with Gray Pixels
  command(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<126;i++)
  {
    for (j=0;j<158;j++)                
    {
    data(0x80);
    data(0x80);
    data(0x80);
    }
  }
   delay(100);
}
//--------------------------------------------------------------------------

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fill Screen (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void disp3()
{
  unsigned int i; 
  window_set(0,127,0,159);
  command(0x2C);              //command to begin writing to frame memory
  digitalWrite(RST, HIGH);
        for(i=0;i<20480;i++)  //fill screen with black pixels
  {
            data(0x00);
            data(0x00);
            data(0x00);
  }
  window_set(0,127,0,159);
  command(0x2C);
  digitalWrite(RST, HIGH);
        for(i=0;i<20480;i++)   //fill screen with red pixels
  {
            data(0x00);
            data(0x00);
            data(0xFF);
  }
  delay(100);
  window_set(0,127,0,159);
  command(0x2C);
  digitalWrite(RST, HIGH);
        for(i=0;i<20480;i++)   //fill screen with green pixels
  {
            data(0x00);
            data(0xFF);
            data(0x00);
  }
  delay(100);
  window_set(0,127,0,159);
  command(0x2C);
  digitalWrite(RST, HIGH);
        for(i=0;i<20480;i++)   //fill screen with blue pixels
  {
            data(0xFF);
            data(0x00);
            data(0x00);
  }
  delay(100);
}


/****************************************************
*         Initialization and Setup Routine          *
*****************************************************/

void setup()
{
  DDRB = 0xFF;  //Enable All outputs on PortB   
  PORTB = 0x00;
  DDRD = 0xFF;  //Enable All outputs on PortD 
  PORTD = 0x00;
  
  digitalWrite(RD, HIGH);
  digitalWrite(WR, LOW);
  
  digitalWrite(RST, LOW);
  delay(150);
  digitalWrite(RST, HIGH);
  delay(150);
  
  command(0x11);              //exit SLEEP mode
  delay(100);
  
  command(0x28);              //display off
  
  command(0x26);              //select gamma curve
  data(0x04);
  
  command(0xB1);              //frame rate control
  data(0x0A);
  data(0x14);
  
  command(0xC0);              //power control 1
  data(0x0A); 
  data(0x00);
  
  command(0xC1);              //power control 2
  data(0x02);
  
  command(0xC5);              //VCOM control 1
  data(0x2F); 
  data(0x3E);
  
  command(0xC7);              //VCOM control 2
  data(0x40);
  
  command(0x2A);              //column address set
  data(0x00);                  
  data(0x00);                 //start 0x0000
  data(0x00);
  data(0x7F);                 //end 0x007F
  
  command(0x2B);              //page address set
  data(0x00);                  
  data(0x00);                 //start 0x0000
  data(0x00);
  data(0x9F);                 //end 0x009F
  
  command(0x36);              //memory access control
  data(0xC8);                 //C0 = RGB; C8 = BGR
  
  command(0x3A);              //pixel format = 18 bit per pixel
  data(0x06);                  
  
  command(0x29);              //display ON
  delay(10);
}

/****************************************************
*          Loop function to run repeatedly          *
*****************************************************/

void loop()
{ 
  disp3();
  delay(1000);
  disp2();
  delay(1000);
}
