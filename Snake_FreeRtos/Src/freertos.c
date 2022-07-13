/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "lcd.h"
#include "queue.h"
#include "snake.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
const uint8_t KeyCodeLeft =0x01; //按键代码 KeyLeft
const uint8_t KeyCodeRight=0x02; //按键代码 KeyRight
const uint8_t KeyCodeUp =0x03; //按键代码 KeyUp
const uint8_t KeyCodeDown =0x04; //按键代码 KeyDown
uint16_t curScreenX=100; //LCD 当前 X
uint16_t curScreenY=260; //LCD 当前 Y
uint16_t lastScreenX=100; // LCD 前一步的 X
uint16_t lastScreenY=260; // LCD 前一步的 Y
uint8_t KEYSET=1;
uint32_t RECORD;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId_t LCD_SHOWHandle;
osThreadId_t KEY_SCANHandle;
osThreadId_t Rule_JudgmentHandle;
osThreadId_t GAMEOVERTASKHandle;
osThreadId_t Game_StartHandle;
osThreadId_t SETTTINGHandle;
osThreadId_t LED_BLINKHandle;
osThreadId_t SHOW_RECORDHandle;
osThreadId_t SHOW_AUTHORHandle;
osMessageQueueId_t Queue_KeysHandle;
osMessageQueueId_t Queue_key2Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void APP_LCD_SHOW(void *argument);
void APP_KEY_SCAN(void *argument);
void APP_Rule_judgment(void *argument);
void APP_Game_Over(void *argument);
void APP_Game_Start(void *argument);
void APP_Setting(void *argument);
void APP_blink(void *argument);
void StartTask08(void *argument);
void APP_author(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Queue_Keys */
  const osMessageQueueAttr_t Queue_Keys_attributes = {
    .name = "Queue_Keys"
  };
  Queue_KeysHandle = osMessageQueueNew (16, sizeof(uint8_t), &Queue_Keys_attributes);

  /* definition and creation of Queue_key2 */
  const osMessageQueueAttr_t Queue_key2_attributes = {
    .name = "Queue_key2"
  };
  Queue_key2Handle = osMessageQueueNew (16, sizeof(uint8_t), &Queue_key2_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LCD_SHOW */
  const osThreadAttr_t LCD_SHOW_attributes = {
    .name = "LCD_SHOW",
    .priority = (osPriority_t) osPriorityAboveNormal,
    .stack_size = 1024
  };
  LCD_SHOWHandle = osThreadNew(APP_LCD_SHOW, NULL, &LCD_SHOW_attributes);

  /* definition and creation of KEY_SCAN */
  const osThreadAttr_t KEY_SCAN_attributes = {
    .name = "KEY_SCAN",
    .priority = (osPriority_t) osPriorityHigh,
    .stack_size = 256
  };
  KEY_SCANHandle = osThreadNew(APP_KEY_SCAN, NULL, &KEY_SCAN_attributes);

  /* definition and creation of Rule_Judgment */
  const osThreadAttr_t Rule_Judgment_attributes = {
    .name = "Rule_Judgment",
    .priority = (osPriority_t) osPriorityAboveNormal1,
    .stack_size = 512
  };
  Rule_JudgmentHandle = osThreadNew(APP_Rule_judgment, NULL, &Rule_Judgment_attributes);

  /* definition and creation of GAMEOVERTASK */
  const osThreadAttr_t GAMEOVERTASK_attributes = {
    .name = "GAMEOVERTASK",
    .priority = (osPriority_t) osPriorityNormal7,
    .stack_size = 256
  };
  GAMEOVERTASKHandle = osThreadNew(APP_Game_Over, NULL, &GAMEOVERTASK_attributes);

  /* definition and creation of Game_Start */
  const osThreadAttr_t Game_Start_attributes = {
    .name = "Game_Start",
    .priority = (osPriority_t) osPriorityAboveNormal7,
    .stack_size = 512
  };
  Game_StartHandle = osThreadNew(APP_Game_Start, NULL, &Game_Start_attributes);

  /* definition and creation of SETTTING */
  const osThreadAttr_t SETTTING_attributes = {
    .name = "SETTTING",
    .priority = (osPriority_t) osPriorityAboveNormal6,
    .stack_size = 256
  };
  SETTTINGHandle = osThreadNew(APP_Setting, NULL, &SETTTING_attributes);

  /* definition and creation of LED_BLINK */
  const osThreadAttr_t LED_BLINK_attributes = {
    .name = "LED_BLINK",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  LED_BLINKHandle = osThreadNew(APP_blink, NULL, &LED_BLINK_attributes);

  /* definition and creation of SHOW_RECORD */
  const osThreadAttr_t SHOW_RECORD_attributes = {
    .name = "SHOW_RECORD",
    .priority = (osPriority_t) osPriorityAboveNormal6,
    .stack_size = 256
  };
  SHOW_RECORDHandle = osThreadNew(StartTask08, NULL, &SHOW_RECORD_attributes);

  /* definition and creation of SHOW_AUTHOR */
  const osThreadAttr_t SHOW_AUTHOR_attributes = {
    .name = "SHOW_AUTHOR",
    .priority = (osPriority_t) osPriorityNormal6,
    .stack_size = 256
  };
  SHOW_AUTHORHandle = osThreadNew(APP_author, NULL, &SHOW_AUTHOR_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_APP_LCD_SHOW */
/**
  * @brief  Function implementing the LCD_SHOW thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_APP_LCD_SHOW */
void APP_LCD_SHOW(void *argument)
{
    
    
    
    
    
    
    
    
    
    

  /* USER CODE BEGIN APP_LCD_SHOW */

	uint8_t keyCode;

  /* Infinite loop */
  for(;;)
	  {
	  TickType_t previousWakeTime=xTaskGetTickCount();
	   vTaskResume(Rule_JudgmentHandle);
	   vTaskSuspend(Game_StartHandle);
	   GET_FOOD();//是否吃到食物
	   Put_Food();//显示食物
	   BaseType_t result=xQueueReceive(Queue_KeysHandle, &keyCode, pdMS_TO_TICKS(1));
		switch(keyCode)
		{
		case m_up:
			if(DIR==m_down){Snake_Down();break;}
			Snake_Up();
			DIR=m_up;
			break;
		case m_down:
			if(DIR==m_up){Snake_Up();break;}
			Snake_Down();
			DIR=m_down;
			break;
		case m_left:
			if(DIR==m_right){Snake_Right();break;}
			Snake_Left();
			DIR=m_left;
			break;
		case m_right:
			if(DIR==m_left){Snake_Left();break;}
			Snake_Right();
			DIR=m_right;
			break;
		default:switch(DIR)
		{
		case(m_left):Snake_Left();break;
		case(m_right):Snake_Right();break;
		case(m_up):Snake_Up();break;
		case(m_down):Snake_Down();break;
		default:break;
		}
		}
	   vTaskDelayUntil(&previousWakeTime,pdMS_TO_TICKS(Snake.Level)); //如果延时较长，可能导致队列里有多个项
	   }
    //osDelay(1);
  /* USER CODE END APP_LCD_SHOW */
}

/* USER CODE BEGIN Header_APP_KEY_SCAN */
/**
* @brief Function implementing the KEY_SCAN thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_KEY_SCAN */
void APP_KEY_SCAN(void *argument)
{
  /* USER CODE BEGIN APP_KEY_SCAN */
	BaseType_t err;
	BaseType_t err1;
	GPIO_PinState keyState=GPIO_PIN_SET;
	TickType_t delayAfterPress=pdMS_TO_TICKS(200); //按键按下后的延时，用于消除按键抖动
  /* Infinite loop */
  for(;;)
  {
	  	  keyState=HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_3); //PH3=KeyRight, 上拉
	  	  if (keyState==GPIO_PIN_RESET) //KeyRight 是低输入有效
	  	  {
	  	  err= xQueueSendToBack(Queue_KeysHandle, &KeyCodeRight, pdMS_TO_TICKS(100));
		err1= xQueueSendToBack(Queue_key2Handle, &KeyCodeRight, pdMS_TO_TICKS(100));
	  	  vTaskDelay(delayAfterPress); //延时，去除抖动,同时让任务调度执行
	  	  }
	  	  keyState=HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_2); //PH2=KeyDown, 上拉
	  	  if (keyState==GPIO_PIN_RESET) //KeyDown 是低输入有效
	  	  {
	  	  err= xQueueSendToBack(Queue_KeysHandle, &KeyCodeDown, pdMS_TO_TICKS(100));
	  	 err1= xQueueSendToBack(Queue_key2Handle, &KeyCodeDown, pdMS_TO_TICKS(100));
	  	  vTaskDelay(delayAfterPress); //延时，去除抖动,同时让任务调度执行
	  	  }
	  	  keyState=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13); //PE4=KeyLeft, 上拉
	  	  if (keyState==GPIO_PIN_RESET) //KeyLeft 是低输入有效
	  	  {
	  	  err= xQueueSendToBack(Queue_KeysHandle, &KeyCodeLeft, pdMS_TO_TICKS(100));
	  	 err1= xQueueSendToBack(Queue_key2Handle, &KeyCodeLeft, pdMS_TO_TICKS(100));
	  	  vTaskDelay(delayAfterPress); //延时，去除抖动,同时让任务调度执行
	  	  }
	  	  keyState=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); //PA0=KeyUp, 下拉
	  	  if (keyState==GPIO_PIN_SET) //KeyUp 是高输入有效
	  	  {
	  	  err= xQueueSendToBack(Queue_KeysHandle, &KeyCodeUp, pdMS_TO_TICKS(100));
	  	err1= xQueueSendToBack(Queue_key2Handle, &KeyCodeUp, pdMS_TO_TICKS(100));
	  	  vTaskDelay(delayAfterPress); //延时，去除抖动,同时让任务调度执行
	  	  }
	  	  if (err == errQUEUE_FULL) //如果队列满了，就复位队列
	  	  xQueueReset(Queue_KeysHandle);
	  	  if (err1 == errQUEUE_FULL) //如果队列满了，就复位队列
	  	  xQueueReset(Queue_key2Handle);
	  	  vTaskDelay(pdMS_TO_TICKS(10)); //进入阻塞状态'
  }
  /* USER CODE END APP_KEY_SCAN */
}

