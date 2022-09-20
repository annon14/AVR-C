// author : saeid
// created: 7/8/2017
// make simple scientific calculator

#define   pp    3.1415927   
#define SIN   PINC.0 
#define COS   PINC.1 
#define TAN   PINC.2 
#define LOG   PINC.3 
#define SHIFT PINC.4 
#include <mega16.h> 
#include <delay.h>
#include <stdio.h>  
#include <stdlib.h>   
#include <string.h>   
#include <math.h>
#asm
   .equ __lcd_port=0x1B ;PORTA
#endasm

#include <lcd.h> 
#include "keypad.h"  
float  number1 =0; 
float  number2 =0;
float  result=0.0;    
static char last_key;
unsigned char mmz=0,count=0,num_dis=0;    //hasl momayez ra taeein mikonad    
char *save_str="";  //reshteh asli baray zakhireh addad vared shodeh 

//******************************************************

void display(){
	char lcd_buf[32]; 
	sprintf(lcd_buf,"%f",number1);   
	lcd_clear(); 
	lcd_gotoxy(0,0);
	lcd_puts(lcd_buf); 
	delay_ms(500);  
}

//*******************************************************

void config_number(float f_number){
	static float divsion[]={1,10,100,1000,10000,100000,1000000}; 
	f_number=(f_number/divsion[mmz]);
	result=f_number; 
}

//******************************************************* 

void sin_DRG(){
	number1 = atof(save_str);
	*save_str=NULL;                
	config_number(number1);
	number1=result;  
	number1=(number1*pp)/180;
	number1=sin(number1); 
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';
	display(); 
	while(SHIFT==1 && SIN==0) {};  
}

//*******************************************************

void arcsin_DRG(){ 
	number1 = atof(save_str);
	*save_str=NULL;          
	config_number(number1);
	number1=result; 
	number1=asin(number1); 
	number1=(number1*180)/pp;
	display();
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';  
	while(SHIFT==0 && SIN==0) {}; 
}

//*******************************************************

void cos_DRG(){
	number1 = atof(save_str);
	*save_str=NULL;                
	config_number(number1);
	number1=result;  
	number1=(number1*pp)/180;
	number1=cos(number1);
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';
	display();
	while(SHIFT==1 && COS==0) {}; 
} 
   
//*******************************************************

void arccos_DRG(){ 
	number1 = atof(save_str);
	*save_str=NULL;                
	config_number(number1);
	number1=result;  
	number1=acos(number1); 
	number1=(number1*180)/pp;
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';       
	display(); 
	while(SHIFT==0 && COS==0) {}; 
}

//*******************************************************

void tan_DRG(){    
	number1 = atof(save_str);
	*save_str=NULL;                
	config_number(number1);
	number1=result;  
	number1=(number1*pp)/180;
	number1=tan(number1); 
	number1=number1+0.0001; 
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';  
	display(); 
	while(SHIFT==1 && TAN==0) {}; 
}  

//*******************************************************

void arctan_DRG(){  
	number1 = atof(save_str);
	*save_str=NULL;                
	config_number(number1);
	number1=result;  
	number1=atan(number1); 
	number1=(number1*180)/pp;
	number1=number1+0.0001; 
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';  
	display(); 
	while(SHIFT==0 && TAN==0) {};  
}  

//*******************************************************

void log_10(){
	number1 = atof(save_str);
	*save_str=NULL;                
	config_number(number1);
	number1=result;  
	number1=log10(number1); 
	num_dis=1;
	count=0;
	mmz=0;
	last_key='+';  
	display();
	while(LOG==0) {}; 
}

//*******************************************************

signed char test_key (char key){ 
	if ((key == '.')|| ((key >= '0') && (key <= '9')))
		return 1;
	else 
		return 0;
 }
 
//*******************************************************

void ack_opretion(char opretion){
   signed char res = 0;
   switch (opretion)
    { 
		case '+' :
			number1 += number2; 
			break;
		case '-' : 
			number1 -= number2;
			break;
		case '*' :
			number1 *= number2;
			break;
		case '%' : 
			if (number2 != 0) 
				number1 /= number2;
			else
				res = 2;
			break;      
    }
   if (res == 0){ 
      if ((number1 >= -999999999) && (number1 <= 999999999))
		  display(); 
      else {
		  lcd_gotoxy(0,0);
		  lcd_putsf("error number");
		  delay_ms(1000);
		  lcd_clear();
		  lcd_gotoxy(0,0);      
		  lcd_putchar('0');
		  lcd_gotoxy(0,0);
      }
     }
   if (res == 2){ 
		lcd_clear();
		lcd_putsf("*ERROR*"); 
    }  
 } 
 
//*****************************************************
void calculator(){
	unsigned char  key=0;        //dadehi ke az keypad migirim
	char *helper="";       //reshteh komaki
	signed char data_in; 
	   number1  = 0;
	   number2  = 0;
	   last_key = 0;    
	   lcd_clear();
	   lcd_gotoxy(0,0);
	   lcd_putsf("0"); 
	   data_in = 0; 
	   lcd_gotoxy(0,0);  
	   while(1){ 
			start:   
			key = keypad();
			if (test_key(key)){ 
				if(num_dis==1){
					lcd_clear();
					num_dis=0;
				}   
			if (data_in != 9){ 
			   data_in++;
			   if((key=='.')&&(count!=1)){
				   count=1;
				   data_in--;
				   lcd_putchar(key);
				   goto start;
				} 
			   if(key=='.') {
				   data_in--;
				   goto start;
				}
			   lcd_putchar(key); 
			   if(count==1) 
				   mmz++;     
			   helper=0; 
			   key&=0x0f;
			   itoa(key,helper);         
			   strcat(save_str,helper); 
          }
        }
			else{ 
			if (data_in != 0){              
				if (last_key == 0){       
				   number1 = atof(save_str);
				   *save_str=NULL;                
				   config_number(number1);
				   number1=result;
				   count=0;
				   mmz=0;
				   num_dis=1; 
				   }
				else{           
				   number2 = atof(save_str); 
				   *save_str=NULL;                              
				   config_number(number2);
				   number2=result; 
				   count=0;
				   mmz=0; 
				   num_dis=1;                       
				   }
				}
			data_in = 0;      
			if (last_key != 0)
				ack_opretion(last_key);
			if (key != '=')
				last_key = key;
			else 
				last_key = 0;  
       }
    }
} 
 
//***************************************************
void main()
{ 
	setting();
	calculator();
	while(1){};
} 