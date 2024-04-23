/*
 * Prelab5.c
 *
 * Created: 15/04/2024 01:13:57 a. m.
 * Author : Eber Alexander
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "PWM1/PWM1.h"

void initADC(void);

int main(void)
{
	cli();
	DDRB |= (1 << PORTB2);		//PB6 as output (OC0A and OCR0B)
	/*TCCR1A = 0;
	TCCR1B = 0;*/
	
	DDRC = 0;		//entire port c as input
	
	
	initFastPWM1(settedUp, 8);
	channel(channelB, nop);
	topValue(39999);
	
	/*
	//no inverted and fast PWM, with ICR1 as TOP
	TCCR1A |= (1 << COM1B1);
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	//Prescaler 8
	TCCR1B |= (1 << CS11);
	
	//setting TOP value, 39999
	ICR1 = 39999;
	*/
	
	/*
	uint16_t i = 0;
	//uint8_t restart = 0;
	uint16_t j = 1000;
	OCR1B = j;
	*/
	initADC();
	sei();
	
	while (1)
	{
		ADCSRA |= (1 << ADSC);
		_delay_ms(2);
		
		
		convertServo(ADCH, channelB);
		/*i = ADCH;
		j = (200/12)*i+1000;
		
		OCR1B = j;*/
	}
}




void initADC(void){
	ADMUX = 0;
	//Vref = AVcc = 5Vs
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	
	ADMUX |= (1 << ADLAR);	//left adjust
	
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN);	//turn on ADC
	ADCSRA |= (1 << ADIE);	//interruption
	
	//prescaler 128 > 125kHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	DIDR0 |= (1 << ADC0D);	//disable PC0 digital input
}


ISR (ADC_vect){
	//PORTD = ADCH;			//show in portd value of adc
	ADCSRA |= (1 << ADIF);	//turn off flag
}
