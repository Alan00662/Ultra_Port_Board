#include "app.h"
#include "gt911.h"
#include "../lvgl/demos/lv_demos.h"

/* lvgl驱动初始化 */
void app_lvgl_init(void)
{
	gt911_init();
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
}

/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

static lv_obj_t *switch_cool;            /* 制冷模式开关 */
static lv_obj_t *switch_heat;            /* 制暖模式开关 */
static lv_obj_t *switch_dry;             /* 干燥模式开关 */
static const lv_font_t *font;            /* 定义字体 */

/**
 * @brief  回调事件
 * @param  *e ：事件相关参数的集合，它包含了该事件的所有数据
 * @return 无
 */
static void switch_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);                      /* 获取触发源 */

    if(target == switch_cool)                                       /* 制冷开关触发 */
    {
        if(lv_obj_has_state(switch_cool, LV_STATE_CHECKED))         /* 判断开关状态 */
        {
            lv_obj_clear_state(switch_heat, LV_STATE_CHECKED);      /* 制冷模式已打开，关闭制暖模式 */
        }
    }
    else if(target == switch_heat)                                  /* 制暖开关触发 */
    {
        if(lv_obj_has_state(switch_heat, LV_STATE_CHECKED))         /* 判断开关状态 */
        {
            lv_obj_clear_state(switch_cool, LV_STATE_CHECKED);      /* 制暖模式已打开，关闭制冷模式 */
        }
    }

}

/**
 * @brief  功能文本标签
 * @param  无
 * @return 无
 */
static void lv_example_label(void)
{
    /* 根据活动屏幕宽度选择字体 */

        font = &lv_font_montserrat_14;

    lv_obj_t *label = lv_label_create(lv_scr_act());                            /* 创建标签 */
    lv_label_set_text(label, "Control  Center");                                /* 设置文本内容 */
    lv_obj_set_style_text_font(label, font, LV_STATE_DEFAULT);                  /* 设置字体 */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -scr_act_height() / 3 );            /* 设置位置 */
}

/**
 * @brief  制冷功能开关
 * @param  无
 * @return 无
 */
static void lv_example_switch1(void)
{
    /* 制冷模式基础对象（矩形背景） */
    lv_obj_t *obj_cool = lv_obj_create(lv_scr_act());                               /* 创建基础对象 */
    lv_obj_set_size(obj_cool,scr_act_height() / 3, scr_act_height() / 3 );          /* 设置大小 */
	    lv_obj_align(obj_cool, LV_ALIGN_CENTER, 0, 0 );              /* 设置位置 */

    /* 制冷模式开关标签 */
    lv_obj_t *label_cool = lv_label_create(obj_cool);                               /* 创建标签 */
    lv_label_set_text(label_cool, "Cool");                                          /* 设置文本内容 */
    lv_obj_set_style_text_font(label_cool, font, LV_STATE_DEFAULT);                 /* 设置字体 */
    lv_obj_align(label_cool, LV_ALIGN_CENTER, 0,-scr_act_height() / 16 );          /* 设置位置 */

    /* 制冷模式开关 */
    switch_cool = lv_switch_create(obj_cool);                                       /* 创建开关 */
    lv_obj_set_size(switch_cool,scr_act_height() / 6, scr_act_height() / 12 );      /* 设置大小 */
    lv_obj_align(switch_cool, LV_ALIGN_CENTER,0,scr_act_height() / 16);          /* 设置位置 */
    lv_obj_add_event_cb(switch_cool, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);/* 添加事件 */
}



void app_entry(void){
	
	lv_example_label();
	lv_example_switch1();
//	lv_demo_widgets();


}
