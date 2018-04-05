/*
 * GOTO3LF.ino Written by Igor Ovchinnikov 12/10/2017
 */

//Все файлы #include "ххххх.h"должны находиться в той же директории, что GOTO3LFG.ino

//#include "G3ConMy.h"
#include "G3ConLe.h"
#include "Maths.h"

int imStepsXPS; //Микрошагов в секунду на двигателе X задается в Force_X()
int imStepsYPS; //Микрошагов в секунду на двигателе Y задается в Force_Y()

unsigned long ulSPRA; //Микрошагов двигателя X на полный оборот оси прямого восхождения задается в Force_X()
unsigned long ulSPDE; //Микрошагов двигателя Y на полный оборот оси склонения задается в Force_Y()

double drRaPerStep;  //Единиц прямого восхождения на 1 шаг двигателя задается в Force_X()
double drDePerStep;  //Единиц склонения на 1 шаг двигателя задается в Force_Y()

double drDRaPerStep; //Поправка вращения Земли на 1 шаг ДПВ задается в Force_X()
double drDDePerStep; //Поправка на 1 шаг ДПВ задается в Force_Y()

unsigned long StarMSPS=86164091; //Милисекунд в Звездных сутках

double dRaStepsPms; //Микрошагов двигателя X на 1 мс задается в Force_X()

//int iStDX = -1;      //Исходное направление шага двигателя Х
//int iStDY =  1;      //Исходное направление шага двигателя Y
int iMovement = 0;   //Может пригодиться 
int iLastMovement=0; //Может пригодиться

double ulMilisec=0.0;   //Виртуальное время трекера
double dStartTimer=0.0; //Только для отладки
unsigned long ulPortTimer=0; //Таймер порта

boolean bDebug  = false;  //Режим отладки
boolean bRun    = true;   //Трекинг включен изначально
boolean bLCD    = false;  //Скоро пригодится
boolean bForceX = true;   //Ускоренный режим Х
boolean bForceY = true;   //Ускоренный режим Y
boolean bAlignment=false; //Монтировка не выровнена
boolean bFocus=false;     //Фокусер выключен

//unsigned long ulRA=0;   //Текущее (исходное) значение прямого восхождения
//unsigned long ulDE=0;   //Текущее (исходное) значение склонения
//unsigned long ulToRA=0; //Целевое значение прямого восхождения
//unsigned long ulToDE=0; //Целевое значение склонения

struct RaRa {double AtX; double AtY; double ToX; double ToY; int FLXY;}; //Координаты наведения в радианах

RaRa RaDe  {0.0, 0.0, 0.0, 0.0, 0}; //Исходные и целевые RaDe

const unsigned long ulMaxValue = 0xFFFFFFFF;  //Максимальное значение величины unsigned long

String STR= "", STR1="", STR2="";
byte P[7], H[8], W[8];

//GOTO3.h должен находиться в той же директории, что GOTO3.ino

#include "GOTO3.h"
#include "MTime.h"

int AskControl()
{
  return AskJOY();
}

int GetString (void)
{
  int GetString=0;
  STR="";
  char c;
  while (Serial.available()) //если есть что читать;
  {
   c = Serial.read(); //читаем символ
   STR += c;
   GetString+=1;
   delay(1); //Задержка при считывании каждого символа, до 06/06/17 была 2 мс
   if (STR.length()==1&&c=='P') {GetString+=Serial.readBytes(P,7); if(GetString==8) Serial.print("#");}
   if (STR.length()==1&&c=='H') {GetString+=Serial.readBytes(H,8); if(GetString==9) Serial.print("#");}
   if (STR.length()==1&&c=='W') {GetString+=Serial.readBytes(W,8); if(GetString==9) Serial.print("#");}
  }
  return GetString;
}

int GetSubStr ()
{
  int i;
  i=STR.indexOf(',');
  STR2=STR.substring(i+1);
  if (i<=1) STR1="";
  else STR1=STR.substring(1,i);
};

