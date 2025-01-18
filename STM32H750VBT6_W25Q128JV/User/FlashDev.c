

#include "FlashOS.H"       



struct FlashDevice const FlashDevice  =  
{
    FLASH_DRV_VERS,                   /* 驱动版本，勿修改，这个是MDK定的 */
    "H750VBT6_QSPI_W25Q128JV",        /* 算法名，添加算法到MDK安装目录会显示此名字 */
    EXTSPI,                           /* 设备类型 */
    0X90000000,                       /* Flash起始地址 0X90000000 定义内存的起始地址 */
    16 * 1024 * 1024,                 /* Flash大小，16MB  W25Q128的内存大小就是16MB */
    4 * 1024,                         /* 编程页大小 */
    0,                                /* 保留，必须为0 */
    0xFF,                             /* 擦除后的数值 */
    1000,                             /* 页编程等待时间 */
    6000,                             /* 扇区擦除等待时间 */
    64 * 1024, 0x000000,              /* 扇区大小，扇区地址 */
    SECTOR_END    
};







