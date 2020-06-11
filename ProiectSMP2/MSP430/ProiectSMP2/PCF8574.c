#include "PCF8574.h"
#include "I2C.h"

void PCF8574_WRITE(unsigned char data)
{
    I2C_SET_ADDRESS(PCF8574_actual_address);

    I2C_WRITE_BYTES(&data,1,STOP);


}