void action(String STRA)
{
  char cAction;
  cAction=STRA.charAt(0);
  switch (cAction)
  {
    case '?': {Serial.print(char(0)); Serial.print("#"); break;}
    case 'E': {Serial.print(HexToStr(RaToUL(RaDe.AtX,ulMaxValue)>>16,4)+','+HexToStr(RaToUL(RaDe.AtY,ulMaxValue)>>16,4)+'#'); break;}     
    case 'e': {Serial.print(HexToStr(RaToUL(RaDe.AtX,ulMaxValue),8)); Serial.print(","); Serial.print(HexToStr(RaToUL(RaDe.AtY,ulMaxValue),8)); Serial.print("#"); break;}
    case 'H': {SetTime();  Serial.print("#"); break;}
    case 'h': {SendTime(); break;}
    case 'J': {if(true) Serial.print(char(1)); else Serial.print(char(0)); Serial.print("#"); break;}
    case 'K': {GetSubStr(); Serial.print(STR1+"#"); break;}
    case 'L': {if (RaDe.FLXY==0) Serial.print("0#"); else Serial.print("1#"); break;}
    case 'm': {Serial.print(char(6)); Serial.print("#"); break;} // 6 - Edvanced GT
    case 'M': {RaDe.ToX=RaDe.AtX; RaDe.ToY=RaDe.AtY; RaDe.FLXY=0; Serial.print("#"); break;}           
    case 'r': {To_PRADEC(); Serial.print("#"); break;}
    case 't': {Serial.print(char(2)); Serial.print("#"); break;}
    case 'V': {Serial.print("12#"); break;} //Версия протокола 
    case 'W': {SetLatLon();  Serial.print("#"); break;}
    case 'w': {SendLatLon(); Serial.print("#"); break;}
    default:  {Serial.print("#"); break;}
  };
}

int Force_X(boolean bForce)
{
 int iXSX=0; 
 if(!bForceX && bForce) //Включаем полношаговый режим
 {
   iXSX = 1; //Кратность шага драйвера X
   digitalWrite(DX_FORCE_PIN, LOW);
   imStepsXPS = iStepsXPS*iXSX; //Шагов в секунду на двигателе X
   ulSPRA = iStepsDX*dRDX*iXSX; //Шагов двигателя X на полный оборот оси прямого восхождения
   drRaPerStep=drMaxValue/double(ulSPRA); //Изменение прямого восхождения за 1 шаг двигателя
   dRaStepsPms=double(ulSPRA)/double(StarMSPS); //Микрошагов двигателя X на 1 мс
 //  dRaStepsPms=(double(imStepsXPS)/1000.0);
   drDRaPerStep=drMaxValue/double(StarMSPS)*1000.0/double(imStepsXPS); //Поправка вращения Земли на 1 шаг ДПВ
   bForceX=true;
 }
 if(bForceX && !bForce) //Включаем микрошаговый режим
 {
   iXSX = iXStepX; //Кратность шага драйвера X
   digitalWrite(DX_FORCE_PIN, HIGH);
   imStepsXPS = 500; //Микрошагов в секунду на двигателе X
   ulSPRA = iStepsDX*dRDX*iXSX; //Микрошагов двигателя X на полный оборот оси прямого восхождения
   drRaPerStep=drMaxValue/double(ulSPRA); //Изменение прямого восхождения за 1 шаг двигателя
   dRaStepsPms=double(ulSPRA)/double(StarMSPS); //Микрошагов двигателя X на 1 мс
//   dRaStepsPms=(double(imStepsXPS)/1000.0);
   drDRaPerStep=drMaxValue/double(StarMSPS)*1000.0/double(imStepsXPS); //Поправка вращения Земли на 1 шаг ДПВ
   bForceX=false;
  }
}

int Force_Y(boolean bForce)
{
  int iYSX=0;
  if(!bForceY && bForce) //Включаем полношаговый режим
  {
    iYSX = 1; //Кратность шага драйвера Y
    digitalWrite(DY_FORCE_PIN, LOW);
    imStepsYPS = iStepsYPS*iYSX; //Шагов в секунду на двигателе Y
    ulSPDE = iStepsDY*dRDY*iYSX; //Шагов двигателя Y на полный оборот оси склонений
    drDePerStep=drMaxValue/double(ulSPDE); //Изменение склонения за 1 шаг двигателя
    drDDePerStep=0.0; //Поправка (доворот) ДСК в единицах радианах на 1 шаг ДСК
    bForceY=true;
   }
  if(bForceY && !bForce) //Включаем микрошаговый режим
  {
    iYSX = iYStepX; //Кратность шага драйвера Y
    digitalWrite(DY_FORCE_PIN, HIGH);
    imStepsYPS = 500; //Микрошагов в секунду на двигателе Y
    ulSPDE = iStepsDY*dRDY*iYSX; //Микрошагов двигателя Y на полный оборот оси склонений
    drDePerStep=drMaxValue/double(ulSPDE); //Изменение склонения за 1 шаг двигателя
    drDDePerStep=0.0; //Поправка (доворот) ДСК в радианах на 1 шаг ДСК
    bForceY=false;
  }
}

