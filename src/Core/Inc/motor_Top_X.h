/*
 * motor_Top_X.h
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#ifndef INC_MOTOR_TOP_TOP_X_H_
#define INC_MOTOR_TOP_TOP_X_H_


#define		HWO_M4_LOW				    HAL_GPIO_WritePin (TOP_X_PU_GPIO_Port, TOP_X_PU_Pin,0)
#define     HWO_M4_TRIGGER	            HAL_GPIO_TogglePin(TOP_X_PU_GPIO_Port, TOP_X_PU_Pin)
#define		HWO_M4_BACKWARD			    HAL_GPIO_WritePin (TOP_X_DR_GPIO_Port, TOP_X_DR_Pin,1)
#define		HWO_M4_FORWARD			    HAL_GPIO_WritePin (TOP_X_DR_GPIO_Port, TOP_X_DR_Pin,0)
#define		HWO_M4_ENABLE			    HAL_GPIO_WritePin (TOP_X_LC_GPIO_Port, TOP_X_LC_Pin,0)
#define		HWO_M4_DISABLE			    HAL_GPIO_WritePin (TOP_X_LC_GPIO_Port, TOP_X_LC_Pin,1)
#define     HWO_M4_RESET_ON 		    HAL_GPIO_WritePin (TOP_X_RESET_GPIO_Port, TOP_X_RESET_Pin,1)
#define     HWO_M4_RESET_OFF		    HAL_GPIO_WritePin (TOP_X_RESET_GPIO_Port, TOP_X_RESET_Pin,0)

#define		HWI_SENSOR_M4_OVER_CUEERNT  (HAL_GPIO_ReadPin(TOP_X_Flag_GPIO_Port, TOP_X_Flag_Pin) == 0x01)
#define     HWI_M4_CW_LIMIT             (HAL_GPIO_ReadPin(TOP_X_END_GPIO_Port, TOP_X_END_Pin) == 0x01)
#define     HWI_M4_CCW_LIMIT            (HAL_GPIO_ReadPin(TOP_X_HOME_GPIO_Port, TOP_X_HOME_Pin) == 0x01)

//------------------------------------------------------------------------------
// 設定好要使用的TIMER編號
#define M4_TIMER    TIMER7

//開啟 M4 TIMER中斷
#define HWO_M4_EINT     TIMER_ON(M4_TIMER)
//關閉 M4 TIMER中斷
#define HWO_M4_DINT     TIMER_OFF(M4_TIMER)
//設定 M4 TIMER COUNT
#define	HWO_M4_TIME(x)  TIMER_SET_RELOAD(M4_TIMER,x)

//---------------------------------------------------------
// 初始化IO
void M4_IO_Init(void);
//---------------------------------------------------------
// 初始化變數
void CtrlM4_Initial(void);
//---------------------------------------------------------
// 設定失步極限步數
void M4_Set_Protect_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定超越極限後幾步停止
void M4_Set_Over_Limit_Step(uint8_t on_off,uint32_t val);
//---------------------------------------------------------
// 設定要走幾步
void M4_Set_Count(uint32_t val);
//---------------------------------------------------------
// 設定使用者指定的速度
void M4_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd);
//---------------------------------------------------------
// Time Function
// 設定好Timer之後，把M4_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM4_Time_Target和xVarM4_Time_Now即可
void M4_Set_Time(uint32_t val);
void M4_Incrase_Time(uint32_t val);
//---------------------------------------------------------
// 是否已滿足條件停止
uint8_t M4_Get_Stop(void);
void CtrlM4_Start(uint8_t Action);
void CtrlM4_Stop(void);
void CtrlM4_Finish(void);
void CtrlM4_Error(uint8_t State);
void CtrlM4_Trigger(void);

//---------------------------------------------------------------
void CtrlM4_Behavior_Goto_END(void);
void CtrlM4_Behavior_Goto_HOME(void);
void CtrlM4_Behavior_Goto_HOME_STEPS(uint32_t count);
void CtrlM4_Behavior_Goto_END_STEPS(uint32_t count);

#endif /* INC_MOTOR_TOP_TOP_X_H_ */
