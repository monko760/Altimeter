#include "U8glib.h"
#include "BMP280.h"
#include "Wire.h"
#define P0 1021.97  //1013.25 
BMP280 bmp;

// OLED Type
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

char sT[20];
char sP[9];
char sA[9];
char sA_MIN[9];
char sA_MAX[9];
double A_MIN = 0;
double A_MAX = 0;

void draw(double T, double P, double A) {
  u8g.setFont(u8g_font_timB24);



  dtostrf(A, 4, 2, sA);


  u8g.drawStr( 10, 24, "Alt : ");

  u8g.drawStr( 10, 60, sA);
}



void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("BMP init failed!");
    while (1);
  }
  else Serial.println("BMP init success!");

  bmp.setOversampling(4);

  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_unifont);
}

void loop(void) {
  double T, P;
  char result = bmp.startMeasurment();

  if (result != 0) {
    delay(result);
    result = bmp.getTemperatureAndPressure(T, P);

    if (result != 0) {
      double A = bmp.altitude(P, P0);

      if ( A > A_MAX) {
        A_MAX = A;
      }

      if ( A < A_MIN || A_MIN == 0) {
        A_MIN = A;
      }

      //      Serial.print("T = \t"); Serial.print(T, 2); Serial.print(" degC\t");
      //      Serial.print("P = \t"); Serial.print(P, 2); Serial.print(" mBar\t");
      //      Serial.print("A = \t"); Serial.print(A, 2); Serial.println(" m");

      u8g.firstPage();
      do {
        draw(T, P, A);
      } while ( u8g.nextPage() );
      u8g.firstPage();
      
      
    }
    else {
      Serial.println("Error.");
    }
  }
  else {
    Serial.println("Error.");
  }

  delay(100);

}
