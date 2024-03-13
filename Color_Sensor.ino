// 3 bits Color Sensor
#include <Servo.h>
#include <LiquidCrystal.h>
#define n 10

Servo myservo;
LiquidCrystal lcd(11, 12, 6, 7, 4, 5);
const int r_LED = A3;
const int b_LED = A4;
const int g_LED = A5;
const int SENSOR1 = A2;
const int SENSOR2 = A1;
const int TRIG = 3; // 超音波距離センサのトリガーピン
const int ECHO = 2; // 超音波距離センサのエコーピン

int r2_Value = 0;
int g2_Value = 0;
int b2_Value = 0;
int r1_Value = 0;
int g1_Value = 0;
int b1_Value = 0;
double speed_of_sound = 331.5 + 0.6 * 25; // 25℃の気温の想定
double Distance(double *distance);
void updateValue(int *R_Value, int *G_Value, int *B_Value, int *r2_Value, int *g2_Value, int *b2_Value);
String judge_color(int *R_Value, int *G_Value, int *B_Value, int *r1_Value, int *g1_Value, int *b1_Value);
void show_color(const char *color);

void setup() {
  pinMode(r_LED, OUTPUT);
  pinMode(g_LED, OUTPUT);
  pinMode(b_LED, OUTPUT);
  pinMode(ECHO, INPUT );
  pinMode(TRIG, OUTPUT );
  analogWrite(r_LED, 1023);
  analogWrite(g_LED, 1023);
  analogWrite(b_LED, 1023);
  Serial.begin(9600);
  lcd.begin(8,2);
  lcd.setCursor(0,0);
  lcd.print("Color");
  lcd.setCursor(0,1);
  lcd.print("Sensor");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LOW");
  delay(1000);
  updateValue(&r2_Value, &g2_Value, &b2_Value, 0, 0, 0); //show black
  lcd.setCursor(0,0);
  lcd.print("Done");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MAX");
  updateValue(&r1_Value, &g1_Value, &b1_Value, &r2_Value, &g2_Value, &b2_Value); //show white
  lcd.setCursor(0,0);
  lcd.print("Done");
  delay(500);
  lcd.clear();
  delay(1000);
}

void loop() {
  double distance = 0.0;
  Distance(&distance);
  if (distance <= 4){
    int R_Value = 0;
    int G_Value = 0;
    int B_Value = 0;
    lcd.clear();
    String color ;
    delay(100);
    updateValue(&R_Value, &G_Value, &B_Value, &r2_Value, &g2_Value, &b2_Value);
    color = judge_color(&R_Value, &G_Value, &B_Value, &r1_Value, &g1_Value, &b1_Value);
    show_color(color);
    }
  delay(2000);
}

double Distance(double *distance)
{
  double duration=0;
  *distance=0;
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite( TRIG, HIGH );
  delayMicroseconds( 10 ); 
  digitalWrite( TRIG, LOW );
  duration = pulseIn( ECHO, HIGH ); // 往復にかかった時間が返却される[マイクロ秒]
  if (duration > 0) {
    duration = duration / 2; // 往路にかかった時間
    *distance = duration * speed_of_sound * 100 / 1000000;
    //Serial.print("Distance:");
    //Serial.print(*distance);
    //Serial.println(" cm");
  }
  return *distance;
}

void updateValue(int *R_Value, int *G_Value, int *B_Value, int *r2_Value, int *g2_Value, int *b2_Value)
{
  int R1_Value = 0;
  int R2_Value = 0;
  int G1_Value = 0;
  int G2_Value = 0;
  int B1_Value = 0;
  int B2_Value = 0;
  *R_Value = *G_Value = *B_Value = 0;
  int r1[n];
  int r2[n];
  int g1[n];
  int g2[n];
  int b1[n];
  int b2[n];
  // 赤色LEDを点灯
  analogWrite(r_LED, 0);
  analogWrite(g_LED, 1023);
  analogWrite(b_LED, 1023);
  delay(500);
  for(int i = 0; i < n; i++)
  {
    r1[i] = analogRead(SENSOR1);
    r2[i] = analogRead(SENSOR2);
    R1_Value += r1[i];
    R2_Value += r2[i];
  }
  *R_Value = (R1_Value/n + R2_Value/n)/2 - *r2_Value;
  if(*R_Value<0)*R_Value = 0;
  Serial.println(*R_Value);
  delay(100);
  // 緑色LEDを点灯
  analogWrite(r_LED, 1023);
  analogWrite(g_LED, 0);
  delay(500);
  for(int i = 0; i < n; i++)
  {
    g1[i] = analogRead(SENSOR1);
    g2[i] = analogRead(SENSOR2);
    G1_Value += g1[i];
    G2_Value += g2[i];
  }
  *G_Value = (G1_Value/n + G2_Value/n)/2 - *g2_Value;
  if(*G_Value<0)*G_Value = 0;
  Serial.println(*G_Value);
  delay(100);
  // 青色LEDを点灯
  analogWrite(g_LED, 1023);
  analogWrite(b_LED, 0);
  delay(500);
  for(int i = 0; i < n; i++)
  {
    b1[i] = analogRead(SENSOR1);
    b2[i] = analogRead(SENSOR2);
    B1_Value += b1[i];
    B2_Value += b2[i];
  }
  *B_Value = (B1_Value/n + B2_Value/n)/2 - *b2_Value;
  if(*B_Value<0)*B_Value = 0;
  Serial.println(*B_Value);
  analogWrite(b_LED, 1023);
  delay(100);
}
String judge_color(int *R_Value, int *G_Value, int *B_Value, int *r1_Value, int *g1_Value, int *b1_Value)
{
  int R = static_cast<float>(static_cast<float>(*R_Value) / *r1_Value * 255);
  int G = static_cast<float>(static_cast<float>(*G_Value) / *g1_Value * 255);
  int B = static_cast<float>(static_cast<float>(*B_Value) / *b1_Value * 255);
  delay(100);
  if(R>255)R = 255;
  if(G>255)G = 255;
  if(B>255)B = 255;
  Serial.print("(");
  Serial.print(R);
  Serial.print(",");
  Serial.print(G);
  Serial.print(",");
  Serial.print(B);
  Serial.println(")");
  delay(100);
  //0<=R<128
  if(0<=R && R<128){
    if(0<=G && G<128){
      if(0<=B && B<128) return "Black"; // (0,0,0)
      else return "Blue"; // (0,0,255)
    }
    else{
      if(0<=B && B<85) return "Lime"; // (0,255,0)
      else return "Aqua"; // (0,255,255)
    }
  }
  //170<=R
  else{
    if(0<=G && G<128){
      if(0<=B && B<128) return "Red"; // (255,0,0)
      else return "Fuchsia"; // (255,0,255)
    }
    else{
      if(0<=B && B<128) return "Yellow"; // (255,255,0)
      else return "White"; // (255,255,255)
    }
  } 
  delay(100);
}

void show_color(String color){
  lcd.setCursor(0,0);
  lcd.print(color);
  delay(50);
}