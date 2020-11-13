/*
 * File:   main.c
 * Author: dtek0068
 *
 * Created on 12 November 2020, 10:16
 */

#define F_CPU 3333333

#include <avr/io.h>
#include <util/delay.h>

void pwm_period(int duty, int x) 
{
    if (duty > 0) 
    {
        PORTF.OUT = PORTF.OUT | PIN5_bm;
        for (int i = 0; i < duty; i = i + 1) 
        {
            _delay_us(7);
        }
    }
    if (duty < x) 
    {
        PORTF.OUT = PORTF.OUT & ~PIN5_bm;
        for (int j = 0; j < x - duty; j = j + 1) 
        {
            _delay_us(7);
        }
    }
}

int main(void) 
{

    // Led as output
    PORTF.DIRSET = PIN5_bm; 
    // Button as input
    PORTF.DIRCLR = PIN6_bm; 

    int duty = 0;
    int period = 255;
    int current_state = 0; 
    int previous_state = 0; 

    while (1) 
    {
        if (~PORTF.IN & PIN6_bm) 
        {
            if (previous_state == 0) 
            {
                if (current_state == 0) 
                {
                    current_state = 1;
                } else 
                {
                    current_state = 0;
                }
            }
            if (current_state == 0 && duty > 0) 
            {
                duty = duty - 1;
            }
            if (current_state == 1 && duty < 255) 
            {
                duty = duty + 1;
            }
            pwm_period(duty, period);
            previous_state = 1;
        } 
        else 
        {
            pwm_period(duty, period);
            previous_state = 0;
        }
    }
}