/*
 * File:   main.c
 * Author: dtek0068
 *
 * Created on 13 November 2020, 12:57
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

int main(void)
{
    // Push button PF6 is input
    PORTF.DIRCLR = PIN6_bm; 
    // Button trigger an interrupt when pressed
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
    // LED PF5 is output
    PORTF.DIRSET = PIN5_bm;
    // LED off
    PORTF.OUTTGL = PIN5_bm;
    // Set IDLE sleep mode
    set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
    // Enable interrupts
    sei();
    while (1)
    {
        // Sleep mode after each interrupt wake-up
        sleep_mode();
    }
    return 0;
}

ISR(PORTF_PORT_vect)
{
    // LED is turned on
    PORTF.OUTTGL = PIN5_bm;
    while(1) 
    {
        // Checks if button is released
        if(PORTF.IN & PIN6_bm) 
        {
            // Turn off the LED and break the loop
            PORTF.OUTTGL = PIN5_bm;
            break;
        }
        
    }
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xFF;
}
