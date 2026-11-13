/*
 * motor_Botton_Z.h
 *
 *  Created on: Nov 24, 2025
 *      Author: Bob Yu
 */

#ifndef INC_MOTOR_BOTTON_Z_H_
#define INC_MOTOR_BOTTON_Z_H_

#define		HWO_M1_LOW				    HAL_GPIO_WritePin (Botton_Z_PU_GPIO_Port, Botton_Z_PU_Pin,0)
#define     HWO_M1_TRIGGER	            HAL_GPIO_TogglePin(Botton_Z_PU_GPIO_Port, Botton_Z_PU_Pin)
#define		HWO_M1_BACKWARD			    HAL_GPIO_WritePin (Botton_Z_DR_GPIO_Port, Botton_Z_DR_Pin,1)
#define		HWO_M1_FORWARD			    HAL_GPIO_WritePin (Botton_Z_DR_GPIO_Port, Botton_Z_DR_Pin,0)
#define		HWO_M1_ENABLE			    HAL_GPIO_WritePin (Botton_Z_LC_GPIO_Port, Botton_Z_LC_Pin,0)
#define		HWO_M1_DISABLE			    HAL_GPIO_WritePin (Botton_Z_LC_GPIO_Port, Botton_Z_LC_Pin,1)
#define     HWO_M1_RESET_ON 		    HAL_GPIO_WritePin (Botton_Z_RESET_GPIO_Port, Botton_Z_RESET_Pin,1)
#define     HWO_M1_RESET_OFF		    HAL_GPIO_WritePin (Botton_Z_RESET_GPIO_Port, Botton_Z_RESET_Pin,0)

#define		HWI_SENSOR_M1_OVER_CUEERNT  (HAL_GPIO_ReadPin(Botton_Z_Flag_GPIO_Port, Botton_Z_Flag_Pin) == 0x01)
#define     HWI_M1_CW_LIMIT             (HAL_GPIO_ReadPin(Botton_Z_END_GPIO_Port, Botton_Z_END_Pin) == 0x01)
#define     HWI_M1_CCW_LIMIT            (HAL_GPIO_ReadPin(Botton_Z_HOME_GPIO_Port, Botton_Z_HOME_Pin) == 0x01)

//------------------------------------------------------------------------------
// 設定好要使用的TIMER編號
#define M1_TIMER    TIMER4

//開啟 M1 TIMER中斷
#define HWO_M1_EINT     TIMER_ON(M1_TIMER)
//關閉 M1 TIMER中斷
#define HWO_M1_DINT     TIMER_OFF(M1_TIMER)
//設定 M1 TIMER COUNT
#define	HWO_M1_TIME(x)  TIMER_SET_RELOAD(M1_TIMER,x)

//---------------------------------------------------------
// 初始化IO
void M1_IO_Init(void);
//---------------------------------------------------------
// 初始化變數
void CtrlM1_Initial(void);
//---------------------------------------------------------
// 設定失步極限步數
void M1_Set_Protect_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定超越極限後幾步停止
void M1_Set_Over_Limit_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定要走幾步
void M1_Set_Count(uint32_t val);
//---------------------------------------------------------
// 設定使用者指定的速度
void M1_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd);
//---------------------------------------------------------
// Time Function
// 設定好Timer之後，把M1_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM1_Time_Target和xVarM1_Time_Now即可
void M1_Set_Time(uint32_t val);
void M1_Incrase_Time(uint32_t val);
//---------------------------------------------------------
// 是否已滿足條件停止
uint8_t M1_Get_Stop(void);
void CtrlM1_Start(uint8_t Action);
void CtrlM1_Stop(void);
void CtrlM1_Finish(void);
void CtrlM1_Error(uint8_t State);
void CtrlM1_Trigger(void);

//---------------------------------------------------------------
void CtrlM1_Behavior_Goto_END(void);
void CtrlM1_Behavior_Goto_HOME(void);
void CtrlM1_Behavior_Goto_HOME_STEPS(uint32_t count);
void CtrlM1_Behavior_Goto_END_STEPS(uint32_t count);

#endif /* INC_MOTOR_BOTTON_Z_H_ */
