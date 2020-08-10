/**
 *Name        : capacitive_tp_hynitron_cst0xx.c
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-9
 *Copyright   : zxzz
 */
#define __TOUCH_PANEL_CAPACITY__
#if defined(__TOUCH_PANEL_CAPACITY__)
/*****************************************************************/
//
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
#include "ctp_hynitron_ext.h"

#include "stm32f4xx.h"
/*****************************************************************/

#define DBG_CTP
#ifdef DBG_CTP
#define ctp_dbg_print		printf/*kal_prompt_trace*/
#else
#define ctp_dbg_print(...)
#endif

#if !defined(IC_MODULE_TEST)
extern kal_uint32 /*lint -e(526)*/L1I_GetTimeStamp(void);
#endif

/*****************************************************************/

//CTP_parameters_struct CTP_parameters;

/*
This struct recorded the basic infomation of the CTP vendor and chip id
*/
/*CTP_custom_information_struct*/ u8 ctp_custom_information_def[][10] = 
{
	"HYNITRON",
	"cst0",
	"UNKNOWN ",	
};

/*****************************************************************/

/*
  *
  */
 kal_bool ctp_hynitron_cst0_init(void)
 {
     kal_uint8 lvalue;
     kal_uint8 write_data[2];
     kal_bool temp_result = CTP_TRUE;
 
     hctp_i2c_power_on();
#if CTP_HYNITRON_EXT==1
    /*ctp_hynitron_update();*/
#endif
     hctp_i2c_init(CTP_SLAVER_ADDR,300);
     CTP_SET_RESET_PIN_LOW;
     hctp_delay_ms(10);
     CTP_SET_RESET_PIN_HIGH;
 
     /*kal_sleep_task(1);*/

     /*
     config EINT debounce time and sensitivity
     MUST set 0 to EINT debounce
     */
     /*EINT_Set_HW_Debounce(custom_eint_get_channel(touch_panel_eint_chann), 0);*/
     /*EINT_SW_Debounce_Modify(custom_eint_get_channel(touch_panel_eint_chann),0);*/
     /*EINT_Set_Sensitivity(custom_eint_get_channel(touch_panel_eint_chann), EDGE_SENSITIVE);*/
 
     ctp_dbg_print("\r\nctp_cst0_init succeed! ");
     return CTP_TRUE;
 }

/*
 *
 */
kal_bool ctp_hynitron_cst0_power_on(kal_bool enable)
{
	//_TODO:  Implement this funciton by customer
	if (enable){
        CTP_SET_RESET_PIN_LOW;
        hctp_delay_ms(10);
        CTP_SET_RESET_PIN_HIGH;
    }else{
        kal_uint8 enterSleep[] = {0x03};
        hctp_write_bytes(0xA5,enterSleep,1,1);
    }
    return CTP_TRUE;
}
/*
 *
 */
kal_bool ctp_hynitron_cst0_device_mode(u8/*ctp_device_mode_enum*/ mode)
{

	//_TODO:  if needed, add this function to switch decive work mode
	//_TODO: Implement this function by customer

	if(mode == 0/*CTP_ACTIVE_MODE*/)
	{
	}
	else if(mode == 0/*CTP_IDLE_MODE*/)
	{
	}
	else if(mode == 0/*CTP_SLEEP_MODE*/)
	{
	}
	
	return CTP_TRUE;
}


/*
The following function is JUST used in timer trigger mode. 
Now we use interrupt mode to get data. So this function is not used.

ATTENTION: DO NOT delete this function!!
This function is one member of the CTP function pointer struct.
*/
void/*Touch_Panel_PenState_enum*/ ctp_hynitron_cst0_hisr(void)
{
}
/*
This function is a INTERNAL FUNCTION in CTP driver.
By now, it has not been used~
If needed, implement it by yourselves.
But NO implement is OK~
*/
kal_bool ctp_hynitron_cst0_get_version(void/*CTP_custom_information_struct  *version*/)
{
	return CTP_TRUE;
}

