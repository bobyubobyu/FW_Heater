/*
 * SysFunc_Flag.c
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#include "main.h"

int16_t     xVarFlag_Signal[NUM_OF_SIGNAL];
uint16_t    xVarFlag_System[NUM_OF_FLAG_TYPE];

void	SysFlag_Release(uint16_t Type, uint16_t Mask)
{
	xVarFlag_System[Type] &= ~Mask;
}

void	SysFlag_Set(uint16_t Type, uint16_t Mask)
{
	if (Type < NUM_OF_FLAG_TYPE)
	{
		xVarFlag_System[Type] |= Mask;
	}
}

uint8_t	SysFlag_Get(uint16_t Type, uint16_t Mask)
{
	if (Type >= NUM_OF_FLAG_TYPE)
        return	FALSE;

	uint8_t	Result = ((xVarFlag_System[Type] & Mask) == Mask ? TRUE : FALSE);
    SysFlag_Release(Type, Mask);
	return	Result;
}

void	SysFlag_Initial()
{
	uint8_t	i;

	for (i = 0; i < NUM_OF_SIGNAL; i++)
        xVarFlag_Signal[i] = 0;

	for (i = 0; i < NUM_OF_FLAG_TYPE; i++)
        xVarFlag_System[i] = 0;
}
