/*
 * File:   main.c
 * Author: dtek0068
 *
 * Created on 08 November 2020, 20:46
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
            _delay_us(10);
        }
    }
    if (duty < x) 
    {
        PORTF.OUT = PORTF.OUT & ~PIN5_bm;
        for (int j = 0; j < x - duty; j = j + 1) 
        {
            _delay_us(10);
        }
    }
}

int main(void) 
{

    // Set the led as output
    PORTF.DIRSET = PIN5_bm;

    int duty = 0;
    int period = 255;

    while (1) 
    {
        for (duty = 1; duty < period; duty = duty + 1) 
        {
            pwm_period(duty, period);
        }
        for (duty = 255; duty > 0; duty = duty - 1) 
        {
            pwm_period(duty, period);
        }
    }
}