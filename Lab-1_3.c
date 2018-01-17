

/*

* Author: Emmey Teja

* Description: This code will toggle RGB LED each time sw1 is pressed

* Filename: Lab-1_3.c

* Functions: setup(), pin_config(), main(), delay()

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

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.

* Set PORTF Pin 0 and Pin 4 as input for reading SW1 and SW2. SW1 is connected to PF4 and SW2 to PF0.

* Internal Pull-up resistor for Pin 0 and Pin 4 of PORTF is activated by GPIOPadConfigSet() function

* Example Call: led_pin_config();

*/
void pin_config(void)
{
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

/*

* Function Name: delay()

* Input: 8-bit int

* Output: none

* Description: delay(1) correspond to delay of about 0.5s

* Example Call: delay(1);

*/
void delay(uint8_t n)
{
    uint8_t i;
    for (i=0;i<n;i++)
    {
        SysCtlDelay(6700000);
    }
}

/*
'sw1status' variable is to count the number of times sw1 is pressed
'sw2status' variable is to count the number of times sw2 is pressed
'ui8LED' variable is to toggle LED between RGB
'status' variable is to store PortF_BASE value from GPIO read
*/
int main(void)
{
    uint8_t ui8LED = 2,sw1status=1,sw2status=1;
    int32_t status;
    setup();
    pin_config();
    while(1)
    {
        //Read the Pin 4 and Pin 0 value to check if SW1 or SW2 is pressed
        status = GPIOPinRead(GPIO_PORTF_BASE,0x11);

        //If SW1 is open Pin 4 is at HIGH level. If SW1 is pressed Pin 4 becomes LOW.
        //similarly if SW2 is open Pin 0 is at HIGH level and vice versa.
        //so status = 1 represent SW1 press and status = 16 represent SW2 press.
        while(status==1)
        {
            //check if SW1 is released or still pressed, if not released then continue reading as single press.
            status = GPIOPinRead(GPIO_PORTF_BASE,0x11);
            if(status == 1)continue;
            sw1status = sw1status * 2 ;
        }
        //If SW1 is pressed 4th time cycle back to delay of 0.5s and reset sw1status variable
        if (sw1status > 4)
        {
            sw1status=1 ;
        }
        while(status==16)
        {
            //check if SW2 is released or still pressed, if not released then continue reading as single press.
            status = GPIOPinRead(GPIO_PORTF_BASE,0x11);
            if(status == 16)continue;
            sw2status = sw2status + 1;
            ui8LED=ui8LED*2;
        }
        if (sw2status == 4)
        {
            sw2status=1;
            ui8LED = 2;
        }

        //Turn on LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
        delay(sw1status);

        //Read the Pin 4 and Pin 0 value to check if SW1 or SW2 is pressed
        status = GPIOPinRead(GPIO_PORTF_BASE,0x11);

        //If SW1 is open Pin 4 is at HIGH level. If SW1 is pressed Pin 4 becomes LOW.
        //similarly if SW2 is open Pin 0 is at HIGH level and vice versa.
        //so status = 1 represent SW1 press and status = 16 represent SW2 press.
        while(status==1)
        {
            //check if SW1 is released or still pressed, if not released then continue reading as single press.
            status = GPIOPinRead(GPIO_PORTF_BASE,0x11);
            if(status == 1)continue;
            sw1status = sw1status * 2;
        }
        //If SW1 is pressed 4th time cycle back to delay of 0.5s and reset sw1status variable
        if (sw1status > 4)
        {
            sw1status=1;
        }
        while(status==16)
        {
            //check if SW2 is released or still pressed, if not released then continue reading as single press.
            status = GPIOPinRead(GPIO_PORTF_BASE,0x11);
            if(status == 16)continue;
            sw2status = sw2status + 1;
            ui8LED=ui8LED*2;
        }
        if (sw2status == 4)
        {
            sw2status=1;
            ui8LED = 2;
        }

        //Turn off the LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        delay(sw1status);
    }
}
