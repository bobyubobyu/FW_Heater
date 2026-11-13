/*
 * Sys_buzzer.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Bob Yu
 */

#include "main.h"

const uint16_t Freqs_table[NOTE_MAX] = { 262, 294, 330, 349, 392, 440, 494,
										 523, 587, 659, 698, 784, 880, 988,
										 1046,1175,1318,1397,1568,1760,1976,
										 0xFFFF
									   };

const uint16_t Timer_table[NOTE_MAX] = { 11450, 10204, 9091, 8850, 7653, 6818, 6073,
										  5736,  5111, 4552, 4298, 3826, 3409, 3060,
										  2868,  2553, 2276, 2147, 1913, 1704, 1518,
										  0xFFFF
									   };

const struct SHEET_MUSIC SOUNDS_BEEP[] = { { DO,2},{ PAUSE,1},{ DO,2},{ PAUSE,1},{ DO,2},};
const uint16_t 	SOUNDS_BEEP_NUM = (sizeof(SOUNDS_BEEP) / sizeof(struct SHEET_MUSIC));


const struct SHEET_MUSIC SOUNDS_DOREMI_SHORT[] = {{ DO,2},{ RE,2},{ MI,2},{ FA,2},{ SO,2},{ LA,2},{ SI,2},{ H_DO,2}};
const uint16_t 	SOUNDS_DOREMI_SHORT_NUM = (sizeof(SOUNDS_DOREMI_SHORT) / sizeof(struct SHEET_MUSIC));


const struct SHEET_MUSIC FUR_ELISE[] = {{ MI,1},{ RE,1},{ MI,1},{ RE,1},{ MI,1},{ SI,1},{ RE,1},{ DO,1},{ LA,1}};
const uint16_t	FUR_ELISE_NUM = (sizeof(FUR_ELISE) / sizeof(struct SHEET_MUSIC));


const struct SHEET_MUSIC SOUNDS_SHORT_BEEP[] = { { DO,1}};
const uint16_t 	SOUNDS_SHORT_BEEP_NUM = (sizeof(SOUNDS_SHORT_BEEP) / sizeof(struct SHEET_MUSIC));


const struct SHEET_MUSIC *sound_array;
uint16_t sound_num;
uint16_t sound_step;
void Buzzer_Init(void)
{
	HWO_BUZZER_DINT;
}

void Buzzer_Start(const struct SHEET_MUSIC *_sound , uint16_t num )
{
	if(sound_step !=0)
		return;
	sound_array = _sound;
	sound_num = num;
	HWO_BUZZER_TIME(2500);
	HWO_BUZZER_EINT;
}

void Buzzer_Stop(void)
{
	HWO_BUZZER_DINT;
	HW_BUZZER_LO;
	sound_step = 0;
	sound_array = NULL;
	sound_num = 0;
}

void Buzzer_Trigger(void)
{
	static unsigned int sound_idx = 0;
	static unsigned int reload_val = 0;
	static unsigned int bpm480 = 0;
	static unsigned char mute = 0;
	switch (sound_step)
	{
		case 0:  //無音階 離開
		{
			if(sound_num == 0)
			{
				Buzzer_Stop();
				break;
			}
			HWO_BUZZER_TIME(2500);
			sound_idx = 0;  //索引 = 0
			bpm480 = 0;     //演奏時長 = 0
			sound_step = 1;
			break;
		}
		case 1:
		{
			bpm480 = sound_array[sound_idx].BEAT;  //每個單位為1/8S
			if( Timer_table[sound_array[sound_idx].Note] == 0xFFFF)
			{
				mute = 1;
				bpm480 = (bpm480 * Freqs_table[DO]) /(8/2);
				reload_val = Timer_table[DO];
				HWO_BUZZER_TIME(reload_val);  //用DO計算時間
			}
			else
			{
				mute = 0;
				bpm480 = (bpm480 * Freqs_table[sound_array[sound_idx].Note]) /(8/2);
				reload_val = Timer_table[sound_array[sound_idx].Note];
				HWO_BUZZER_TIME(reload_val);
			}
			sound_step = 2;
			break;
		}
		case 2:
		{
			if(mute == 1)
				HW_BUZZER_LO;
			else
				HW_BUZZER_TRIGGER;
			HWO_BUZZER_TIME(reload_val);
			bpm480--;
			if(bpm480 == 0)
				sound_step = 3;
			break;
		}
		case 3:
		{
			sound_idx++;
			sound_step = 1;
			if(sound_idx >= sound_num)
				Buzzer_Stop();
			break;
		}

		default:
			break;
	}
}