/* USER CODE BEGIN Header_APP_Rule_judgment */
/**
* @brief Function implementing the Rule_Judgment thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_Rule_judgment */
void APP_Rule_judgment(void *argument)
{
  /* USER CODE BEGIN APP_Rule_judgment */
  /* Infinite loop */
  for(;;)
  {
	  DEC_EAT_ITSELF();//是否咬到自己
	  DEC_HIT_WALL();//是否撞墙
	  if(Snake.isLife==0)
	  {
		  Snake.isSTART=0;
		  vTaskSuspend(LCD_SHOWHandle);
		  vTaskSuspend(SETTTINGHandle);
		  vTaskSuspend(Game_StartHandle);
		  xQueueReset(Queue_key2Handle);
		  vTaskResume(GAMEOVERTASKHandle);
		  vTaskSuspend(NULL);
	  }
      osDelay(Snake.Level/2);
  }
  /* USER CODE END APP_Rule_judgment */
}

/* USER CODE BEGIN Header_APP_Game_Over */
/**
* @brief Function implementing the GAMEOVERTASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_Game_Over */
void APP_Game_Over(void *argument)
{
  /* USER CODE BEGIN APP_Game_Over */
	 uint8_t key3Code;
  /* Infinite loop */
  for(;;)
  {
	  DIR=m_right;
	 Snake.isSTART=0;
	 vTaskSuspend(Rule_JudgmentHandle);
	 RECORD=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0);
	 if(Snake.Len-Snake.len_set>RECORD)
	 {
		 LCD_FS=LcdFont_Size16;
		 LCD_ShowString(80,175, (uint8_t*)"NEW RECORD!!!");
		 LCD_FS=LcdFont_Size16;
		 LCD_ShowString(100,150, (uint8_t*)"RECORD:");
		 LCD_ShowNum(150,150,RECORD,3,12);
	     HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,Snake.Len-Snake.len_set);
	 }
	 else
	 {
		 LCD_FS=LcdFont_Size16;
		 LCD_ShowString(100,150, (uint8_t*)"RECORD:");
		 LCD_ShowNum(150,150,RECORD,3,12);
	 }
	BaseType_t result3=xQueueReceive(Queue_key2Handle, &key3Code, pdMS_TO_TICKS(100));
	OVER_InterFace();
	//HAL_Delay(500);
	if (result3 != pdTRUE)
	continue;

	else{
		 Snake.isLife=1;
		 START_InterFace();
		 xQueueReset(Queue_key2Handle);
		 xQueueReset(Queue_KeysHandle);
		 vTaskResume(Game_StartHandle);
		 vTaskSuspend(NULL);
	}


	}

    osDelay(100);

  /* USER CODE END APP_Game_Over */
}

