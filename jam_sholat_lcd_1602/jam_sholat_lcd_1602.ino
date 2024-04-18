#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "RTClib.h"
#include <PString.h>

LiquidCrystal_I2C lcd(0x20,16,2);
double fajr, sunRise, zuhr, asr, maghrib, isha;

RTC_DS1307 RTC;

int hours, mm, ss;
char buffer[17];
PString str(buffer, sizeof(buffer));

//Init Settings GPS Location
double setLongitude = 110.50;       //Untuk Kota Surakarta 7.32 LS 110.50 BT
double setLatitude = -7.32;         //Untuk Kota Surakarta 7.32 LS 110.50 BT
int setTimeZone = 7;                //UTC+07:00 WAKTU INDONESIA
double setFajarTwilight = -20;      //Penetapan Waktu Shubuh 20.0 deg kemiringan matahari
double setIshaTwilight = -18;       //Penetapan Waktu Isya 18.0 deg kemiringan matahari

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Wire.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  RTC.begin(); 
  Wire.beginTransmission(0X68);
  Wire.write(0x07);
  Wire.write(0x10);
  Wire.endTransmission();
}

void loop() {
  DateTime now = RTC.now();
  int count = 0;
  int a = 0;
  int x = 0;

  //AWAL TAMPILKAN JAM
  while (count < 5) {
    a++;
    DateTime now = RTC.now();
    hours = now.hour();
    mm = now.minute();
    ss = now.second();
    str.begin();
    if (hours < 10) {
      str.print(0);
      }
    str.print(hours); //tampilkan HOUR
    lcd.setCursor(4,0);
    lcd.print(str);

    //delay(5000);
    str.begin();
    str.print(":"); //tampilkan :
    if (a >= 21) {
      lcd.setCursor(6,0);
      lcd.print(str);
      }
    else {
      lcd.setCursor(6,0);
      lcd.print(str);
      }

    str.begin();
    if (mm < 10) {
      str.print(0);
      }
    str.print (mm); //tampilkan MINUTE
    lcd.setCursor(7,0);
    lcd.print(str);

    str.begin();
    str.print(":"); //tampilkan :
    if (a >= 21) {
      lcd.setCursor(9,0);
      lcd.print(str);
      }
    else {
      lcd.setCursor(9,0);
      lcd.print(str);
      }

    str.begin();
    if (ss < 10) {
      str.print(0);
    }
    str.print(ss); //tampilkan SECOND
    lcd.setCursor(10,0);
    lcd.print(str);
    
    if(a==72) {
      a=0;
      count++;
    }
  }
  ///////////////////////////////////

  //MULAI TAMPILKAN JADWAL
  count = 0;
  lcd.clear();
  while (count < 40) {
    a++;
    DateTime now = RTC.now();
    calcPrayerTimes(now.year(), now.month(), now.day(), setLongitude, setLatitude, setTimeZone, setFajarTwilight, setIshaTwilight, fajr, sunRise, zuhr, asr, maghrib, isha);
    int hours, minutes;

    hours = now.hour();
    mm = now.minute();
    ss = now.second();
    
    str.begin();
    if (hours < 10) {
      str.print(0);
    }
    str.print (hours); //tampilkan HOUR
    lcd.setCursor(0,0);
    lcd.print(str);

    str.begin();
    str.print(":"); //tampilkan :
    if (a > 5) {
      lcd.setCursor(2,0);
      lcd.print(str);
    }
    else {
      lcd.setCursor(2,0);
      lcd.print(str);
    }

    str.begin();
    if (mm < 10) {
      str.print(0);
    }
    str.print (mm); //tampilkan MINUTE
      lcd.setCursor(3,0);
      lcd.print(str);

    str.begin();
    str.print(":"); //tampilkan :
    if (a > 5) {
      lcd.setCursor(5,0);
      lcd.print(str);
    }
    else {
      lcd.setCursor(5,0);
      lcd.print(str);
    }

    str.begin();
    if (ss < 10) {
      str.print(0);
    }
    str.print(ss); //tampilkan SECOND
    lcd.setCursor(6,0);
    lcd.print(str);

    if (count >= 32) {
      x = 1;
      //doubleToHrMin(fajr, hours, minutes);
      doubleToHrMin(isha, hours, minutes);
    }
    else if (count >= 24) {
      // doubleToHrMin(zuhr, hours, minutes);
      doubleToHrMin(maghrib, hours, minutes);
      x = 2;
    }
    else if (count >= 16) {
      doubleToHrMin(asr, hours, minutes);
      x = 3;
    }
    else if (count >= 8) {
      //doubleToHrMin(maghrib, hours, minutes);
      doubleToHrMin(zuhr, hours, minutes);
      x = 4;
    }
    else if (count >= 0) {
      //doubleToHrMin(isha, hours, minutes);
      doubleToHrMin(fajr, hours, minutes);
      x = 5;
    }

    str.begin();
    if (hours < 10) {
      str.print("0");
    }
    str.print(hours); //tampilkan JADWAL HOUR
    lcd.setCursor(11,0);
    lcd.print(str);

    str.begin();
    str.print(":"); //tampilkan :
    if (a > 5) {
      lcd.setCursor(13,0);
      lcd.print(str);
    }
    else {
      lcd.setCursor(13,0);
      lcd.print(str);
    }

    str.begin();
    if (minutes < 10) {
      str.print("0");
    }
    str.print(minutes); //tampilkan JADWAL MINUTE
    lcd.setCursor(14,0);
    lcd.print(str);
    str.begin();


    switch (x) {
      case 1:
        str.print("            ISYA");
        lcd.setCursor(0,1);
        lcd.print(str);
        break;
      case 2:
        str.print("         MAGHRIB");
        lcd.setCursor(0,1);
        lcd.print(str);
        break;
      case 3:
        str.print("           ASHAR");
        lcd.setCursor(0,1);
        lcd.print(str);
        break;
      case 4:
        str.print("          DZUHUR");
        lcd.setCursor(0,1);
        lcd.print(str);
        break;
      case 5:
        str.print("          SHUBUH");
        lcd.setCursor(0,1);
        lcd.print(str);
        break;
    }
    if (a == 10) {
      a = 0;
      count++;
    }
  }
  ///////////////////////////////////
  
  lcd.clear();
}


