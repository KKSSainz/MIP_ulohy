#include "mbed.h"

DigitalOut myled(LED1);
DigitalOut myButton(PI_11);

#define t_long 0.6
#define t_short 0.2
#define t_blank 1.0

void blinkLED(float dur, float dur2)
{
    myled = 1; // LED is ON
    wait(dur); //
    myled = 0; // LED is OFF
    wait(dur2); //
}

static char *alphabet[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};

int main()
{
    char* strToShow = "victor";    //small characters only
    
    while(true)
    {
        for (int i = 0; i < strlen(strToShow); i++)
        {
            char znak = strToShow[i];
            char * morse = alphabet[znak - 'a'];
            
            for (int j = 0; j < strlen(morse); j++)
            {
                char zn = morse[j];
                if (zn == '.')
                {
                    blinkLED(t_short, t_blank);
                }
                else
                {
                    blinkLED(t_long, t_blank);
                }
            }
        }
        
        
        if(myButton)
        {
            // do Nothing yet
        }   
    }
}
