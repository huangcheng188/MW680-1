/**
 *Name        : ctp_hynitron_iic.c
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-23
 *Copyright   : zxzz
 */
#define __TOUCH_PANEL_CAPACITY__	//add for compilation conditions

#if defined(__TOUCH_PANEL_CAPACITY__)

//#include "kal_release.h"
//#include "sccb_v2.h"
//#include "touch_panel.h"
//#include "eint.h"
//#include "drv_comm.h"
//#include "Capacitive_TP_I2C.h"
//#include "I2c.h"
//#include "Dcl.h"
#include "capacitive_tp_hynitron_cst0xx.h"
#include "ctp_hynitron_iic.h"

#include "myiic.h"

/*****************************************************************/
//
static kal_uint8 ctp_device_addr;
static kal_uint16 ctp_i2c_delay_us;

/*****************************************************************/
//

/*
 *
 */
void hctp_delay_us(kal_uint16 i){
	/*while(i--){
		kal_uint32 j;
		for (j=0;j<70;j++);
	}*/
	delay_us(i);
}
/*
 *
 */
void hctp_delay_ms(kal_uint16 i){
    while(i--){
        hctp_delay_us(1000);
    }
}

/*****************************************************************/
//
#if CTP_USE_SW_I2C==1
/*
 *
 */
static kal_bool hctp_i2c_send_byte(kal_uint8 send_byte){
    volatile signed char i;
    kal_bool ack;
#if 0
    // send byte
    for(i=7;i>=0;i--){
        if (send_byte & (1<<i)){
            CTP_SET_I2C_DATA_HIGH;
        }else{
            CTP_SET_I2C_DATA_LOW;
        }
        hctp_delay_us(ctp_i2c_delay_us);
        CTP_SET_I2C_CLK_HIGH;
        hctp_delay_us(ctp_i2c_delay_us);
        CTP_SET_I2C_CLK_LOW;
        if (i == 0){
            CTP_SET_I2C_DATA_HIGH;
            CTP_SET_I2C_DATA_INPUT;
        }
        hctp_delay_us(ctp_i2c_delay_us);
    }
    CTP_SET_I2C_DATA_HIGH;
    CTP_SET_I2C_DATA_INPUT;
    CTP_SET_I2C_CLK_HIGH;
    CTP_SET_I2C_CLK_INPUT;
    hctp_delay_us(ctp_i2c_delay_us);
    for(i=0;i<100;i++){
        if (/*CTP_GET_I2C_CLK_BIT*/0 == 1){
            break;
        }
		hctp_delay_us(5);
    }
    CTP_SET_I2C_CLK_OUTPUT;
    for(i=0;i<100;i++){
        if(/*CTP_GET_I2C_DATA_BIT*/1==0){
            ack = CTP_TRUE;
            break;
        }else{
            ack = CTP_FALSE;
        }
		hctp_delay_us(5);
    }
    CTP_SET_I2C_CLK_LOW;
    CTP_SET_I2C_DATA_OUTPUT;
    hctp_delay_us(ctp_i2c_delay_us);
    return ack;
#endif
    //add huangcheng send one byte(no start) and wait ACK
    IIC_Send_Byte(send_byte);
    ack = IIC_Wait_Ack();  // 1:success 0:error ==> 1:error  0: sussess	
    //printf("\r\n0x%02x   ack = %d",send_byte,!ack);
    //delay_ms(100);
    return ack;
    
}
/*
 *
 */
static kal_uint8 hctp_i2c_get_byte(kal_uint8 sendAck)
{
    volatile signed char i;
    kal_uint8 get_byte=0;
#if 0
    CTP_SET_I2C_DATA_INPUT;
    hctp_delay_us(ctp_i2c_delay_us);
    for (i=7;i>=0;i--){
        CTP_SET_I2C_CLK_LOW;
        hctp_delay_us(ctp_i2c_delay_us);
        CTP_SET_I2C_CLK_HIGH;
        hctp_delay_us(ctp_i2c_delay_us);
        //get_byte |= CTP_GET_I2C_DATA_BIT ? (1<<i) : 0;
        hctp_delay_us(ctp_i2c_delay_us);
    }

    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_CLK_LOW;
    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_DATA_OUTPUT;
    
    if (sendAck){
        CTP_SET_I2C_DATA_LOW;
    }else{
        CTP_SET_I2C_DATA_HIGH;
    }
    
    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_CLK_HIGH;

    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_CLK_LOW;

    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_DATA_LOW;
    
    return get_byte;
#endif
    //
    return IIC_Read_Byte(sendAck);
    //return 0;
}
/*
 *
 */
