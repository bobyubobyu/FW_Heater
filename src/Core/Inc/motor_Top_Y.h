/*
 * motor_TOP_Y.h
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#ifndef INC_MOTOR_TOP_Y_H_
#define INC_MOTOR_TOP_Y_H_

#define		HWO_M3_LOW				    HAL_GPIO_WritePin (TOP_Y_PU_GPIO_Port, TOP_Y_PU_Pin,0)
#define     HWO_M3_TRIGGER	            HAL_GPIO_TogglePin(TOP_Y_PU_GPIO_Port, TOP_Y_PU_Pin)
#define		HWO_M3_BACKWARD			    HAL_GPIO_WritePin (TOP_Y_DR_GPIO_Port, TOP_Y_DR_Pin,1)
#define		HWO_M3_FORWARD			    HAL_GPIO_WritePin (TOP_Y_DR_GPIO_Port, TOP_Y_DR_Pin,0)
#define		HWO_M3_ENABLE			    HAL_GPIO_WritePin (TOP_Y_LC_GPIO_Port, TOP_Y_LC_Pin,0)
#define		HWO_M3_DISABLE			    HAL_GPIO_WritePin (TOP_Y_LC_GPIO_Port, TOP_Y_LC_Pin,1)
#define     HWO_M3_RESET_ON 		    HAL_GPIO_WritePin (TOP_Y_RESET_GPIO_Port, TOP_Y_RESET_Pin,1)
#define     HWO_M3_RESET_OFF		    HAL_GPIO_WritePin (TOP_Y_RESET_GPIO_Port, TOP_Y_RESET_Pin,0)

#define		HWI_SENSOR_M3_OVER_CUEERNT  (HAL_GPIO_ReadPin(TOP_Y_Flag_GPIO_Port, TOP_Y_Flag_Pin) == 0x01)
#define     HWI_M3_CW_LIMIT             (HAL_GPIO_ReadPin(TOP_Y_END_GPIO_Port, TOP_Y_END_Pin) == 0x01)
#define     HWI_M3_CCW_LIMIT            (HAL_GPIO_ReadPin(TOP_Y_HOME_GPIO_Port, TOP_Y_HOME_Pin) == 0x01)

//------------------------------------------------------------------------------
// 設定好要使用的TIMER編號
#define M3_TIMER    TIMER6

//開啟 M3 TIMER中斷
#define HWO_M3_EINT     TIMER_ON(M3_TIMER)
//關閉 M3 TIMER中斷
#define HWO_M3_DINT     TIMER_OFF(M3_TIMER)
//設定 M3 TIMER COUNT
#define	HWO_M3_TIME(x)  TIMER_SET_RELOAD(M3_TIMER,x)

//---------------------------------------------------------
// 初始化IO
void M3_IO_Init(void);
//---------------------------------------------------------
// 初始化變數
void CtrlM3_Initial(void);
//---------------------------------------------------------
// 設定失步極限步數
void M3_Set_Protect_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定超越極限後幾步停止
void M3_Set_Over_Limit_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定要走幾步
void M3_Set_Count(uint32_t val);
//---------------------------------------------------------
// 設定使用者指定的速度
void M3_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd);
//---------------------------------------------------------
// Time Function
// 設定好Timer之後，把M3_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM3_Time_Target和xVarM3_Time_Now即可
void M3_Set_Time(uint32_t val);
void M3_Incrase_Time(uint32_t val);
//---------------------------------------------------------
// 是否已滿足條件停止
uint8_t M3_Get_Stop(void);
void CtrlM3_Start(uint8_t Action);
void CtrlM3_Stop(void);
void CtrlM3_Finish(void);
void CtrlM3_Error(uint8_t State);
void CtrlM3_Trigger(void);

//---------------------------------------------------------------
void CtrlM3_Behavior_Goto_END(void);
void CtrlM3_Behavior_Goto_HOME(void);
void CtrlM3_Behavior_Goto_HOME_STEPS(uint32_t count);
void CtrlM3_Behavior_Goto_END_STEPS(uint32_t count);

#endif /* INC_MOTOR_TOP_Y_H_ */
