/*
 * iic.c
 *
 *  Created on: 2022��5��1��
 *      Author: ZGJ
 *      @brief: I2Cͨ��Ӧ�ú����ӿ�
 */

#include "iic.h"

/**
 *  @brief               IICģ���ʼ��������IICģ��Ĺ��������Լ�����IIC�Ĺ�����ʽ
 *  @parameter              ��
 *  @return_value           ��
 */
void Init_I2CA(void)
{
    I2caRegs.I2CMDR.all = 0x0000;   // ��λIIC

    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // ʹ��(SDAA)����
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //  ʹ�� (SCLA)����
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // ͬ�� (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // ͬ�� (SCLA)
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // ���� GPIO32Ϊ SDAA
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // ����GPIO33 ΪSCLA

    // Ԥ��Ƶ����ʱ��ģ���Ƶ��
    I2caRegs.I2CPSC.all = 14;       // Ԥ��Ƶ   IICģ��ʱ��������Ϊ7-12MHz����ʵ������Ϊ (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 10;   //ʱ�ӵ͵�ƽʱ��ֵ
    I2caRegs.I2CCLKH = 5;   //ʱ�Ӹߵ�ƽʱ��ֵ
    I2caRegs.I2CIER.all = 0x00;      // Enable SCD & ARDY interrupts

    I2caRegs.I2CFFTX.all = 0x0000;   // Disable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x0000;   // Disable RXFIFO, clear RXFFINT,
    I2caRegs.I2CMDR.all = 0x0020;   // IIC׼������
    EDIS;

}

/**
 *  @brief              IICģ�鷢��׼��OK
 *  @parameter              ��
 *  @return_value           ��
 */
Uint16  I2C_xrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;   // IICģ�鷢��׼��OK
    return t;
}

/**
 *  @brief              IICģ�����׼��OK
 *  @parameter              ��
 *  @return_value           ��
 */
Uint16  I2C_rrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;   //  IICģ�����׼��OK
    return t;
}

/**
 *  @brief                  IICд����
 *  @parameter              SlaveAddress���ӻ���ַ��  RomAddress���Ĵ�����ַ�� number��д�����ݵ��ֽ�����  *Wdata��д�����ݵĵ�ַ
 *  @return_value           ״̬��־
 */
Uint16 WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, unsigned char *Wdata)
{
   Uint16 i;
   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
      return I2C_BUS_BUSY_ERROR;   //��������æ����״̬
   }
   while(!I2C_xrdy());      //�ȴ����ݷ��;�����XRDY=1,�������ͼĴ����Ѿ�׼���ý����µ�����
   I2caRegs.I2CSAR = SlaveAddress&0xff;    //�豸�ӵ�ַ
   I2caRegs.I2CCNT = number + 1;   //��Ҫ���͵��ֽ���
   I2caRegs.I2CDXR = RomAddress&0xff;    //��һ�������ֽ�Ϊ�������ݵ�Ŀ��Ĵ�����ַ
   DELAY_US(1);//�ȴ�������ȫ��ֵ��I2CDXR����ʹ��IIC������Ҫ���ϣ�
   I2caRegs.I2CMDR.all = 0x6E20;    //������ʼ�źţ��ڲ����ݼ���������0ʱ������ֹͣ�źţ���������ģʽ��ʹ��IICģʽ��
   for (i=0; i<number; i++)
   {
      while(!I2C_xrdy());   //�ȴ����ݷ��;�����������һ������
      I2caRegs.I2CDXR = *Wdata&0xFF;
      Wdata++;
      if (I2caRegs.I2CSTR.bit.NACK == 1)    //�͵���Ӧ���źŷ��ش���
          return    I2C_BUS_BUSY_ERROR;
   }
  // DELAY_US(1);
   return I2C_SUCCESS;         //���ͳɹ�
}


/**
 *  @brief                  IICд����
 *  @parameter              SlaveAddress���ӻ���ַ��  RomAddress���Ĵ�����ַ�� number��д�����ݵ��ֽ�����  *RamAddr���������ݴ�ŵĵ�ַ
 *  @return_value           ״̬��־
 */
Uint16 ReadData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint16  *RamAddr)
{
   Uint16  i,Temp;

   if (I2caRegs.I2CSTR.bit.BB == 1)  //��������æ����״̬
   {
       return I2C_BUS_BUSY_ERROR;
   }
   while(!I2C_xrdy());
   I2caRegs.I2CSAR = SlaveAddress&0xff;      //�豸�ӵ�ַ
   I2caRegs.I2CCNT = 1;        //����һ���ֽ�ΪҪ��ȡ���ݵļĴ�����ַ
   I2caRegs.I2CDXR = RomAddress&0xff;
   DELAY_US(1);//�ȴ�������ȫ��ֵ��I2CDXR����ʹ��IIC������Ҫ���ϣ�
   I2caRegs.I2CMDR.all = 0x6620;   //��������ģʽ���Զ����ֹͣλ������ֹͣ�ź�
   if (I2caRegs.I2CSTR.bit.NACK == 1)    //�͵���Ӧ���źŷ��ش���
        return  I2C_BUS_BUSY_ERROR;
   DELAY_US(50);
   while(!I2C_xrdy());                  //���Թ�������ʱ�Ῠ����һ��,��ʾͨѶ������
   I2caRegs.I2CSAR = SlaveAddress&0xff;
   I2caRegs.I2CCNT = number;
   DELAY_US(1);//�ȴ�������ȫ��ֵ��I2CDXR����ʹ��IIC������Ҫ���ϣ�
   I2caRegs.I2CMDR.all = 0x6C20;   //��������ģʽ��������ֹͣ�ź�
   if (I2caRegs.I2CSTR.bit.NACK == 1)
        return  I2C_BUS_BUSY_ERROR;
   for(i=0;i<number;i++)
   {
         while(!I2C_rrdy());
      Temp = I2caRegs.I2CDRR;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
          return    I2C_BUS_BUSY_ERROR;
      *RamAddr = Temp;
      RamAddr++;
   }
   return I2C_SUCCESS;    //�������ݳɹ�
}

/**
 *  @brief                 IIC��Ĵ�����ĳһλд����
 *  @parameter              SlaveAddress���ӻ���ַ��  regaddress���Ĵ�����ַ�� bitNum��ĳһλ��  data��д������
 *  @return_value           ��
 */
void IICwriteBit(Uint16 slaveaddress, Uint16 regaddress, unsigned char bitNum, unsigned char data)
{
    Uint16 a;
    unsigned char b;
    DELAY_US(50);
    ReadData(slaveaddress,regaddress,1,&a);
    b=(unsigned char )(a&0xff);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    DELAY_US(50);
    WriteData(slaveaddress,regaddress,1,&b);
}

/**
 *  @brief                 IIC��Ĵ�����ĳ��λд����
 *  @parameter              SlaveAddress���ӻ���ַ��  regaddress���Ĵ�����ַ�� bitStart����ʼλ��  length�����ȣ�data��д������
 *  @return_value           ��
 */
void IICwriteBits(Uint16 slaveaddress,Uint16 regaddress,unsigned char bitStart,unsigned char length,unsigned char data)
{
    unsigned char b,mask;
    Uint16 a;
    DELAY_US(50);
    ReadData(slaveaddress,regaddress,1,&a);
    b=(unsigned char )(a&0xff);
    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    data <<= (8 - length);
    data >>= (7 - bitStart);
    b &= mask;
    b |= data;
    DELAY_US(50);
    WriteData(slaveaddress,regaddress,1, &b);
}




