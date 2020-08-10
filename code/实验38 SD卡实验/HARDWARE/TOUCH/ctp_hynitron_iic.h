/**
 *Name        : ctp_hynitron_iic.h
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-23
 *Copyright   : zxzz
 */


#ifndef CTP_HYNITRON_IIC_H__
#define CTP_HYNITRON_IIC_H__

#include "stm32f4xx.h"

#define 	kal_uint8 	u8
#define 	kal_uint16 	u16
#define 	kal_uint32  u32

#define 	kal_bool	u8
#define 	CTP_TRUE	1
#define 	CTP_FALSE	0
 void hctp_delay_us(kal_uint16 i);
extern void hctp_delay_ms(kal_uint16 i);

extern void hctp_i2c_init(kal_uint8 slave_addr,kal_uint16 iic_speed_k);
extern void hctp_i2c_power_on(void);
extern kal_bool hctp_write_bytes(kal_uint16 reg,kal_uint8 *data, kal_uint16 len,kal_uint8 regLen);
extern kal_bool hctp_read_bytes(kal_uint16 reg, kal_uint8 *value, kal_uint16 len,kal_uint8 regLen);


#endif /*CTP_HYNITRON_IIC_H__*/