void To_PRADEC(void)
{
  int DirectRA=0;
  int DirectDE=0;
  double drRa=0.0; // unsigned long uldRA=0;
  double drDe=0.0; // unsigned long uldDE=0;
  unsigned long ulStartMilis=millis();

  GetSubStr ();
  RaDe.ToX=ULToDRad(StrToHex(STR1),ulMaxValue);
  RaDe.ToY=ULToDRad(StrToHex(STR2),ulMaxValue);
  
  Force_X(true);
  Force_Y(true);
  
  if (RaDe.ToX > RaDe.AtX) {drRa = (RaDe.ToX-RaDe.AtX); DirectRA=  1;}
  if (RaDe.ToX < RaDe.AtX) {drRa = (RaDe.AtX-RaDe.ToX); DirectRA= -1;}
  if (drRa>drMaxValue/2.0) {drRa = drMaxValue-drRa; DirectRA = -(DirectRA);}

  if (RaDe.ToY > RaDe.AtY) {drDe = (RaDe.ToY-RaDe.AtY); DirectDE=  1;}
  if (RaDe.ToY < RaDe.AtY) {drDe = (RaDe.AtY-RaDe.ToY); DirectDE= -1;}
  if (drDe>drMaxValue/2.0) {drDe = drMaxValue-drDe; DirectDE = -(DirectDE);}
  
  if (drRa>drMaxValue/2.0) return; //Ошибка в расчете шагов по прямому восхождению
  if (drDe>drMaxValue/2.0) return; //Ошибка в расчете шагов по склонению
  
  while ((((drRa > drRaPerStep) && iStDX!=0) || ((drDe > drDePerStep) && iStDY!= 0)) && bAlignment)
  {
    if (drRa > drRaPerStep)
    {
      if (DirectRA > 0) {Stepper_X_step(-iStDX); drRa-=(drRaPerStep+drDRaPerStep); RaDe.AtX+=(drRaPerStep+drDRaPerStep);}
      if (DirectRA < 0) {Stepper_X_step( iStDX); drRa-=(drRaPerStep-drDRaPerStep); RaDe.AtX-=(drRaPerStep-drDRaPerStep);}
      if (drRa>drMaxValue/2.0) drRa=0.0;
    } else Force_X(false);
    if (drDe > drDePerStep)
    {
      if (DirectDE > 0) {Stepper_Y_step(-iStDY); drDe-=(drDePerStep+drDDePerStep); RaDe.AtY+=(drDePerStep+drDDePerStep);}
      if (DirectDE < 0) {Stepper_Y_step( iStDY); drDe-=(drDePerStep+drDDePerStep); RaDe.AtY-=(drDePerStep+drDDePerStep);}
      if (drDe>drMaxValue/2.0) drDe=0.0;
    } else Force_Y(false);
   if ((millis()-ulStartMilis)>1000) {bLCD=false; /*LCDPrint();*/ STR="e"; action(STR); ulStartMilis=millis();}
  }
  if (!bAlignment)  //Первая команда GOTO задает координаты наведения телескопа, без его реального перемещения
  {
    RaDe.AtX=RaDe.ToX; // ulRA=ulToRA;
    RaDe.AtY=RaDe.ToY; // ulDE=ulToDE;
    bAlignment=true;
  }
  bLCD=false;
 };

