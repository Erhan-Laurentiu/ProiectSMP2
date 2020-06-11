#include"main.h"
volatile float temperature;
volatile float humidity;
volatile float pressure;
volatile float lux;
volatile unsigned int eCO2;
volatile unsigned int tVOC;

int main(void)
{
    WATCHDOG_DISABLE();
    HW_INIT();

    DELAY_TIMER_A2_ACLK(667); //20ms

    SENSORS_INIT();

    DELAY_TIMER_A2_ACLK_LongerPeriod(15); //30s  delay

    UART0_SendChar('z'); //send ESP to deepSleep

    while(1)
    {
        SENSORS_READ();
        SENSORS_SEND_LCD();
        SENSORS_SEND_UART();

    }
}


inline void WATCHDOG_DISABLE()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
}

void HW_INIT()
{
    I2C_INIT();
    UART1_Init();
    UART0_Init();
    ESP8266_PIN_INIT();
    LCD4bit_Init();
    LCD4bit_Cursor_Off();
    LCD4bit_SendString("Waiting Readings");

}


void SENSORS_INIT()
{
    BME280_init();
    initCCS811();
    MAX44009_INIT();
}



void SENSOR_READ()
{
    BME280_READ(&temperature, &humidity, &pressure);
    CCS811_READ(&eCO2 , &tVOC );
    MAX44009_READ(&lux);
}


void SendInt(long sensorValue)
{
    long var=sensorValue;
    int i;
    char buff_uart[10]={0x00};

    for(i=9;i>=0;i--)
    {
        buff_uart[i]=(var%10)+'0';
        var=var/10;
    }
    UART0_SendString_KnownLenght(buff_uart,10); //easier on the memory than sprintf
}


void SENSOR_SEND_UART1(){
    char ___buff[25];
    sprintf(___buff,"Temperature:%.2f\n\r\0",temperature);
    UART1_SendString_UnknownLenght(___buff);
    sprintf(___buff,"Humidity:%.2f\n\r\0",humidity);
    UART1_SendString_UnknownLenght(___buff);
    sprintf(___buff,"Pressure:%.2f\n\r\0",pressure);
    UART1_SendString_UnknownLenght(___buff);
    sprintf(___buff,"Lux:%.2f\n\r\0",lux);
    UART1_SendString_UnknownLenght(___buff);
    sprintf(___buff,"eCO2:%d\n\r\0",eCO2);
    UART1_SendString_UnknownLenght(___buff);
    sprintf(___buff,"tVOC:%d\n\r\0",tVOC);
    UART1_SendString_UnknownLenght(___buff);
    UART1_SendNewLine();
}

void SENSORS_SEND_UART()
{
    ESP8266_RESET();
    DELAY_TIMER_A2_ACLK_LongerPeriod(10); //1 min delay
    SendInt((long)(temperature*100));
    SendInt((long)(humidity*100));
    SendInt((long)((float)(pressure)*1000));
    SendInt((long)((float)(lux)*1000));
    SendInt((long)(eCO2));
    SendInt((long)(tVOC));
    SENSOR_SEND_UART1();
}




void SENSORS_READ()
{

    DELAY_TIMER_A2_ACLK_LongerPeriod(31); //1 min delay
    SENSOR_READ();

}

void SENSORS_SEND_LCD(){
    char _buff[25]={0};

    LCD4bit_Clear();
    sprintf(_buff, "T:%.1fC H:%.1f% \0", temperature,humidity);
    LCD4bit_SendString(_buff);
    LCD4bit_Cursor_Position_V2(0,1);
    sprintf(_buff, "eCO2:%d TVOC:%d\0 ", eCO2,tVOC);
    LCD4bit_SendString(_buff);
}






