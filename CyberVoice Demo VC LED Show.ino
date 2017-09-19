// 程式參考來源《FLAG'S 創客‧自造者工作坊 #2 LED 酷炫燈舞秀 15+ 特效》
// 請先安裝 Adafruit NeoPixel 函式庫
#include <Adafruit_NeoPixel.h>

// 建立燈條(燈環)物件，參數為：共16顆LED，資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

const byte buttonPin = 3;       // 按鈕接在 Pin 3
int buttonState = LOW;          // 儲存按鈕的狀態
unsigned long lastTime = 0;     // 儲存中斷觸發的時間
int id = 0;                     // 儲存 Serial 的回傳值

void setup() {
  pinMode(buttonPin, INPUT);    // 將按鈕腳位設為數位輸入腳位
  
  Serial.begin(115200);         // 初始化 Serial 物件並設定 115200bps
  
  // 指定 1 號中斷發生 RISING (電位由低變高) 時，
  // 呼叫自訂的函式 myISR
  attachInterrupt(1, myISR, RISING);

  strip.begin();                // 將燈條物件初始化，並設為全暗
  strip.setBrightness(32);      // 將整體亮度降為 1/8
  strip.show();                 // 讓燈條顯示出所設定的狀態
}

void loop() {
  byte rr=255, gg=255, bb=255;  // 設定特效要使用的顏色

  // 若偵測到 Serial 回傳值，儲存到 id
  if (Serial.available())
    id = Serial.read();

  // 依照 id 值來判斷應該執行哪一個特效
  switch (id) {
    // Serial 回傳 0，代表向 Serial 傳送指令執行成功
    case 0: 
      showAll(0, 255, 0);  // 點亮所有 LED 的綠燈
      break;
    // Serial 回傳正整數，代表辨識到的指令 ID
    case 1:
      rgbLoop();  // 漸變循環
      break;
    case 2:
      theaterChase(strip.Color(rr, gg, bb), 50);  // 劇院追逐
      break;
    case 3:
      rainbow(20);  // 彩虹
      break;
    case 4:
      rainbowCycle(1);  // 彩虹圓圈
      break;
    case 5:
      strobe(rr, gg, bb, 10, 50, 1000);  // 閃光
      break;
    case 6:
      runningLights(rr, gg, bb, 80);  // 流光
      break;
    case 7:
      cylonBounce(rr, gg, bb, 2, 70, 70);  // 賽隆人
      break;
    case 8:
      sparkle(rr, gg, bb, 0);  // 發泡
      break;
    // Serial 回傳負數，代表向 Serial 傳送指令執行失敗
    default: 
      showAll(255, 0, 0);  // 點亮所有 LED 的綠燈
      break;
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

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}

// 漸變循環特效
void rgbLoop() {
  for (int j = 0; j < 3; j++ ) {
    // Fade IN 漸強
    for (int k = 0; k < 256; k++) {
      switch (j) {
        case 0: showAll(k, 0, 0); break;
        case 1: showAll(0, k, 0); break;
        case 2: showAll(0, 0, k); break;
      }
      delay(5);
      if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
    }

    // Fade OUT 漸弱
    for (int k = 255; k >= 0; k--) {
      switch (j) {
        case 0: showAll(k, 0, 0); break;
        case 1: showAll(0, k, 0); break;
        case 2: showAll(0, 0, k); break;
      }
      delay(5);
      if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
    }
  }
}

// 劇院追逐特效
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) {
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);
      }
      strip.show();
      delay(wait);
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);
      }
      if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
    }
  }
}

// 彩虹特效
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
  }
}

// 彩虹圓圈特效
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 
                  Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
  }
}

// 閃光特效
void strobe(byte red, byte green, byte blue, 
              int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    showAll(red, green, blue);
    delay(FlashDelay);
    showAll(0, 0, 0);
    delay(FlashDelay);
  }
  delay(EndPause);
}

// 流光特效
void runningLights(byte red, byte green, byte blue, int WaveDelay) {
  int pos;
  
  while (true) {
    pos = 0;
    for (int i = 0; i < strip.numPixels() * 2; i++)
    {
      pos++; // = 0; //pos + Rate;
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, ((sin(i + pos) * 127 + 128) / 255)*red,
                             ((sin(i + pos) * 127 + 128) / 255)*green,
                              ((sin(i + pos) * 127 + 128) / 255)*blue);
      }
      strip.show();
      delay(WaveDelay);
      if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
    }
  }
}

// 賽隆人特效
void cylonBounce(byte red, byte green, byte blue, 
                  int EyeSize, int SpeedDelay, int ReturnDelay) {
  while (true) {
    for (int i = 0; i < strip.numPixels() - EyeSize - 2; i++) {
      showAll(0, 0, 0);
      strip.setPixelColor(i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++) {
        strip.setPixelColor(i + j, red, green, blue);
      }
      strip.setPixelColor(i + EyeSize + 1, 
                            red / 10, green / 10, blue / 10);
      strip.show();
      delay(SpeedDelay);
    }
    delay(ReturnDelay);

    for (int i = strip.numPixels() - EyeSize - 2; i > 0; i--) {
      showAll(0, 0, 0);
      for (int j = 1; j <= EyeSize; j++) {
        strip.setPixelColor(i + j, red, green, blue);
      }
      strip.setPixelColor(i + EyeSize + 1, 
                            red / 10, green / 10, blue / 10);
      strip.show();
      delay(SpeedDelay);
    }
    delay(ReturnDelay);
    if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
  }
}

// 發泡特效
void sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  while (true) {
    int px = random(strip.numPixels());
    strip.setPixelColor(px, red, green, blue); strip.show();
    delay(SpeedDelay);
    strip.setPixelColor(px, 0, 0, 0); strip.show();

    if (Serial.available()) return;  // 若偵測到 Serial 回傳值則停止目前特效
  }
}

// 有關彩虹的特效中, 產生漸變顏色值的函式
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

