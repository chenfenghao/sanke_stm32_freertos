/*
 * sanke.c
 *
 *  Created on: 2019年12月20日
 *      Author: 13466
 */


#include "snake.h"
#include "lcd.h"
#include "main.h"
#include "rng.h"
#include "rtc.h"


void Snake_Init(void)
{

	LCD_Fill(20,60,300,300,LcdColor_BLACK);//黑色
	lcdFRONT_COLOR=GRAYBLUE;
	LCD_DrawRectangle(WALL_Xmin-3,WALL_Ymin-3,WALL_Xmax+4,WALL_Ymax);
	lcdFRONT_COLOR=LcdColor_WHITE;
	LCD_ShowString(20,260, (uint8_t*)"SCORE:");
	//LCD_ShowString(130,260, (uint8_t*)"LENGTH:");
	Snake.Len=Snake.len_set;
	Snake.isLife=1;
	KEY=0x11;
	DIR=m_right;
	Snake.Level=Snake.speed_set;
	Food.ISHOW=0;
	LCD_ShowNum(180,260,(Snake.Len-Snake.len_set),2,16);
	//DIR=m_right;
	//LCD_ShowNum(180,260,(Snake.Len),2,16);
	//LCD_ShowNum(70,260,(Snake.Level),2,16);
  	for(int s1=0;s1<Snake.Len;s1++)
  	{
  		Snake.X[s1]=s1*8+Init_X;//初始横坐标
  		Snake.Y[s1]=Init_Y;//初始纵坐标
  	}
  	Draw_HEAD(Snake.X[Snake.Len-1],Snake.Y[Snake.Len-1]);
  	 for(int s2=0;s2<Snake.Len-1;s2++)
  	{
  	Draw_Point(Snake.X[s2],Snake.Y[s2]);
  	}
}
void Draw_Point(uint16_t x,uint16_t y)
{
	LCD_Fill((uint16_t)(x+1),(uint16_t)(y+1),(uint16_t)(x+7),(uint16_t)(y+7),LcdColor_RED);//红色

}//画点
void Erase_Point(uint16_t x,uint16_t y)
{
	LCD_Fill((uint16_t)(x),(uint16_t)(y),(uint16_t)(x+8),(uint16_t)(y+8),LcdColor_BLACK);//黑色

}//擦点
void Draw_Food(uint16_t x,uint16_t y)
{
	LCD_Fill((uint16_t)(x+2),(uint16_t)(y+2),(uint16_t)(x+6),(uint16_t)(y+6),LcdColor_GREEN);//绿
}//画吃的
void Draw_HEAD(uint16_t x,uint16_t y)
{
	LCD_Fill((uint16_t)(x+1),(uint16_t)(y+1),(uint16_t)(x+7),(uint16_t)(y+7),LcdColor_BRRED);//
}//画头
void Draw_Snake()
{
	Draw_HEAD(Snake.X[Snake.Len-1],Snake.Y[Snake.Len-1]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Draw_Point(Snake.X[i],Snake.Y[i]);
	}
}//画蛇
void Snake_Up()
{
	Erase_Point(Snake.X[0],Snake.Y[0]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Snake.X[i]=Snake.X[i+1];
	Snake.Y[i]=Snake.Y[i+1];
	}
	Snake.X[Snake.Len-1]=Snake.X[Snake.Len-2]+0*8;
	Snake.Y[Snake.Len-1]=Snake.Y[Snake.Len-2]-1*8;
	/*Draw_HEAD(Snake.X[Snake.Len-1],Snake.Y[Snake.Len-1]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Draw_Point(Snake.X[i],Snake.Y[i]);
	}*/
	Draw_Snake();
}//向上一格
void Snake_Down()
{
	Erase_Point(Snake.X[0],Snake.Y[0]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Snake.X[i]=Snake.X[i+1];
	Snake.Y[i]=Snake.Y[i+1];
	}
	Snake.X[Snake.Len-1]=Snake.X[Snake.Len-2];
	Snake.Y[Snake.Len-1]=Snake.Y[Snake.Len-2]+1*8;
	/*Draw_HEAD(Snake.X[Snake.Len-1],Snake.Y[Snake.Len-1]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Draw_Point(Snake.X[i],Snake.Y[i]);
	}*/
	Draw_Snake();
}//向下一格
void Snake_Left()
{
	Erase_Point(Snake.X[0],Snake.Y[0]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Snake.X[i]=Snake.X[i+1];
	Snake.Y[i]=Snake.Y[i+1];
	}
	Snake.X[Snake.Len-1]=Snake.X[Snake.Len-2]-1*8;
	Snake.Y[Snake.Len-1]=Snake.Y[Snake.Len-2]+0*8;
	/*Draw_HEAD(Snake.X[Snake.Len-1],Snake.Y[Snake.Len-1]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Draw_Point(Snake.X[i],Snake.Y[i]);
	}*/
	Draw_Snake();
}//向左一格
void Snake_Right()
{
	Erase_Point(Snake.X[0],Snake.Y[0]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Snake.X[i]=Snake.X[i+1];
	Snake.Y[i]=Snake.Y[i+1];
	}
	Snake.X[Snake.Len-1]=Snake.X[Snake.Len-2]+1*8;
	Snake.Y[Snake.Len-1]=Snake.Y[Snake.Len-2]+0*8;
	/*Draw_HEAD(Snake.X[Snake.Len-1],Snake.Y[Snake.Len-1]);
	for(int i=0;i<Snake.Len-1;i++)
	{
	Draw_Point(Snake.X[i],Snake.Y[i]);
	}*/
	Draw_Snake();
}//向右一格
void Rand_Number(void)
{

		Food.X=8*(2+HAL_RNG_GetRandomNumber(&hrng)%27);
		Food.Y=8*(7+HAL_RNG_GetRandomNumber(&hrng)%24);

}//随机数
void Put_Food(void)
{
	if(Food.ISHOW==1)
	{
		Draw_Food(Food.X,Food.Y);
	}
	if(Food.ISHOW==0)
	{
		while(Snake.Len<Snake_Max_Long+1)
		{
			Rand_Number();
			int m=0;
			for(m;m<Snake.Len;m++)
			{
				if((Food.X==Snake.X[m])&&(Food.Y==Snake.Y[m]))
					break;
			}
			if(m==Snake.Len){Food.ISHOW=1;break;}
		}

	}
}//生成食物

