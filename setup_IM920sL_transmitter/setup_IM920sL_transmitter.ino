// IM920sLを送信機(親機)として設定する


// 変数宣言
String c;  // 受信データ（文字）格納用
String command; //送信コマンド（文字）格納用

// コマンド送受信関数
void sendCommand(String command) {
  Serial1.print(command);             // コマンドを送信
  Serial1.print("\r");
  Serial.println("Sent: " + command);

  String c = Serial1.readStringUntil('\r');  // 応答を取得（\rまで）
  c.trim();                                  // \nや空白を除去

  Serial.println("Response: " + c);  // 応答を表示
  Serial.println("");
  delay(500);                        // 通信間隔のための待機
}

// 初期設定 -------------------------------------------------------------------
void setup() {
  delay(2000);
  Serial.println("Setting up IM920sL as a transmitter.....\n");

  
  Serial1.begin(19200);
  delay(1000);
  while (Serial1.available()) {
    c = Serial1.readStringUntil('\r');
    c.trim();
    Serial.println(c);
  }
  delay(500);

  // 各種設定
  sendCommand("SRST"); // ソフトウェアリセット
  sendCommand("RDID"); // 固有ID確認
  sendCommand("ENWR"); // 書き込み許可

  sendCommand("STPO 2");  // 送信出力を2(10mW)に設定
  sendCommand("RDPO");    // 確認
  
  sendCommand("STRT 1");  // 無線通信モードを1(短距離)に設定
  sendCommand("RDRT");    // 確認
  
  sendCommand("STCH 31");  // 無線通信チャンネルを31に設定
  sendCommand("RDCH");     // 確認

  sendCommand("STNN 0001");  // ノード番号設定　親機にする場合は0001
  sendCommand("RDNN");       // 確認

  sendCommand("RDGN");  //グループ番号読み出し
  sendCommand("STGN");  // グループ番号設定（ペアリング開始）

}


// メイン処理（繰り返し）-------------------------------------------------------
void loop() {


}