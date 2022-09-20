// author : saeid
// created: 7/7/2017

// make a Amper meter 
#include <mega16.h>
#include <stdio.h>
#include <delay.h> 
#include <math.h>
#asm
   .equ __lcd_port=0x18 ;PORTB
#endasm
#include <lcd.h>
#define ADC_VREF_TYPE 0X40
#define relay PORTC.3
unsigned int adc_data=0;      
eeprom float v_ref_E=0.0;
float v_ref=0.0,set_amper=0.0;
unsigned char set_state=0;
//----------------------------------------------
interrupt [ADC_INT] void adc_isr(void){
adc_data=ADCW;
}
//______________________________________________
void dec_vref(){
if(v_ref>=2.6) v_ref-=0.0024; 
}
//-----------------------------------------------
void inc_vref(){ 
if(v_ref<=2.8) v_ref+=0.0024; 
}
//----------------------------------------------
void set(){
unsigned char set_loop=1; 
char lcd_buf_s[32]; 
lcd_clear();  
lcd_gotoxy(0,0);
lcd_putsf("set-> Inc or Dec"); 
while(PINC.2==0) {}; 
while(set_loop){
if((PINC.1==0) && (set_amper>=-20.1)) set_amper-=0.1; 
if((PINC.0==0) && (set_amper<=20.1))  set_amper+=0.1; 
sprintf(lcd_buf_s,"set=%3.1f%CA",set_amper);
lcd_clear();  
lcd_puts(lcd_buf_s);
delay_ms(250);
if(PINC.2==0) set_loop=0; 
 } 
lcd_clear();                   
lcd_gotoxy(0,0);
lcd_putsf("set-> ok");  
set_state=1;
while(PINC.2==0) {};  
}
//______________________________________________
void r_sensor(){
float amper;
char lcd_buf[32]; 
ADCSRA=0XCD;
delay_ms(50);
amper=adc_data*5.0/1024; 
amper=(amper-v_ref)*100/7.8; 
if(set_state==1){ 
  if(fabs(amper)>=fabs(set_amper)) relay=1;
  else relay=0;
}  
sprintf(lcd_buf,"current=%3.3f%CA",amper);
lcd_clear();  
lcd_puts(lcd_buf);
}
//______________________________________
void set_ref(){ 
//float volt;
lcd_gotoxy(0,0);
lcd_putsf("Set Reference");
ADCSRA=0XCD;
delay_ms(100);
v_ref=adc_data*5.0/1024; //5.0049
v_ref_E=v_ref; 
delay_ms(10); 
lcd_putsf(" OK");
delay_ms(1000); 
}
//----------------------------------
void main(){ 
DDRC.0=0;
DDRC.1=0; 
DDRC.2=0;
DDRC.3=1;
relay=0;
ACSR=0X80;
SFIOR=0X00;
ADMUX=ADC_VREF_TYPE;
ADCSRA=0X8D; 
lcd_init(16);
#asm("sei")  
if(PINC.2==0) set_ref(); 
while(PINC.2==0); 
lcd_clear();
v_ref=v_ref_E;
delay_ms(10); 
while(1){ 
r_sensor(); 
delay_ms(450);    
if(PINC.0==0) dec_vref(); 
if(PINC.1==0) inc_vref(); 
if(PINC.2==0) set(); 
 };
}  