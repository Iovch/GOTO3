/*
 Config.h File Written by Igor Ovchinnikov 24/07/2016
 */

#define ENABLE_XYZ_PIN 4 //Enable X,Y,Z pin
#define DX_STEP_PIN  9   //Контакт ардуино идущий на STEP драйвера X
#define DX_DIR_PIN   8   //Контакт ардуино идущий на DIR  драйвера X
#define DX_FORCE_PIN 2   //Разгонный пин драйвера X
#define DY_STEP_PIN  10   //Контакт ардуино идущий на STEP драйвера Y
#define DY_DIR_PIN   11   //Контакт ардуино идущий на DIR  драйвера Y
#define DY_FORCE_PIN 3  //Разгонный пин драйвера Y
#define X_JOY_SENCE  A5  //Сенсор оси Х джойстика
#define Y_JOY_SENCE  A4  //Сенсор оси У джойстика
#define SW_JOY_SENCE A3  //Сенсор кнопки джойстика
#define DZ_STEP_PIN  6   //Контакт ардуино идущий на STEP драйвера Z
#define DZ_DIR_PIN   7   //Контакт ардуино идущий на DIR  драйвера Z
#define DZ_FORCE_PIN 5  //Разгонный пин драйвера Z
#define SW_FOC_SENCE A1  //Сенсор кнопки фокусера
#define LIHT_FOC_PIN 12  //Пин индикатора фокусера

int iStepsDX  =   200;    //Полных шагов на 1 оборот двигателя X
int iStepsXPS =  1000;    //Полных шагов в секунду на двигателе X
int iXStepX   =   8;    //Кратность шага драйвера X
double dRDX   = 772.8; //Передаточное число редуктора X

int iStepsDY  =   200;    //Полных шагов на 1 оборот двигателя Y
int iStepsYPS = 1000;    //Полных шагов в секунду на двигателе Y
int iYStepX   =    8;    //Кратность шага драйвера Y
double dRDY   = 492.8; //Передаточное число редуктора Y

int iStepsDZ  =   192;    //Полных шагов на 1 оборот двигателя Z
int iStepsZPS = 1000;    //Полных шагов в секунду на двигателе Z
int iZStepX   =    8;    //Кратность шага драйвера Z


int iStDX = -1;      //Исходное направление шага двигателя Х
int iStDY = -1;      //Исходное направление шага двигателя Y
int iStDZ = -1;      //Исходное направление шага двигателя Z

