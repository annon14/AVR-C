#define c1  PIND.4
#define c2  PIND.5
#define c3  PIND.6
#define c4  PIND.7 
#define SIN   PINC.0 
#define COS   PINC.1 
#define TAN   PINC.2 
#define LOG   PINC.3 
#define SHIFT PINC.4 
flash char row[]={~1,~2,~4,~8};
flash char data_key[]={'7','8','9','%','4','5','6','*','1','2','3','-','.','0','=','+'};
unsigned char ac,table;
unsigned int r;  

//**************** lcd_display keypad4*4 ******************************

unsigned char keypad(){  
	while (1){
		for (r=0;r<4;r++){
			ac=4;
			PORTD=row[r]; 
			DDRD=0x0f;      
			if (c1==0)  
				ac=0; 
			if (c2==0)  
				ac=1;  
			if (c3==0)  
				ac=2;    
			if (c4==0)  
				ac=3;  
			//*********************
			if (LOG==0)  
			   log_10(); 
			if (SHIFT==1 && SIN==0) 
			   sin_DRG();      
			if (SHIFT==1 && COS==0) 
			   cos_DRG();  
			if (SHIFT==1 && TAN==0) 
			   tan_DRG();           
			if (SHIFT==0 && SIN==0) 
			   arcsin_DRG();      
			if (SHIFT==0 && COS==0) 
			   arccos_DRG();  
			if (SHIFT==0 && TAN==0) 
			   arctan_DRG();        
		   //*********************
			if (!(ac==4)){            
				table=data_key[(r*4)+ac];   
				
				while (c1==0){} 
				while (c2==0){} 
				while (c3==0){} 
				while (c4==0){}   
				delay_ms(20);
				while (c1==0){} 
				while (c2==0){} 
				while (c3==0){} 
				while (c4==0){} 
				delay_ms(20);  
				return table;  
			}
	  }
	 };
}
//************************************
void setting(){
	PORTD=0xff;
	DDRD=0x0f; 
	PORTC=0xFF;
	DDRC=0x00; 
	lcd_init(16);
}