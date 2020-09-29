
#include "SGX_G12864.h"

void delay(unsigned int n)
{
    while (n != 0)
    {
        n--;
    }
}

void delay_Long(unsigned long n)
{
    unsigned int i;
    for (i = 0; i < n; i++)
        delay(100);
}

void set_lcmGPIO(void)
{
    SDA_DIR;
    SCK_DIR;
    A0_DIR;
    RST_DIR;
    CS_DIR;
}
//*********************************************************************
//������ţ�˫�ֽڣ��ı�������
//*********************************************************************
const unsigned int Graph[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x08,
    0x08, 0x0C, 0x0C, 0x18, 0x30, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, /*"��",12*/

    0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x01, 0x01, 0x01, 0x01, 0x00, /*^",24*/

    0x00, 0x80, 0x80, 0x80, 0x80, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x80, 0x80, 0x80, 0x80,
    0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, /*"v",25*/

    0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, /*"<",26*/

    0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, /*">",27*/

    0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xB8, 0x98, 0x88, 0x80, 0x80, 0x80, 0xFC, 0xFC, 0xFC, 0x00,
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3B, 0x33, 0x23, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, /*�س���,28*/

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"��",4*/

};

//������
void SendByte(unsigned char Dbyte)
{
    unsigned char i, TEMP;
    TEMP = Dbyte;
    for (i = 0; i < 8; i++)
    {
        if (TEMP & 0x80)
            SDA_SET;
        else
            SDA_CLR;
        //		delay(10);
        SCK_CLR;
        //   delay(10);
        SCK_SET;
        //  delay(10);
        TEMP = TEMP << 1;
    }
}
//д����
void write_cmd(unsigned char Cbyte)
{
    CS_CLR;
    // delay(10);
    A0_CLR;
    //	delay(10);
    SendByte(Cbyte);
    //	A0_SET;
    CS_SET;
}
//д����
void write_data(unsigned char Dbyte)
{
    CS_CLR;
    // delay(10);
    A0_SET;
    // delay(10);
    SendByte(Dbyte);
    // A0_CLR;
    // CS_SET;
    CS_SET;
}
//��ʾӢ���ַ�
void PUTchar8x16(unsigned char row, unsigned char col, unsigned char *pcStr, unsigned char flag)
{
    unsigned char i;
    unsigned int X = 0;

    while (*pcStr != 0)
    {
        if (col > 15)
            return;
        if (row > 8)
            return;

        X = (*pcStr++ - 32) * 0x10;

        write_cmd(0xb0 + row);
        write_cmd(0x10 + (8 * col / 16));
        write_cmd(0x00 + (8 * col % 16));
        for (i = 0; i < 8; i++)
        {
            if (flag)
                write_data(~ENGLISH[X++]);
            else
                write_data(ENGLISH[X++]);
        }

        write_cmd(0xb1 + row);
        write_cmd(0x10 + (8 * col / 16));
        write_cmd(0x00 + (8 * col % 16));
        for (i = 0; i < 8; i++)
        {
            if (flag)
                write_data(~ENGLISH[X++]);
            else
                write_data(ENGLISH[X++]);
        }
        col++;
    }
}
//��ʾ��ֵ
void PUTNum(unsigned char row, unsigned char col, unsigned long Num, unsigned char flag)
{
    unsigned long temp = 0;
    unsigned int X     = 0;
    unsigned char i, BitNum = 0;
    if (Num == 0)
    {
        PUTchar8x16(row, col, "0", flag);
    }
    else
    {
        temp = Num;
        while (temp)
        {
            temp /= 10;
            BitNum++;
        }
        temp = 1;
        for (i = 0; i < (BitNum - 1); i++)
        {
            temp *= 10;
        }
        while (temp != 0)
        {
            if (col > 15)
                return;
            if (row > 8)
                return;

            X = (Num / temp + 16) * 16;

            write_cmd(0xb0 + row);
            write_cmd(0x10 + (8 * col / 16));
            write_cmd(0x00 + (8 * col % 16));
            for (i = 0; i < 8; i++)
            {
                if (flag)
                    write_data(~ENGLISH[X++]);
                else
                    write_data(ENGLISH[X++]);
            }

            write_cmd(0xb1 + row);
            write_cmd(0x10 + (8 * col / 16));
            write_cmd(0x00 + (8 * col % 16));
            for (i = 0; i < 8; i++)
            {
                if (flag)
                    write_data(~ENGLISH[X++]);
                else
                    write_data(ENGLISH[X++]);
            }

            col++;
            Num %= temp;
            temp /= 10;
        }
    }
}
//��ʾ�����ַ�
void PUTchar16x16(unsigned char row, unsigned char col, unsigned char *pcStr, unsigned char flag)
{
    unsigned char i;
    unsigned int X = 0;
    unsigned int GBC;
    unsigned char qh, wh;

    while (*pcStr != 0)
    {
        qh  = *(pcStr)-0xa0;
        wh  = *(pcStr + 1) - 0xa0;
        GBC = qh * 100 + wh;
        for (i = 0; i < HZ_SIZE; i++)
            if (GBC == GBC_Index[i])
                break;

        if (col > 15)
            return;
        if (row > 8)
            return;

        X = i * 0x20;

        write_cmd(0xb0 + row);
        write_cmd(0x10 + (8 * col / 16));
        write_cmd(0x00 + (8 * col % 16));
        for (i = 0; i < 16; i++)
        {
            if (flag)
                write_data(~CHINESE[X++]);
            else
                write_data(CHINESE[X++]);
        }
        write_cmd(0xb1 + row);
        write_cmd(0x10 + (8 * col / 16));
        write_cmd(0x00 + (8 * col % 16));
        for (i = 0; i < 16; i++)
        {
            if (flag)
                write_data(~CHINESE[X++]);
            else
                write_data(CHINESE[X++]);
        }
        col += 2;
        pcStr += 2;
    }
}
//��ʾͼƬ
void PUTImage(const unsigned char *pcStr)
{
    unsigned char x, y;
    unsigned int i = 0;
    for (y = 0; y < 8; y++)
    {
        write_cmd(0xb0 + y);
        write_cmd(0x10);
        write_cmd(0x00);
        for (x = 0; x < 128; x++)
            write_data(pcStr[i++]);
    }
}

