#ifndef serialreader_h
#define serialreader_h

#include "Arduino.h"

void checkMessages()
{
    if(Serial.available() > 0)
    {
        lastCheck = millis();
        Serial.print(lastCheck);
        Serial.println(" : New message recieved.");
        char d = Serial.read();
        Serial.println(d);
        switch (d)
        {
            case 'R':
                setColor(red);
                Serial.println("red");
                break;
            case 'G':
                setColor(green);
                Serial.println("green");
                break;
            case 'B':
                setColor(blue);
                Serial.println("blue");
                break;
            case 'Y':
                setColor(yellow);
                Serial.println("yellow");
                break;
            case 'W':
                setColor(white);
                Serial.println("white");
                break;
            default:
            break;
        }
    }
}

#endif