void GET_FOOD()
{
	if((Snake.X[Snake.Len-1]==Food.X)&&(Snake.Y[Snake.Len-1]==Food.Y))
	{
		Erase_Point(Food.X,Food.Y);
		Snake.Len++;
		Snake.Level++;
		Snake.X[Snake.Len-1]=Food.X;
		Snake.Y[Snake.Len-1]=Food.Y;
		//Score
		Draw_HEAD(Food.X,Food.Y);
		Food.ISHOW=0;
		LCD_ShowNum(180,260,(uint32_t)(Snake.Len-Snake.len_set),2,16);
	}
}//是否吃到食物

void DEC_EAT_ITSELF()
{
	if(Snake.Len>3)
	{
	for(int m=3;m<Snake.Len;m++)
	{
		if(Snake.X[Snake.Len-m]==Snake.X[Snake.Len-1]&&Snake.Y[Snake.Len-m]==Snake.Y[Snake.Len-1])
		{
			//LCD_ShowString(80,80, (uint8_t*)"YAO DAO ZI JI");
			Snake.isLife=0;
		}
	}
	}
}//是否咬到自己

void DEC_HIT_WALL()
{
	if((Snake.X[Snake.Len-1]<WALL_Xmin-8)||(Snake.X[Snake.Len-1]>WALL_Xmax)||(Snake.Y[Snake.Len-1]>WALL_Ymax-8)||(Snake.Y[Snake.Len-1]<WALL_Ymin-8))
	{
		//LCD_ShowString(80,80, (uint8_t*)"ZHUANG QIANG");
		Snake.isLife=0;
	}

}

