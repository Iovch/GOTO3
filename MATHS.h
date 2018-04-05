/*
 * MATHS.h Written by Igor Ovchinnikov 10/04/2017
*/

double drMaxValue=2.0*PI; //Максимальное значение величин в радианах

double NorRad (double pRad)
 {
  double NorRad=pRad;
  while(NorRad<0) NorRad+=drMaxValue;
  while(NorRad>drMaxValue) NorRad-=drMaxValue;
  return NorRad;
 }

unsigned long RaToUL (double Ra, unsigned long MaxValue) //Перевод величин из радиан в unsigned long [0,MaxValue]
 {
  double   D0=Ra, D2=MaxValue;
  unsigned long RaToUL;
  while(D0<0.0) D0+=drMaxValue;
  RaToUL=D0/drMaxValue*D2;
  return RaToUL;
 }

double ULToDRad (unsigned long Value, unsigned long MaxValue)
 {
   long double D1=Value>>8;
   long double D2=MaxValue>>8;
   return D1/D2*2.0*PI;
 }

unsigned long StrToHex (String STR)
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
}

String HexToStr (unsigned long ulpHex, int iDigits)
{
 String HexStr="";
 unsigned long ulHex=ulpHex;
 int iDigit=iDigits;
 do {
 if((ulHex&0xF)==0xF) HexStr="F"+HexStr; else 
 if((ulHex&0xE)==0xE) HexStr="E"+HexStr; else
 if((ulHex&0xD)==0xD) HexStr="D"+HexStr; else
 if((ulHex&0xC)==0xC) HexStr="C"+HexStr; else
 if((ulHex&0xB)==0xB) HexStr="B"+HexStr; else
 if((ulHex&0xA)==0xA) HexStr="A"+HexStr; else
 if((ulHex&0x9)==0x9) HexStr="9"+HexStr; else
 if((ulHex&0x8)==0x8) HexStr="8"+HexStr; else
 if((ulHex&0x7)==0x7) HexStr="7"+HexStr; else
 if((ulHex&0x6)==0x6) HexStr="6"+HexStr; else
 if((ulHex&0x5)==0x5) HexStr="5"+HexStr; else
 if((ulHex&0x4)==0x4) HexStr="4"+HexStr; else
 if((ulHex&0x3)==0x3) HexStr="3"+HexStr; else
 if((ulHex&0x2)==0x2) HexStr="2"+HexStr; else
 if((ulHex&0x1)==0x1) HexStr="1"+HexStr; else
 HexStr="0"+HexStr;
 ulHex=(ulHex>>4);
 iDigit--;
 } while (iDigit>0);
 return HexStr;
}
