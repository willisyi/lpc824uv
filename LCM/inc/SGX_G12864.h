#include "board.h"
#include "eng.h"
#include "chi.h"

#include "logo.h"


#define S_duigou	0X00
#define S_up		  0X01
#define S_down		0X02
#define S_left		0X03
#define S_right		0X04
#define S_hcf		  0X05
#define S_maohao	0X06

#define Chip_GPIO_SetPinDIROutput(GPIO_Base, port, pin) (GPIO_Base->DIR[port] |= 1UL<<(pin))

#define SDA_SET     GPIO_PinWrite(GPIO, 0, 24, 1) //P1OUT |= BIT0
#define SDA_CLR     GPIO_PinWrite(GPIO, 0, 24, 0)//P1OUT &= (~BIT0)
#define SDA_DIR     Chip_GPIO_SetPinDIROutput(GPIO, 0, 24)

#define SCK_SET     GPIO_PinWrite(GPIO, 0 ,25, 1)//P1OUT |= BIT1
#define SCK_CLR     GPIO_PinWrite(GPIO, 0, 25, 0)//P1OUT &= (~BIT1)
#define SCK_DIR     Chip_GPIO_SetPinDIROutput(GPIO, 0, 25)

#define A0_SET      GPIO_PinWrite(GPIO, 0, 26, 1)//P1OUT |= BIT2
#define A0_CLR      GPIO_PinWrite(GPIO, 0, 26, 0)//P1OUT &= (~BIT2)
#define A0_DIR      Chip_GPIO_SetPinDIROutput( GPIO, 0, 26);

#define RST_SET     GPIO_PinWrite( GPIO, 0, 13, 1)//P1OUT |= BIT3
#define RST_CLR     GPIO_PinWrite( GPIO, 0 , 13, 0)// P1OUT &= (~BIT3)
#define RST_DIR     Chip_GPIO_SetPinDIROutput( GPIO, 0, 13)

#define CS_SET      GPIO_PinWrite( GPIO, 0, 17, 1)//P1OUT |= BIT4
#define CS_CLR      GPIO_PinWrite( GPIO, 0, 17, 0)//P1OUT &= (~BIT4)
#define CS_DIR      Chip_GPIO_SetPinDIROutput( GPIO, 0, 17)



extern void delay(unsigned int n); 
extern void delay_Long(unsigned long n);
extern void set_lcmGPIO(void);

// LCD ³õÊ¼»¯
extern void LcmInit(void);

extern void  LcmClear(void);

extern void PUTchar8x16(unsigned char row,unsigned char col,unsigned char *pcStr,unsigned char flag);

extern void PUTNum(unsigned char row,unsigned char col,unsigned long Num,unsigned char flag);

extern void PUTchar16x16(unsigned char row,unsigned char col,unsigned char *pcStr,unsigned char flag);

extern void PUTImage(const unsigned char *pcStr);

//extern void Show_Chinese(unsigned char *pcStr,unsigned long x_pos,unsigned long y_pos);

extern void PUTGraph(unsigned char row,unsigned char col,unsigned char num,unsigned char flag);// void Show_Graph(unsigned int pcStr,unsigned long x_pos,unsigned long y_pos);

extern void Welcome_on(void);
extern void UV_data(void);
extern void RGB_data(void);
extern void Proximity_data(void);

extern void menu_pwm(int y_pos);
extern void menu_gpio(int y_pos);
extern void menu_Timer(int y_pos);
extern void menu_adc(int y_pos);
//extern void menu_shuai(int y_pos);
extern void duigou(int y_pos);
extern void menu_all(void);

extern void Icon_GPIO(int y_pos);
extern void Icon_pwm(int y_pos);
extern void Icon_timer(int y_pos);
extern void Icon_adc(int y_pos);
