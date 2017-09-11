// original sketch from http://educ8s.tv/arduino-real-time-clock/
// Nicu FLORICA (niq_ro) from http://www.arduinotehniq.com 
// made some chenages for animate display with blinking signs
// eliminate library for clock, use just subroutines
// use 3 buttons for manual adjust the data & clock
// add alarm
// put humidity and temperature from DHT sensor

//#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET 0 // Can alternately just connect to Arduino's reset pin

//#define TFT_CS     9
//#define TFT_RST    7                      
//#define TFT_DC     8
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
//#define TFT_SCLK 13   // set these to be whatever pins you like!
//#define TFT_MOSI 11   // set these to be whatever pins you like!

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

char timeChar[100];
char dateChar[50];
char temperatureChar[10];

float temperature = 0;
float previousTemperature = 0;

String dateString;
int minuteNow=0;
int minutePrevious=0;


#define DS3231_I2C_ADDRESS 104
byte tMSB, tLSB;
float temp3231;
float temperatura, temperatura0;
byte hh, mm, ss;
byte yy, ll, dd, zz;

#define meniu 10 
#define minus 12
#define plus 11
//#define alarm 5
//#define buzzer 6
int nivel = 0;   // if is 0 - clock
                 // if is 1 - hour adjust
                 // if is 2 - minute adjust

int hh1, mm1, zz1, dd1, ll1, yy1;   
int maxday;
boolean initial = 1;
char chartemp[3];
//int hha, mma;   // for alarm
//byte al;     // for alarm 
//byte xa = 80;
//byte ya = 70;

#include "DHT.h"
#define DHTPIN 13     // what pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
///#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);
//float temperatura;
int umiditate;
int tzeci, tunit, tzecimi, trest;
int tsemn, ttot;
int hzeci, hunit;
int previousHumidity = 0;
char umiditateChar[10];
unsigned long masurare;
unsigned long intervalmasurare = 500;
uint16_t g_identifier;
void setup () 
{
    dht.begin(); 
   // tft.initR(INITR_BLACKTAB);

 uint16_t ID = tft.readID(); //
   tft.begin(ID);  
    tft.setRotation(1);
    tft.fillScreen(BLACK);
    Serial.begin(9600);
    Wire.begin();
pinMode(meniu, INPUT); 
pinMode(plus, INPUT); 
pinMode(minus, INPUT); 
//pinMode(alarm, INPUT); 
//pinMode(buzzer, OUTPUT);
digitalWrite(meniu, HIGH);  // put inputs in high state (when push is to ground -> low state)
digitalWrite(plus, HIGH);
digitalWrite(minus, HIGH);
//digitalWrite(alarm, HIGH);
//digitalWrite(buzzer, LOW);
    printText("TEMPERATURA", RED,35,165,1);  // Temperature Static Text
    printText("UMIDITATE", GREEN,225,165,1);
printText("Ceas si clima cu Arduino, DS1307, DHT11 V1.01",BLUE,10,223,1);
printText("Adaptat de Vlad Gheorghe @2017",BLUE,130,233,1);
// setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
//setDS3231time(0, 45, 13, 0, 25, 11, 2016;

//  hha = 7;
//  mma = 00;
 // al = 1;  // 0 = alarm is off (must put in 1 foar active)

temperature = dht.readTemperature();
umiditate = dht.readHumidity();
masurare = millis();
}


void loop () 
{

if (nivel == 0)
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  hh1=hh;
  mm1=mm;
  zz1=zz;
  yy1=yy;
  ll1=ll;
  dd1=dd;
  delay(500);
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,128,160,BLACK);
  }

