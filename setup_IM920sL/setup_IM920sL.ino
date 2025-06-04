// 変数宣言
String c;  // 受信データ（文字）格納用

// 初期設定 -------------------------------------------------------------------
void setup() {
  Serial1.begin(19200);
  delay(1000);
  while (Serial1.available()) {
    c = Serial1.readStringUntil('\r');
    Serial.print(c);
  }
  delay(500);

  // 無線通信モードを3(長距離)に設定
  Serial1.print("STRT 3\r");
  c = Serial1.readStringUntil('\r');
  Serial.print(c);
  delay(500);
}


// メイン処理（繰り返し）-------------------------------------------------------
void loop() {
  Serial.println("");

  // 固有IDの確認
  Serial1.print("RDID");
  Serial1.print("\r");
  c = Serial1.readStringUntil('\r');
  c.trim();
  Serial.print("Device ID: RDID = ");
  Serial.println(c);

  // 送信出力
  Serial1.print("RDPO");
  Serial1.print("\r");
  c = Serial1.readStringUntil('\r');
  c.trim();
  Serial.print("Transmission Power: RDPO = ");
  Serial.println(c);

  // 無線通信モード
  Serial1.print("RDRT");
  Serial1.print("\r");
  c = Serial1.readStringUntil('\r');
  c.trim();
  Serial.print("Communication Mode: RDRT = ");
  Serial.println(c);

  // 無線通信チャンネル
  Serial1.print("RDGN");
  Serial1.print("\r");
  c = Serial1.readStringUntil('\r');
  c.trim();
  Serial.print("Group: RDGN = ");
  Serial.println(c);

  // 無線通信チャンネル
  Serial1.print("RDCH");
  Serial1.print("\r");
  c = Serial1.readStringUntil('\r');
  c.trim();
  Serial.print("Channel: RDCH = ");
  Serial.println(c);

  delay(1000);
}