void PUTGraph(unsigned char row, unsigned char col, unsigned char num, unsigned char flag)
{
    unsigned char i;
    unsigned int X = 0;
    if (col > 15)
        return;
    if (row > 8)
        return;

    X = num * 0x20;

    write_cmd(0xb0 + row);
    write_cmd(0x10 + (8 * col / 16));
    write_cmd(0x00 + (8 * col % 16));
    for (i = 0; i < 16; i++)
    {
        if (flag)
            write_data(~Graph[X++]);
        else
            write_data(Graph[X++]);
    }

    write_cmd(0xb1 + row);
    write_cmd(0x10 + (8 * col / 16));
    write_cmd(0x00 + (8 * col % 16));
    for (i = 0; i < 16; i++)
    {
        if (flag)
            write_data(~Graph[X++]);
        else
            write_data(Graph[X++]);
    }
    // col +=2;
    // pcStr +=2;
}

//����
void LcmClear(void)
{
    unsigned char x, y;
    for (y = 0; y < 8; y++)
    {
        write_cmd(0xb0 + y);
        write_cmd(0x10);
        write_cmd(0x00);
        for (x = 0; x < 128; x++)
            write_data(0);
    }
}

//��ʼ��
void LcmInit(void)
{
    CS_CLR;

    RST_CLR;
    delay(5000);
    RST_SET;
    delay(5000);

    write_cmd(0xe2); // system reset
    delay(5000);

    write_cmd(0x24); // SET VLCD RESISTOR RATIO
    write_cmd(0xa2); // BR=1/9
    write_cmd(0xa0); // set seg direction
    write_cmd(0xc8); // set com direction
    write_cmd(0x2f); // set power control
    write_cmd(0x40); // set scroll line
    write_cmd(0x81); // SET ELECTRONIC VOLUME
    write_cmd(0x20); // set pm: ??????????????
    write_cmd(0xa7); // set inverse display	   a6 off, a7 on
                     // write_cmd(0xa4);                                                     // set all pixel on
    write_cmd(0xaf); // set display enable
    LcmClear();
}

//*********************************************************************
//��ʾϵͳ��Ϣ
//*********************************************************************
void Welcome_on(void)
{
    PUTchar8x16(0, 0, "WPI ATU ", 0);
    PUTchar8x16(2, 0, "Vishay Sensor", 0);
    PUTchar8x16(4, 0, "RD: Chay", 0);
    PUTchar8x16(6, 0, "2020-09-30 V1.0", 0);
    // Delay(5000);
}

//*********************************************************************
//��ʾUV������Ϣ
//*********************************************************************
void UV_data(void)
{
    PUTchar8x16(0, 0, "    UV Sensor ", 0);
    PUTchar8x16(2, 0, "    VEML6075", 0);
    PUTchar8x16(4, 0, " UVA Data = ", 0);
    PUTchar8x16(6, 0, " UVB Data = ", 0);
    // Delay(5000);
}

//*********************************************************************
//��ʾ RGB ������Ϣ
//*********************************************************************
void RGB_data(void)
{
    PUTchar8x16(0, 0, "RGB Sensor", 0);
    PUTchar8x16(2, 0, "    VEML6040", 0);
    PUTchar8x16(4, 0, "RGB data =", 0);
    PUTchar8x16(6, 0, "                 ", 0);
    // Delay(5000);
}

