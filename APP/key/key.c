/*
 * key.c
 *
 *  Created on: 2022��3��18��
 *      Author: ZGJ
 */

#include "key.h"

/*------------------------------------------------------------------------------
Default Initializers for the Key Object
------------------------------------------------------------------------------*/
KEY key_module = {
            .init = Key_Init,
            .scan = Key_Scan,
};


/*--------������ʼ������--------*/
//���ܣ���ʼ������IO��
//���룺��
//�������
/*---------------------------*/
void Key_Init(void)
{
    EALLOW;

    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;    //IO������ΪGPIO����
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 0; //����Ϊ����
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0; //��������

    EDIS;

    GpioDataRegs.GPASET.bit.GPIO17 = 1; //Ĭ�ϸߵ�ƽ
}


/*--------����������--------*/
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY1����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2
/*---------------------------*/
unsigned char Key_Scan(unsigned char mode)
{
    static unsigned char key_up=1;//�������ɿ���־
    if(mode)key_up=1;  //֧������
    if(key_up && KEY1==0)
    {
        DELAY_US(10000);    //ȥ����
        key_up=0;
        if(KEY1==0)return KEY1_PRES;
    }else if(KEY1==1)key_up=1;
    return 0;// �ް�������
}
