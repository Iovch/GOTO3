/*
 * GOTO3.h Written by Igor Ovchinnikov 12/10/2017
 */

void SetStDX(void)
{
 if(analogRead(DX_SW_PIN)>512) iStDX= -1; else iStDX= 1;
}

void SetStDY(void)
{
 if(analogRead(DY_SW_PIN)>512) iStDY=1; else iStDY=-1; 
}
 
long Stepper_step(long ipSteps, unsigned uStepPin, unsigned uDirPin, unsigned uStepsPS)
{
 long iSteps=ipSteps, lRetVal=0;
 if((uStepPin>53)||(uDirPin>53)) return lRetVal;
 
 if(iSteps > 0) digitalWrite(uDirPin,  LOW);
 if(iSteps < 0) digitalWrite(uDirPin,  HIGH);
 iSteps=abs(iSteps);

 while (iSteps>0)
 {
  digitalWrite(13, HIGH);        // Зажигаем LCD
  digitalWrite(uStepPin,  HIGH);
  delay(1000/uStepsPS);
  digitalWrite(uStepPin,  LOW);
  digitalWrite(13, LOW);         // Тушим LCD
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

  if(!bRun) 
  {
  if(iA1<10)              { iRetValue=iRetValue | 16; } // Полный шаг X+
  if(iA1>=10 && iA1 < 40) { iRetValue=iRetValue |  1; } // Микрошаг X+
  if(iA1> 70 && iA1<= 90) { iRetValue=iRetValue |  4; } // Микрошаг X-
  if(iA1>90  && iA1<=500) { iRetValue=iRetValue | 64; } // Полный шаг X-
  }
  
  if(iA2<10)              { iRetValue=iRetValue | 32; } // Полный шаг Y+
  if(iA2>=10 && iA2 < 40) { iRetValue=iRetValue |  2; } // Микрошаг Y+
  if(iA2>70  && iA2<= 90) { iRetValue=iRetValue |  8; } // Микрошаг Y-
  if(iA2>90  && iA2<=500) { iRetValue=iRetValue | 128;} // Полный шаг Y-

  if(iA3<500) {iRetValue=iRetValue | 256; delay(250);}    // Включить/отключить трекинг
 
  if(iA1>0) SetStDX(); //Установка направления вращения оси Х (задержка 0,1 мс)
  if(iA2>0) SetStDY(); //Установка направления вращения оси Y (задержка 0,1 мс)
   
  return iRetValue;
}

void SetLatLon(void)
{
  Latitude =double(W[0])+double(W[1])/60.0+double(W[2])/3600.0;
  if(W[3]==1) Latitude=-Latitude;
  Longitude=double(W[4])+double(W[5])/60.0+double(W[6])/3600.0;
  if(W[7]==0) Longitude=-Longitude;
}

void SendLatLon(void)
{
  W[0]=int(abs(Latitude));
  W[1]=int((abs(Latitude)-W[0])*60);
  W[2]=int((abs(Latitude)-W[0]-W[1]*60)*3600);
  if(Latitude<0) W[3]=1; else W[3]=0;
  W[4]=int(abs(Longitude));
  W[5]=int((abs(Longitude)-W[4])*60);
  W[6]=int((abs(Longitude)-W[4]-W[5]*60)*3600);
  if(Longitude<0) W[7]=0; else W[7]=1;
  Serial.flush();
  Serial.write(W,8);
}
