#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "sram.h"   
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "timer.h"
//bad apple 90fps刷屏程序，未进行底层优化，只是用了一些小技巧，底层优化，可以速度更快

//有问题可联系qq362276794

//本程序只能在STM32F407探索板2.8寸屏上运行，其他屏需要自行修改程序和重新合成bin播放文件

//本程序使用定时器3进行刷屏测速，串口打印刷屏速度，串口波特率为115200

//全速运行时请注释掉测速函数，不然影响刷屏速度

//请把制作的二进制播放文件放到存储卡apple目录下，并且文件名为apple.bin，完整路径为/apple/apple.bin

//如果出现花屏，请按复位键，有时是SD卡初始化不成功导致
int main(void)
{
	u8 *tempbuf;
	FIL * fftemp;
	u32 bread;
	u32 len_num;
	u32 mm_num;
	u32 num;
	u8 dt;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED  
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化  
	W25QXX_Init();				//初始化W25Q128
	usmart_dev.init(168);		//初始化USMART
	
	
	TIM3_Int_Init(9999,8399);//初始化这个定时器用于测速使用
	TIM_Cmd(TIM3,DISABLE); 
	
	FSMC_SRAM_Init();			//初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMEX);		//初始化外部内存池
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存  
  f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH.
	delay_ms(10);
	font_init();
	delay_ms(10);
	SD_Init();
	delay_ms(10);
	mm_num=9600; //设置一个屏幕需要的内存
	while(1)
	{
		tempbuf=mymalloc(SRAMEX,mm_num);
		fftemp=(FIL*)mymalloc(SRAMEX,sizeof(FIL));
		f_open (fftemp,"0:/apple/apple.bin", FA_READ); //打开路径下文件
		LCD_Scan_Dir(L2R_U2D);//从左到右,从上到下
		LCD_Set_Window(0,0,320,240);
		LCD_SetCursor(0,0);//设置光标位置 
		LCD_WriteRAM_Prepare();   	//开始写入GRAM
		len_num=mm_num;			//写入的数据长度
		bread=mm_num;
		
//下面注释的5行代码，是测速函数，如果需要测速，请把注释解除，可用串口查看刷屏速度，串口波特率为115200		

		while(bread==mm_num)//死循环执行读SD卡
		{
//			TIM_Cmd(TIM3,ENABLE);
//			TIM3->CNT=0;
			f_read(fftemp,tempbuf,mm_num,(UINT *)&bread);
			for(len_num=0;len_num<9600;len_num++)
			{
				dt=tempbuf[len_num];
				for(num=0;num<8;num++)
				{
					if((dt<<num)&0x80) LCD->LCD_RAM=0x0000;
					else LCD->LCD_RAM=0xFFFF;
				}
			}
//			num=TIM_GetCounter(TIM3);
//			printf("刷屏速度:%d FPS\r\n",10000/num);
//			TIM_Cmd(TIM3,DISABLE);
		}
		while(1); //播放完，while死循环停住播放
	} 
}