if ((millis() - masurare) > intervalmasurare)
{
temperature = dht.readTemperature();
umiditate = dht.readHumidity();
delay(500);
masurare = millis();
}

  readDS3231time(&ss, &mm, &hh, &zz, &dd, &ll,&yy);

  /*  String halarma = "";
    if(hha<10)
    {
         halarma = halarma+" "+String(hha);
    }else  
    halarma = halarma+ String(hha);
    if(mma<10)
    {
        halarma = halarma+":0"+String(mma);
    }else
    {
        halarma = halarma+":"+String(mma);
    }
    halarma.toCharArray(timeChar,100);
*/
//if (digitalRead(alarm) == LOW)
//  {
 // al = al + 1;
 // delay(500);
 // }
////if (mma == mm & hha == hh & al%2)
///{
//digitalWrite(buzzer, HIGH);
 //  tft.drawCircle(xa, ya, 5, ST7735_RED); 
 //  tft.drawLine(xa, ya, xa, ya-5, ST7735_RED);
 //  tft.drawLine(xa, ya, xa+4, ya+4, ST7735_RED); 
 //  printText(timeChar, ST7735_RED,xa+7,ya,1);
//}
//else
//{
//if (al%2 == 0)
//{
  //  tft.drawCircle(xa, ya, 5, ST7735_BLUE); 
 //   tft.drawLine(xa, ya, xa, ya-5, ST7735_BLUE);
//    tft.drawLine(xa, ya, xa+4, ya+4, ST7735_BLUE); 
//  digitalWrite(buzzer, LOW);
 //   printText(timeChar, BLUE,xa+7,ya,1);

//if (al%2 == 1)
//{
  //  tft.drawCircle(xa, ya, 5, WHITE); 
   // tft.drawLine(xa, ya, xa, ya-5, WHITE);
  //  tft.drawLine(xa, ya, xa+4, ya+4, WHITE); 
 // digitalWrite(buzzer, LOW);
 //   printText(timeChar, WHITE,xa+7,ya,1);
//}
//}
 
  minuteNow = mm;
  if(minuteNow!=minutePrevious || initial)
  {
    initial = 0;
    dateString = getDayOfWeek(zz)+", ";
    dateString = dateString+String(dd)+" - "+month(ll);
    dateString= dateString+" - 20"+ String(yy); 
    minutePrevious = minuteNow;
    String hours = "";
   if(hh<10)
   {
         hours = hours+"0"+String(hh);
    }else  
    hours = hours+ String(hh);
    if(mm<10)
    {
        hours = hours+" 0"+String(mm);
    }else
    {
        hours = hours+" "+String(mm);
    }
    
    hours.toCharArray(timeChar,100);
    tft.fillRect(0,0,320,135,BLACK);
  //  printText(timeChar, WHITE,20,25,3);
  tft.setFont(&FreeSansBold18pt7b);
    printText(timeChar, CYAN,20,125,3);
     tft.setFont();
    dateString.toCharArray(dateChar,50);
    printText(dateChar, MAGENTA,5,10,2);
  }
  
  if(temperature != previousTemperature)
  {
    previousTemperature = temperature; tft.setFont(&FreeSansBold18pt7b);
    String temperatureString = String(temperature,1);
    temperatureString.toCharArray(temperatureChar,10);
    tft.fillRect(0,180,150,37,BLACK);
    printText(temperatureChar, RED,10,210,1);
   // printText("o", RED,110,190,1);
    printText("C", RED,100,210,1);
     tft.drawCircle(90,190,5,RED);
     tft.drawCircle(90,190,4,RED);
  }tft.setFont();

  if(umiditate != previousHumidity)
  {
    previousHumidity = umiditate;
    String umiditateString = String(umiditate);
    umiditateString.toCharArray(umiditateChar,10);
    tft.fillRect(190,180,290,37,BLACK);tft.setFont(&FreeSansBold18pt7b);
    printText(umiditateChar, GREEN,195,210,1);
    printText("%RH", GREEN,240,210,1);
    tft.setFont();
  }


/*delay(500);
     printText("o", YELLOW,145,65,3);
     printText("o", YELLOW,145,95,3);

delay(500);
     printText("o", BLACK,145,65,3);
     printText("o", BLACK,145,95,3);
     */
     delay(500);
