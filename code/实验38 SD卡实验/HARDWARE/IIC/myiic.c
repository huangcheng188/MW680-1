#include "myiic.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "capacitive_tp_hynitron_cst0xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	IIC_SCL=1;
	IIC_SDA=1;

	//add huangcheng TP reset pin & init pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//tp reset pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //�������
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);		//reset tp
	hctp_delay_ms(10);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//tp int pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   //�������
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);        //����Ϊ��2���ж����÷�ʽ����1bits������ռʽ��3bits������Ӧʽ�����������misc.c�ļ��У������misc.h�ļ���
	/* �����ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;       //�ⲿ�ж�0�����ֱ�ʾ�������ж�0~4��ʾ���������ƣ��ж�5~9��ʾ��ʽͳһΪEXTI9_5IRQn���ж�10~15ͳһΪEXTI15_10IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //��Ӧʽ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,  EXTI_PinSource0);
	 EXTI_InitStructure.EXTI_Line = EXTI_Line0;    //�ⲿ�ж�0���������Դ�����
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //����Ϊ�ж�ģʽ
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
    {
      	//printf("i");
      	//ctp_hynitron_cst0_get_data(NULL);
      	tp_t.int_flag = 1;
        EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־λ
    } 
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	 
	//return 0;//huangcheng !!!
	return 1;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}



