/* USER CODE BEGIN Header_APP_Game_Start */
/**
* @brief Function implementing the Game_Start thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_Game_Start */
void APP_Game_Start(void *argument)
{
  /* USER CODE BEGIN APP_Game_Start */

  uint8_t key1Code;
  Snake.isSTART=0;
  Snake.len_set=5;
  Snake.speed_set=100;
  vTaskSuspend(LCD_SHOWHandle);
  vTaskSuspend(SETTTINGHandle);
  vTaskSuspend(GAMEOVERTASKHandle);
  vTaskSuspend(Rule_JudgmentHandle);
  vTaskSuspend(SHOW_RECORDHandle);
  vTaskSuspend(SHOW_AUTHORHandle);
  /* Infinite loop */
  for(;;)
  {
	  vTaskSuspend(LCD_SHOWHandle);
	  vTaskSuspend(SETTTINGHandle);
	  vTaskSuspend(GAMEOVERTASKHandle);
	  vTaskSuspend(Rule_JudgmentHandle);
	  vTaskSuspend(SHOW_RECORDHandle);
	  vTaskSuspend(SHOW_AUTHORHandle);
	  Snake.isSTART=0;
	  START_InterFace();
	  while(Snake.isSTART==0)
	  {

	Snake.isLife=1;
	  BaseType_t result1=xQueueReceive(Queue_key2Handle, &key1Code, pdMS_TO_TICKS(100));
	   if (result1 != pdTRUE)
	   continue;
		switch(key1Code)
		{
		case m_up:
			KEYSET--;
			if(KEYSET>4){KEYSET=1;}
			if(KEYSET<1){KEYSET=4;}
			LCD_Fill(20,60,59,300,LcdColor_BLACK);//黑色
			LCD_Draw_Circle(40,80+60*(KEYSET-1),5);//指向Start
			break;
		case m_down:
			KEYSET++;
			if(KEYSET>4){KEYSET=1;}
			if(KEYSET<1){KEYSET=4;}
			LCD_Fill(20,60,59,300,LcdColor_BLACK);//黑色
			LCD_Draw_Circle(40,80+60*(KEYSET-1),5);//指向Start
			break;
		case m_left:
			switch(KEYSET)
			{
			case 1:
				Snake.isSTART=1;
				xQueueReset(Queue_key2Handle);
				Snake_Init();
				vTaskResume(LCD_SHOWHandle);
				xQueueReset(Queue_KeysHandle);
				vTaskResume(Rule_JudgmentHandle);
				break;
			case 2:
				xQueueReset(Queue_key2Handle);
				xQueueReset(Queue_KeysHandle);
				SET_InterFace();KEYSET=1;
				vTaskResume(SETTTINGHandle);
				vTaskSuspend(NULL);
				break;
			case 3:
			xQueueReset(Queue_key2Handle);
			xQueueReset(Queue_KeysHandle);
			RECORD_InterFace();KEYSET=1;
			vTaskResume(SHOW_RECORDHandle);
			vTaskSuspend(NULL);
			break;
			case 4:
				xQueueReset(Queue_key2Handle);
				xQueueReset(Queue_KeysHandle);
				AUTHOR_InterFace();
				vTaskResume(SHOW_AUTHORHandle);
				vTaskSuspend(NULL);
				break;
			}
			break;
			default:break;
	   }
		xQueueReset(Queue_KeysHandle);
	  }
	  vTaskSuspend(NULL);
      osDelay(1);
  }
  /* USER CODE END APP_Game_Start */
}

