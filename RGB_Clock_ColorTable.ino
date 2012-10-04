//RGB Clock with Color Table

#include "Wire.h"
#include "BlinkM_funcs.h"
#include "colortable.h"

int SEC_addr = 0x09;
int MIN_addr = 0x10;
int HR_addr = 0x11;

float r=0, g=0, b=0;
float h=0;

int i = 0; //used for RTC
int x = 0;
int curSec = 0;
int prevSec = 0;

int time=0;

int seconds=0, minutes=0, hours=0;

static RGB SEC = {0,0,0};
static RGB MIN = {0,0,0};
static RGB HR = {0,0,0};

void setup() {  
  Wire.begin();
  Serial.begin(9600);
  //SetClock();
}

void HSVtoRGB( float red, float green, float blue, float h, float s, float v );

void loop() {
  GetTime();  
  //Serial.print("seconds: ");Serial.print(seconds,DEC);
  //Serial.print(" minutes: ");Serial.print(minutes,DEC);
  //Serial.print(" hours: ");Serial.print(hours,DEC);  
  //Serial.print(" hue: ");Serial.print(h,DEC);
  //Serial.print(" x: ");Serial.println(x,DEC);
  //Serial.print("hue: ");Serial.println(h,DEC);
  //Serial.print("R: ");Serial.println(r,DEC);
  //Serial.print("G: ");Serial.println(g,DEC);
  //Serial.print("B: ");Serial.println(b,DEC);
  SEC = colorMinSec[seconds];
  BlinkM_setRGB(SEC_addr,SEC.r,SEC.g,SEC.b);
  MIN = colorMinSec[minutes];
  BlinkM_setRGB(SEC_addr,MIN.r,MIN.g,MIN.b);
  HR = color24h[hours];
  BlinkM_setRGB(HR_addr,HR.r,HR.g,HR.b);
  delay(100);  
}

//Found this at: http://www.cs.rit.edu/~ncs/color/t_convert.html
void HSVtoRGB( float red, float green, float blue, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		red = green = blue = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			red = v;
			green = t;
			blue = p;
			break;
		case 1:
			red = q;
			green = v;
			blue = p;
			break;
		case 2:
			red = p;
			green = v;
			blue = t;
			break;
		case 3:
			red = p;
			green = q;
			blue = v;
			break;
		case 4:
			red = t;
			green = p;
			blue = v;
			break;
		default:		// case 5:
			red = v;
			green = p;
			blue = q;
			break;
	}
        r = red * 255;
        g = green * 255;
        b = blue * 255;
}

static void GetTime()
{
  Wire.beginTransmission(104); // 104 is DS3231 device address
  Wire.write(i); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(104, 3); // request three bytes (seconds, minutes, hours)

  while(Wire.available())
  { 
    seconds = Wire.read(); // get seconds
    minutes = Wire.read(); // get minutes
    hours = Wire.read();   // get hours

    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); // convert BCD to decimal
    minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); // convert BCD to decimal
    hours = (((hours & 0b00110000)>>4)*10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)
  }
}

/*static void GetTimeTEST()
{
  seconds += 5;
  if(seconds > 59)
  {
    seconds=0;
    minutes += 5;
    if(minutes > 59)
    {
      minutes=0;
      hours += 1;
      if(hours > 23)
      {
        hours=0;
      }
    }
  }
}*/

void SetClock()
{
  Wire.beginTransmission(104);
  Wire.write(i);
  Wire.write(0x80); //sec
  Wire.write(0x02 + 0x80); //min
  Wire.write(0x19 + 0x80); //hour (24)
  Wire.endTransmission();
}
