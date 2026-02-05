#include "app.h"
#include "gt911.h"
#include "../lvgl/demos/lv_demos.h"

/* lvglÇý¶¯³õÊ¼»¯ */
void app_lvgl_init(void)
{
	gt911_init();
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
}

void app_entry(void){
	
	lv_demo_widgets();
	
}
