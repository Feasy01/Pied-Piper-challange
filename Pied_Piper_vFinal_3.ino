#include "Adafruit_TCS34725.h"
#include <string.h>

#define COLOUR_TEMP_THRESHOLD 2450
#define COLOUR_TEMP_MAX 5000
#define SPEED_MIN 100
#define SPEED_MAX 255

#define E1 6  //M1 Speed Control
#define E2 5  //M2 Speed Control
#define M1 8  //M1 Direction Control
#define M2 7  //M2 Direction Control
//This setup allows for higher sampling rate (50kHz)
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define LEFT A0
#define RIGHT A2
#define REAR A1
const int n = 300;
int x[n];
int y[n];

Adafruit_TCS34725 tcs;
uint16_t r, g, b, c, colourTemp;

int xcorr(uint8_t mic1, uint8_t mic2)
{
  int i,j,d;
   double mx,my,sx,sy,sxy,denom,r;
   for ( int i = 0; i<n;i++){
    y[i] = analogRead(mic1);
    x[i] = analogRead(mic2);
    
    }
//    for ( int i = 0; i<n;i++){
//    Serial.print("0 1024 ");
//    Serial.print(x[i]);
//    Serial.print(" ");
//    Serial.print(y[i]);
//    Serial.println();
//    }
   /* Calculate the mean of the two series x[], y[] */
   mx = 0;
   my = 0;   
   for (i=0;i<n;i++) {
      mx += x[i];
      my += y[i];
   }
   mx /= n;
   my /= n;

   /* Calculate the denominator */
   sx = 0;
   sy = 0;
   for (i=0;i<n;i++) {
      sx += (x[i] - mx) * (x[i] - mx);
      sy += (y[i] - my) * (y[i] - my);
   }
   denom = sqrt(sx*sy);
    int maxdelay = 12;
    float r1 = 0;
    int dd= 0;
    int summ= 0;
   /* Calculate the correlation series */
   
   
   for(int z = 0; z<5; z++){
    r1 = 0;
   for (d=-maxdelay;d<=maxdelay;d++) {
      sxy = 0;
      for (i=0;i<n;i++) {
         j = i + d;
         if (j < 0 || j >= n)
            continue;
         else
            sxy += (x[i] - mx) * (y[j] - my);
         /* Or should it be (?)
         if (j < 0 || j >= n)
            sxy += (x[i] - mx) * (-my);
         else
            sxy += (x[i] - mx) * (y[j] - my);
         */
      }
      r = sxy / denom;
      if (r>r1){
        r1 = r;
        dd = d;}
//      Serial.print(r);
//      Serial.print(" ");
//      Serial.print(d);
//      Serial.println();
      /* r is the correlation coefficient at "d" */
    
   }
  summ += dd;
      
//   int anagle = map(dd,-10,10,-90,90);
//   delay(20);
   }
   return summ / 5;
}

void setup(void)
{
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.begin(57600);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT,INPUT);
  pinMode(REAR, INPUT);

  for (int i = 5; i <= 8; i++)
  {
    pinMode(i, OUTPUT);
  }

  Serial.begin(115200);

  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    for (;;);
  }
  delay(500);

  // Check if sound source is in front of or behind robot

  // int summ = xcorr(LEFT, RIGHT);

  // if (xcorr(LEFT, REAR) > 2) // behind
  // {
  //   left(255, 90 / 72.0 * 1000); // Flip robot to face correct direction
  //   return;
  // }
  // if (xcorr(RIGHT, REAR) > 2)
  // {
  //   right(255, 90 / 72.0 * 1000);
  //   return;
  // }
