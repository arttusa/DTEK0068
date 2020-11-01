/*
 * File:   main.c
 * Author: dtek0068
 *
 * Created on 30 October 2020, 10:51
 */


#include <avr/io.h>

int main(void) {
    PORTF.DIR = PORTF.DIR | PIN5_bm; // Sets pin 5 in port F as output
    PORTF.OUT = PORTF.OUT & ~PIN5_bm; // Drive pin 5 in port F LOW
    PORTF.IN = PORTF.IN & PIN6_bm; // Sets pin6 in port F as input
    
    while (1) {
        if(PORTF.IN & PIN6_bm) { // Light on when button is pressed
            PORTF.OUT = PIN5_bm;
        } 
        else {
            PORTF.OUT = ~PIN5_bm;
        }
        
    }
}
