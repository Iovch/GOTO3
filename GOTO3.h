/*
 * GOTO3.h Written by Igor Ovchinnikov 24/07/2016
 */
 
long Stepper_step(long ipSteps, unsigned uStepPin, unsigned uDirPin, unsigned uStepsPS)
{
 long iSteps=ipSteps, lRetVal=0;
 if((uStepPin>53)||(uDirPin>53)) return lRetVal;
 
 if(iSteps > 0) digitalWrite(uDirPin,  LOW);
 if(iSteps < 0) digitalWrite(uDirPin,  HIGH);
 iSteps=abs(iSteps);

 while (iSteps>0)
 {
  digitalWrite(uStepPin,  HIGH);
  delay(1000/uStepsPS);
  delayMicroseconds(1000*(1000%uStepsPS));
  digitalWrite(uStepPin,  LOW);
  iSteps--;
  if (ipSteps>0) lRetVal++; else lRetVal--;
 }
 return lRetVal;
}

void Stepper_X_step(int ipSteps)
{
  Stepper_step(ipSteps, DX_STEP_PIN, DX_DIR_PIN, imStepsXPS);
}

void Stepper_Y_step(int ipSteps)
{
  Stepper_step(ipSteps, DY_STEP_PIN, DY_DIR_PIN, imStepsYPS);
}

void Stepper_Z_step(int ipSteps)
{
  Stepper_step(ipSteps, DZ_STEP_PIN, DZ_DIR_PIN, imStepsZPS);
}

void AscFoSw(void)
{
 if(analogRead(SW_FOC_SENCE)<200)
 {
  bFocus=true;
  analogWrite(LIHT_FOC_PIN,150); // Светодиод фокусера включен
 }
 else
 {
  bFocus=false;
  analogWrite(LIHT_FOC_PIN,0); // Светодиод фокусера выключен
 }
}

// Функция int AskJoy() возвращает при ее вызове следующие значения:

//    0 - когда ничего не надо делать
//    1 - когда надо сделать микрошаг вперед по оси Х
//   16 - когда надо сделать полныйшаг вперед по оси Х
//    4 - когда надо сделать микрошаг назад по оси Х
//   64 - когда надо сделать полныйшаг назад по оси Х
//    2 - когда надо сделать микрошаг вверх по оси У
//   32 - когда надо сделать полныйшаг вверх по оси У
//    8 - когда надо сделать микрошаг вниз по оси У
//  128 - когда надо сделать полныйшаг вниз по оси У
//  256 - включить/отключить трекинг

int AskJOY()
{
  int iA1=0, iA2=0, iA3=0;
  int iRetValue=0;

  iA1 = analogRead(X_JOY_SENCE);
  iA2 = analogRead(Y_JOY_SENCE);
  iA3 = analogRead(SW_JOY_SENCE);
    
  if(iA1<25)                { iRetValue=iRetValue | 16; } // Полный шаг X+
  if(iA1>=25 && iA1 < 490)  { iRetValue=iRetValue |  1; } // Микрошаг X+
  if(iA1>520 && iA1<=1000)  { iRetValue=iRetValue |  4; } // Микрошаг X-
  if(iA1>1000)              { iRetValue=iRetValue | 64; } // Полный шаг X-
  
  if(iA2<25)                { iRetValue=iRetValue | 32; } // Полный шаг Y+
  if(iA2>=25  && iA2 < 490) { iRetValue=iRetValue |  2; } // Микрошаг Y+
  if(iA2>510  && iA2<=1000) { iRetValue=iRetValue |  8; } // Микрошаг Y-
  if(iA2>1000)              { iRetValue=iRetValue | 128;} // Полный шаг Y-

  if(iA3<500) {iRetValue=iRetValue | 256; delay(250);}    // Включить/отключить трекинг

  return iRetValue;
}

unsigned long StrToHEX (String STR)
{
  int  i;
  char c;
  unsigned long ulVal=0;
  for (i=0; i<STR.length(); i++)
  {
   ulVal=ulVal*16;
   c=STR.charAt(i);
   switch (c) 
    {
      case 'f': ;
      case 'F': ulVal++;
      case 'e': ;
      case 'E': ulVal++;
      case 'd': ;
      case 'D': ulVal++;
      case 'c': ;
      case 'C': ulVal++;
      case 'b': ;
      case 'B': ulVal++;
      case 'a': ;
      case 'A': ulVal++;
      case '9': ulVal++;
      case '8': ulVal++;
      case '7': ulVal++;
      case '6': ulVal++;
      case '5': ulVal++;
      case '4': ulVal++;
      case '3': ulVal++;
      case '2': ulVal++;
      case '1': ulVal++;
    };
  };
 return ulVal;
};

String HexTo8D (unsigned long Hex)
{
  String STR0="";
  char c = '0';
  if (Hex<0x10000000) STR0 += c;
  if (Hex<0x1000000)  STR0 += c;
  if (Hex<0x100000)   STR0 += c;
  if (Hex<0x10000)    STR0 += c;
  if (Hex<0x1000)     STR0 += c;
  if (Hex<0x100)      STR0 += c;
  if (Hex<0x10)       STR0 += c;
  return STR0;
};

