/**
 *Name        : capacitive_tp_hynitron_cst0xx.h
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-9
 *Copyright   : zxzz
 */


#ifndef CAPACITIVE_TP_HYNITRON_CST0XX_H__
#define CAPACITIVE_TP_HYNITRON_CST0XX_H__

#include "stm32f4xx.h"
//need 80us to turn on vcamd
#define CTP_I2C_LDO         VCAMD //MT6276E2 use VACMA, MT6276E1 use VCAMD
#define CTP_I2C_LDO_VOLTAGE PMU_VOLT_02_800000_V

typedef struct {
	u16 x;
	u16 y;
}tp_struct;
extern tp_struct tp_t;
//_TODO:
#define CTP_SLAVER_ADDR            0x2A
#define CTP_HYNITRON_EXT           0// 1 // enable update
    #define CTP_HYNITRON_EXT_CST816S_UPDATE   0
    
#define CTP_USE_SW_I2C          1

#define CTP_TRUE    KAL_TRUE
#define CTP_FALSE   KAL_FALSE

#define OUTPUT 1
#define INPUT 0

// TODO:
extern const char gpio_ctp_reset_pin ;
#define CTP_RESET_PIN	    gpio_ctp_reset_pin

#ifndef __CUST_NEW__
#define CTP_SET_RESET_PIN_OUTPUT			//GPIO_InitIO_FAST(OUTPUT, CTP_RESET_PIN)
#define CTP_SET_RESET_PIN_HIGH				//GPIO_WriteIO_FAST(1, CTP_RESET_PIN)
#define CTP_SET_RESET_PIN_LOW				//GPIO_WriteIO_FAST(0, CTP_RESET_PIN)
#else
#define CTP_SET_RESET_PIN_OUTPUT			//GPIO_InitIO(OUTPUT, CTP_RESET_PIN)
#define CTP_SET_RESET_PIN_HIGH				//GPIO_WriteIO(1, CTP_RESET_PIN)
#define CTP_SET_RESET_PIN_LOW				//GPIO_WriteIO(0, CTP_RESET_PIN)
#endif

#if CTP_USE_SW_I2C==1
// TODO:
extern const char gpio_ctp_i2c_scl_pin;
extern const char gpio_ctp_i2c_sda_pin;
#define CTP_I2C_DATA_PIN	    gpio_ctp_i2c_sda_pin 
#define CTP_I2C_CLK_PIN			gpio_ctp_i2c_scl_pin 

#define GPIO_CTP_I2C_SDA_PIN  CTP_I2C_DATA_PIN
#define GPIO_CTP_I2C_SCL_PIN  CTP_I2C_CLK_PIN

#ifndef __CUST_NEW__
#define CTP_SET_I2C_DATA_OUTPUT			    //GPIO_InitIO_FAST(OUTPUT,CTP_I2C_DATA_PIN)
#define CTP_SET_I2C_DATA_INPUT				//GPIO_InitIO_FAST(INPUT,CTP_I2C_DATA_PIN)
#define CTP_SET_I2C_DATA_HIGH				//GPIO_WriteIO_FAST(1,CTP_I2C_DATA_PIN)
#define CTP_SET_I2C_DATA_LOW				//GPIO_WriteIO_FAST(0,CTP_I2C_DATA_PIN)
#define CTP_GET_I2C_DATA_BIT				//GPIO_ReadIO_FAST(CTP_I2C_DATA_PIN)

#define CTP_SET_I2C_CLK_OUTPUT				//GPIO_InitIO_FAST(OUTPUT,CTP_I2C_CLK_PIN)
#define CTP_SET_I2C_CLK_INPUT				//GPIO_InitIO_FAST(INPUT,CTP_I2C_CLK_PIN)
#define CTP_SET_I2C_CLK_HIGH				//GPIO_WriteIO_FAST(1,CTP_I2C_CLK_PIN)
#define CTP_SET_I2C_CLK_LOW			        //GPIO_WriteIO_FAST(0,CTP_I2C_CLK_PIN)
#define CTP_GET_I2C_CLK_BIT					//GPIO_ReadIO_FAST(CTP_I2C_CLK_PIN)
#else
#define CTP_SET_I2C_DATA_OUTPUT			    //GPIO_InitIO(OUTPUT,CTP_I2C_DATA_PIN)
#define CTP_SET_I2C_DATA_INPUT				//GPIO_InitIO(INPUT,CTP_I2C_DATA_PIN)
#define CTP_SET_I2C_DATA_HIGH				//GPIO_WriteIO(1,CTP_I2C_DATA_PIN)
#define CTP_SET_I2C_DATA_LOW				//GPIO_WriteIO(0,CTP_I2C_DATA_PIN)
#define CTP_GET_I2C_DATA_BIT				//GPIO_ReadIO(CTP_I2C_DATA_PIN)

#define CTP_SET_I2C_CLK_OUTPUT				//GPIO_InitIO(OUTPUT,CTP_I2C_CLK_PIN)
#define CTP_SET_I2C_CLK_INPUT			    //GPIO_InitIO(INPUT,CTP_I2C_CLK_PIN)
#define CTP_GET_I2C_CLK_BIT					//GPIO_ReadIO(CTP_I2C_CLK_PIN)
#define CTP_SET_I2C_CLK_HIGH				//GPIO_WriteIO(1,CTP_I2C_CLK_PIN)
#define CTP_SET_I2C_CLK_LOW				    //GPIO_WriteIO(0,CTP_I2C_CLK_PIN)
#endif
#endif //SW_I2CS

#endif /*CAPACITIVE_TP_HYNITRON_CST0XX_H__*/

