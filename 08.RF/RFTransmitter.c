// author: Saeid
// created: 7/10/2017
// create a RF Transmitter

#include <mega16.h>
#include <delay.h>
#include <stdio.h> 

//****************************************                         
interrupt [ADC_INT] void adc_isr(){ 
	unsigned int adc_data;                  
	float   volt;
	adc_data=ADCW;
	volt=adc_data*2.56/1024;  
	volt=volt*100;  
	if(volt>10) 
		printf("$%2.2f#",volt);
	else  
		printf("$0%2.2f#",volt);
	delay_ms(250);
	ADCSRA=0XCF;
} 
               
//****************************************

void main(){ 
	DDRD=0XFF;
	PORTD=0X00;
	ACSR=0XC0;
	SFIOR=0X00;
	ADMUX=0XC0;
	ADCSRA=0X8F; 
	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: Off
	// USART Transmitter: On
	// USART Mode: Asynchronous
	// USART Baud rate: 110
	UCSRA=0x00;
	UCSRB=0x08;
	UCSRC=0x86;
	UBRRH=0x08;
	UBRRL=0xE0;
	#asm("sei") 
	ADCSRA=0XCF;
	while(1){ };
} 