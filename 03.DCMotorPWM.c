// author : saeid
// created: 7/3/2017
// write a program to control a DC Motor Using PWM

#include <mega16.h>  
#include <Delay.h>
#asm
   .equ __lcd_port=0x1B ;PORTA
#endasm
#include <lcd.h>
#define  pwm1        PORTD.4 
#define  pwm2        PORTD.5 
unsigned int SP=25,PW=25; 
bit state,stop_pwm;   


void HEX_to_DEC(unsigned char i){
	unsigned char ch;
	 ch = (i / 10);
	 lcd_putchar(ch|0x30); 
	 ch = (i % 10);
	 lcd_putchar(ch|0x30);  
} 
 
//******************** Stop***********************

interrupt [TIM1_OVF] void timer1_ovf_isr(void){  
	lcd_gotoxy(7,0);
	lcd_putsf("Stop "); 
	TCNT1H=0xFF;
	TCNT1L=0xFF;
	pwm1=1;
	pwm2=1;
	stop_pwm=1;
}

//******************** Right/Up ********************

interrupt [EXT_INT0] void ext_int0_isr(void){   
	unsigned int y1;           
	if(PINB.2==1){ 
		lcd_gotoxy(7,0);
		lcd_putsf("Right");
		state=0;
		pwm1=1;
		pwm2=1;
	} 
	else{ 
		if(SP==5) goto EXIT_int0;
		PW+=4;
		SP-=4;
		y1=((PW*5)/25)*10;
		lcd_gotoxy(5,1);
		HEX_to_DEC(y1);
		EXIT_int0: 
	 }  
}
//_____________ Left/Down
interrupt [EXT_INT1] void ext_int1_isr(void){
	unsigned int y2;
	if(PINB.2==1){  
		lcd_gotoxy(7,0);
		lcd_putsf("Left ");
		state=1; 
		pwm1=1;   
		pwm2=1; 
	}
	else{
		if(PW==5)
			goto EXIT_int1;
		PW-=4;
		SP+=4;
		y2=((PW*5)/25)*10;
		lcd_gotoxy(5,1);
		HEX_to_DEC(y2);
		EXIT_int1:
	 }       
}   
         
//********************** Start ***************************

void pwm_generator1(){  
	//TCNT0=0xFF;
	if(state==0){
		lcd_gotoxy(7,0);
		lcd_putsf("Right");
	}
	else{
		lcd_gotoxy(7,0);
		lcd_putsf("Left ");
	}    
	//-----------------------------------------------------
	pulse_pwm1:
	pwm2=0;   
	while(1){
		pwm1=1; 
		if(state==1)    
			goto pulse_pwm2;
		if(stop_pwm==1) 
			goto exit_pwm;
		lcd_gotoxy(8,0);
		delay_ms(PW); 
		pwm1=0;   
		if(state==1)    
			goto pulse_pwm2; 
		if(stop_pwm==1) 
			goto exit_pwm;  
		delay_ms(SP); 
	 }
	 
	//----------------------------------------------------- 
	pulse_pwm2:
	pwm1=0;             
	while(1){
		pwm2=1;
		if(state==0)    
			goto pulse_pwm1; 
		if(stop_pwm==1) 
			goto exit_pwm;     
		delay_ms(PW); 
		pwm2=0;    
		if(state==0)    
			goto pulse_pwm1;
		if(stop_pwm==1) 
			goto exit_pwm;
		delay_ms(SP); 
	 } 
	exit_pwm:
	pwm1=1;
	pwm2=1;
	stop_pwm=0;
} 

//_____________________________________________________________
void main(void){          
	DDRD.4=1;  // out pwm1  
	DDRD.5=1;  // out pwm2  
	DDRB.0=0; 
	DDRB.2=0; 
	PINB.2=1;  
	GICR|=0xC0;
	MCUCR=0x0A;
	MCUCSR=0x00;
	GIFR=0xC0; 
	TCCR1A=0x00;
	TCCR1B=0x06;
	TCNT1H=0xFF;
	TCNT1L=0xFF;
	TIMSK=0x44;    
	lcd_init(16); 
	lcd_gotoxy(0,0);
	lcd_putsf("Control Motor DC"); 
	delay_ms(2000);
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_putsf("State~ Stop");
	lcd_gotoxy(0,1);
	lcd_putsf("RPM=%50");
	#asm("sei") 
	while (1){
		if(PINB.0==0) 
			pwm_generator1();          
	 };
}