// author : saeid
// created: 7/1/2017
// write a program to count from 0 to 9999 using timer1 and display it on 7seg 

#include <mega16.h>
#include <delay.h> 
unsigned char  part1=0,part2=0,part3=0,part4=0;
unsigned int   number=0;
  
//____________________________________________________________

interrupt [TIM1_OVF] void timer1_ovf_isr(void){
	TCNT1H=0x85;
	TCNT1L=0xEE;
	if(number==9999)
		number=0;
	number++;
}
//____________________________________________________________
unsigned short mask(unsigned short num) {
  switch (num) {
    case 0 :
		return 0x3f;
		break;
    case 1 :
		return 0x06;
		break;
    case 2 :
		return 0x5b;
		break;
    case 3 : 
		return 0x4f;
		break;
    case 4 : 
		return 0x66;
		break;
    case 5 : 
		return 0x6d;
		break;
    case 6 : 
		return 0x7d;
		break;
    case 7 : 
		return 0x07;
		break;
    case 8 : 
		return 0x7f;
		break;
    case 9 : 
		return 0x6f;
		break;
    default: 
		return 0x00;   
		break;		
  } 
}

//_____________________________________________________________
void HEX_to_DEC(unsigned int k){ 
    part1 = (k % 10);
    part1 = mask(part1); 
    part2 = (k / 10)%10;
    part2 = mask(part2);
    part3 = (k / 100)%10;
    part3 = mask(part3);    
    part4 = (k / 100)/10;
    part4 = mask(part4);    
} 
//____________________________________________________________
void main(void){
	PORTA=0x00;
	DDRA=0xff;
	PORTB=0x00;
	DDRB=0xFF;
	PORTC=0x00;
	DDRC=0xff;
	PORTD=0x00;
	DDRD=0x00;
	/* Timer/Counter 1 initialization
	 Clock source: System Clock
	 Clock value: 31.250 kHz
	 Mode: Normal top=FFFFh
	 OC1A output: Discon.
	 OC1B output: Discon.
	 Noise Canceler: Off
	 Input Capture on Falling Edge
	 Timer 1 Overflow Interrupt: On
	 Input Capture Interrupt: Off
	 Compare A Match Interrupt: Off
	 Compare B Match Interrupt: Off
	*/
	TCCR1A=0x00;

	TCNT1H=0x85;
	TCNT1L=0xEE;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;
	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK=0x04;
	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	ACSR=0x80;
	SFIOR=0x00;
	#asm("sei")
	TCCR1B=0x04;
	while (1){        
			HEX_to_DEC(number);
	//--------------------------- 
	PORTC=0b00001110; 
		//    PORTC.0=0;
		  //  PORTC.1=1;
		 //   PORTC.2=1;   
		   // PORTC.3=1;
			PORTA=part1;            
			delay_ms(5); 
	PORTC=0b00001101;         
	 //       PORTC.0=1;
	   //     PORTC.1=0;
		 //   PORTC.2=1;   
		   // PORTC.3=1;   
			PORTA=part2;   
			delay_ms(5);  
	PORTC=0b00001011;          
	 //       PORTC.0=1;
	   //     PORTC.1=1;
		 //   PORTC.2=0;   
		   // PORTC.3=1;
			PORTA=part3;      
			delay_ms(5); 
	PORTC=0b00000111;         
	//        PORTC.0=1;
	  //      PORTC.1=1;
		//    PORTC.2=1;   
		  //  PORTC.3=0;
			PORTA=part4; 
			delay_ms(5); 
	 };
}
