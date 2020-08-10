/**
 *Name        : ctp_hynitron_ext.c
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-23
 *Copyright   : zxzz
 */

#if defined(__TOUCH_PANEL_CAPACITY__)

#include "kal_release.h"
#include "sccb_v2.h"
#include "touch_panel.h"
#include "eint.h"
#include "drv_comm.h"
#include "Capacitive_TP_I2C.h"
#include "I2c.h"
#include "Dcl.h"
#include "capacitive_tp_hynitron_cst0xx.h"
#include "ctp_hynitron_iic.h"
#include "ctp_hynitron_ext.h"

#if CTP_HYNITRON_EXT==1
#define REG_LEN_1B  1
#define REG_LEN_2B  2

#if CTP_HYNITRON_EXT_CST816S_UPDATE==1
/*****************************************************************/
// For CSK0xx update
 /*
  *
  */
static int cst816s_enter_bootmode(void){
     char retryCnt = 10;

     CTP_SET_I2C_DATA_OUTPUT;
     CTP_SET_I2C_DATA_HIGH;
     CTP_SET_I2C_CLK_OUTPUT;
     CTP_SET_I2C_CLK_HIGH;
     CTP_SET_RESET_PIN_LOW;
     hctp_delay_ms(10);
     CTP_SET_RESET_PIN_HIGH;
     mdelay(5);
     while(retryCnt--){
         u8 cmd[3];
         cmd[0] = 0xAB;
         if (-1 == hctp_write_bytes(0xA001,cmd,1,REG_LEN_2B)){  // enter program mode
             mdelay(2); // 4ms
             continue;                   
         }
         if (-1 == hctp_read_bytes(0xA003,cmd,1,REG_LEN_2B)) { // read flag
             mdelay(2); // 4ms
             continue;                           
         }else{
             if (cmd[0] != 0x55){
                 msleep(2); // 4ms
                 continue;
             }else{
                 return 0;
             }
         }
     }
     return -1;
 }
 /*
  *
  */
static int cst816s_update(u16 startAddr,u16 len,u8* src){
     u16 sum_len;
     u8 cmd[10];

     if (cst816s_enter_bootmode() == -1){
        return -1;
     }
     sum_len = 0;
 
#define PER_LEN	512
     do{
         if (sum_len >= len){
             return;
         }
         
         // send address
         cmd[0] = startAddr&0xFF;
         cmd[1] = startAddr>>8;
         hctp_write_bytes(0xA014,cmd,2,REG_LEN_2B);
         
         hctp_write_bytes(0xA018,src,PER_LEN,REG_LEN_2B);
 
         cmd[0] = 0xEE;
         hctp_write_bytes(0xA004,cmd,1,REG_LEN_2B);
         msleep(100);
 
         {
             u8 retrycnt = 50;
             while(retrycnt--){
                 cmd[0] = 0;
                 hctp_read_bytes(0xA005,cmd,1,REG_LEN_2B);
                 if (cmd[0] == 0x55){
                     // success
                     break;
                 }
                 msleep(10);
             }
         }
         startAddr += PER_LEN;
         src += PER_LEN;
         sum_len += PER_LEN;
     }while(len);
     
     // exit program mode
     cmd[0] = 0x00;
     hctp_write_bytes(0xA003,cmd,1,REG_LEN_2B);
 }
 /*
  *
  */
static u32 cst816s_read_checksum(u16 startAddr,u16 len){
     union{
         u32 sum;
         u8 buf[4];
     }checksum;
     char cmd[3];
     char readback[4] = {0};
 
     if (cst816s_enter_bootmode() == -1){
        return -1;
     }
     
     cmd[0] = 0;
     if (-1 == hctp_write_bytes(0xA003,cmd,1,REG_LEN_2B)){
         return -1;
     }
     msleep(500);

     checksum.sum = 0;
     if (-1 == hctp_read_bytes(0xA008,checksum.buf,2,REG_LEN_2B)){
         return -1;
     }
     return checksum.sum;
}
#endif 

/*****************************************************************/
// common

 /*
  *
  */
 kal_bool ctp_hynitron_update(void)
 {
     kal_uint8 lvalue;
     kal_uint8 write_data[2];
     kal_bool temp_result = CTP_TRUE;

#if CTP_HYNITRON_EXT_CST816S_UPDATE==1
    hctp_i2c_init(0x6A<<1,50);     
    if (cst816s_enter_bootmode() == 0){
#include "capacitive_hynitron_cst816s_update.h"
        if(sizeof(app_bin) > 10){
            kal_uint16 startAddr = app_bin[1];
            kal_uint16 length = app_bin[3];
            kal_uint16 checksum = app_bin[5];
            startAddr <<= 8; startAddr |= app_bin[0];
            length <<= 8; length |= app_bin[2];
            checksum <<= 8; checksum |= app_bin[4];   
            if(cst816s_read_checksum(startAddr, length) != checksum){
                cst816s_update(startAddr, length, app_bin+6);
                cst816s_read_checksum(startAddr, length);
            }
        }
        return CTP_TRUE;
    }
#endif

      return CTP_FALSE;
 }

#endif  //CTP_HYNITRON_EXT==1

#endif //defined(__TOUCH_PANEL_CAPACITY__)

