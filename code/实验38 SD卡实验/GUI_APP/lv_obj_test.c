#include "lvgl/lvgl.h"
#include "ff.h"  
#include "exfuns.h" 
#include "stm32f4xx.h"
#include "malloc.h" 

extern  unsigned char *picture_map_data;
unsigned char *picture_map_data1;
typedef struct {
	FIL fp;
	u32 header;
	lv_img_dsc_t img_dsc;
}lv_img_t;
lv_img_t lv_img;
lv_img_t lv_img1;
//LV_IMG_DECLARE(img_dsc);

lv_obj_t * scr  = NULL;
lv_obj_t * obj1 = NULL;
lv_obj_t * obj2 = NULL;
lv_obj_t * obj3 = NULL;
lv_style_t style;

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
	int res,br;
	scr = lv_scr_act();
	lv_style_copy(&style, &lv_style_plain_color);
	style.body.main_color = LV_COLOR_BLACK;
	style.body.grad_color = LV_COLOR_BLACK;
	lv_obj_set_style(scr, &style);

#if 1
	static lv_obj_t *tabview; 
	tabview = lv_tabview_create( scr, NULL);
	lv_obj_t *obj1 = lv_tabview_add_tab(tabview, "TAB1");
	lv_obj_set_style( obj1, &style);
	
	lv_obj_t *obj2 = lv_tabview_add_tab(tabview, "TAB2");
	//lv_tabview_set_sliding( tabview, true);
	//lv_tabview_set_btns_hidden(tabview, true);
#endif
	res = f_open( &lv_img.fp, "0:/picture/picture.bin", FA_READ|FA_OPEN_EXISTING);
	printf("\r\n f_open ret =%d size =  %d",res,lv_img.fp.fsize);
	picture_map_data = mymalloc( SRAMEX, lv_img.fp.fsize);
	if(picture_map_data == NULL){
		printf("\r\nalloc error!!!");
	}
	res = f_read( &lv_img.fp, picture_map_data, lv_img.fp.fsize, &br);
	printf("\r\n f_read ret =%d br =  %d",res,br);
	f_close( &lv_img.fp);
	
	lv_img.header = (u32)picture_map_data[3] << 24;
	lv_img.header |= (u32)picture_map_data[2] << 16;
	lv_img.header |= (u32)picture_map_data[1] << 8;
	lv_img.header |= (u32)picture_map_data[0];
	printf("\r\nwidth:%d height:%d\r\n", (uint16_t)(lv_img.header >> 10), (uint16_t)(lv_img.header >> 21));
	
	lv_img.img_dsc.header.cf = picture_map_data[0];
	lv_img.img_dsc.header.always_zero = 0;
	lv_img.img_dsc.header.w = (u16)(lv_img.header>>10);
	lv_img.img_dsc.header.h = (u16)(lv_img.header>>21);
	lv_img.img_dsc.data_size = lv_img.fp.fsize -4;
	lv_img.img_dsc.data = picture_map_data + 4;
	printf("\r\nheader.cf:%d header.always_zero:%d header.w:%d header.w:%d data_size:%d",\
		lv_img.img_dsc.header.cf,\
		lv_img.img_dsc.header.always_zero,\
		lv_img.img_dsc.header.w,\
		lv_img.img_dsc.header.h,\
		lv_img.img_dsc.data_size\
		);
	lv_obj_t *img = lv_img_create( obj1, NULL);
	//lv_img_set_src( img, "P:/picture/picture.bin");
	lv_img_set_src( img, &lv_img.img_dsc);
	//lv_img_set_src( img, &picture_1);
	//myfree(SRAMEX, picture_map_data);

	delay_ms(1000);
	res = f_open( &lv_img1.fp, "0:/picture/picture1.bin", FA_READ|FA_OPEN_EXISTING);
	printf("\r\n f_open ret =%d size =  %d",res,lv_img1.fp.fsize);
	picture_map_data1 = mymalloc( SRAMEX, lv_img.fp.fsize);
	if(picture_map_data1 == NULL){
		printf("\r\nalloc error!!!");
	}
	res = f_read( &lv_img1.fp, picture_map_data1, lv_img1.fp.fsize, &br);
	printf("\r\n f_read ret =%d br =  %d",res,br);
	f_close( &lv_img1.fp);
	printf("\r\n2");
	lv_img1.header = (u32)picture_map_data1[3] << 24;
	lv_img1.header |= (u32)picture_map_data1[2] << 16;
	lv_img1.header |= (u32)picture_map_data1[1] << 8;
	lv_img1.header |= (u32)picture_map_data1[0];
	printf("\r\nwidth:%d height:%d\r\n", (uint16_t)(lv_img1.header>>10), lv_img1.header>>21);
	
	lv_img1.img_dsc.header.cf = picture_map_data1[0];
	lv_img1.img_dsc.header.always_zero = 0;
	lv_img1.img_dsc.header.w = (u16)(lv_img1.header>>10);
	lv_img1.img_dsc.header.h = (u16)(lv_img1.header>>21);
	lv_img1.img_dsc.data_size = lv_img1.fp.fsize -4;
	lv_img1.img_dsc.data = picture_map_data1 + 4;
	printf("\r\nheader.cf:%d header.always_zero:%d header.w:%d header.w:%d data_size:%d",\
		lv_img1.img_dsc.header.cf,\
		lv_img1.img_dsc.header.always_zero,\
		lv_img1.img_dsc.header.w,\
		lv_img1.img_dsc.header.h,\
		lv_img1.img_dsc.data_size\
		);
	//picture_map_data = picture_map_data + 4;
	//img_dsc.data = picture_map_data;

	lv_obj_t *img1= lv_img_create( obj2, NULL);
	lv_img_set_src( img1, &lv_img1.img_dsc);
	//lv_img_set_src( img, "P:/picture/picture1.bin");
	//myfree(SRAMEX, picture_map_data1);
	//0:/PICTURE/
	//lv_obj_set_size( img, 100, 100);
	//lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);

	
}
