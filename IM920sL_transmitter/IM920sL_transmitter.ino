// IM920sLを送信機(親機)として使用する

// 変数宣言
String c;        // 受信データ（文字列）格納用
String command;  // 送信コマンド（文字列）格納用
int cnt = 0;     // カウンタ（未使用）

// ピン定義
const int PIN_ADC0 = 26;  // GPIO26：アナログ入力0（ジョイスティックYなど）
const int PIN_ADC1 = 27;  // GPIO27：アナログ入力1（ジョイスティックXなど）
const int PIN_SW = 22;    // GPIO22：スイッチ入力（ボタン）
const int LED_PIN = 25;

// IM920にコマンドを送信し、応答を表示する関数
void sendCommand(String command) {
  Serial1.print(command);              // コマンド送信
  Serial1.print("\r");                 // CR（キャリッジリターン）で終了
  Serial.println("Sent: " + command);  // シリアルモニタに送信内容を表示

  String c = Serial1.readStringUntil('\r');  // 応答を1行（\rまで）読み取る
  c.trim();                                  // 不要な改行や空白を削除
  Serial.println("Response: " + c);          // 応答を表示
  Serial.println("");
  delay(100);  // 通信の間隔を空ける
}

// 初期設定 -------------------------------------------------------------------
void setup() {
  delay(1000);          // 起動安定のための待機
  Serial.begin(19200);  // PCとのシリアル通信開始
  delay(1000);

  pinMode(PIN_SW, INPUT);        // スイッチピンを入力に設定
  pinMode(LED_BUILTIN, OUTPUT);  // LEDピンを出力に設定（PIN_LEDは別で定義が必要）

  Serial.println("Operating IM920sL as a transmitter.....\n");

  Serial1.begin(19200);  // IM920とのシリアル通信開始
  delay(1000);

  // IM920からの初期応答をすべて読み出して表示
  while (Serial1.available()) {
    c = Serial1.readStringUntil('\r');
    c.trim();
    Serial.println(c);
  }
  delay(500);

  // IM920の初期設定確認コマンドを送信
  sendCommand("RDID");  // 固有ID確認
  sendCommand("RDPO");  // 出力確認（例：2 = 10 mW）
  sendCommand("RDRT");  // 通信モード確認（例：1 = 短距離）
  sendCommand("RDCH");  // 通信チャンネル確認
  sendCommand("RDNN");  // ノード番号（親機 = 0001）
  sendCommand("RDGN");  // グループ番号確認
  sendCommand("ECIO");  // キャラクタ入力有効化
}


// メイン処理（繰り返し）-------------------------------------------------------
void loop() {
  // アナログ入力から値を取得
  int valueX = analogRead(PIN_ADC1);  // X方向
  int valueY = analogRead(PIN_ADC0);  // Y方向
  int valueSW = digitalRead(PIN_SW);  // ボタンの状態（0 or 1）

  // ADC値（0〜1023）を8ビット（0〜255）にマッピング
  valueX = map(valueX, 0, 1023, 0, 255);
  valueY = map(valueY, 0, 1023, 0, 255);

  // それぞれ16進文字列に変換
  String hexStringValueX = String(valueX, HEX);
  String hexStringValueY = String(valueY, HEX);
  String hexStringValueSW = String(valueSW, HEX);  // スイッチは 0 または 1

  // ※デバッグ用（コメントアウト中）
  // Serial.print(valueX); Serial.print(" ");
  // Serial.print(hexStringValueX); Serial.print(" ");
  // Serial.print(valueY); Serial.print(" ");
  // Serial.print(hexStringValueY); Serial.print(" ");
  // Serial.print(valueSW); Serial.print(" ");
  // Serial.println("0" + hexStringValueSW);

  // コマンド構成：TXDA XXYY0Z の形式で送信（X, Y はジョイスティック、Z はスイッチ）
  command = "TXDA " + hexStringValueX + hexStringValueY + "0" + hexStringValueSW;

  // IM920にコマンドを送信
  sendCommand(command);

  // 受信データが12バイト以上ある場合に処理開始
  if (Serial1.available() >= 12) {
    String buf = Serial1.readStringUntil('\r');  // \rまでを1行として読み取る

    // 親機（0001）からのデータか確認
    if (buf.substring(4, 8) == "0002") {
      Serial.println(buf);  // 受信した生データを表示
      // 受信データから16進の各値を抽出（TXDAのデータ部）
      String receivedHexStringValueSW = buf.substring(13, 15);  // SW（2文字）

      // 通信の確認（スイッチの状態に応じてLEDをON/OFF）
      if (receivedHexStringValueSW == "01") {
        digitalWrite(LED_BUILTIN, HIGH);  // スイッチが押されたとき
      } else {
        digitalWrite(LED_BUILTIN, LOW);  // スイッチが押されたとき
      }
    }
  }
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}