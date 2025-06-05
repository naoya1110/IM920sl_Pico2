// IM920sLを受信機(子機)として使用する

// 変数宣言
String c;        // 受信データ（文字）格納用
String command;  //送信コマンド（文字）格納用
int cnt = 0;

// コマンド送受信関数
void sendCommand(String command) {
  Serial1.print(command);  // コマンドを送信
  Serial1.print("\r");
  Serial.println("Sent: " + command);

  String c = Serial1.readStringUntil('\r');  // 応答を取得（\rまで）
  c.trim();                                  // \nや空白を除去

  Serial.println("Response: " + c);  // 応答を表示
  Serial.println("");
  delay(500);  // 通信間隔のための待機
}

// 初期設定 -------------------------------------------------------------------
void setup() {
  delay(1000);
  Serial.begin(19200);
  delay(1000);

  Serial.println("Operating IM920sL as a receiver.....\n");

  Serial1.begin(19200);
  delay(1000);
  while (Serial1.available()) {
    c = Serial1.readStringUntil('\r');
    c.trim();
    Serial.println(c);
  }
  delay(500);

  sendCommand("RDID");  // 固有ID確認
  sendCommand("RDPO");  // 送信出力確認 2 = 10 mW
  sendCommand("RDRT");  // 無線通信モード確認 1 = 短距離
  sendCommand("RDCH");  // 無線通信チャンネル確認
  sendCommand("RDNN");  // ノード番号確認 0002 = 子機
  sendCommand("RDGN");  // グループ番号確認
  sendCommand("ECIO");  // キャラクタ入力有効
}


// メイン処理（繰り返し）-------------------------------------------------------
void loop() {

  if (Serial1.available() >= 12) {               //1つ以上データが届いていたら
    String buf = Serial1.readStringUntil('\r');  //改行コードまで読み込み
    
    if (buf.substring(4, 8) == "0001") {         //親機からの受信であれば
      
      Serial.print(buf);
    }
  }
}