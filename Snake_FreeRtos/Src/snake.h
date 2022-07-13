/*
 * snake.h
 *
 *  Created on: 2019年12月20日
 *      Author: 13466
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include "stdint.h"
#define Snake_Max_Long 648//最大长度
#define Init_X 16;
#define Init_Y 160;
#define Init_Len 5;
#define SPEED 100
#define WALL_Xmin 10
#define WALL_Ymin 50
#define WALL_Xmax 230
#define WALL_Ymax 250
uint8_t KEY;
uint8_t DIR;
#define m_up    0x03
#define m_down  0x04
#define m_left  0x01
#define m_right 0x02

struct
{
	uint16_t  X;
	uint16_t  Y;
	unsigned char ISHOW;
}Food;//实物

struct
{
 	uint16_t X[Snake_Max_Long];
	uint16_t Y[Snake_Max_Long];
	unsigned char Len;
	unsigned char isLife;
	unsigned char Level;
	uint8_t isSTART;
	uint16_t len_set;
	uint16_t speed_set;
}Snake;//蛇


/*画蛇*/
void START_InterFace(void);
void SET_InterFace(void);//
void OVER_InterFace(void);
void RECORD_InterFace(void);
void AUTHOR_InterFace(void);
void Draw_Point(uint16_t,uint16_t);//画点
void Erase_Point(uint16_t,uint16_t);//擦点
void Draw_Food(uint16_t,uint16_t);//画吃的
void Draw_HEAD(uint16_t,uint16_t);//画头
void Draw_Snake();//画蛇
void Snake_Init(void);//蛇的初始化
void Snake_DATA_FRESH(void);
/*蛇的移动*/
void Snake_Up();
void Snake_Down();
void Snake_Left();
void Snake_Right();
/*随机生成食物*/
void Rand_Number(void);
void Put_Food(void);
/*规则判定*/
void GET_FOOD();//是否吃到食物
void DEC_EAT_ITSELF();//是否咬到自己
void DEC_HIT_WALL();//是否撞墙

#endif /* SNAKE_H_ */
