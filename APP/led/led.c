/*
 * led.c
 *
 *  Created on: 2022��3��18��
 *      Author: ZGJ
 */
#include "led.h"

void Led_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0;   //��GPIO75����ΪGPIO����
    GpioCtrlRegs.GPCDIR.bit.GPIO75 = 1; //��GPIO75��������Ϊ���
    EDIS;
}