/* USER CODE BEGIN Header_APP_Setting */
/**
* @brief Function implementing the SETTTING thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_Setting */
void APP_Setting(void *argument)
{
  /* USER CODE BEGIN APP_Setting */
  /* Infinite loop */
	uint8_t key2Code;
  for(;;)
  {
	  vTaskSuspend(Game_StartHandle);
		BaseType_t result2=xQueueReceive(Queue_key2Handle, &key2Code, pdMS_TO_TICKS(100));
	   if (result2 != pdTRUE)
	   continue;
		switch(key2Code)
		{
	case m_up:
	KEYSET--;
	if(KEYSET>3){KEYSET=1;}
	if(KEYSET<1){KEYSET=3;}
	LCD_Fill(201,50,250,300,LcdColor_BLACK);//黑色
	LCD_Draw_Circle(210,69+40*(KEYSET-1),5);
	break;
	case m_down:
	KEYSET++;
	if(KEYSET>3){KEYSET=1;}
	if(KEYSET<1){KEYSET=3;}
	LCD_Fill(201,50,250,300,LcdColor_BLACK);//黑色
	LCD_Draw_Circle(210,69+40*(KEYSET-1),5);
	break;
	case m_left:
		switch(KEYSET)
		{
		case 1:Snake.len_set--;
		if(Snake.len_set<2||Snake.speed_set<10)
		{
			Snake.len_set=2;
			LCD_FS=LcdFont_Size24;
			LCD_ShowString(2,200, (uint8_t*)"length_OUTRANGE");
		}
		else
		{
			LCD_Fill(2,200,300,224,LcdColor_BLACK);//l
		}
		LCD_ShowNum(155,60,Snake.len_set,2,16);
		break;
		case 2:Snake.speed_set=Snake.speed_set-10;
		if(Snake.len_set<2||Snake.speed_set<10)
		{
			Snake.speed_set=10;
			LCD_FS=LcdFont_Size24;
			LCD_ShowString(2,225, (uint8_t*)"Speed_OUTRANGE");
		}
		else
		{
		LCD_Fill(2,225,300,249,LcdColor_BLACK);//s
		}
		LCD_ShowNum(155,100,Snake.speed_set,3,16);
		break;
		case 3:
		START_InterFace();
		xQueueReset(Queue_KeysHandle);
		xQueueReset(Queue_key2Handle);
		vTaskResume(Game_StartHandle);
		vTaskSuspend(NULL);
		break;
		default:break;
		}
		break;
	case m_right:
		switch(KEYSET)
			{
			case 1:Snake.len_set++;
			if(Snake.len_set>7||Snake.speed_set>500)
			{
				Snake.len_set=7;
				LCD_FS=LcdFont_Size24;
				LCD_ShowString(2,200, (uint8_t*)"length_OUTRANGE");

			}
			else
			{
				LCD_Fill(2,200,300,224,LcdColor_BLACK);//l
			}
			LCD_ShowNum(155,60,Snake.len_set,2,16);
			break;
			case 2:
				Snake.speed_set=Snake.speed_set+10;

				if(Snake.len_set>7||Snake.speed_set>500)
				{
				Snake.speed_set=500;
				LCD_FS=LcdFont_Size24;
				LCD_ShowString(2,225, (uint8_t*)"Speed_OUTRANGE");
				}
				else
			   {
					LCD_Fill(2,225,300,249,LcdColor_BLACK);//s
				}
				LCD_ShowNum(155,100,Snake.speed_set,3,16);
				break;
			default:break;
			}
	break;
		}

    osDelay(1);
  }
  /* USER CODE END APP_Setting */
}

