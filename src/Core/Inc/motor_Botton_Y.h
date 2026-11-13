/*
 * motor_Botton_Y.h
 *
 *  Created on: Nov 24, 2025
 *      Author: Bob Yu
 */

#ifndef INC_MOTOR_BOTTON_Y_H_
#define INC_MOTOR_BOTTON_Y_H_

#define		HWO_M2_LOW				    HAL_GPIO_WritePin (Botton_Y_PU_GPIO_Port, Botton_Y_PU_Pin,0)
#define     HWO_M2_TRIGGER	            HAL_GPIO_TogglePin(Botton_Y_PU_GPIO_Port, Botton_Y_PU_Pin)
#define		HWO_M2_BACKWARD			    HAL_GPIO_WritePin (Botton_Y_DR_GPIO_Port, Botton_Y_DR_Pin,1)
#define		HWO_M2_FORWARD			    HAL_GPIO_WritePin (Botton_Y_DR_GPIO_Port, Botton_Y_DR_Pin,0)
#define		HWO_M2_ENABLE			    HAL_GPIO_WritePin (Botton_Y_LC_GPIO_Port, Botton_Y_LC_Pin,0)
#define		HWO_M2_DISABLE			    HAL_GPIO_WritePin (Botton_Y_LC_GPIO_Port, Botton_Y_LC_Pin,1)
#define     HWO_M2_RESET_ON 		    HAL_GPIO_WritePin (Botton_Y_RESET_GPIO_Port, Botton_Y_RESET_Pin,1)
#define     HWO_M2_RESET_OFF		    HAL_GPIO_WritePin (Botton_Y_RESET_GPIO_Port, Botton_Y_RESET_Pin,0)

#define		HWI_SENSOR_M2_OVER_CUEERNT  (HAL_GPIO_ReadPin(Botton_Y_Flag_GPIO_Port, Botton_Y_Flag_Pin) == 0x01)
#define     HWI_M2_CW_LIMIT             (HAL_GPIO_ReadPin(Botton_Y_END_GPIO_Port, Botton_Y_END_Pin) == 0x01)
#define     HWI_M2_CCW_LIMIT            (HAL_GPIO_ReadPin(Botton_Y_HOME_GPIO_Port, Botton_Y_HOME_Pin) == 0x01)

//------------------------------------------------------------------------------
// 設定好要使用的TIMER編號
#define M2_TIMER    TIMER5

//開啟 M2 TIMER中斷
#define HWO_M2_EINT     TIMER_ON(M2_TIMER)
//關閉 M2 TIMER中斷
#define HWO_M2_DINT     TIMER_OFF(M2_TIMER)
//設定 M2 TIMER COUNT
#define	HWO_M2_TIME(x)  TIMER_SET_RELOAD(M2_TIMER,x)

//---------------------------------------------------------
// 初始化IO
void M2_IO_Init(void);
//---------------------------------------------------------
// 初始化變數
void CtrlM2_Initial(void);
//---------------------------------------------------------
// 設定失步極限步數
void M2_Set_Protect_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定超越極限後幾步停止
void M2_Set_Over_Limit_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定要走幾步
void M2_Set_Count(uint32_t val);
//---------------------------------------------------------
// 設定使用者指定的速度
void M2_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd);
//---------------------------------------------------------
// Time Function
// 設定好Timer之後，把M2_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM2_Time_Target和xVarM2_Time_Now即可
void M2_Set_Time(uint32_t val);
void M2_Incrase_Time(uint32_t val);
//---------------------------------------------------------
// 是否已滿足條件停止
uint8_t M2_Get_Stop(void);
void CtrlM2_Start(uint8_t Action);
void CtrlM2_Stop(void);
void CtrlM2_Finish(void);
void CtrlM2_Error(uint8_t State);
void CtrlM2_Trigger(void);

//---------------------------------------------------------------
void CtrlM2_Behavior_Goto_END(void);
void CtrlM2_Behavior_Goto_HOME(void);
void CtrlM2_Behavior_Goto_HOME_STEPS(uint32_t count);
void CtrlM2_Behavior_Goto_END_STEPS(uint32_t count);

#endif /* INC_MOTOR_BOTTON_Y_H_ */