static void hctp_i2c_start(void){
    /*CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_CLK_HIGH;
    CTP_SET_I2C_DATA_HIGH;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_DATA_LOW;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_CLK_LOW;
    hctp_delay_us(ctp_i2c_delay_us*10);*/
    IIC_Start();
}
/*
 *
 */
static void hctp_i2c_stop(void){
    /*CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_CLK_LOW;
    CTP_SET_I2C_DATA_LOW;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_CLK_HIGH;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_DATA_HIGH;
    hctp_delay_us(ctp_i2c_delay_us*10);*/
    IIC_Stop();
}
/*
 *
 */
void hctp_i2c_init(kal_uint8 slave_addr,kal_uint16 iic_speed_k){    
    /*GPIO_ModeSetup(CTP_I2C_CLK_PIN, 0);
    GPIO_ModeSetup(CTP_I2C_DATA_PIN, 0);
    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_DATA_HIGH;
    CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_CLK_HIGH;

    CTP_SET_RESET_PIN_OUTPUT;
    CTP_SET_RESET_PIN_LOW;
    hctp_delay_ms(2);
    CTP_SET_RESET_PIN_HIGH;*/
    IIC_Init();//need add reset pin 
    ctp_device_addr = slave_addr;
    ctp_i2c_delay_us = 1000/(iic_speed_k+1)/3;
}
/*
 *
 */
void hctp_i2c_power_on(void){
    /*
    pmu_set_vcam_d_sel(VCAM_D_SEL_2_8);     //<==neen set VDD = 2.8V
    pmu_set_vcam_d_en(CTP_TRUE);
    kal_sleep_task(1);
    */
//  ctp_i2c_power_on(1,CTP_I2C_LDO, CTP_I2C_LDO_VOLTAGE);
}
/*
 *
 */
kal_bool hctp_write_bytes(kal_uint16 reg,kal_uint8 *data, kal_uint16 len,kal_uint8 regLen){    
    hctp_i2c_start();       
    hctp_delay_us(ctp_i2c_delay_us);
    
    if(!hctp_i2c_send_byte((ctp_device_addr))){
        hctp_i2c_stop();  
		return (CTP_FALSE);
    }
//¼Ä´æÆ÷³¤¶È8 bit »ò16bit
    if (regLen == 2){
        hctp_delay_us(ctp_i2c_delay_us);
        if(!hctp_i2c_send_byte(reg>>8)){
			hctp_i2c_stop();  
            return (CTP_FALSE);
        }
    }
    hctp_delay_us(ctp_i2c_delay_us);
    if(!hctp_i2c_send_byte(reg)){
		hctp_i2c_stop();  
       return (CTP_FALSE);
    }

    while(len--){
        hctp_delay_us(ctp_i2c_delay_us);
        if(!hctp_i2c_send_byte(*data++)){
			hctp_i2c_stop();  
           return (CTP_FALSE);
        }
    }
    hctp_delay_us(ctp_i2c_delay_us);
    hctp_i2c_stop();  
    return CTP_TRUE;
}
/*
 *
 */
