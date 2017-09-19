#include <Adafruit_NeoPixel.h>

// 建立燈條物件，參數為：共 15顆LED，資料線接在 Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, 6);

const byte buttonPin = 3;       // 按鈕接在 Pin 3
int buttonState = LOW;          // 儲存按鈕的狀態
unsigned long lastTime = 0;     // 儲存中斷觸發的時間
int id = 0;                     // 儲存 Serial 的回傳值

int gFunction[3][4] = {0};

void setup() {
  pinMode(buttonPin, INPUT);    // 將按鈕腳位設為數位輸入腳位
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  Serial.begin(115200);         // 初始化 Serial 物件並設定 115200bps
  
  // 指定 1 號中斷發生 RISING (電位由低變高) 時，
  // 呼叫自訂的函式 myISR
  attachInterrupt(1, myISR, RISING);

  strip.begin();                // 將燈條物件初始化，並設為全暗
  strip.setBrightness(32);      // 將整體亮度降為 1/8
  strip.show();                 // 讓燈條顯示出所設定的狀態
}

void loop() {
  // 若偵測到 Serial 回傳值，儲存到 id
  if (Serial.available()) {
    id = Serial.read();
  
    // 依照 id 值來判斷應該執行哪一個特效
    switch (id) {
      // Serial 回傳 0，代表向 Serial 傳送指令執行成功
      case 0: 
        showAll(0, 255, 0);  // 點亮所有 LED 的綠燈
        break;
      // Serial 回傳正整數，代表辨識到的指令 ID
      case 1:
        showAll(0, 0, 255);
        editFunction(gFunction);
        break;
      case 3:
        showAll(0, 0, 0);
        doFunction(gFunction[0]);
        break;
      case 4:
        showAll(0, 0, 0);
        doFunction(gFunction[1]);
        break;
      case 5:
        showAll(0, 0, 0);
        doFunction(gFunction[2]);
        break;
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
      // 向 Serial 傳送指令，開始錄音
      Serial.println("CYB6");
    }
    else
    {
      buttonState = LOW;
      // 向 Serial 傳送指令，停止錄音
      Serial.println("CYB2");
    }
    
    lastTime = millis();  // 記錄本次中斷的時間
  }
}

void editFunction(int function[3][4]) {
  int editing = 1;
  int cmd = -1;
  while(editing) {
    if (Serial.available()) {
      id = Serial.read();
      if ((id != 2 && id != 3 && id != 4 && id != 5) && (cmd != 0 && cmd != 1 && cmd != 2))
        continue;
      switch (id) {
        case 2:
          editing = 0;
          showCurrentCmd(-1);
          showAll(0, 0, 0);
          break;
        case 3:
          cmd = 0;
          showCurrentCmd(0);
          doFunction(function[0]);
          break;
        case 4:
          cmd = 1;
          showCurrentCmd(1);
          doFunction(function[1]);
          break;
        case 5:
          cmd = 2;
          showCurrentCmd(2);
          doFunction(function[2]);
          break;
        case 6:
          if (function[cmd][0] == 0) {
            function[cmd][0] = 1;
            strip.setPixelColor(0, 255, 255, 255);
          }
          else  {
            function[cmd][0] = 0;
            strip.setPixelColor(0, 0, 0, 0);
          }
          strip.show();
          break;
        case 7:
          if (function[cmd][1] == 0) {
            function[cmd][1] = 1;
            strip.setPixelColor(1, 255, 255, 255);
          }
          else  {
            function[cmd][1] = 0;
            strip.setPixelColor(1, 0, 0, 0);
          }
          strip.show();
          break;
        case 8:
          if (function[cmd][2] == 0) {
            function[cmd][2] = 1;
            strip.setPixelColor(2, 255, 255, 255);
          }
          else  {
            function[cmd][2] = 0;
            strip.setPixelColor(2, 0, 0, 0);
          }
          strip.show();
          break;
        case 9:
          if (function[cmd][3] == 0) {
            function[cmd][3] = 1;
            strip.setPixelColor(3, 255, 255, 255);
          }
          else  {
            function[cmd][3] = 0;
            strip.setPixelColor(3, 0, 0, 0);
          }
          strip.show();
          break;
        default: 
          break;
      }
    }
  }
}

void doFunction(int function[4]) {
  for (int i = 0; i < 4; i++) {
    if (function[i] == 1)
      strip.setPixelColor(i, 255, 255, 255);
    else 
      strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();    // 讓燈條顯示出來
}

void showCurrentCmd(int cmd) {
  if (cmd == 0) {
    digitalWrite(8, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
  else if (cmd == 1) {
    digitalWrite(8, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  }
  else if (cmd == 2) {
    digitalWrite(8, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}
