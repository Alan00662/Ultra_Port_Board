/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "lvgl.h"
#include "gt911.h"

/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES 320
#define MY_DISP_VER_RES 480

uint16_t LTDC_BUFF[MY_DISP_VER_RES*MY_DISP_HOR_RES];

volatile uint8_t g_gpu_state=0;
lv_disp_drv_t* g_disp_drv;//全局lv_disp_drv_t指针，用于中断回调函数的入口参数
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
//    static lv_disp_draw_buf_t draw_buf_dsc_1;
//    static lv_color_t buf_1[MY_DISP_HOR_RES * 136];                          /*A buffer for 10 rows*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 136);   /*Initialize the display buffer*/

    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 68];                        /*A buffer for 10 rows*/
    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 68];                        /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 68);   /*Initialize the display buffer*/

//    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/


void DMA2D_IRQHandler(void)
{
	if ((DMA2D->ISR & DMA2D_FLAG_TC) != 0U)
	{
		if ((DMA2D->CR & DMA2D_IT_TC) != 0U)
		{

			DMA2D->CR &= ~DMA2D_IT_TC;
			DMA2D->IFCR =DMA2D_FLAG_TC;
			
			if(g_gpu_state==1)
			{
				g_gpu_state = 0;
				lv_disp_flush_ready(g_disp_drv);
			}
		}
	}
}


static void disp_init(void)
{
    /*You code here*/
//	dma2d_use_hal_init(&hdma2d);
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
/*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
	//LTDC_Color_Fill(area->x1,area->y1,area->x2,area->y2,(uint16_t*)(color_p));

	uint32_t h = area->y2 - area->y1;
	uint32_t w = area->x2 - area->x1;
	
	uint32_t OffLineSrc = MY_DISP_HOR_RES - (area->x2 - area->x1 +1);
	uint32_t addr = (uint32_t)&(LTDC_BUFF[area->y1 * MY_DISP_HOR_RES + area->x1]);
//	
	g_gpu_state = 0;
    g_disp_drv=disp_drv;
	
	// --- 阻塞传输---
	// 模式
	DMA2D->CR      = 0x00000000UL | (1 << 9);
	// 源地址
	DMA2D->FGMAR   = (uint32_t)(uint16_t*)(color_p);
	// 目标地址
	DMA2D->OMAR    = (uint32_t)addr;
	
	// 输入偏移
	DMA2D->FGOR    = 0;
	// 输出偏移
	DMA2D->OOR     = OffLineSrc;
	
	/* 前景层和输出区域都采用的RGB565颜色格式 */
	DMA2D->FGPFCCR = DMA2D_OUTPUT_RGB565;
	DMA2D->OPFCCR  = DMA2D_OUTPUT_RGB565;
	
	DMA2D->NLR     = (area->y2-area->y1+1) | ((area->x2 -area->x1 +1) << 16);
	
    // 开启中断
	DMA2D->CR |= DMA2D_IT_TC|DMA2D_IT_TE|DMA2D_IT_CE;
	
	/* 启动传输 */
	DMA2D->CR   |= DMA2D_CR_START;   
	g_gpu_state = 1;
}

/*OPTIONAL: GPU INTERFACE*/




/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
