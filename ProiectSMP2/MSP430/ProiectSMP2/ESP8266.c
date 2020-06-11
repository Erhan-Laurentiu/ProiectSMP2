#include"ESP8266.h"

void ESP8266_RESET()
{
    P2DIR|=BIT0; //OUTPUT HIGH
    P2REN|=BIT0;
    P2OUT|=BIT0;

    __delay_cycles(10000);
    P2OUT&=~BIT0; //OUTPUT LOW
    __delay_cycles(10000);
    P2OUT|=BIT0;
    P2DIR&=~BIT0; // INPUT PULLED UP

}

void ESP8266_PIN_INIT()
{
    P2DIR|=BIT0; // INPUT PULLED UP
    P2REN|=BIT0;
    P2OUT|=BIT0;

}
