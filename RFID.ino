//Code made by TJ/@ubla_coder(GitHub)
//Copyright Protected  by MIT License(© gitHub Inc.2024)
//Date - 3/2/2024
//Check Readme.txt for more information on pinout
//Hardware version-esp32-v6-37496
//build:1.0.16 Build 20230828 rel.44667
//Author:          Tanmay Joshi <tj_coder12@github.io>
// Available from:  https://github.com/ubla_coder/rfid
//License:         GNU GPLv3(Issued by MIT)

#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

int code[] = {99, 117, 63, 46}; // This is the stored UID
int codeRead = 0;
String uidString;
boolean locked = true; // Initial state is locked
int rled = 8;
int bled = 7;
int led = 6;
int rl = 5;
void setup() {
  Serial.begin(9600);
  pinMode(bled,OUTPUT);
  pinMode(rled,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(rl,OUTPUT);
  digitalWrite(bled,LOW);
  digitalWrite(rled,HIGH);
  digitalWrite(led,HIGH);
  digitalWrite(rl,HIGH);
  SPI.begin();              // Init SPI bus
  rfid.PCD_Init();          // Init MFRC522

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2); // Text size 2 for "RFID Lock"
  display.setCursor(10, 0); // Move cursor to first line
  display.print("TJ Lights");
  display.setTextSize(1); // Text size 1 for "Tap to unlock"
  display.setCursor(10, 20); // Move cursor to second line
  display.print("  Tap to turn on");
  display.display();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    readRFID();
  }
  delay(100);
}

void readRFID() {
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the PICC is of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  clearDisplay();

  Serial.println("Scanned PICC's UID:");
  printDec(rfid.uid.uidByte, rfid.uid.size);

  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);

  if (locked) {
    delay(1);
  } else {
    printMessage("Tap to lock"); // Print "Tap to lock" if unlocked
  }

  int i = 0;
  boolean match = true;
  while (i < rfid.uid.size) {
    if (!(rfid.uid.uidByte[i] == code[i])) {
      match = false;
    }
    i++;
  }

  if (match) {
    Serial.println("\nI know this card!");
    if (locked) {
      locked = false; // Unlock the system
      printUnlockMessage();
    } else {
      locked = true; // Lock the system again
      printLockMessage();
    }
  } else {
    Serial.println("\nUnknown Card");
    display.clearDisplay();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(2); // Text size 2 for "RFID Lock"
    display.setCursor(10, 0); // Move cursor to first line
    display.print("INVALID!");
    display.setTextSize(1); // Text size 1 for "Tap to lock"
    display.setCursor(10, 20); // Move cursor to second line
    display.print("Access Denied!");
    display.display();
    digitalWrite(bled,HIGH);
    digitalWrite(rled,LOW);
    delay(1500);
    digitalWrite(rled,HIGH);
    delay(1500);
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(2); // Text size 2 for "RFID Lock"
    display.setCursor(10, 0); // Move cursor to first line
    display.print("TJs Light");
    display.setTextSize(1); // Text size 1 for "Tap to unlock"
    display.setCursor(10, 20); // Move cursor to second line
    display.print("  Tap to turn on");
    display.display();
    digitalWrite(rled,HIGH);
    digitalWrite(bled,LOW);
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void clearDisplay() {
  display.clearDisplay();
}

void printMessage(String message) {
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.print(message);
  display.display();
}

void printUnlockMessage() {
  digitalWrite(bled,HIGH);
  display.clearDisplay();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2); // Text size 2 for "RFID Lock"
  display.setCursor(10, 0); // Move cursor to first line
  display.print("Welcome!");
  display.display();
  delay(1500);
  display.setTextColor(BLACK); // or BLACK);
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print("Welcome!");
  display.display();
  display.clearDisplay();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print(" Turning  Clock  on");
  display.display();
  delay(1500);
  digitalWrite(rl,LOW);
  delay(1500);
  display.clearDisplay();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print(" Turning  Lights on");
  display.display();
  delay(2750);
  digitalWrite(led,LOW);
  delay(250);
  digitalWrite(led,HIGH);
  delay(1500);
  display.clearDisplay();  
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print("Tap Again");
  display.setTextSize(1); // Text size 1 for "Tap to lock"
  display.setCursor(10, 20); // Move cursor to second line
  display.print("to turn off lights");
  display.display();
  delay(10000);
  display.clearDisplay();
  display.setTextSize(2); // Text size 1 for "Tap to lock"
  display.setCursor(10, 0); // Move cursor to second line
  display.print("TJ Lights");
  display.setTextSize(1); // Text size 1 for "Tap to lock"
  display.setCursor(10, 20); // Move cursor to second line
  display.print("Tap to turn off");
  display.display();
  }

void printLockMessage() {
  digitalWrite(bled,LOW);
  display.clearDisplay();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2); // Text size 2 for "RFID Lock"
  display.setCursor(10, 0); // Move cursor to first line
  display.print("Turning  Lights off");
  display.display();
  delay(1500);
  digitalWrite(led,LOW);
  delay(1500);
  digitalWrite(led,HIGH);
  display.clearDisplay();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2); // Text size 2 for "RFID Lock"
  display.setCursor(10, 0); // Move cursor to first line
  display.print("Turning  Clock off");
  display.display();
  delay(750);
  digitalWrite(rl,HIGH);
  delay(1750);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2); // Text size 2 for "RFID Lock"
  display.setCursor(10, 0); // Move cursor to first line
  display.print("TJs Light");
  display.setTextSize(1); // Text size 1 for "Tap to unlock"
  display.setCursor(10, 20); // Move cursor to second line
  display.print("Tap to turn on");
  display.display();
}