/*
This function is used to get parameter from CTP IC or set parameter to CTP IC.

By now, Maybe this function has not been used by upper layer.
ATTENTION: If need to implement it, DO NOT get and set the same parameter in the same time
*/
kal_bool ctp_hynitron_cst0_parameters(u8/*CTP_parameters_struct*/ *para, kal_uint32 get_para, kal_uint32 set_para)
{
	//_TODO: not implement. if needed,  please add them.
	//_TODO: Implement this function by customer


	kal_bool result = CTP_TRUE;
	
	/*this function can NOT	 get and set the same parameter in the same time*/
	if (get_para & set_para)
	{
		/*ASSERT(0);*/
	}
	
	if (set_para /*& CTP_PARA_RESOLUTION*/)
	{
		result &= CTP_FALSE; 
	}
	else if (set_para /*& CTP_PARA_THRESHOLD*/)
	{
		result &= CTP_FALSE; 
	}
	else if (set_para /*& CTP_PARA_REPORT_INTVAL*/)
	{
		result &= CTP_TRUE;
	}
	else if (set_para /*& CTP_PARA_IDLE_INTVAL*/)
	{
		result &= CTP_FALSE;
	}
	else if (set_para /*& CTP_PARA_SLEEP_INTVAL*/)
	{
		result &= CTP_FALSE;
	}
	
	if (get_para /*& CTP_PARA_RESOLUTION*/)
	{
		result &= CTP_FALSE;
	}
	else if (get_para /*& CTP_PARA_THRESHOLD*/)
	{
		result &= CTP_FALSE;
	}
	else if (get_para /*& CTP_PARA_REPORT_INTVAL*/)
	{
		result = CTP_TRUE;
	}
	else if (get_para /*& CTP_PARA_IDLE_INTVAL*/)
	{
		result &= CTP_FALSE;
	}
	else if (get_para /*& CTP_PARA_SLEEP_INTVAL*/)
	{
		result &= CTP_FALSE;
	}
			
	return result;
}

/*
This function is used to get the raw data of the fingures that are pressed.
When CTP IC send intterupt signal to BB chip, this function will be called in the interrupt handler function.

ATTENTION: Becasue this function is called in the interrupt handler function, it MUST NOT run too long.
That will block the entire system.
If blocking too long, it generally will cause system crash *....*
*/
kal_bool ctp_hynitron_cst0_get_data(/*voidTouchPanelMultipleEventStruct*tpes*/ tp_struct *tp_t )
{
	kal_bool temp_result;
	kal_uint8 lvalue[5];
	kal_uint32 counter = 0;
	kal_uint32 model = 0;

	kal_uint16 x,y;// x & y coordinate
	
	/*ASSERT(tpes);*/

	/*tpes->time_stamp = (kal_uint16)L1I_GetTimeStamp();*/
	/*tpes->padding = CTP_PATTERN;*/
	for(int i=0; i<5; i++){
		lvalue[i] = 0;
	}
	hctp_read_bytes(0x02,lvalue,5,1);
	/*printf("\r\n 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",\
		lvalue[0],lvalue[1],lvalue[2],lvalue[3],lvalue[4]);*/
	
	model = lvalue[0];
	//ctp_dbg_print(/*MOD_TP_TASK,*/ "\r\nctp_get_data finger_num=%d", model);
		
	/*tpes->model = (kal_uint16)model;*/

	/*
	0 fingure meas UP EVENT, so return FALSE;
	And now we only support FIVE fingures at most, so if more than 5 fingures also return FALSE 
	*/	
	if ((model == 0)||(model > 2))
	{
		//printf("\r\n%s %d",__func__,__LINE__);
		return CTP_FALSE;
	}
	
    /*tpes->points[0].x = (((kal_uint16)(lvalue[1]&0x0f))<<8) | lvalue[2];;*/
    /*tpes->points[0].y = (((kal_uint16)(lvalue[3]&0x0f))<<8) | lvalue[4];*/
    /*ctp_dbg_print(MOD_TP_TASK, "piont[%d], x:%d, y:%d", 0, tpes->points[0].x, tpes->points[0].y);*/
	x = (((kal_uint16)(lvalue[1]&0x0f))<<8) | lvalue[2];
        y = (((kal_uint16)(lvalue[3]&0x0f))<<8) | lvalue[4];

	tp_t->x = x;
	tp_t->y = y;
        ctp_dbg_print("\r\npiont[%d], x:%d, y:%d", 0, x, y);
	return CTP_TRUE;    
}

/*
JUST return CTP information.
Not Need to modify it!
*/
void/*CTP_custom_information_struct*/ *ctp_Get_Data(void) 
{
	return (&ctp_custom_information_def);
}

/*
This structure is to initialize function pointer to CTP driver.
NOT all function MUST BE implemented in this struct, 
JUST doing function declaration is OK!!

But the following TWO functions MUST BE implemented:
ctp_hynitron_cst0_init
ctp_hynitron_cst0_get_data

Other functions should be implemented by customer for better performance.
*/
/*CTP_customize_function_struct ctp_custom_func=
{
	ctp_hynitron_cst0_init,
	ctp_hynitron_cst0_device_mode,
	ctp_hynitron_cst0_hisr,
	ctp_hynitron_cst0_get_data,
	ctp_hynitron_cst0_parameters,
	ctp_hynitron_cst0_power_on
};*/

/*
Upper layer use this hook to get CTP driver function
*/
void/*CTP_customize_function_struct*/ *ctp_GetFunc(void)
{
	/*return (&ctp_custom_func);*/  
}
#endif //#if defined(__TOUCH_PANEL_CAPACITY__)

