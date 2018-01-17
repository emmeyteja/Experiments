
/*

* Author: Emmey Teja

* Description: This code will toggle RGB LED each time sw1 is pressed

* Filename: Lab-1_1.c

* Functions: setup(), pin_config(), main()

* Global Variables: none

*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
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

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.

* Set PORTF Pin 4 as input for reading SW1. Switch 1 is connected to this pin.

* Internal Pull-up resistor for Pin 4 of PORTF is activated by GPIOPadConfigSet() function

* Example Call: pin_config();

*/
void pin_config(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


/*
'count' variable is to count the number of times sw1 is pressed
'ui8LED' variable is to toggle LED between RGB
'status' variable is to store PortF_BASE value from GPIO read
*/
int main(void)
{
    uint8_t ui8LED = 2,count=1;
    int32_t status;
    setup();
    pin_config();
    while(1)
    {
        //Read the Pin 4 value to check if SW1 is pressed
        status = GPIOPinRead(GPIO_PORTF_BASE,0x10);

        //If SW1 is open Pin 4 is at HIGH level. If SW1 is pressed Pin 4 becomes LOW.
        //so status=0 represent switch press.
        while(status == 0)
        {
            //if Switch 1 is pressed turn on the RGB LED
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
            //check if SW1 is released or not, if not then continue reading as single press.
            status = GPIOPinRead(GPIO_PORTF_BASE,0x10);
            if(status == 0)continue;
            count = count + 1;
            ui8LED=ui8LED*2;
        }

	//Turn Off LED if SW1 is not pressed
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

        //If button is pressed 4th time cycle back to Red LED and reset count and ui8LED variables.
        if (count == 4)
        {
            ui8LED=2;
            count=1;
        }
    }
}
