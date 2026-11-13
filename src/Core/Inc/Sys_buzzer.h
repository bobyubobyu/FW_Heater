/*
 * Sys_buzzer.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Bob Yu
 */

#ifndef INC_SYS_BUZZER_H_
#define INC_SYS_BUZZER_H_

//define BUZZER用的 TIMER和CH
#define BUZZER_TIMER TIMER3

//開啟BUZZER TIMER中斷
#define HWO_BUZZER_EINT TIMER_ON(BUZZER_TIMER)
//關閉BUZZER TIMER中斷
#define HWO_BUZZER_DINT TIMER_OFF(BUZZER_TIMER)
//設定BUZZER TIMER COUNT
#define	HWO_BUZZER_TIME(x) TIMER_SET_RELOAD(BUZZER_TIMER,x)
//define buzzer io 腳的命令
#define HW_BUZZER_LO	    HAL_GPIO_WritePin (BUZZER_GPIO_Port, BUZZER_Pin,0)
#define	HW_BUZZER_HI	    HAL_GPIO_WritePin (BUZZER_GPIO_Port, BUZZER_Pin,1)
#define HW_BUZZER_TRIGGER	HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin)

struct	SHEET_MUSIC
{
	uint8_t Note;
	uint8_t BEAT;
};

enum
{
    L_DO,
    L_RE,
    L_MI,
    L_FA,
    L_SO,
    L_LA,
    L_SI,
    DO,
    RE,
    MI,
    FA,
    SO,
    LA,
    SI,
    H_DO,
    H_RE,
    H_MI,
    H_FA,
    H_SO,
    H_LA,
    H_SI,
    PAUSE,
    NOTE_MAX,
};

extern const struct SHEET_MUSIC SOUNDS_BEEP[];
extern const uint16_t SOUNDS_BEEP_NUM;

extern const struct SHEET_MUSIC SOUNDS_DOREMI_SHORT[];
extern const uint16_t SOUNDS_DOREMI_SHORT_NUM;

extern const struct SHEET_MUSIC FUR_ELISE[];
extern const uint16_t FUR_ELISE_NUM;

extern const struct SHEET_MUSIC SOUNDS_SHORT_BEEP[];
extern const uint16_t 	SOUNDS_SHORT_BEEP_NUM;

void Buzzer_Init(void);
void Buzzer_Start(const struct SHEET_MUSIC *_sound , uint16_t num );
void Buzzer_Stop(void);
void Buzzer_Trigger(void);

#endif /* INC_SYS_BUZZER_H_ */