//     printText("o", WHITE,62,28,1);
//     printText("o", WHITE,62,35,1);
   //  printText("o", ORANGE,225,75,4);
     tft.fillCircle(147,65,8,CYAN);
     tft.fillCircle(147,115,8,CYAN);
   //  printText("o", ORANGE,225,105,4);

delay(500);
    // printText("o", BLACK,225,75,4);
    // printText("o", BLACK,225,105,4);
    tft.fillCircle(147,65,7,BLACK);
     tft.fillCircle(147,115,7,BLACK);
} // end usual case (clock)


if (nivel == 1)   // change hours
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
   delay(500);
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,128,160,BLACK);
  }    

printText("SETTING", GREEN,10,18,2);  // Temperature Static Text
printText("HOUR:", GREEN,10,36,2);  // Temperature Static Text
dtostrf(hh1,3, 0, chartemp); 
      //tft.fillRect(50,50,70,18,ST7735_BLACK);
      tft.fillRect(50,50,70,38,BLACK);
      delay(50);
      printText(chartemp, WHITE,60,50,2);
      delay(50);
      
if (digitalRead(plus) == LOW)
    {
    hh1 = hh1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    hh1 = hh1-1;
    delay(150);
    }    
if (hh1 > 23) hh1 = 0;
if (hh1 < 0) hh1 = 23;       
     
}  // end loop nivel = 1 (change the hours)

if (nivel == 2)   // change minutes
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,128,160,BLACK);
  }    

printText("SETTING", GREEN,10,18,2);  // Temperature Static Text
tft.fillRect(10,35,70,18,BLACK);
printText("MINUTE:", GREEN,10,35,2);  // Temperature Static Text
Serial.println(mm1);
dtostrf(mm1,3, 0, chartemp); 
      tft.fillRect(50,50,70,18,BLACK);
      delay(50);
      printText(chartemp, WHITE,60,50,2);
      delay(50);
     
if (digitalRead(plus) == LOW)
    {
    mm1 = mm1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    mm1 = mm1-1;
    delay(150);
    }    
if (mm1 > 59) mm1 = 0;
if (mm1 < 0) mm1 = 59;       
     
}  // end loop nivel = 2 (change the minutes)

if (nivel == 3)   // change day in week
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,128,160,BLACK);
  }    

printText("SETTING", GREEN,10,18,2);  // Temperature Static Text
tft.fillRect(0,35,128,18,BLACK);
printText("Day in Week:", GREEN,10,40,1);  // Temperature Static Text
//Serial.println(mm1);
dtostrf(zz1,3, 0, chartemp); 
      tft.fillRect(50,50,70,18,BLACK);
      delay(50);
      printText(chartemp, WHITE,60,50,2);
tft.fillRect(10,80,100,18,BLACK);      
if (zz1 == 1) printText("1 - Luni", RED,10,80,1);  // Temperature Static Text
if (zz1 == 2) printText("2 - Marti", RED,10,80,1);  // Temperature Static Text
if (zz1 == 3) printText("3 - Miercuri", RED,10,80,1);  // Temperature Static Text
if (zz1 == 4) printText("4 - Joi", RED,10,80,1);  // Temperature Static Text
if (zz1 == 5) printText("5 - Vineri", RED,10,80,1);  // Temperature Static Text
if (zz1 == 6) printText("6 - Simbata", RED,10,80,1);  // Temperature Static Text
if (zz1 == 0) printText("0 - Duminica", RED,10,80,1);  // Temperature Static Text  
    delay(50);
     
if (digitalRead(plus) == LOW)
    {
    zz1 = zz1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    zz1 = zz1-1;
    delay(150);
    }    
if (zz1 > 6) zz1 = 0;
if (zz1 < 0) zz1 = 6;       
     
}  // end loop nivel = 3 (change the day of the week)

if (nivel == 4)   // change year
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,128,160,BLACK);
  tft.fillRect(10,80,100,18,BLACK);    
  }    

