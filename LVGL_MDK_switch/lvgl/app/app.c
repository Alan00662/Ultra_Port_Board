#include "app.h"
#include "gt911.h"
#include "../lvgl/demos/lv_demos.h"

/* lvgl������ʼ�� */
void app_lvgl_init(void)
{
	gt911_init();
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
}

/* ��ȡ��ǰ���Ļ�Ŀ�� */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

static lv_obj_t *switch_cool;            /* ����ģʽ���� */
static lv_obj_t *switch_heat;            /* ��ůģʽ���� */
static lv_obj_t *switch_dry;             /* ����ģʽ���� */
static const lv_font_t *font;            /* �������� */

/**
 * @brief  �ص��¼�
 * @param  *e ���¼���ز����ļ��ϣ��������˸��¼�����������
 * @return ��
 */
static void switch_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);                      /* ��ȡ����Դ */

    if(target == switch_cool)                                       /* ���俪�ش��� */
    {
        if(lv_obj_has_state(switch_cool, LV_STATE_CHECKED))         /* �жϿ���״̬ */
        {
            lv_obj_clear_state(switch_heat, LV_STATE_CHECKED);      /* ����ģʽ�Ѵ򿪣��ر���ůģʽ */
        }
    }
    else if(target == switch_heat)                                  /* ��ů���ش��� */
    {
        if(lv_obj_has_state(switch_heat, LV_STATE_CHECKED))         /* �жϿ���״̬ */
        {
            lv_obj_clear_state(switch_cool, LV_STATE_CHECKED);      /* ��ůģʽ�Ѵ򿪣��ر�����ģʽ */
        }
    }

}

/**
 * @brief  �����ı���ǩ
 * @param  ��
 * @return ��
 */
static void lv_example_label(void)
{
    /* ���ݻ��Ļ���ѡ������ */

        font = &lv_font_montserrat_14;

    lv_obj_t *label = lv_label_create(lv_scr_act());                            /* ������ǩ */
    lv_label_set_text(label, "Control  Center");                                /* �����ı����� */
    lv_obj_set_style_text_font(label, font, LV_STATE_DEFAULT);                  /* �������� */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -scr_act_height() / 3 );            /* ����λ�� */
}

/**
 * @brief  ���书�ܿ���
 * @param  ��
 * @return ��
 */
static void lv_example_switch1(void)
{
    /* ����ģʽ�������󣨾��α����� */
    lv_obj_t *obj_cool = lv_obj_create(lv_scr_act());                               /* ������������ */
    lv_obj_set_size(obj_cool,scr_act_height() / 3, scr_act_height() / 3 );          /* ���ô�С */
	    lv_obj_align(obj_cool, LV_ALIGN_CENTER, 0, 0 );              /* ����λ�� */

    /* ����ģʽ���ر�ǩ */
    lv_obj_t *label_cool = lv_label_create(obj_cool);                               /* ������ǩ */
    lv_label_set_text(label_cool, "Cool");                                          /* �����ı����� */
    lv_obj_set_style_text_font(label_cool, font, LV_STATE_DEFAULT);                 /* �������� */
    lv_obj_align(label_cool, LV_ALIGN_CENTER, 0,-scr_act_height() / 16 );          /* ����λ�� */

    /* ����ģʽ���� */
    switch_cool = lv_switch_create(obj_cool);                                       /* �������� */
    lv_obj_set_size(switch_cool,scr_act_height() / 6, scr_act_height() / 12 );      /* ���ô�С */
    lv_obj_align(switch_cool, LV_ALIGN_CENTER,0,scr_act_height() / 16);          /* ����λ�� */
    lv_obj_add_event_cb(switch_cool, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);/* ����¼� */
}



void app_entry(void){
	
	lv_example_label();
	lv_example_switch1();
//	lv_demo_widgets();


}
