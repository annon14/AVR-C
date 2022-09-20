// author: Saeid
// created: 7/10/2017
// create a RF reciever
#include <mega16.h>
#include <delay.h>
#asm
   .equ __lcd_port=0x1B ;PORTA
#endasm
#include <lcd.h>  
#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
// USART Receiver buffer
#define RX_BUFFER_SIZE 8
char rx_buffer[RX_BUFFER_SIZE];
#if RX_BUFFER_SIZE<256
unsigned char rx_wr_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif 
#include <stdio.h> 
#include <ctype.h>
void get_str(char data_char);

bit rx_buffer_overflow;  
unsigned char count=5; 
unsigned char  oclock=0,minute=0,second=0; 
char temp_time1[8]={0,0,0,0,0,0,0,0};
char temp_time2[8]={0,0,0,0,0,0,0,0};
char temp_time3[8]={0,0,0,0,0,0,0,0};
char temp_time4[8]={0,0,0,0,0,0,0,0};
char temp_time5[8]={0,0,0,0,0,0,0,0};
char temp_time6[8]={0,0,0,0,0,0,0,0};
char temp_time7[8]={0,0,0,0,0,0,0,0};
char temp_time8[8]={0,0,0,0,0,0,0,0};
unsigned char array1=0,array2=0,array3=0,array4=0;   
unsigned char array5=0,array6=0,array7=0,array8=0;
bit start=0;

interrupt [USART_RXC] void usart_rx_isr(){ 
	char lcd_buffer[32];
	char status,data,ok;  
	status=UCSRA;
	data=UDR; 
	get_str(data);
	if(count==10) 
		count=5;
	if(data=='$') 
		goto no_display; 
	if(data=='#'){
		count=5;
		goto no_display; 
	}   
	
	if(data=='.') 
		goto dis_allow; 
	ok=data;
	ok=isdigit(ok);
	if(ok!=1){
		lcd_gotoxy(count,0);
		goto exit;
	} 
	dis_allow:
	//*********************************
	lcd_gotoxy(count,0); 
	lcd_putchar(data); 
	exit:
	count++; 
	no_display: 
	sprintf(lcd_buffer,"Time=%02u:%02u:%02u",oclock,minute,second);
	lcd_gotoxy(0,1);
	lcd_puts(lcd_buffer);
	lcd_gotoxy(count,0); 
	
	if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){
		rx_buffer[rx_wr_index]=data;
		if (++rx_wr_index == RX_BUFFER_SIZE) 
			rx_wr_index=0;
		if (++rx_counter == RX_BUFFER_SIZE){
			rx_counter=0;
			rx_buffer_overflow=1;
		}
	}
}

//***********************************************************
interrupt [TIM1_OVF] void timer1_ovf_isr(void){
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	second++; 
	if (second==60) { 
		second=0;
		minute++; 
		if (minute==60){
			minute=0;
			oclock++;
			if (oclock==24){
				oclock=0;
				}
			}
		}  
} 

//**************************************************

void inc_houre(){ 
	oclock++;
	if(oclock==24) 
		oclock=0; 
	delay_ms(250);
	while(PINC.0==0){}
} 

//**************************************************

void inc_minute(){ 
	minute++;
	if(minute==60) 
		minute=0;  
	delay_ms(250);
	while(PINC.1==0){}
} 

//*************************************************

