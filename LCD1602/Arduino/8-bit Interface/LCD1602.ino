#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80
#define CURSOR_SHIFT 0x18

uint8_t DISPLAY_SETTINGS = 0x0C;

const uint8_t DB7 = 4;
const uint8_t DB6 = 5;
const uint8_t DB5 = 6;
const uint8_t DB4 = 7;
const uint8_t EN = 8;
const uint8_t RS = 9;

void pulse_enable_pin() {
  digitalWrite(EN, HIGH);
  delayMicroseconds(1);
  digitalWrite(EN, LOW);
  delayMicroseconds(50);
}

void latch_pins(uint8_t dat) {
  uint8_t data_pins[] = {DB4, DB5, DB6, DB7};
  for (int i = 0; i < 4; i++) {
    digitalWrite(data_pins[i], (dat >> i) & 0x01);
  }
  pulse_enable_pin();
}

void write_cmd(uint8_t cmd) {
  // 命令RS=0，RW=0
  digitalWrite(RS, LOW);
  // 高四位
  latch_pins(cmd >> 4);
  // 低四位
  latch_pins(cmd);
}

void write_data(uint8_t dat) {
  // 数据RS=1，RW=0
  digitalWrite(RS, HIGH);
  // 高四位
  latch_pins(dat >> 4);
  // 低四位
  latch_pins(dat);
}

void clear() {
  write_cmd(LCD_CLEAR);
  delayMicroseconds(2000);
}

void home() {
  write_cmd(LCD_HOME);
  delayMicroseconds(2000);
}

void set_cursor(uint8_t x, uint8_t y) {
  uint8_t addr;
  if (y == 0)
    addr = 0x00 + x; //第一行字符地址从 0x00 起始
  else
    addr = 0x40 + x; //第二行字符地址从 0x40 起始
  write_cmd(addr | LCD_SET_DDRAM_ADDR); //设置 RAM 地址
}

void print(const char* str) {
  while (*str) {
    write_data(*str++); // 先取指针的值，再将指针自增
  }
}

void creat_char(uint8_t num, uint8_t* bit_map) {
  write_cmd(LCD_SET_CGRAM_ADDR + 8 * num);
  for (uint8_t i = 0; i < 8; i++) {
    write_data(bit_map[i]);
  }
}

void display_off() {
  DISPLAY_SETTINGS &= 0x04;
  write_cmd(DISPLAY_SETTINGS);
}

void display_on() {
  DISPLAY_SETTINGS |= 0x04;
  write_cmd(DISPLAY_SETTINGS);
}

void cursor_off() {
  DISPLAY_SETTINGS &= ~0x02;
  write_cmd(DISPLAY_SETTINGS);
}

void cursor_on() {
  DISPLAY_SETTINGS |= 0x02;
  write_cmd(DISPLAY_SETTINGS);
}

void blink_off() {
  DISPLAY_SETTINGS &= ~0x01;
  write_cmd(DISPLAY_SETTINGS);
}

void blink_on() {
  DISPLAY_SETTINGS |= 0x01;
  write_cmd(DISPLAY_SETTINGS);
}

void scroll_left() {
  write_cmd(CURSOR_SHIFT);
}

void scroll_right() {
  write_cmd(CURSOR_SHIFT | 0x04);
}

void lcd_init() {
  pinMode(DB4, OUTPUT);
  pinMode(DB5, OUTPUT);
  pinMode(DB6, OUTPUT);
  pinMode(DB7, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(RS, OUTPUT);

  digitalWrite(RS, LOW);
  digitalWrite(EN, LOW);

  delay(15);
  for (uint8_t i = 0; i < 3; i++) {
    latch_pins(0x03);
    delayMicroseconds(4500);
  }
  latch_pins(0x02);  // 设置4总线模式

  write_cmd(0x28);              // 4总线，2行显示区域，5x8字体
  write_cmd(DISPLAY_SETTINGS);  // 显示开，光标关，闪烁关
  clear();                      // 清屏
  write_cmd(0x06);              // 地址自动+1，文字不动
}

uint8_t duck[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
const char* message = "Hello world!";

void setup() {
  lcd_init();
  creat_char(0, duck);
  set_cursor(7, 0);
  write_data(0);
  set_cursor(2, 1);
  print(message);
}

void loop() {
  cursor_on();
  delay(1000);
  cursor_off();
  delay(1000);
}
