//
// Created by 18716 on 2022/9/28.
//

#ifndef LCD_FRAMEBUFFER_H
#define LCD_FRAMEBUFFER_H

#include "lcd.h"

#define   BLACK     0x0000       //   黑色    
#define   NAVY      0x000F      //    深蓝色  
#define   DGREEN    0x03E0        //  深绿色  
#define   DCYAN     0x03EF       //   深青色  
#define   MAROON    0x7800       //   深红色      
#define   PURPLE    0x780F       //   紫色  
#define   OLIVE     0x7BE0       //   橄榄绿      
#define   LGRAY     0xC618        //  灰白色
#define   DGRAY     0x7BEF        //  深灰色      
#define   BLUE      0x001F        //  蓝色    
#define   GREEN     0x07E0        //  绿色          
#define   CYAN      0x07FF        //  青色  
#define   RED       0xF800        //  红色       
#define   MAGENTA   0xF81F        //  品红    
#define   YELLOW    0xFFE0        //  黄色        
#define   WHITE     0xFFFF        //  白色


// 画点
void fb_draw_pixel(unsigned int x, unsigned int y, unsigned int color);

// 限制
void fb_limit_xy(unsigned int *px, unsigned int *py);


void fb_clear(unsigned int color);



#endif //LCD_FRAMEBUFFER_H
