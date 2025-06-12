// IM920sLを受信機(子機)として使用する

// 変数宣言
String c;        // 受信データ（文字列）格納用
String command;  // 送信コマンド（文字列）格納用（このスケッチでは主に設定用）
int cnt = 0;     // カウンタ（未使用）

// LED出力ピンの定義
const int PIN_LED_X = 20;   // LED X方向出力用
const int PIN_LED_Y = 18;   // LED Y方向出力用
const int PIN_LED_SW = 17;  // スイッチ表示用LED出力

// IM920にコマンドを送信し、応答を受け取る関数
void sendCommand(String command) {
  Serial1.print(command);       // コマンド送信
  Serial1.print("\r");          // 終端にCRを追加
  Serial.println("Sent: " + command);

  String c = Serial1.readStringUntil('\r');  // 応答を読み込み
  c.trim();                                  // 前後の空白/改行などを除去
  Serial.println("Response: " + c);          // 応答を表示
  Serial.println("");
  delay(500);                                // 通信間隔調整
}

// 初期設定 -------------------------------------------------------------------
void setup() {
  delay(1000);                 // 電源投入時の安定化
  Serial.begin(19200);         // USBシリアル開始
  delay(1000);

  // LEDピンを出力に設定
  pinMode(PIN_LED_X, OUTPUT);
  pinMode(PIN_LED_Y, OUTPUT);
  pinMode(PIN_LED_SW, OUTPUT);

  Serial.println("Operating IM920sL as a receiver.....\n");

  Serial1.begin(19200);        // IM920とのシリアル通信開始
  delay(1000);

  // 受信バッファが残っていればクリア
  while (Serial1.available()) {
    c = Serial1.readStringUntil('\r');
    c.trim();
    Serial.println(c);
  }
  delay(500);

  // IM920の設定確認コマンドを送信
  sendCommand("RDID");  // 固有ID確認
  sendCommand("RDPO");  // 送信出力確認
  sendCommand("RDRT");  // 無線通信モード確認
  sendCommand("RDCH");  // 通信チャンネル確認
  sendCommand("RDNN");  // ノード番号確認（子機 = 0002）
  sendCommand("RDGN");  // グループ番号確認
  sendCommand("ECIO");  // キャラクタ入力有効化
}


// メイン処理（繰り返し）-------------------------------------------------------
void loop() {

  // 受信データが12バイト以上ある場合に処理開始
  if (Serial1.available() >= 12) {
    String buf = Serial1.readStringUntil('\r');  // \rまでを1行として読み取る

    // 親機（0001）からのデータか確認
    if (buf.substring(4, 8) == "0001") {
      
      Serial.println(buf);  // 受信した生データを表示

      // 受信データから16進の各値を抽出（TXDAのデータ部）
      String hexStringValueX  = buf.substring(13, 15);  // X軸（2文字）
      String hexStringValueY  = buf.substring(15, 17);  // Y軸（2文字）
      String hexStringValueSW = buf.substring(17, 19);  // SW（2文字）

      // 抽出結果を確認出力
      Serial.println(hexStringValueX + " " + hexStringValueY + " " + hexStringValueSW);

      // 16進 → 10進 に変換
      int valueX = strtol(hexStringValueX.c_str(), NULL, 16);
      int valueY = strtol(hexStringValueY.c_str(), NULL, 16);

      // 受信値をPWM出力値（0〜255）にマッピング
      valueX = map(valueX, 50, 200, 0, 255);  // 最小50, 最大200としてスケーリング
      valueY = map(valueY, 50, 200, 0, 255);

      // LEDにPWM出力（明るさ制御）
      // analogWrite(PIN_LED_X, valueX);
      // analogWrite(PIN_LED_Y, valueY);

      // スイッチの状態に応じてLEDをON/OFF
      if (hexStringValueSW == "01") {
        digitalWrite(PIN_LED_SW, HIGH);  // スイッチが押されたとき
        digitalWrite(PIN_LED_X, HIGH);
        digitalWrite(PIN_LED_Y, HIGH);
      } else {
        digitalWrite(PIN_LED_SW, LOW);   // スイッチが離されたとき
        digitalWrite(PIN_LED_X, LOW);
        digitalWrite(PIN_LED_Y, LOW);
      }
    }
  }
}
