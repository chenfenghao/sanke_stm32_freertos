/*
 * snake.h
 *
 *  Created on: 2019��12��20��
 *      Author: 13466
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include "stdint.h"
#define Snake_Max_Long 648//��󳤶�
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
}Food;//ʵ��

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
}Snake;//��


/*����*/
void START_InterFace(void);
void SET_InterFace(void);//
void OVER_InterFace(void);
void RECORD_InterFace(void);
void AUTHOR_InterFace(void);
void Draw_Point(uint16_t,uint16_t);//����
void Erase_Point(uint16_t,uint16_t);//����
void Draw_Food(uint16_t,uint16_t);//���Ե�
void Draw_HEAD(uint16_t,uint16_t);//��ͷ
void Draw_Snake();//����
void Snake_Init(void);//�ߵĳ�ʼ��
void Snake_DATA_FRESH(void);
/*�ߵ��ƶ�*/
void Snake_Up();
void Snake_Down();
void Snake_Left();
void Snake_Right();
/*�������ʳ��*/
void Rand_Number(void);
void Put_Food(void);
/*�����ж�*/
void GET_FOOD();//�Ƿ�Ե�ʳ��
void DEC_EAT_ITSELF();//�Ƿ�ҧ���Լ�
void DEC_HIT_WALL();//�Ƿ�ײǽ

#endif /* SNAKE_H_ */