kal_bool hctp_read_bytes(kal_uint16 reg, kal_uint8 *value, kal_uint16 len,kal_uint8 regLen){    
    hctp_i2c_start();       
    hctp_delay_us(ctp_i2c_delay_us);

    if(!hctp_i2c_send_byte((ctp_device_addr))){
       hctp_i2c_stop();  
	   printf("\r\n%s %d",__func__,__LINE__);
       return(CTP_FALSE);
    }
    if (regLen == 2){
        hctp_delay_us(ctp_i2c_delay_us);
        if(!hctp_i2c_send_byte(reg>>8)){
			hctp_i2c_stop();  
			printf("\r\n%s %d",__func__,__LINE__);
            return(CTP_FALSE);
        }
    }
    hctp_delay_us(ctp_i2c_delay_us);
    if(!hctp_i2c_send_byte(reg)){
		printf("\r\n%s %d",__func__,__LINE__);
        hctp_i2c_stop();  
        return(CTP_FALSE);
    }
    hctp_delay_us(ctp_i2c_delay_us);
    hctp_i2c_stop();

    hctp_delay_us(ctp_i2c_delay_us);

    hctp_i2c_start();       
    if(!hctp_i2c_send_byte((ctp_device_addr)+1)){
        hctp_i2c_stop();  
		printf("\r\n%s %d",__func__,__LINE__);
        return(CTP_FALSE);
    }
    while(len--){
        *value++ = hctp_i2c_get_byte(len);
		//printf("\r\n*value=%02x len=%d",*value,len);
    }
    hctp_i2c_stop();
     
    return CTP_TRUE;
}
#else //CTP_USE_SW_I2C==1
#if 0 //remove hardware IIC 
/*
Enable HW I2C power domain.
JUST for HW I2C.

CTP_I2C_LDO             ==>     HW I2C using LDO    (see to enum PMU_LDO_BUCK_LIST_ENUM)
CTP_I2C_LDO_VOLTAGE ==>     The voltage of VIO  (see to enum PMU_VOLTAGE_ENUM)
CTP_I2C_LDO and CTP_I2C_LDO_VOLTAGE are defined in the file "touch_panel_custom.h".
The customer can modify them according to the actual hardware design
*/
void hctp_i2c_power_on(void){
    /*
    pmu_set_vcam_d_sel(VCAM_D_SEL_2_8);     //<==neen set VDD = 2.8V
    pmu_set_vcam_d_en(CTP_TRUE);
    kal_sleep_task(1);
    */
    ctp_i2c_power_on(1,CTP_I2C_LDO, CTP_I2C_LDO_VOLTAGE);
}

/*
Do HW I2C initialization
JUST for HW I2C.

About "I2C CLOCK EXTENSION",  it is a feature of the Hardware I2C.
Enable I2C clock extension. It means host will hold the clk pin low until client pull the clk pin high.
To some CTP chip that need much strict I2C timing, we should enable I2C clock extension feature.
*/
void hctp_i2c_init(kal_uint8 slave_addr,kal_uint16 iic_speed_k){
    ctp_i2c_configure(slave_addr, iic_speed_k);     //set hw i2c speed is 300K (fast mode)
    ENABLE_I2C_CLOCK_EXTENSION; //<==Must enable i2c_clk_extension for being ensure controller timing matching with HW I2C timing
    ctp_device_addr = slave_addr;

    CTP_SET_RESET_PIN_OUTPUT;
    CTP_SET_RESET_PIN_LOW;
    hctp_delay_ms(2);
    CTP_SET_RESET_PIN_HIGH;
}
/*
 *
 */
kal_bool hctp_write_bytes(kal_uint16 reg,kal_uint8 *data, kal_uint16 len,kal_uint8 regLen){
    kal_bool tempResult;
    
    do{
        if (regLen == 2){
            tempResult = qms_i2c_send_ext(ctp_device_addr, reg, data, 1);
        }else{
            tempResult = qms_i2c_send(ctp_device_addr, (kal_uint8)reg, data, 1);
        }
        if (tempResult == CTP_FALSE){
            break;
        }
        data++;
        reg++;
        len--;
    }while(len);

    if (CTP_FALSE == tempResult){
        ctp_dbg_print(MOD_TP_TASK, "ctp Write bytes fail!");
    }
    return tempResult;
}

/*
 *
 */
kal_bool hctp_read_bytes(kal_uint16 reg, kal_uint8 *value, kal_uint16 len,kal_uint8 regLen){
    kal_bool tempResult;
    
    if (regLen == 2){
        tempResult = qms_i2c_receive_ext(ctp_device_addr, reg, value, len);
    }else{
        tempResult = qms_i2c_receive(ctp_device_addr, (kal_uint8)reg, value, len);
    }
    
    if (CTP_FALSE == tempResult){
        ctp_dbg_print(MOD_TP_TASK, "ctp Read bytes fail!");     
    }
    return tempResult;
}
#endif
#endif //CTP_USE_SW_I2C==1

#endif // defined(__TOUCH_PANEL_CAPACITY__)

