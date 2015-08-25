//#include "serialreader.h"

#define REDPIN     5
#define BLUEPIN    6
#define GREENPIN   3

#define MAXCOLOR    255
#define NUMCOLORS   3

int black[3]  = { 0, 0, 0 };
int white[3]  = { 255, 255, 255 };
int red[3]    = { 255, 0, 0 };
int green[3]  = { 0, 170, 0 };
int blue[3]   = { 0, 0, 255 };
int yellow[3] = { 255, 255, 0 };
int dimWhite[3] = { 50, 50, 50 };

int waitingColor[3] = { 0, 0, 255 };

long lastCheck = 0;
long keepalivemax = 180000; // 3*60*1000

int currentColor[3] = { 0, 0, 0 };
int color[3] = { 0, 0, 0 };
int currentDirection = 1;
int speed = 10;
long time = 0;

void setup() 
{
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    Serial.begin(9600);
    setColor(blue);
}

void loop()
{
    checkMessages();
    checkFade();
    if (lastCheck+keepalivemax<millis())
    {
        Serial.print(lastCheck);
        Serial.println(" : Waiting too long for a message. Switching to default color.");
        setColor(waitingColor);
        lastCheck = millis();
    }
}

bool colorCompare(int col1[NUMCOLORS], int col2[NUMCOLORS])
{
    for (int x=0; x<NUMCOLORS; x++)
    {
        if (col1[x]!=col2[x])
            return false;
    }
    return true;
}

void setColor(int newColor[NUMCOLORS])
{
    // do nothing if the new color is the same as the old one.
    if (colorCompare(newColor,color))
        return;
 
    if (colorCompare(newColor,green))
    {
        drawColor(newColor);
    }

    Serial.print("Changing to: ");
    Serial.print(newColor[0]);
    Serial.print(", ");
    Serial.print(newColor[1]);
    Serial.print(", ");
    Serial.println(newColor[2]);
    
    for( int x = 0; x < NUMCOLORS; x++ )
    {
        color[x] = newColor[x];
    }
    currentDirection = -1;
}

void drawColor(int newColor[NUMCOLORS])
{
    analogWrite(REDPIN, newColor[0]);
    analogWrite(GREENPIN, newColor[1]);      
    analogWrite(BLUEPIN, newColor[2]); 
}

void checkFade()
{
    
    if (millis() < time + speed)
        return;
    
    if (colorCompare(color, green))
    {
        return;
    }
    
    for ( int x = 0; x < NUMCOLORS; x++)
    {
       if (currentDirection>0)
       {
           if (currentColor[x]<color[x])
           {
               currentColor[x]+=currentDirection;
           }
       } else {
           if (currentColor[x]>0)
           {
               currentColor[x]+=currentDirection;
           }
       }  
    }
    if (colorCompare(color,currentColor) || colorCompare(black, currentColor))
    {
        currentDirection=-currentDirection;
    }
    
    drawColor(currentColor);
    time = millis();
    
}

