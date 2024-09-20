#include <LiquidCrystal.h> 
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
const int AOUTpin=0;
int bac;
//#define BAC_START 410
// New BAC_START after calibration_1 is 120. This means that 120 is the base and
// everything greater than this will have a reading and it will follow the shape
// of the curve.
#define BAC_START 120

#define BAC_END 859
const uint8_t bac_chart[] = 
{2, 3, 4, 5, 6, 6, 7, 8, 8, 9, 10, 10, 11, 11, 12, 13, 13, 14, 14, 15, 15, 16, 
16, 16, 17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 
23, 23, 23, 23, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 
26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 
28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 
29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 
29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 
31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 
33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 
35, 35, 36, 36, 36, 36, 36, 36, 37, 37, 37, 37, 37, 37, 37, 38, 38, 38, 38, 38, 
38, 39, 39, 39, 39, 39, 39, 40, 40, 40, 40, 40, 40, 41, 41, 41, 41, 41, 41, 42, 
42, 42, 42, 42, 43, 43, 43, 43, 43, 43, 44, 44, 44, 44, 44, 44, 45, 45, 45, 45, 
45, 46, 46, 46, 46, 46, 46, 47, 47, 47, 47, 47, 48, 48, 48, 48, 48, 48, 49, 49, 
49, 49, 49, 49, 50, 50, 50, 50, 50, 51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 
53, 53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 55, 55, 55, 55, 55, 56, 56, 56, 56, 
56, 57, 57, 57, 57, 58, 58, 58, 58, 58, 59, 59, 59, 59, 60, 60, 60, 60, 61, 61, 
61, 61, 62, 62, 62, 62, 63, 63, 63, 64, 64, 64, 64, 65, 65, 65, 66, 66, 66, 67, 
67, 68, 68, 68, 69, 69, 69, 70, 70, 71, 71, 72, 72, 72, 73, 73, 74, 74, 75, 75, 
76, 76, 77, 78, 78, 79, 79, 80, 80, 81, 82, 82, 83, 84, 84, 85, 86, 87, 87, 88, 
89, 90, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 
106, 107, 108, 109, 111, 112, 113, 114, 116, 117, 118, 120, 121, 122, 124, 125, 
127, 128, 130, 131, 133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 153, 
155, 157, 159, 161, 163, 165, 168, 170, 172, 174, 177, 179, 182, 184, 187, 189, 
192, 194};

LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
byte block[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
void setup()
{
  analogWrite(11, 120); // makes the contrast to the optimal level
  lcd.begin(16,2);      // set up the LCD's number of columns and rows:
  lcd.setCursor(0,0);   // set the cursor to column 0, line 0
  lcd.print("Calibrating:"); //// Print a message to the LCD.
  lcd.createChar(0, block); // Creates the block character
  for (int i=0; i<16;i++) {
    lcd.setCursor(i,1);   // Move to the second row, ith column
    lcd.write(byte(0));
    delay(1500);
  }
  
}
void loop()
{
  lcd.clear();
  lcd.setCursor(0,0);   // set the cursor to column 0, line 0
  lcd.print("Drunk level:"); //// Print a message to the LCD.
  lcd.setCursor(0,1);   // set the cursor to column 0, line 1
  int sensorRead= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
  lcd.print(sensorRead);
  lcd.setCursor(6,1);
  lcd.print("/");
  lcd.setCursor(8,1);

  
  if (sensorRead < BAC_START) {
    lcd.print("0.000");
  } else if (sensorRead > BAC_END) {
    lcd.print("EEEE");
  } else {
    int newSensorRead = sensorRead - BAC_START;
    bac = bac_chart[newSensorRead];
    if (bac < 10) {
      String bac_string = "0.00";
      bac_string = bac_string + bac;
      lcd.print(bac_string);
    } else if (bac < 100) {
      String bac_string = "0.0";
      bac_string = bac_string + bac;
      lcd.print(bac_string);
    } else {
      String bac_string = "0.";
      bac_string = bac_string + bac;
      lcd.print(bac_string);
    }
    
  }

  delay (500);
  }
