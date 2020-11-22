/*
 * File:   main.c
 * Author: dtek0068
 *
 * Created on 22 November 2020, 16:28
 */
#define F_CPU 3333333
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void RTC_init(void);
void SLPCTRL_init(void);

uint16_t counted_seconds = 0;

static void USART0_sendChar(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
    {
        ;
    }
    USART0.TXDATAL = c;
}

static int USART0_printChar(char c, FILE *stream)
{ 
    USART0_sendChar(c);
    return 0; 
}

static FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

static void USART0_init(void)
{
    PORTA.DIR |= PIN0_bm;
    
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600); 
    
    USART0.CTRLB |= USART_TXEN_bm;  
    
    stdout = &USART_stream;
}

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
    // Enable periodic interrupt
    RTC.PITINTCTRL = RTC_PI_bm;
    // Enable the Periodic Interrupt Timer.
    RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;
    // Set IDLE sleep mode
    set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
    // Enable interrupts
    sei();
    while (1)
    {
        USART0_init();
 
        // Sleep mode after each interrupt wake-up
        sleep_mode();
    }
    return 0;
}

// Button press triggers this
ISR(PORTF_PORT_vect)
{
    // If clock is running -> stop it
    if(RTC.PITCTRLA & RTC_PITEN_bm) 
    {
        RTC.PITCTRLA = RTC.PITCTRLA & ~RTC_PITEN_bm;
    }
    else 
    {
        if (counted_seconds) 
        {
            counted_seconds = 0;
            printf("RESET \n\r");
        }
        else 
        {
            // Start the clock
            RTC.PITCTRLA = RTC.PITCTRLA | RTC_PITEN_bm;
        }
    }
    
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xFF;
}
ISR(RTC_PIT_vect)
{
    printf("%d \n\r", counted_seconds);
    PORTF.OUTTGL = PIN5_bm;
    RTC.PITINTFLAGS = RTC_PI_bm;
    counted_seconds++;
}
