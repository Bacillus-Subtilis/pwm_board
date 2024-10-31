
#include "lcdAQ0802.h"

void lcdCmd(byte cmd) {
  Wire.beginTransmission(LCDA);
  Wire.write((byte)0x00);       // コマンド指定
  Wire.write(cmd);          // コマンド出力
  Wire.endTransmission();       // ストップ

  if( (cmd==0x01) || (cmd==0x02) ) {
    delay(2);             // 2ms待ち
  }
  else {
    delayMicroseconds(30);      // 30us待ち
  }
}

//  ----------------------------------------
//  LCDにデータ送信
//  ----------------------------------------
void lcdData(byte dat) {
  Wire.beginTransmission(LCDA);   // これから...
  Wire.write(0x40);         // データ送るよ
  Wire.write(dat);          // データ
  Wire.endTransmission();       // おしまい
  delayMicroseconds(30);
}

void lcdPut(int x, int y, byte dat){
  lcdLocate( x, y );
  lcdData(dat);
}

void lcdReg(int a, char *str){
  byte H, L;
  byte adr;
  byte dat;
  char *buf;
  
  buf = str;
  if(a == 1){
    adr = 0x40;
  }else if(a == 2){
    adr = 0x48;
  }else if(a == 3){
    adr = 0x50;
  }else if(a == 4){
    adr = 0x58;
  }else if(a == 5){
    adr = 0x60;
  }else if(a == 6){
    adr = 0x68;
  }else if(a == 7){
    adr = 0x70;
  }else if(a == 8){
    adr = 0x78;
  }else {
    adr = 0x40;
  }
  Wire.write(adr);
  Wire.beginTransmission(LCDA);
  for(int i = 0; i < 8; i++){
    H = *buf - '0';
    buf++;
    if(H > 9){
      H = H + '0' - 'a' + 10;
    }
    L = *buf - '0';
    buf++;
    if(L > 9){
      L = L + '0' - 'a' + 10;
    }
    H = H << 4;
    dat = H + L;
    
    Wire.write(dat);
  }
  Wire.endTransmission();
}

//  ----------------------------------------
//  表示位置の指定
//  ----------------------------------------
void lcdLocate(byte col, byte row) {
  byte add = 0x80 + col;        // 1行目+x座標
  if( row>0 ) add += 0x40;      // 2行目に変更
  lcdCmd(add);
}

//  ----------------------------------------
//  初期化
//  ----------------------------------------
void lcdInit() {
  //I2c.begin();            // begin I2C master
  delay(10);
  lcdCmd(0x38);           // 8bit 2line Normal mode
  lcdCmd(0x39);           // 8bit 2line Extend mode
  lcdCmd(0x14);           // OSC 183Hz BIAS 1/5

  // コントラスト設定
  lcdCmd((byte)(0x70 + (CONTRAST & 0x0F))); //下位 4bit
  lcdCmd((byte)(0x5c + (CONTRAST >> 4)));   //上位 2bit
  lcdCmd((byte)0x6b);         // Follwer for 3.3V
  delay(1);

  lcdCmd((byte)0x38);         // Set Normal mode
  lcdCmd((byte)0x0C);         // Display On
  lcdCmd((byte)0x01);         // Clear Display
  delay(1);
}

//  ----------------------------------------
//  全消去関数
//  ----------------------------------------
void lcdClear() {
  lcdCmd(0x01);           // 初期化コマンド出力
  delay(2);
}

//  ----------------------------------------
//  文字列表示関数
//  ----------------------------------------
void lcdPrint( const char* cp ) {
  while(*cp!=0x00) {
    lcdData(*cp);
    cp++;
  }
}

//  ----------------------------------------
//  LCDの1行目表示
//  ----------------------------------------
void lcdDisp1( const char* cp ) {
  lcdLocate( 0, 0 );
  lcdDisp8( cp );
}

//  ----------------------------------------
//  LCDの2行目表示
//  ----------------------------------------
void lcdDisp2( const char* cp ) {
  lcdLocate( 0, 1 );
  lcdDisp8( cp );
}

//  ----------------------------------------
//  8文字表示(普通は使わない)
//  ----------------------------------------
void lcdDisp8( const char* cp ) {
  int i;
  for( i=0; i<8; i++ ) {
    if( *cp==0x00 ) break;      // 文字列が終わった
    lcdData( *cp );
    cp++;
  }
  for( ; i<8; i++) {
    lcdData( 0x20 );        // 残りを空白で埋める
    cp++;
  }
}