void reaction () //Обработка команд ПУ
  {
   int iKey=0;

// Здесь мы договариваемся, что функция int AskControl(),
// к чему бы она ни была привязана, возвращает при ее вызове следующие значения:

//   0 - когда ничего не надо делать
//   1 - когда надо сделать микрошаг вперед по оси Х
//  16 - когда надо сделать полныйшаг вперед по оси Х
//   4 - когда надо сделать микрошаг назад по оси Х
//  64 - когда надо сделать полныйшаг назад по оси Х
//   2 - когда надо сделать микрошаг вверх по оси У
//  32 - когда надо сделать полныйшаг вверх по оси У
//   8 - когда надо сделать микрошаг вниз по оси У
// 128 - когда надо сделать полныйшаг вниз по оси У
// 256 - включить/отключить трекинг

  do 
   {
    iMovement=0;
    iKey=AskControl();
    if ((iKey &   4)==  4 && iStDX!=0) {Force_X(false); Stepper_X_step(-iStDX); iMovement=iMovement |   4;} // Микрошаг назад
    if ((iKey &  64)== 64 && iStDX!=0) {Force_X(true);  Stepper_X_step(-iStDX); iMovement=iMovement |  64;} // Полный шаг назад
    if ((iKey &   1)==  1 && iStDX!=0) {Force_X(false); Stepper_X_step( iStDX); iMovement=iMovement |   1;} // Микрошаг вперед
    if ((iKey &  16)== 16 && iStDX!=0) {Force_X(true);  Stepper_X_step( iStDX); iMovement=iMovement |  16;} // Полный шаг вперед
    if(!bFocus)
     {
      if ((iKey &   8)==  8 && iStDY!=0) {Force_Y(false); Stepper_Y_step( iStDY); iMovement=iMovement |   8;} // Микрошаг вниз
      if ((iKey & 128)==128 && iStDY!=0) {Force_Y(true);  Stepper_Y_step( iStDY); iMovement=iMovement | 128;} // Полный шаг вниз
      if ((iKey &   2)==  2 && iStDY!=0) {Force_Y(false); Stepper_Y_step(-iStDY); iMovement=iMovement |   2;} // Микрошаг вверх
      if ((iKey &  32)== 32 && iStDY!=0) {Force_Y(true);  Stepper_Y_step(-iStDY); iMovement=iMovement |  32;} // Полный шаг вверх    
     }
     else
     {
      if ((iKey &   8)==  8 && iStDZ!=0) {Stepper_Z_step( iStDZ);} // Микрошаг фокусера F+
      if ((iKey & 128)==128 && iStDZ!=0) {Stepper_Z_step( iStDZ);} // Микрошаг фокусера F+
      if ((iKey &   2)==  2 && iStDZ!=0) {Stepper_Z_step(-iStDZ);} // Микрошаг фокусера F-
      if ((iKey &  32)== 32 && iStDZ!=0) {Stepper_Z_step(-iStDZ);} // Микрошаг фокусера F-
     }
    if ((iKey & 256)==256) 
     {
     if(!bRun) {bRun=true;  bLCD=false; bForceX = true; Force_X(false); ulMilisec=millis(); dStartTimer=ulMilisec;} //Включить  ведение  (Tracking ON)
     else      {bRun=false; bLCD=false;} //Отключить ведение (Tracking OFF)
     iMovement=iMovement | 256;
     }
   } while (iKey!=0);
  if (iMovement!=0) ulMilisec=millis(); 
 } 

void p(void)
{
 if (P[0]==1) //PHD2
   {
    if(P[1]==17&&P[2]==71&&P[6]==1) ; // lcd.print (" PHD2 Connected ");
   }
  if (P[0]==2) //Монтировка, фиксированные скорости
   {
    if(P[1]==16) //Ось Х
    {
     if(P[2]==36&&P[3]>=1&&P[3]<=6) {Force_X(false); Stepper_X_step(iStDX*P[3]);}      //По Х вправо
     if(P[2]==36&&P[3]>=7&&P[3]<=9) {Force_X(true);  Stepper_X_step(iStDX*(P[3]-6));}  //По Х вправо
     if(P[2]==37&&P[3]>=1&&P[3]<=6) {Force_X(false); Stepper_X_step(-iStDX*P[3]);}     //По Х влево
     if(P[2]==37&&P[3]>=7&&P[3]<=9) {Force_X(true);  Stepper_X_step(-iStDX*(P[3]-6));} //По Х влево
     if(P[2]==39) ; //Стоп Х
     if(P[2]==254&&P[6]==2) Serial.print("#"); //Ось Х управляется
    }
    if(P[1]==17) //Ось Y
    {
     if(P[2]==36 && P[3]>=1 && P[3]<=6) {Force_Y(false); Stepper_Y_step(iStDY*P[3]);}      //По Y вверх
     if(P[2]==36 && P[3]>=7 && P[3]<=9) {Force_Y(true);  Stepper_Y_step(iStDY*(P[3]-6));}  //По Y вверх
     if(P[2]==37 && P[3]>=1 && P[3]<=6) {Force_Y(false); Stepper_Y_step(-iStDY*P[3]);}     //По Y вниз
     if(P[2]==37 && P[3]>=7 && P[3]<=9) {Force_Y(true);  Stepper_Y_step(-iStDY*(P[3]-6));} //По Y вниз
     if(P[2]==39) ; //Стоп У
     if(P[2]==254&&P[6]==2) Serial.print("#"); //Ось У управляется
    }
   }
  if (P[0]==3) //Монтировка, Push (толчки)
   {
    if(P[1]==16) //Ось Х
    {
     if(P[2]==38&&P[3]==13)  {Force_X(false); Stepper_X_step(iStDX*P[4]);} //По Х вправо P[4]*10 ms
     if(P[2]==38&&P[3]==243) {Force_X(false); Stepper_X_step(-iStDX*P[4]);} //По Х влево  P[4]*10 ms
    }
    if(P[1]==17) //Ось Y
    {
     if(P[2]==38&&P[3]==13)  {Force_Y(false); Stepper_Y_step( iStDY*P[4]);} //По Y вверх P[4]*10 ms
     if(P[2]==38&&P[3]==243) {Force_Y(false); Stepper_Y_step(-iStDY*P[4]);} //По Y вниз  P[4]*10 ms
    }
   }
   if (P[0]==4) ; //Остановка монтировки
}