printText("SETTING", GREEN,10,18,2);  // Temperature Static Text
tft.fillRect(10,35,70,18,BLACK);
printText("YEAR:", GREEN,10,35,2);  // Temperature Static Text
dtostrf(yy1,3, 0, chartemp); 
      tft.fillRect(40,50,80,18,BLACK);
      tft.fillRect(10,80,100,18,BLACK);  // erase last explication...
      delay(50);
      printText("20", WHITE,45,50,2);
      printText(chartemp, WHITE,60,50,2);
      delay(50);

     
if (digitalRead(plus) == LOW)
    {
    yy1 = yy1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    yy1 = yy1-1;
    delay(150);
    }    
if (yy1 > 49) yy1 = 49;
if (yy1 < 16) yy1 = 16;       
     
}  // end loop stare = 4 (change the year)


if (nivel == 5)   // change mounth
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
  tft.fillScreen(BLACK);
   tft.fillRect(10,80,100,18,BLACK);    
  }    
printText("SETTING", GREEN,10,18,2);  // Temperature Static Text
tft.fillRect(10,35,70,18,BLACK);
printText("MONTH:", GREEN,10,35,2);  // Temperature Static Text
dtostrf(ll1,3, 0, chartemp); 
      tft.fillRect(40,50,80,18,BLACK);
      tft.fillRect(10,80,100,18,BLACK);  // erase last explication...
      delay(50);
      printText(chartemp, WHITE,60,50,2);
      delay(50);
     
if (digitalRead(plus) == LOW)
    {
    ll1 = ll1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    ll1 = ll1-1;
    delay(150);
    }    
   
if (ll1 > 12) ll1 = 1;
if (ll1 < 1) ll1 = 12;       
     
}  // end loop stare = 5 (change the day as data)

if (nivel == 6)   // change day as data
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
  tft.fillScreen(BLACK);
 tft.fillRect(10,80,100,18,BLACK);      
  }    

printText("SETTING", GREEN,10,18,2);  // Temperature Static Text
tft.fillRect(10,35,70,18,BLACK);
printText("DAY:", GREEN,10,35,2);  // Temperature Static Text
dtostrf(dd1,3, 0, chartemp); 
      tft.fillRect(50,50,70,18,BLACK);
      tft.fillRect(10,80,100,18,BLACK);  // erase last explication...
      delay(50);
      printText(chartemp, WHITE,60,50,2);
      delay(50);
 
if (digitalRead(plus) == LOW)
    {
    dd1 = dd1+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    dd1 = dd1-1;
    delay(150);
    }    
    
 // Dawn & Dusk controller. http://andydoz.blogspot.ro/2014_08_01_archive.html
 if (ll == 4 || ll == 5 || ll == 9 || ll == 11) { //30 days hath September, April June and November
    maxday = 30;
  }
  else {
  maxday = 31; //... all the others have 31
  }
  if (ll ==2 && yy % 4 ==0) { //... Except February alone, and that has 28 days clear, and 29 in a leap year.
    maxday = 29;
  }
  if (ll ==2 && ll % 4 !=0) {
    maxday = 28;
  }

if (dd1 > maxday) dd1 = 1;
if (dd1 < 1) dd1 = maxday;       
     
}  // end loop nivel = 6 (change the day as data)

//if (nivel == 7)   // change hours alarm
//{
//i//f (digitalRead(meniu) == LOW)
  //{
//  nivel = nivel+1;
//  delay(500);
 // tft.fillScreen(ST7735_BLACK);
  //tft.fillRect(0,0,128,160,ST7735_BLACK);
  //}    
