/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

int main(void)
{
	
	
	//WDTCSR = 0;
	
//	MCUSR &= ~(1<<3);

	//the following is based off of the prescaler change example in the atmega168 datasheet p52

	__asm__ __volatile__ (
	"cli\n\t" \
	"wdr \n\t" \
	"lds r16, 0x60 \n\t" \
	"ori r16, (1<<4) | (1<<3) \n\t" \
	"sts 0x60, r16 \n\t" \
	"ldi r16, (1<<6) | (1<<5) | (1<<0) \n\t" \
	"sts 0x60, r16 \n\t" 
	);
	
	DDRD |= 0xFF;
	PORTD = 0;
	DDRC |= 0xFF;
	PORTC = 0;
	DDRB |= 0XFF;
	PORTB = 0;
	
	DIDR1 |= (1<<1)|(1<<0);
	DIDR0 |= (1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0);
	ACSR |= (1<<7);
	PRR = (1<<7)|(1<<6)|(1<<5)|(1<<3)|(1<<2)|(1<<1)|(1<<0);

	sei();
	
	SMCR = (1<<2); //power down mode
	SMCR |= 1; 
	__asm__ __volatile__ ("sleep");
	
    //we shouldn't really get here ever
    for(;;){
    }

    return 0;   /* never reached */
}

ISR(WDT_vect){
//	__asm__ __volatile__ ( "wdr" );
	cli();
	MCUSR &= ~(1<<3);
	PORTD |= 1;
	_delay_ms(100);
	PORTD &= ~1;
	sei();
	SMCR = (1<<2);
	SMCR |= 1; //sleep!
	__asm__ __volatile__ ("sleep");
}

ISR(BADISR_vect)
{
}