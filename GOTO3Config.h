/*
 Config.h File Written by Igor Ovchinnikov 24/07/2016
 */

#define ENABLE_XYZ_PIN 8 //Enable X,Y,Z pin
#define DX_STEP_PIN  5   //Контакт ардуино идущий на STEP драйвера X
#define DX_DIR_PIN   2   //Контакт ардуино идущий на DIR  драйвера X
#define DX_FORCE_PIN 9   //Разгонный пин драйвера X
#define DY_STEP_PIN  6   //Контакт ардуино идущий на STEP драйвера Y
#define DY_DIR_PIN   3   //Контакт ардуино идущий на DIR  драйвера Y
#define DY_FORCE_PIN 10  //Разгонный пин драйвера Y
#define X_JOY_SENCE  A6  //Сенсор оси Х джойстика
#define Y_JOY_SENCE  A7  //Сенсор оси У джойстика
#define SW_JOY_SENCE A3  //Сенсор кнопки джойстика
#define DZ_STEP_PIN  7   //Контакт ардуино идущий на STEP драйвера Z
#define DZ_DIR_PIN   4   //Контакт ардуино идущий на DIR  драйвера Z
#define SW_FOC_SENCE A1  //Сенсор кнопки фокусера
#define LIHT_FOC_PIN 11  //Пин индикатора фокусера

int iStepsDX  =   48;    //Полных шагов на 1 оборот двигателя X
int iStepsXPS =  250;    //Полных шагов в секунду на двигателе X
int iXStepX   =   16;    //Кратность шага драйвера X
double dRDX   = 1780.69; //Передаточное число редуктора X

int iStepsDY  =   96;    //Полных шагов на 1 оборот двигателя Y
int iStepsYPS = 5000;    //Полных шагов в секунду на двигателе Y
int iYStepX   =    4;    //Кратность шага драйвера Y
double dRDY   = 3168.00; //Передаточное число редуктора Y

int imStepsZPS = 60;    //Микрошагов в секунду на двигателе Y

int iStDX = -1;      //Исходное направление шага двигателя Х
int iStDY =  1;      //Исходное направление шага двигателя Y
int iStDZ =  1;      //Исходное направление шага двигателя Z
