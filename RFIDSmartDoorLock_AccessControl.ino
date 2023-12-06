#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "time.h"            
#include "addons/TokenHelper.h"                                 
#include "addons/RTDBHelper.h"

#define SCK_PIN 13   
#define MOSI_PIN 15  
#define MISO_PIN 2   
#define RST_PIN 14   
#define SS_PIN 12    

#define SERVO_PIN 4  
#define GREEN_LED_PIN 22   
#define RED_LED_PIN 23    
#define BUZZER_PIN 21    

MFRC522 mfrc522(SS_PIN, RST_PIN); 
Servo myServo;                

const char* ntpServer = "pool.ntp.org"; // request the time from pool.ntp.org
const long gmtOffset_sec = -18000;           // Change the time gmtOffset_sec variable to match your time zone
const int daylightOffset_sec = 0;    // defines the offset in seconds for daylight saving time (seconds)

// rmb to censor this!
#define API_KEY "AIzaSyAqmwOKmyoXf7j96Gn1mbUORkPuumv8HHE"
#define PROJECT_ID "esp32-fb-147df"
#define DATABASE_URL "https://esp32-fb-147df-default-rtdb.firebaseio.com/"
#define USER_EMAIL "rfidsmartdoor@gmail.com"
#define USER_PASSWORD "RFIDSmartDoorLock"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN); 
  mfrc522.PCD_Init();

  myServo.attach(SERVO_PIN);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin("test", "freewifi434436438");
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);


  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
}

void loop() {
  Serial.println("check for card");
  if (Firebase.ready() && mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("firebase ready and card found");
    String rfidUid = getRFIDUid();
    Serial.println(rfidUid);

    // checking if the user RFID is registered
    if (isRegisteredUser(rfidUid)) {
      // Successful Entry
      Serial.println("registered card found!");
      successfulEntryActions();
      logTimestamp("successful", rfidUid);
    } else {
      // Unsuccessful Entry
      Serial.println("UNregistered card found!");
      unsuccessfulEntryActions();
      logTimestamp("unsuccessful", rfidUid);
    }
    //mfrc522.PICC_HaltA();
  }

  if (remote_control()) {
    // Remote Servo Control Entry
    Serial.println("remote control activated");
    successfulEntryActions();
    logTimestamp("remote", "REMOTE");
    setRemoteControlFalse();
  }
}

String getRFIDUid() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "%20");
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  return uid;
}

bool isRegisteredUser(String rfidUid) {
  Serial.println("isRegisteredUser() enter");
  String userPath = "/users/" + rfidUid;
  Firebase.RTDB.getBool(&firebaseData, userPath);
  if (firebaseData.boolData()) {
    Serial.println("Successful READ from " + firebaseData.dataPath() +  ": " + firebaseData.boolData() + "( " + firebaseData.dataType() + ")" );
    return true;
  }
  else {
    Serial.println(firebaseData.errorReason());
  }
  Serial.println("Unsuccessful READ of RFID user!");
  return false;
}

void successfulEntryActions() {
  Serial.println("successfulEntryActions() enter");
  myServo.write(0); 
  delay(2000);
  myServo.write(90);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);
  Serial.println("successfulEntryActions() exit");
}

void unsuccessfulEntryActions() {
  Serial.println("unsuccessfulEntryActions() enter");
  Serial.println("Door is close");
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(2000);
  digitalWrite(RED_LED_PIN, LOW); 
  digitalWrite(BUZZER_PIN, LOW); 
  Serial.println("unsuccessfulEntryActions() exit");
}

void logTimestamp(String entryType, String rfidUid) {
  Serial.println("logTimestamp() enter");
  String timestampPath = "/timestamps/" + entryType + "/" + printLocalTime();
  Firebase.RTDB.setString(&firebaseData, timestampPath, "Entry by: " + rfidUid + " at " + printLocalTime());
  Serial.println("logTimestamp() exit");
}

bool remote_control(){
  Serial.println("remote_control() enter");
  String remotePath = "/remoteControl";
  Firebase.RTDB.getBool(&firebaseData, remotePath);
  if (firebaseData.boolData()) {
    Serial.println("remote_control() exit1");
    return true;
  }
  else {
      
  }
  Serial.println("remote_control() exit2");
  return false;
}

void setRemoteControlFalse(){
  Serial.println("setRemoteControlFalse() enter");
  String remotePath = "/remoteControl";
  Firebase.RTDB.setBool(&firebaseData, remotePath, false);
  Serial.println("setRemoteControlFalse() exit");
}

String printLocalTime(){
  Serial.println("printLocalTime() enter");

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "NO TIME";
  }

  char timestamp[100];
  strftime(timestamp, 100, "%A, %B %d %Y %H:%M:%S", &timeinfo);
  Serial.println(timestamp);
  Serial.println("printLocalTime() exit");
  return (timestamp);
}