void setup()
{
  StarMSPS=StarMSPS-lDMSS*((double)StarMSPS/(double)86400000); //Корректировка внутреннего таймера millis()
  pinMode(ENABLE_XYZ_PIN,  OUTPUT);  // ENABLE XYZ PIN
  digitalWrite(ENABLE_XYZ_PIN, LOW); // LOW
  pinMode(DX_STEP_PIN,  OUTPUT);     // DX STEP PIN
  digitalWrite(DX_STEP_PIN, LOW);    // LOW
  pinMode(DX_DIR_PIN,  OUTPUT);      // DX DIR PIN
  digitalWrite(DX_DIR_PIN, LOW);     // LOW
  pinMode(DX_SW_PIN, INPUT_PULLUP);  // HIGH
  pinMode(DX_FORCE_PIN,  OUTPUT);    // DX FORCE PIN
  digitalWrite(DX_FORCE_PIN, HIGH);  // HIGH
  pinMode(DY_STEP_PIN,  OUTPUT);     // DY STEP PIN
  digitalWrite(DY_STEP_PIN, LOW);    // LOW
  pinMode(DY_DIR_PIN,  OUTPUT);      // DY DIR PIN
  digitalWrite(DY_DIR_PIN, LOW);     // LOW
  pinMode(DY_SW_PIN, INPUT_PULLUP);  // HIGH
  pinMode(DY_FORCE_PIN,  OUTPUT);    // DY FORCE PIN
  digitalWrite(DY_FORCE_PIN, HIGH);  // HIGH
  pinMode(DZ_STEP_PIN,  OUTPUT);     // DZ STEP PIN
  digitalWrite(DZ_STEP_PIN, LOW);    // LOW
  pinMode(DZ_DIR_PIN,  OUTPUT);      // DZ DIR PIN
  digitalWrite(DZ_DIR_PIN, LOW);     // LOW
  pinMode(SW_JOY_SENCE, INPUT_PULLUP); // Сенсор кнопки джойстика
  pinMode(X_JOY_SENCE, INPUT);         // Сенсор оси X джойстика
  pinMode(Y_JOY_SENCE, INPUT);         // Сенсор оси Y джойстика
  pinMode(PW_JOY_SENCE, INPUT_PULLUP); // Питание джойстика отсюда
  pinMode(13,  OUTPUT);  // LED на плате ардуино
  digitalWrite(13, LOW); // Выключен
  Serial.begin(9600);    // Подключаем COM порт
  Serial.setTimeout(10); // Максимальная задержка при чтении байтов из порта
  Serial.flush();        // Сбрасываем содержимое COM порта
  Force_X(!bForceX);     // Инициализация переменных движения Х
  Force_Y(!bForceY);     // Инициализация переменных движения У
  ulMilisec=millis();    // Фиксируем время начала работы
 }

void loop()
{
 double LoopTime;
 double ddLST;   //Только для отладки
 int iH, iM, iS; //Только для отладки
 long StepsNeed;
 
 if (GetString()>0) action(STR); else if((millis()-ulPortTimer)>=1000) {action("e"); ulPortTimer=millis();}
 p();
 reaction ();
 
 if(bRun)
 {
  Force_X(false); //Микрошаговый режим
  LoopTime=millis()-ulMilisec;
  StepsNeed=double(LoopTime)*dRaStepsPms;
  if(StepsNeed>=1)
  {
   Stepper_X_step(StepsNeed*iStDX);  // Шагаем
   ulMilisec+=(double(StepsNeed)/dRaStepsPms); // Виртуальное время трекера
  }
 if (bDebug)
  {
   Serial.print("StepsNeed: "); Serial.print(StepsNeed);
   Serial.print(" dRaStepsPms: "); Serial.print(dRaStepsPms,5);
   Serial.print(" SystemTime: "); Serial.print(millis()/1000);
   Serial.print(" MountTime: "); Serial.print(ulMilisec/1000);
   Serial.print(" = ");
   ddLST=(ulMilisec-dStartTimer)/(double)StarMSPS*24.0;
   iH=int(ddLST);
   iM=int((ddLST-iH)*60.0);
   iS=int(((ddLST-iH)-iM/60.0)*60.0*60.0);
   Serial.print(iH); Serial.print(":");
   Serial.print(iM); Serial.print(":");
   Serial.println(iS);  
  }
 }
}