/*tft.fillRect(10,15,100,38,ST7735_BLACK);
printText("ALARM", ST7735_GREEN,10,18,2);  // Temperature Static Text
printText("HOUR:", ST7735_GREEN,10,36,2);  // Temperature Static Text
dtostrf(hha,3, 0, chartemp); 
      //tft.fillRect(50,50,70,18,ST7735_BLACK);
      tft.fillRect(50,50,70,38,ST7735_BLACK);
      delay(50);
      printText(chartemp, ST7735_WHITE,60,50,2);
      delay(50);
      
if (digitalRead(plus) == LOW)
    {
    hha = hha+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    hha = hha-1;
    delay(150);
    }    
if (hha > 23) hha = 0;
if (hha < 0) hha = 23;       
     
}  // end loop nivel = 7 (change the hours)

if (nivel == 8)   // change minutes for alarm
{
if (digitalRead(meniu) == LOW)
  {
  nivel = nivel+1;
  delay(500);
  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(0,0,128,160,ST7735_BLACK);
  }    
tft.fillRect(10,15,70,18,ST7735_BLACK);
printText("ALARM", ST7735_GREEN,10,18,2);  // Temperature Static Text
tft.fillRect(10,35,70,18,ST7735_BLACK);
printText("MINUTE:", ST7735_GREEN,10,35,2);  // Temperature Static Text
Serial.println(mm1);
dtostrf(mma,3, 0, chartemp); 
      tft.fillRect(50,50,70,18,ST7735_BLACK);
      delay(50);
      printText(chartemp, ST7735_WHITE,60,50,2);
      delay(50);
     
if (digitalRead(plus) == LOW)
    {
    mma = mma+1;
    delay(150);
    }
if (digitalRead(minus) == LOW)
    {
    mma = mma-1;
    delay(150);
    }    
if (mma > 59) mma = 0;
if (mma < 0) mma = 59;       
     
}  // end loop nivel = 8 (change the minutes)

*/
 if (nivel >=7)  // readfy to return to main loop
 {
  setDS3231time(0, mm1, hh1, zz1, dd1, ll1, yy1);
  nivel = 0;  
// omm = 99;
 previousTemperature= 0.0;
 initial = 1;
    //printText("TEMPERATURE", GREEN,5,185,1);  // Temperature Static Text
   // printText("HUMIDITY", RED,5,130,1);      // Humidity Static Text
 printText("TEMPERATURA", RED,35,165,1);  // Temperature Static Text
    printText("UMIDITATE", GREEN,225,165,1);
printText("Ceas si clima cu Arduino, DS1307, DHT11 V1.01",BLUE,10,223,1);
printText("Adaptat de Vlad Gheorghe @2017",BLUE,130,233,1);
 
 }
}  // end main loop


void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}

String getDayOfWeek(int i)
{
  switch(i)
  {
  /*  case 0: return "Luni";break;
    case 1: return "Marti";break;
    case 2: return "Miercuri";break;
    case 3: return "Joi";break;
    case 4: return "Vineri";break;
    case 5: return "Simbata";break;
    case 6: return "Duminica";break;
    default: return " *** ";break;
*/
    case 0: return "Sunday";break;
    case 1: return "Monday";break;
    case 2: return "Tuesday";break;
    case 3: return "Wednesday";break;
    case 4: return "Thursday";break;
    case 5: return "Friday";break;
    case 6: return "Saturday";break;
    default: return "*****";break; 
  }
}


// http://forum.arduino.cc/index.php?topic=398891.0
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year)
{
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
    return ( (val / 16 * 10) + (val % 16) );
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
    return ( (val / 10 * 16) + (val % 10) );
}

void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}
String month(int i)
{
  switch(i)
  {
 //   case 1: return "IAN";break;
    case 1: return "JAN";break;
    case 2: return "FEB";break;
    case 3: return "MAR";break;
    case 4: return "APR";break;
 // case 5: return "MAI";break;
    case 5: return "MAY";break;
//    case 6: return "IUN";break;
//    case 7: return "IUL";break;
    case 6: return "JUN";break;
    case 7: return "JUL";break;
    case 8: return "AUG";break;
    case 9: return "SEP";break;
    case 10: return "OCT";break;
//    case 11: return "NOI";break;
    case 11: return "NOV";break;
    case 12: return "DEC";break;
    default: return "****";break;
  }
}