// for (;;)
// {
//   tcs.getRawData(&r, &g, &b, &c);
//   Serial.println(tcs.calculateColorTemperature_dn40(r, g, b, c));
// }
  char min_reading[10] = "";

  for (;;)
  {
    int m_left = 1024, m_right = 1024, rear = 1024;

    for (int i = 0; i < 1000; i++) 
    {
      int r = analogRead(m_left);
      if (r < m_left) m_left = r;
      r = analogRead(m_right);
      if (r < m_right) m_right = r;
      r = analogRead(REAR);
      if (r < rear) rear = r;
    }

    int min = min(min(m_left, m_right), rear);
    
    if (min == rear)
    {
      right(250, 180);
    }
    else if (min == m_left)
    {
      if (strcmp(min_reading, "right") == 0) break;
      strcpy(min_reading, "left");
      left(250, 45);
    }
    else
    {
      if (strcmp(min_reading, "left") == 0) break;
      strcpy(min_reading, "right");
      right(250, 45);
    }

    delay(2000);
  }
  // if (front > back)
  // {
  //   left(255, 180 / 72 * 1000);
  // }
  // for (;;)
  // {
  //   Serial.print("0 1024 ");
  //   Serial.print(analogRead(LEFT));
  //   Serial.print(' ');
  //   Serial.print(analogRead(RIGHT));
  //   Serial.print(' ');
  //   Serial.println(analogRead(REAR));
  // }
}

void loop(void)
{
  stop();
   tcs.getRawData(&r, &g, &b, &c);
   colourTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
   
   Serial.print(colourTemp);
    Serial.print(" temperature");
   Serial.println();
   int speed = map(colourTemp, COLOUR_TEMP_THRESHOLD, COLOUR_TEMP_MAX, SPEED_MIN, SPEED_MAX);
   speed = constrain(speed, SPEED_MIN, SPEED_MAX);
  //  int speed2 = (speed / 255.0) * 72.0; // degrees it turns per second
  //  Serial.print(speed2);
  //  Serial.print(" speed2");
   Serial.println();
   int summ = xcorr(LEFT, RIGHT);
   int angle = map(abs(summ), 0, 12, 0, 90);  //degrees it has to turn
   Serial.print(angle);
   Serial.print(" angle");
   Serial.println();
  //  int time = (abs(angle) / ((speed / 255.0) * 72.0)) * 1000;
  //  Serial.print(time);
  //  Serial.print(" time");
   Serial.println();
     if (colourTemp <= COLOUR_TEMP_THRESHOLD)
  {
    stop();
    while(true);
  }
   if(summ > 2){
    right(speed,angle);
    
    }
   else if(summ < (-2)){
    left(speed,angle);
    
    }
   else{
    forward(speed,500);
    }

  delay(500);
}



void stop()          //Stop subroutine
{
  digitalWrite(E1,LOW);
  digitalWrite(E2,LOW);
//  for(;;);
  // delay(60000);
}

void forward(int speed, int time)      //forward subroutine
{
  analogWrite (E1,speed);
  digitalWrite(M1,HIGH);
  analogWrite (E2,speed);
  digitalWrite(M2,HIGH);
  delay(1250);
  stop();
}

//void reverse(int speed, int time)       //reverse subroutine
//{
//  analogWrite (E1,speed);
//  digitalWrite(M1,LOW);
//  analogWrite (E2,speed);
//  digitalWrite(M2,LOW);
//  delay(time);
//  forward(100);
//  stop();
//}

void left(int speed, int angle)     //turn left subroutine
{
  analogWrite (E1,speed);
  digitalWrite(M1,HIGH);
  analogWrite (E2,0);
  digitalWrite(M2,LOW);
  delay((abs(angle) / ((speed / 255.0) * 72.0)) * 1000);
//  forward(255,150);
  stop();
}

void right(int speed, int angle)    //turn right subroutine
{
  analogWrite (E1,0);
  digitalWrite(M1,LOW);
  analogWrite (E2,speed);
  digitalWrite(M2,HIGH);
  delay((abs(angle) / ((speed / 255.0) * 72.0)) * 1000);
//  forward(255,150);
  stop();
}
