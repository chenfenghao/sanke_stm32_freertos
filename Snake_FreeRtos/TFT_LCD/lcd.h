#ifndef __LCD_H
#define __LCD_H		

#include	"main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//2.8��/3.5��/4.3��/7�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/NT35310/NT35510/SSD1963��		    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	 
//////////////////////////////////////////////////////////////////////////////////	 

//LCD��Ҫ������
typedef struct  
{		 	 
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				//LCD ID
	uint8_t  dir;			//���������������ƣ�0��������1��������
	uint16_t wramcmd;		//��ʼдgramָ��
	uint16_t setxcmd;		//����x����ָ��
	uint16_t setycmd;		//����y����ָ��
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint32_t  lcdFRONT_COLOR;//Ĭ�Ϻ�ɫ
extern uint32_t  lcdBACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------MCU�� LCD�˿ڶ���---------------- 
#define	LCD_LED_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
     //PBout(5) 	//LCD����	PB5
//LCD��ַ�ṹ��
typedef struct
{
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
} LCD_TypeDef;
//ʹ��NOR/SRAM�� Bank1.sector1,��ַλHADDR[27,26]=00  A18��Ϊ��������������
//ע������ʱSTM32�ڲ�������һλ����!  			    
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)


//////////////////////////////////////////////////////////////////////////////////
///=============��ά����������==============
//ѡ��ǰʹ�õ�LCD�ͺ�
#define		AUTO_DETECT_LCD_ID		1		//�Ƿ��Զ����LCD ID,  1=�Զ����
#define		LCD_ID_9341
//#define		LCD_ID_5310
//#define		LCD_ID_5510			//4.3�� MCU TFT LCD(������), 800*480 �ֱ���
//#define		LCD_ID_1963

//�����С����
#define		LcdFont_Size12	 12		//����12
#define		LcdFont_Size16	 16		//����16
#define		LcdFont_Size24	 24		//����24
#define		LcdFont_Size32	 32		//����32

//һЩȫ�ֱ���
extern	uint16_t LCD_W;		//LCD ���,����LCD_Display_Dir()�������ʼ��
extern	uint16_t LCD_H;		//LCD �߶�
extern	uint8_t	 LCD_FS;	//�����Сֻ����12��16��24, 32, һ���ַ����Ϊ��������С,��LCD_Init()���ʼ��
extern	uint16_t LCD_CurPosX;	//��ǰλ��X
extern	uint16_t LCD_CurPosY;	//��ǰλ��Y


//////////////////////

//ɨ�跽����
#define L2R_U2D  0 		//������,���ϵ���
#define L2R_D2U  1 		//������,���µ���
#define R2L_U2D  2 		//���ҵ���,���ϵ���
#define R2L_D2U  3 		//���ҵ���,���µ���

#define U2D_L2R  4 		//���ϵ���,������
#define U2D_R2L  5 		//���ϵ���,���ҵ���
#define D2U_L2R  6 		//���µ���,������
#define D2U_R2L  7		//���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define LcdColor_WHITE         	 0xFFFF
#define LcdColor_BLACK         	 0x0000	  
#define LcdColor_BLUE         	 0x001F  
#define LcdColor_BRED            0XF81F
#define LcdColor_GRED 			 0XFFE0
#define LcdColor_GBLUE			 0X07FF
#define LcdColor_RED           	 0xF800
#define LcdColor_MAGENTA       	 0xF81F
#define LcdColor_GREEN         	 0x07E0
#define LcdColor_CYAN          	 0x7FFF
#define LcdColor_YELLOW        	 0xFFE0
#define LcdColor_BROWN 			 0XBC40 //��ɫ
#define LcdColor_BRRED 			 0XFC07 //�غ�ɫ
#define LcdColor_GRAY  			 0X8430 //��ɫ

//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  

//====�Ĵ�����д��������=======
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);

uint16_t LCD_ReadReg(uint16_t LCD_Reg);

void LCD_WriteRAM_Prepare(void);

void LCD_WriteRAM(uint16_t RGB_Code);


//========��ʼ��������========
void LCD_Init(void);	//��ʼ��

void LCD_DisplayOn(void);	//����ʾ

void LCD_DisplayOff(void);	//����ʾ

void LCD_Clear(uint32_t Color);	 	//����

void LCD_SSD_BackLightSet(uint8_t pwm);	//SSD1963 �������

void LCD_Scan_Dir(uint8_t dir);		//������ɨ�跽��

void LCD_Display_Dir(uint8_t dir);	//������Ļ��ʾ����

void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);	//���ô���

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);	//���ù��

//========��ͼ����=========
void LCD_DrawPoint(uint16_t x,uint16_t y);			//����

void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color);	//���ٻ���

uint32_t  LCD_ReadPoint(uint16_t x,uint16_t y); 	//����

void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);	//��Բ

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);	//����

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);	//������

void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);	//��䵥ɫ

void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	//���ָ����ɫ

//==========��ʾ���ֺ��ַ�===================
void LCD_setFontSize(uint8_t	fontSize);//	���������С��ֻ����12��16��24, 32

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);	//��ʾһ���ַ�
//void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);	//��ʾһ���ַ�

void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  //��ʾһ������

void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);	//��ʾ ����

void LCD_ShowString(uint16_t x,uint16_t y,uint8_t *p);	//��ʾһ���ַ���
//void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);	//��ʾһ���ַ���,12/16����



//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��
//���¼����������Զ�����
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



