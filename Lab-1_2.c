
/*

* Author: Emmey Teja

* Description: This code is to check if sw2status will always increses by one.

* Filename: Lab-1_2.c

* Functions: setup(), pin_config(), main()

* Global Variables: none

*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Setting clock frequency and enabling PORTF GPIO

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

/*

* Function Name: pin_config()

* Input: none

* Output: none

* Set PORTF Pin 0 as input for reading SW2. Switch 2 is connected to this pin.

* Internal Pull-up resistor for Pin 0 of PORTF is activated by GPIOPadConfigSet() function

* Example Call: pin_config();

*/
void pin_config(void)
{
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


/*
'sw2status' variable is to count the number of times sw2 is pressed
'status' variable is to store PortF_BASE value from GPIO read
*/
int main(void)
{
    uint8_t sw2status=0;
    int32_t status;
    setup();
    pin_config();
    while(1)
    {
        //Read the Pin 0 value to check if SW2 is pressed
        status = GPIOPinRead(GPIO_PORTF_BASE,0x01);

        //If SW2 is open Pin 0 is at HIGH level. If SW2 is pressed Pin 0 becomes LOW.
        //so status=0 represent switch press.
        while(status == 0)
        {
            //check if SW2 is released or not, if not then continue reading as single press.
            status = GPIOPinRead(GPIO_PORTF_BASE,0x01);
            if(status == 0)continue;
            sw2status = sw2status+1;
        }
    }
}
