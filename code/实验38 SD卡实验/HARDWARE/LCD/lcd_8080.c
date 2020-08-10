/*filename :lcd_8080.c
 *brief : 本文件有lcd供应商提供，模拟8080 可点亮，初始化为黑背景色，
 *		  移植littleVGL，添加lcd_fill_color_1函数
*/


#include "sys.h"
#include "stdio.h"
#include "stm32f4xx.h"


//PA0 - PA7 DB0 - DB7

#define 	RST		PAout(7)//PAout(15)

#define 	CS0 		PAout(3)//PAout(11)
#define 	RS  		PAout(4)//PAout(12)
#define 	RD0  	PAout(5)//PAout(13)
#define 	WR0  	PAout(6)//PAout(14)



#define COL 240
#define ROW 240

void LCD_CtrlWrite(unsigned int i)
{
	CS0=0;
	RD0=1;
	RS=0;	
	
	WR0=0;  
	GPIO_Write( GPIOB, (uint16_t)i&0xFF);//写8位数据，实际是写16位，硬件只接最低八位 
	WR0=1; 
	
	CS0=1; 
}

void LCD_DataWrite(unsigned int i)
{
	CS0=0;
	RS=1;	
	
    WR0=0;  
	GPIO_Write( GPIOB, (uint16_t)i&0xFF);//写8位数据，实际是写16位，硬件只接最低八位 
	WR0=1; 
	
	CS0=1; 
}


#define RED    0xF800		 //定义颜色常量 
#define GREEN  0x07E0

#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000
#define GRAY   0xEF5D	     //0x2410
#define GRAY75 0x39E7 
#define GRAY50 0x7BEF	
#define GRAY25 0xADB5




void lcd_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //IO口复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  //

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //IO口复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	printf("\r\ngpio init");
	
}



void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	
	LCD_CtrlWrite(0x2a);   
	LCD_DataWrite(Xstart>>8);
	LCD_DataWrite(Xstart&0xff);
	LCD_DataWrite(Xend>>8);
	LCD_DataWrite(Xend&0xff);

	LCD_CtrlWrite(0x2b);   
	LCD_DataWrite(Ystart>>8);
	LCD_DataWrite(Ystart&0xff);
	LCD_DataWrite(Yend>>8);
	LCD_DataWrite(Yend&0xff);

	LCD_CtrlWrite(0x2c);
}
void DispColor(unsigned int color)
{
	unsigned int i,j;

	BlockWrite(0,COL-1,0,ROW-1);

	for(i=0;i<ROW;i++){
	    for(j=0;j<COL;j++){    
			LCD_DataWrite(color>>8);
			LCD_DataWrite(color&0xFF);
		}
	}
}

//add huangcheng for littleVGL port
void lcd_fill_color(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u16 height,width;
	u16 i,j;
	
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	
	BlockWrite(sx, ex, sy, ey);
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){    
			//color[i*width+j]=0xF800;
			LCD_DataWrite(color[i*width+j]>>8);
			LCD_DataWrite(color[i*width+j]&0xFF);
		}
	}
}


void lcd_init(void)
{
	int i=0,j;
	u16 color = RED;
	lcd_gpio_init();

	//-------- Reset Sequence-----//
	RST = 1;
	delay_ms(50);
	RST = 0;
	delay_ms(50);
	RST = 1;
	delay_ms(120);
	//------end Reset Sequence-----//
	LCD_CtrlWrite(0xfe);
	LCD_CtrlWrite(0xef);
	LCD_CtrlWrite(0x36);	
	LCD_DataWrite(0x58);// 36 48 58	
	LCD_CtrlWrite(0x3a);	
	LCD_DataWrite(0x05);	
		
		
	LCD_CtrlWrite(0x86);	
	LCD_DataWrite(0x98);
		
	LCD_CtrlWrite(0x89);	
	LCD_DataWrite(0x03);
		
	LCD_CtrlWrite(0x8b);	
	LCD_DataWrite(0x80);	
		
	LCD_CtrlWrite(0x8d);	
	LCD_DataWrite(0x22);	
	LCD_CtrlWrite(0x8e);	
	LCD_DataWrite(0x0f);

	LCD_CtrlWrite(0xe8);
	LCD_DataWrite(0x12);
	LCD_DataWrite(0x00);	

	//LCD_CtrlWrite(0xe9);	
	//LCD_DataWrite(0x08);//2data_en	

	//end vreg

	//source ps
	LCD_CtrlWrite(0xff);
	LCD_DataWrite(0x62);


	LCD_CtrlWrite(0x99);	
	LCD_DataWrite(0x3e);
	LCD_CtrlWrite(0x9d);	
	LCD_DataWrite(0x4b);


	LCD_CtrlWrite(0xc3);	
	LCD_DataWrite(0x27);
	LCD_CtrlWrite(0xc4);	
	LCD_DataWrite(0x18);
		
	//LCD_CtrlWrite(0xc5);	//delete by james 20200708
	//LCD_DataWrite(0x0F);
	//LCD_CtrlWrite(0xc6);	
	//LCD_DataWrite(0x00);
	//LCD_CtrlWrite(0xc7);	
	//LCD_DataWrite(0x10);	 
	//LCD_CtrlWrite(0xc8);	
	//LCD_DataWrite(0x01);
	LCD_CtrlWrite(0xc9);	
	LCD_DataWrite(0x1F);

	LCD_CtrlWrite(0xF0);
	LCD_DataWrite(0x8F);
	LCD_DataWrite(0x1B);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x06);
	LCD_DataWrite(0x07);
	LCD_DataWrite(0x42);

	LCD_CtrlWrite(0xF2);
	LCD_DataWrite(0x5C);
	LCD_DataWrite(0x1F);
	LCD_DataWrite(0x12);
	LCD_DataWrite(0x10);
	LCD_DataWrite(0x07); 
	LCD_DataWrite(0x43);

	LCD_CtrlWrite(0xF1);
	LCD_DataWrite(0x59);
	LCD_DataWrite(0xcf);
	LCD_DataWrite(0xcf);
	LCD_DataWrite(0x35);
	LCD_DataWrite(0x37);
	LCD_DataWrite(0x8F);

	LCD_CtrlWrite(0xF3);
	LCD_DataWrite(0x58);
	LCD_DataWrite(0xcf);
	LCD_DataWrite(0xcf);
	LCD_DataWrite(0x35);
	LCD_DataWrite(0x37);
	LCD_DataWrite(0x8F);
		
	LCD_CtrlWrite(0x35);
	LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x44);
	LCD_DataWrite(0x00);
	LCD_DataWrite(0x0a);
	LCD_CtrlWrite(0x11);
		delay_ms(120);
	LCD_CtrlWrite(0x29);
	LCD_CtrlWrite(0x2c);

	printf("\r\nlcd init ....");
	
	//lcd_fill_color_1( 0, 0, 239, 239, &color);
	DispColor(BLACK);		//test 

}
void Lcd_enter_sleep(void)
{
	LCD_DataWrite(0xF8);
	LCD_DataWrite(0x00);
}
void Lcd_exit_sleep(void)
{
	LCD_CtrlWrite(0x11);
	delay_ms(120) ;
	LCD_CtrlWrite(0x29);
}