// BAGIAN UNTUK HITUNG JADWAL SHOLAT
double degToRad(double degree)   {return ((3.1415926 / 180) * degree);}

double radToDeg(double radian)   {return (radian * (180 / 3.1415926));}

double moreLess360(double value) {
  while (value > 360 || value < 0) {
    if (value > 360) {
      value -= 360;
    }
    else if (value < 0) {
      value += 360;
    }
  }
  return value;
}

double moreLess24(double value) {
  while (value > 24 || value < 0) {
    if (value > 24) {
      value -= 24;
    }
    else if (value < 0) {
      value += 24;
    }
  }
  return value;
}

void doubleToHrMin(double number, int &hours, int &minutes) {
  hours = floor(moreLess24(number));
  minutes = floor(moreLess24(number - hours) * 60);
}

void calcPrayerTimes(int year, int month, int day,
                     double longitude, double latitude, int timeZone,
                     double fajrTwilight, double ishaTwilight,
                     double &fajrTime, double &sunRiseTime, double &zuhrTime,
                     double &asrTime, double &maghribTime, double &ishaTime)
                     {
  double D = (367 * year) - ((year + (int)((month + 9) / 12)) * 7 / 4) + (((int)(275 * month / 9)) + day - 730531.5);

  double L = 280.461 + 0.9856474 * D;
  L = moreLess360(L);

  double M = 357.528 + (0.9856003) * D;
  M = moreLess360(M);

  double Lambda = L + 1.915 * sin(degToRad(M)) + 0.02 * sin(degToRad(2 * M));
  Lambda = moreLess360(Lambda);

  double Obliquity = 23.439 - 0.0000004 * D;
  double Alpha = radToDeg(atan((cos(degToRad(Obliquity)) * tan(degToRad(Lambda)))));
  Alpha = moreLess360(Alpha);

  Alpha = Alpha - (360 * (int)(Alpha / 360));
  Alpha = Alpha + 90 * (floor(Lambda / 90) - floor(Alpha / 90));

  double ST = 100.46 + 0.985647352 * D;
  double Dec = radToDeg(asin(sin(degToRad(Obliquity)) * sin(degToRad(Lambda))));
  double Durinal_Arc = radToDeg(acos((sin(degToRad(-0.8333)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));

  double Noon = Alpha - ST;
  Noon = moreLess360(Noon);

  double UT_Noon = Noon - longitude;

  ////////////////////////////////////////////
  // Calculating Prayer Times Arcs & Times //
  //////////////////////////////////////////

  // 2) Zuhr Time [Local noon]
  zuhrTime = UT_Noon / 15 + timeZone;

  // Asr Hanafi
  double Asr_Alt = radToDeg(atan(2 + tan(degToRad(latitude - Dec))));
  double Asr_Arc = radToDeg(atan(1.7 + tan(degToRad(latitude - Dec))));
  // Asr Shafii
  // double Asr_Alt = radToDeg(atan(1 + tan(degToRad(latitude - Dec))));
  // double Asr_Arc = radToDeg(acos((sin(degToRad(90 - Asr_Alt)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));
  Asr_Arc = Asr_Arc / 14.7;
  
  // 3) Asr Time
  asrTime = zuhrTime + Asr_Arc;

  // 1) Shorouq Time
  sunRiseTime = zuhrTime - (Durinal_Arc / 15);

  // 4) Maghrib Time
  maghribTime = zuhrTime + (Durinal_Arc / 14.7);

  double Esha_Arc = radToDeg(acos((sin(degToRad(ishaTwilight)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));

  // 5) Isha Time
  ishaTime = zuhrTime + (Esha_Arc / 15);

  // 0) Fajr Time
  double Fajr_Arc = radToDeg(acos((sin(degToRad(fajrTwilight)) - sin(degToRad(Dec)) * sin(degToRad(latitude))) / (cos(degToRad(Dec)) * cos(degToRad(latitude)))));
  fajrTime = zuhrTime - (Fajr_Arc / 15);

  return;
}