//*********************************************************************
//��ʾ������������Ϣ
//*********************************************************************
void Proximity_data(void)
{
    PUTchar8x16(0, 0, "Proximity Sensor", 0);
    PUTchar8x16(2, 0, "    VCNL4020", 0);
    PUTchar8x16(4, 0, "Proximity data =", 0);
    PUTchar8x16(6, 0, "                  ", 0);
    // Delay(5000);
}

//*********************************************************************
//ѡ��ʱ���ڲ˵�������ʾ���̡�
//*********************************************************************
void duigou(int y_pos)
{
    PUTGraph(y_pos, 13, S_duigou, 0);
}
//*********************************************************************
//��ʾ�����˵�
//*********************************************************************
void menu_all(void)
{
    menu_gpio(0);
    menu_Timer(2);
    menu_pwm(4);
    menu_adc(6);
}

//*********************************************************************
//��ʾ��PWM ��ʾ���˵�
//*********************************************************************
void menu_pwm(int y_pos)
{
    PUTGraph(y_pos, 0, S_right, 0);
    PUTGraph(y_pos, 2, S_maohao, 0);
    PUTchar8x16(y_pos, 4, "PWM        ", 0);
    PUTchar16x16(y_pos, 9, "��ʾ", 0);
    // Show_Chinese("��ʾ",8,y_pos);
}
//*********************************************************************
//���롰PWM ��ʾ��ҳ��
//*********************************************************************
void Icon_pwm(int y_pos)
{
    LcmClear();
    write_cmd(0x40);
    PUTchar8x16(y_pos, 0, "PWM", 0);
    //	Show_Chinese("��ʾ",4,y_pos);
    PUTchar16x16(y_pos, 4, "��ʾ", 0);

    PUTGraph(6, 8, S_hcf, 0);
    PUTchar16x16(6, 11, "����", 0);
    // Show_Chinese("����",11,6);
    // Show_English("Hz",10,3);
}
//*********************************************************************
//��ʾ��GPIO��ʾ���˵�
//*********************************************************************
void menu_gpio(int y_pos)
{
    PUTGraph(y_pos, 0, S_up, 0);
    PUTGraph(y_pos, 2, S_maohao, 0);
    PUTchar8x16(y_pos, 4, "GPIO       ", 0);
    PUTchar16x16(y_pos, 9, "��ʾ", 0);
    // Show_Chinese("��ʾ",8,y_pos);
}
//*********************************************************************
//���롰GPIO��ʾ��ҳ��
//*********************************************************************
void Icon_GPIO(int y_pos)
{
    LcmClear();
    write_cmd(0x40);
    PUTchar8x16(y_pos, 0, "GPIO", 0);
    // Show_Chinese("��ʾ",4,y_pos);
    PUTchar16x16(y_pos, 4, "��ʾ", 0);

    PUTGraph(6, 8, S_hcf, 0);
    PUTchar16x16(6, 11, "����", 0);
    //	Show_Chinese("����",11,6);
}

//*********************************************************************
//��ʾ��Timer���˵�
//*********************************************************************
void menu_Timer(int y_pos)
{
    PUTGraph(y_pos, 0, S_left, 0);
    PUTGraph(y_pos, 2, S_maohao, 0);
    PUTchar8x16(y_pos, 4, "Timer       ", 0);
    PUTchar16x16(y_pos, 9, "��ʾ", 0);
}

//*********************************************************************
//���롰timer��ʾ��ҳ��
//*********************************************************************
void Icon_timer(int y_pos)
{
    LcmClear();
    write_cmd(0x40);
    PUTchar8x16(y_pos, 0, "Timer", 0);

    PUTchar16x16(y_pos, 5, "��ʾ", 0);

    PUTGraph(6, 8, S_hcf, 0);
    PUTchar16x16(6, 11, "����", 0);
    // Show_Chinese("����",11,6);
}
//*********************************************************************
//��ʾ��SlimPort���˵�
//*********************************************************************
void menu_adc(int y_pos)
{
    PUTGraph(y_pos, 0, S_down, 0);
    PUTGraph(y_pos, 2, S_maohao, 0);
    PUTchar8x16(y_pos, 4, "ADC       ", 0);
    PUTchar16x16(y_pos, 9, "��ʾ", 0);
    //	Show_Chinese("��ʾ",8,y_pos);
}

//*********************************************************************
//���롰ADC��ʾ��ҳ��
//*********************************************************************
void Icon_adc(int y_pos)
{
    LcmClear();
    write_cmd(0x40);
    PUTchar8x16(y_pos, 0, "ADC", 0);
    // Show_Chinese("��ʾ",4,y_pos);
    PUTchar16x16(y_pos, 4, "��ʾ", 0);

    PUTGraph(6, 8, S_hcf, 0);
    PUTchar16x16(6, 11, "����", 0);
    // Show_Chinese("����",11,6);
}