void START_InterFace(void)
{

	LCD_Fill(0,30uj,300,300,LcdColor_BLACK);//黑色
	lcdFRONT_COLOR=LcdColor_WHITE;
	LCD_FS=LcdFont_Size16;
	//LCD_ShowString(0,7, (uint8_t*)"SNAKE GAME   CFH 1705010408");
	LCD_ShowString(0,7, (uint8_t*)"SNAKE GAME   ");
	lcdFRONT_COLOR=GRAYBLUE;
	LCD_DrawLine(0,30,240,30);
	LCD_DrawRectangle(60,60,180,100);
	LCD_DrawRectangle(60,120,180,160);
	LCD_DrawRectangle(60,180,180,220);
	LCD_DrawRectangle(60,240,180,280);
	LCD_FS=LcdFont_Size24;
	LCD_ShowString(90,68, (uint8_t*)"START");
	LCD_ShowString(78,128, (uint8_t*)"SETTING");
	LCD_ShowString(78,188, (uint8_t*)"RECOARD");
	LCD_ShowString(84,248, (uint8_t*)"AUTHOR");
	//LCD_Draw_Circle(40,80,5);//指向Start
}//初始化界面
void SET_InterFace(void)
{
	LCD_Fill(20,60,300,300,LcdColor_BLACK);//黑色
	LCD_FS=LcdFont_Size24;
	LCD_ShowString(2,60, (uint8_t*)"InitLength:");
	LCD_ShowString(2,100, (uint8_t*)"InitSpeed:");
	LCD_ShowString(154,142, (uint8_t*)"SET");
	LCD_DrawRectangle(150,55,200,84);
	LCD_ShowNum(155,60,(uint32_t)(Snake.len_set),2,16);
	LCD_ShowNum(155,100,(uint32_t)(Snake.speed_set),3,16);
	LCD_DrawRectangle(150,95,200,124);
	LCD_DrawRectangle(150,135,200,164);
	//LCD_Draw_Circle(210,69,5);

}//设置界面
void OVER_InterFace(void)
{
	LCD_DrawRectangle(60,80,200,200);
	//LCD_Fill(60,80,200,200,LcdColor_BLACK);//黑色
	LCD_FS=LcdFont_Size24;
	LCD_ShowString(74,100, (uint8_t*)"GAME OVER");
	LCD_FS=LcdFont_Size16;
	LCD_ShowString(100,128, (uint8_t*)"SCORE:");
	LCD_ShowNum(142,128,(uint32_t)(Snake.Len-Snake.len_set),2,16);

}//结束界面

void RECORD_InterFace(void)
{
	LCD_Fill(20,60,300,300,LcdColor_BLACK);//黑色
	LCD_FS=LcdFont_Size24;
	LCD_ShowString(2,60, (uint8_t*)"YOUR RECORD IS");
	uint32_t  Record=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0);
	LCD_ShowNum(40,100,Record,3,32);
	LCD_FS=LcdFont_Size24;
	LCD_ShowString(2,160, (uint8_t*)"    BACK");
	LCD_ShowString(2,185, (uint8_t*)"    CLEAN");
}
void AUTHOR_InterFace(void)
{
	LCD_Fill(20,60,300,300,LcdColor_BLACK);//黑色
	LCD_FS=LcdFont_Size16;
	LCD_ShowString(0,80, (uint8_t*)"Class:Automation 1704");
	LCD_ShowString(0,105, (uint8_t*)"Author:ChenFenghao");
	LCD_ShowString(0,130, (uint8_t*)"Student ID:1705010408");
	LCD_ShowString(0,155, (uint8_t*)"Name of work:");
	LCD_ShowString(0,180, (uint8_t*)"Greedy Snake Based on FreeRtos");




}
