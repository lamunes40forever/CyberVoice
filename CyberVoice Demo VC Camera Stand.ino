#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

const byte buttonPin = 3;       // 按鈕接在 Pin 3
unsigned long lastTime = 0;     // 儲存中斷觸發的時間
int buttonState = LOW;          // 儲存按鈕的狀態
int id = 0;                     // 儲存 Serial 的回傳值
int pos = 90;                   // 一開始先置中 90 度

void setup() {
  pinMode(buttonPin, INPUT);    // 將按鈕腳位設為數位輸入腳位
  myservo.attach(9);            // Servo PWM 輸入接在 Pin 9
  Serial.begin(115200);          // 初始化 Serial 物件並設定 115200bps
  
  // 指定 1 號中斷發生 RISING (電位由低變高) 時，
  // 呼叫自訂的函式 myISR
  attachInterrupt(1, myISR, RISING);
}

void loop() {
  // 若偵測到 Serial 回傳值，儲存到 id
  if (Serial.available()) {
    id = Serial.read();

    // 依照 id 值來判斷應該執行哪一個特效
    switch (id) {
      // Serial 回傳 0，代表向 Serial 傳送指令執行成功
      case 0: 
        break;
      // Serial 回傳正整數，代表辨識到的指令 ID
      case 1:
        TurnAround("LEFT", 10);        // 向左轉 5 度
        break;
      case 2:
        TurnAround("RIGHT", 10);       // 向右轉 5 度
        break;
      case 3:
        TurnAround("LEFT", 30);       // 向左轉 10 度
        break;
      case 4:
        TurnAround("RIGHT", 30);      // 向右轉 10 度
        break;
      case 5:
        TurnAround("LEFT", 180);       // 向左轉到底
        break;
      case 6:
        TurnAround("RIGHT", 180);      // 向右轉到底
        break;
      // Serial 回傳負數，代表向 Serial 傳送指令執行失敗
      default: 
        break;
    }
  }
}


//  自訂的中斷處理函式
void myISR() {
  // 不同的按壓開關硬體，可能需使用不同的時間間隔來消除彈跳，
  // 若測試按鈕時發現異常，請嘗試調整時間間隔
  if (millis() - lastTime > 200) { // 距上次中斷已超過一段時間
    if (buttonState == LOW)
    {
      buttonState = HIGH;
      // 向 Serial 傳送指令: 開始錄音
      Serial.println("CYB6");
    }
    else
    {
      buttonState = LOW;
      // 向 Serial 傳送指令: 停止錄音
      Serial.println("CYB2");
    }
    lastTime = millis();  // 記錄本次中斷的時間
  }
}

void TurnAround(String direct, int angle) {
  if (direct == "LEFT")
    pos += angle;
  else if (direct == "RIGHT")
    pos -= angle;

  if (pos > 180)
    pos = 180;
  else if (pos < 0)
    pos = 0;
  
  myservo.write(pos);       // 通知 servo 轉到 'pos' 度
}