/* USER CODE BEGIN Header_APP_blink */
/**
* @brief Function implementing the LED_BLINK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_blink */
void APP_blink(void *argument)
{
  /* USER CODE BEGIN APP_blink */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1); //PF10=LED2
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); //PF9=LED1n
	  osDelay(500);
  }
  /* USER CODE END APP_blink */
}


/* USER CODE BEGIN Header_StartTask08 */
/**
* @brief Function implementing the SHOW_RECORD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask08 */
void StartTask08(void *argument)
{
  /* USER CODE BEGIN StartTask08 */
	uint8_t key2Code;
  /* Infinite loop */
  for(;;)
  {
	  vTaskSuspend(Game_StartHandle);
		BaseType_t result2=xQueueReceive(Queue_key2Handle, &key2Code, pdMS_TO_TICKS(100));
	   if (result2 != pdTRUE)
	   continue;
		switch(key2Code)
		{
		case m_up:
			KEYSET--;
			if(KEYSET>2){KEYSET=1;}
			if(KEYSET<1){KEYSET=2;}
			LCD_ShowString(2,160, (uint8_t*)"    ");
			LCD_ShowString(2,185, (uint8_t*)"    ");
			LCD_ShowString(2,160+25*(KEYSET-1), (uint8_t*)" ->");

			break;
		case m_down:
			KEYSET++;
			if(KEYSET>2){KEYSET=1;}
			if(KEYSET<1){KEYSET=2;}
			LCD_ShowString(2,160, (uint8_t*)"    ");
			LCD_ShowString(2,185, (uint8_t*)"    ");
			LCD_ShowString(2,160+25*(KEYSET-1), (uint8_t*)" ->");
			break;
		case m_left:
			switch(KEYSET)
			{
			case 1:
				START_InterFace();
				xQueueReset(Queue_KeysHandle);
				xQueueReset(Queue_key2Handle);
				vTaskResume(Game_StartHandle);
				vTaskSuspend(NULL);
				break;
			case 2:

			     HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0);
			     RECORD=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0);
			     LCD_ShowNum(40,100,RECORD,3,32);
				break;
			default:
			break;
			}
			default:
				break;
		}
    osDelay(1);
  }
  /* USER CODE END StartTask08 */
}

/* USER CODE BEGIN Header_APP_author */
/**
* @brief Function implementing the SHOW_AUTHOR thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_author */
void APP_author(void *argument)
{
  /* USER CODE BEGIN APP_author */
  /* Infinite loop */
	uint8_t key3Code;
  for(;;)
  {
	  //AUTHOR_InterFace();
	  BaseType_t result3=xQueueReceive(Queue_key2Handle, &key3Code, pdMS_TO_TICKS(100));
	  if (result3 != pdTRUE)
	  	continue;

	  	else{
			START_InterFace();
			xQueueReset(Queue_KeysHandle);
			xQueueReset(Queue_key2Handle);
			vTaskResume(Game_StartHandle);
			vTaskSuspend(NULL);
	  	}

    osDelay(1);
  }
  /* USER CODE END APP_author */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
