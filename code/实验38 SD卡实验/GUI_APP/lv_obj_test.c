#include "lvgl/lvgl.h"


lv_obj_t * scr  = NULL;
lv_obj_t * obj1 = NULL;
lv_obj_t * obj2 = NULL;
lv_obj_t * obj3 = NULL;
lv_style_t red_style;

void lv_obj_test_callback(struct _lv_obj_t * obj, lv_event_t event)
{
	//user_data_t *data = (user_data_t *)(task->user_data);
	 static unsigned char scale = 255;
	if(event == LV_EVENT_RELEASED){
		printf("\r\nlv_obj_test_callback released %d",lv_tick_get());
		lv_obj_set_opa_scale_enable(obj2,true);//先使能 先使能 obj1的透明度功能 的透明度功能 的透明度功能
		lv_obj_set_opa_scale(obj2,scale-=50);//再设置 再设置 obj1的透明度为 的透明度为 的透明度为 50
	}
}
void lv_obj_test_start(void)
{
	scr = lv_scr_act();
	lv_style_copy(&red_style, &lv_style_plain_color);
	red_style.body.main_color = LV_COLOR_RED;
	red_style.body.grad_color = LV_COLOR_GREEN;
	lv_obj_set_style(scr, &lv_style_pretty_color);
	
	obj1 = lv_obj_create(scr, NULL);
	lv_obj_set_size(obj1, 100, 100);
	 

	obj2 = lv_obj_create(scr, NULL);
	lv_obj_set_size(obj2, 50, 50);
	lv_obj_set_style(obj2, &red_style);
	lv_obj_align(obj2, obj1, LV_ALIGN_OUT_RIGHT_BOTTOM, 0,0);
	lv_obj_set_click(obj2, true);
	//lv_obj_set_drag(obj2, true);
	//点击事件等处理还需要完善
	lv_obj_set_event_cb(obj2, lv_obj_test_callback);
	//lv_event_t

	
}