void display_save(){ 
	#asm("cli") 
	lcd_clear();                
	lcd_gotoxy(0,0); 
	lcd_putsf("T1=00:35 ");
	lcd_puts(temp_time1); 
	lcd_putsf("\xdfC");
	lcd_gotoxy(0,1); 
	lcd_putsf("T2=03:10 ");
	lcd_puts(temp_time2);  
	lcd_putsf("\xdfC");
	delay_ms(4000);
	lcd_clear();               
	lcd_gotoxy(0,0); 
	lcd_putsf("T3=06:30 ");
	lcd_puts(temp_time3);
	lcd_putsf("\xdfC");
	lcd_gotoxy(0,1); 
	lcd_putsf("T4=09:45 ");
	lcd_puts(temp_time4);
	lcd_putsf("\xdfC"); 
	delay_ms(4000);   
	lcd_clear(); 
	lcd_gotoxy(0,0); 
	lcd_putsf("T5=12:15 ");
	lcd_puts(temp_time5);
	lcd_putsf("\xdfC");
	lcd_gotoxy(0,1); 
	lcd_putsf("T6=15:20 ");
	lcd_puts(temp_time6);
	lcd_putsf("\xdfC"); 
	delay_ms(4000);   
	lcd_clear(); 
	lcd_gotoxy(0,0); 
	lcd_putsf("T7=18:40 ");
	lcd_puts(temp_time7);
	lcd_putsf("\xdfC");
	lcd_gotoxy(0,1); 
	lcd_putsf("T8=21:30 ");
	lcd_puts(temp_time8);
	lcd_putsf("\xdfC"); 
	delay_ms(4000);   
	lcd_clear(); 
	lcd_gotoxy(0,0); 
	lcd_putsf("Temp=     \xdfC         ");
	count=5;
	#asm("sei") 
}

//********************************************
void get_str(char data_char){ 
	if(data_char=='#') start=0; 
	if(data_char=='$'){
		start=1;
		goto no_save;
	}
	if(start==1){
		if(oclock==0 && minute==35 && array1<5 ){ 
			temp_time1[array1]=data_char;
			array1++; 
		if(array1==3 && temp_time1[2]!='.') 
			array1=0; 
	} 
	if(oclock==3 && minute==10 && array2<5 ){
		temp_time2[array2]=data_char;
		array2++; 
		if(array2==3 && temp_time2[2]!='.') 
			array2=0; 
	} 
	if(oclock==6 && minute==30 && array3<5 ){ 
		temp_time3[array3]=data_char;
		array3++; 
	if(array3==3 && temp_time3[2]!='.') 
		array3=0; 
	} 
	if(oclock==9 && minute==45 && array4<5 ){
		temp_time4[array4]=data_char;
		array4++;
		if(array4==3 && temp_time4[2]!='.') 
			array4=0;  
	} 
	if(oclock==12 && minute==15 && array5<5 ){
		temp_time5[array5]=data_char;
		array5++;
		if(array5==3 && temp_time5[2]!='.') 
			array5=0;  
	} 
	if(oclock==15 && minute==20 && array6<5 ){
		temp_time6[array6]=data_char;
		array6++;
		if(array6==3 && temp_time6[2]!='.') 
			array6=0;  
	} 
	if(oclock==18 && minute==40 && array7<5 ){
		temp_time7[array7]=data_char;
		array7++;
		if(array7==3 && temp_time7[2]!='.') 
			array7=0;  
	  } 
	if(oclock==21 && minute==30 && array8<5 ){
		temp_time8[array8]=data_char;
		array8++;
		if(array8==3 && temp_time8[2]!='.') 
			array8=0;  
	  } 
	}   
	no_save: 
}             
//_________________________________________________
void main(){ 
	DDRD=0X00;
	PORTD=0XFF;
	DDRC=0X00;
	PORTC=0XFF;  
	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: Off
	// USART Mode: Asynchronous
	// USART Baud rate: 110
	UCSRA=0x00;
	UCSRB=0x90;
	UCSRC=0x86;
	UBRRH=0x08;
	UBRRL=0xE0;
	// Clock value: 62.500 kHz
	// Timer 1 Overflow Interrupt: On
	TCCR1A=0x00;
	TCCR1B=0x04;
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	TIMSK=0x04;
	lcd_init(16);
	lcd_putsf("Temp=~... \xdfC");
	lcd_gotoxy(0,0);
	delay_ms(500); 
	#asm("sei")
	while(1){
		if(PINC.0==0) 
			inc_houre(); 
		if(PINC.1==0) 
			inc_minute(); 
		if(PINC.2==0) 
			display_save(); 
	};
} 