/*
 * main.c
 *
 *  Created on: 2022年3月14日
 *      Author: ZGJ
 */

/*---------------------Include---------------------*/
#include "DSP28x_Project.h"
#include "key.h"
#include "led.h"
#include "iic.h"
#include "oled.h"

void main()
{
    InitSysCtrl();  //Initialize System Control

    InitGpio(); //Initialize GPIO
//    InitI2CGpio();
    DINT;

    InitPieCtrl();  //Initialize the PIE control registers to their default state

    IER = 0x0000;   //Disable CPU interrupts and clear all CPU interrupt flags
    IFR = 0x0000;

    InitPieVectTable(); //Initialize the PIE vector table with pointers to the shell Interrupt't Service Routines(ISR)

    Led_Init(); //初始化外设
    Key_Init();
    Init_I2CA();
    PieCtrlRegs.PIEIER8.bit.INTx1 = 1;
    IER |= M_INT8;

    OLED_Init();//OLED初始化
    OLED_Clear();

    EINT;
    while(1)
    {
        if(Key_Scan(1) == KEY1_PRES)
        {
            LED1_ON;
            OLED_Clear();
            OLED_ShowString(0,0,"This is an OLED",8);//
            OLED_ShowString(8,1,"example using",8);//
            OLED_ShowString(16,2,"hardware I2C",8);//
            delay_1ms(500);
        }
        else
            LED1_OFF;
    }
}
