// author : saeid
// created: 7/2/2017
// write a program to send data to computer and recieve data from it
// using serial port and terminal software

#include <mega16.h>
#asm
  .equ __lcd_port=0x1B ;PORTA
#endasm
#include <lcd.h>
#include <stdio.h>  
unsigned char data=0;


void main(void){   

	// USART Baud rate: 4800
	UCSRA=0x00;
	UCSRB=0x18;
	UCSRC=0x86;
	UBRRH=0x00;
	UBRRL=0x67;
	lcd_init(16); 
	lcd_putsf("test serial");
	printf("Hey Buddy");  
	putchar(':');
	putchar(0X0d);  
	lcd_gotoxy(0,1);
	while (1){  
		data=getchar(); 
		lcd_putchar(data);     
		  };
}