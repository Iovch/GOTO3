/*
 NSMOUNTC.h File Written by Igor Ovchinnikov 12/10/2017
*/

#define ENABLE_XYZ_PIN 8  //Enable X,Y,Z pin
#define DX_STEP_PIN  5    //Контакт ардуино идущий на STEP драйвера X
#define DX_DIR_PIN   2    //Контакт ардуино идущий на DIR  драйвера X
#define DX_SW_PIN    A1   //Контакт переключателя направления вращения оси X
#define DX_FORCE_PIN 9    //Разгонный пин драйвера X
#define DY_STEP_PIN  6    //Контакт ардуино идущий на STEP драйвера Y
#define DY_DIR_PIN   3    //Контакт ардуино идущий на DIR  драйвера Y
#define DY_SW_PIN    A2   //Контакт переключателя направления вращения оси Y
#define DY_FORCE_PIN 10   //Разгонный пин драйвера Y
#define DZ_STEP_PIN  7    //Контакт ардуино идущий на STEP драйвера Z
#define DZ_DIR_PIN   4    //Контакт ардуино идущий на DIR  драйвера Z
#define DZ_FORCE_PIN 11   //Разгонный пин драйвера Z

#define X_JOY_SENCE  A4  //Сенсор оси Х джойстика
#define Y_JOY_SENCE  A5  //Сенсор оси У джойстика
#define SW_JOY_SENCE A6  //Сенсор кнопки джойстика
#define PW_JOY_SENCE A7  //Питающий ждойстик пин

long lDMSS = 0; //Поправка к Millis() за средние солнечные сутки (86400000ms)

int iStepsDX  =   48;    //Полных шагов на 1 оборот двигателя X
int iStepsXPS =  250;    //Полных шагов в секунду на двигателе X
int iXStepX   =   16;    //Кратность шага драйвера X
double dRDX   = 1780.69; //Передаточное число редуктора X

int iStepsDY  =   96;    //Полных шагов на 1 оборот двигателя Y
int iStepsYPS =  350;    //Полных шагов в секунду на двигателе Y
int iYStepX   =    4;    //Кратность шага драйвера Y
double dRDY   = 3168.00; //Передаточное число редуктора Y

int imStepsZPS = 50; //Микрошагов в секунду на двигателе Z

int iStDX = -1;      //Исходное направление шага двигателя Х
int iStDY =  1;      //Исходное направление шага двигателя Y
int iStDZ =  1;      //Исходное направление шага двигателя Z

int iZH = 5; //Часовой пояс
double Latitude = 56.7985; // Широта местности в градусах по умолчанию
double Longitude=-60.5923; // Долгота местности в градусах по умолчанию


