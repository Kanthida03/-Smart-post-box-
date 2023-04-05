#include <WString.h> //เรียกใช้library Wire
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //เรียกใช้library LCD12c
#include <SPI.h> //เรียกใช้library SPI
#include <MFRC522.h>
#include <Servo.h> //เรียกใช้library Servo

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo; //ชื่อเซอร์โว

int x = 0;
int input = A0;
int state = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); //หน้าจอ

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(8);
  myServo.write(0);
  Serial.println("Put your card...");
  Serial.println();

  
  pinMode(3, OUTPUT); //ลำโพง
  //หน้าจอ
  lcd.clear();
  lcd.begin();
  lcd.backlight();
  lcd.noCursor(); //ปิดCursor
  lcd.noBlink(); //ปิดBlink
  lcd.setCursor(0, 0);
  lcd.print("     NUMBER  ");
  lcd.setCursor(7, 1);
  x = 0;
  lcd.print(x);
}

void loop() {
  //เสียง
  int counter = digitalRead(A0);
  if(digitalRead(4) == HIGH){ //ถ้ากดปุ่ม
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("     NUMBER  ");
    lcd.setCursor(7, 1);
    x = 0;
    lcd.print(x);
  }
  if (state == 0) //ถ้าตรวจจับ
  {
    switch (counter) {
      case 1 : state = 1; noTone(3); break;
      case 0 : state = 0; lcd.setCursor(7, 1); x = x+1; lcd.print(x); tone(3, 500, 100); delay(300); tone(3, 430, 100); delay(150); tone(3, 500, 100); delay(100); tone(3, 570, 100); delay(220); break;
    }
  }
  if (counter == LOW) { // ไม่ได้ตรวจจับวัตถุอะไร
    state = 0;
  }

  //คีย์การ์ด
  if (! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.print("UID tag : ");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.println("Message : HELLO PU & AOM");
  content.toUpperCase();
  if(content.substring(1) == "CD 10 06 31") //ถ้าคีย์การ์ดถูก
  {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print(" ALLOW");
    lcd.setCursor(5, 1);
    lcd.print("ACCESS");
    delay(500);
    Serial.println("Allow access");
    Serial.println();
    delay(500);
    tone(3, 660, 100);
    delay(150);
    tone(3, 660, 100);
    delay(300);
    tone(3, 660, 100);
    delay(300);
    tone(3, 510, 100);
    delay(100);
    tone(3, 660, 100);
    delay(300);
    tone(3, 770, 100);
    delay(550);
    tone(3, 380, 100);
    delay(575);

    tone(3, 650, 100);
    delay(300);
    tone(3, 380, 100);
    delay(150);
    tone(3, 430, 100);
    delay(150);

    tone(3, 500, 100);
    delay(300);
    tone(3, 430, 100);
    delay(150);
    tone(3, 500, 100);
    delay(100);
    tone(3, 570, 100);
    delay(220);
    
    myServo.write(270); //เซอร์โวหมุน
    delay(10000); //ดีเลย์10วินาที
    myServo.write(0);

    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("PRESS THE BOTTON");
    lcd.setCursor(4, 1);
    lcd.print("TO RESET");
  }
  else{ //ถ้าคีย์การ์ดไม่ถูกต้อง
    tone(3, 440);
    delay(200); 
    noTone(3);
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print("ACCESS");
    lcd.setCursor(5, 1);
    lcd.print("DENIED");
    delay(2000);
    Serial.println("Access denied");

    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("     NUMBER  ");
    lcd.setCursor(7, 1);
    lcd.print(x);    
  }
}
