#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_SURSOR_SHIFT 0x18
#define LCD_FUNCTION_SET 0x38
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

uint8_t LCD_DISPLAY_CONTROL = 0x0C;

const uint8_t DB7 = 2;
const uint8_t DB6 = 3;
const uint8_t DB5 = 4;
const uint8_t DB4 = 5;
const uint8_t DB3 = 6;
const uint8_t DB2 = 7;
const uint8_t DB1 = 8;
const uint8_t DB0 = 9;
const uint8_t EN = 10;
const uint8_t RW = 11;
const uint8_t RS = 12;

uint8_t data_pins[] = {DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7, EN, RS, RW};

void send_pins(uint8_t dat) {
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(data_pins[i], OUTPUT);
    }
    for (int i = 0; i < 8; i++) {
        digitalWrite(data_pins[i], (dat >> i) & 0x01);
    }
}

uint8_t read_pins() {
    uint8_t sta = 0xFF;
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(data_pins[i], INPUT);
    }
    for (uint8_t i = 0; i < 8; i++) {
        sta &= ((uint8_t)digitalRead(data_pins[i]) << i);
    }
    return sta;
}

void wait_ready() {
    send_pins(0xFF);
    digitalWrite(RS, HIGH);
    digitalWrite(RW, LOW);
    uint8_t sta;
    do {
        digitalWrite(EN, HIGH);
        sta = read_pins();
        digitalWrite(EN, LOW);
    } while (sta & 0x80);  // 读取Bit7的Busy_Flag
}

void write_cmd(uint8_t cmd) {
    // 写命令RS=0，RW=0
    wait_ready();
    digitalWrite(RS, LOW);
    digitalWrite(RW, LOW);
    send_pins(cmd);
    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);
}

void write_data(uint8_t dat) {
    // 写数据RS=1，RW=0
    wait_ready();
    digitalWrite(RS, HIGH);
    digitalWrite(RW, LOW);
    send_pins(dat);
    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);
}

void clear() {
    write_cmd(LCD_CLEAR);
    delayMicroseconds(2000);
}

void home() {
    write_cmd(LCD_HOME);
    delayMicroseconds(2000);
}

void display_off() {
    LCD_DISPLAY_CONTROL &= ~0x04;
    write_cmd(LCD_DISPLAY_CONTROL);
}

void display_on() {
    LCD_DISPLAY_CONTROL |= 0x04;
    write_cmd(LCD_DISPLAY_CONTROL);
}

void cursor_off() {
    LCD_DISPLAY_CONTROL &= ~0x02;
    write_cmd(LCD_DISPLAY_CONTROL);
}

void cursor_on() {
    LCD_DISPLAY_CONTROL |= 0x02;
    write_cmd(LCD_DISPLAY_CONTROL);
}

void blink_off() {
    LCD_DISPLAY_CONTROL &= ~0x01;
    write_cmd(LCD_DISPLAY_CONTROL);
}

void blink_on() {
    LCD_DISPLAY_CONTROL |= 0x01;
    write_cmd(LCD_DISPLAY_CONTROL);
}

void scroll_left() {
    write_cmd(LCD_SURSOR_SHIFT);
}

void scroll_right() {
    write_cmd(LCD_SURSOR_SHIFT | 0x04);
}

void set_cursor(uint8_t x, uint8_t y) {
    uint8_t addr;
    if (y == 0)
        addr = 0x00 + x;  //第一行字符地址从 0x00 起始
    else
        addr = 0x40 + x;  //第二行字符地址从 0x40 起始
    write_cmd(LCD_SET_DDRAM_ADDR | addr);  //设置 RAM 地址
}

void print(const char* str) {
    while (*str) {
        write_data(*str++);  // 先取指针的值，再将指针自增
    }
}

void creat_char(uint8_t num, uint8_t* bit_map) {
    write_cmd(LCD_SET_CGRAM_ADDR + 8 * num);
    for (uint8_t i = 0; i < 8; i++) {
        write_data(bit_map[i]);
    }
}

void lcd_init() {
    for (uint8_t i = 0; i < 10; i++) {
        pinMode(data_pins[i], OUTPUT);
    }

    write_cmd(LCD_FUNCTION_SET);     // 8总线，2行显示区域，5x8字体
    write_cmd(LCD_DISPLAY_CONTROL);  // 显示开，光标关，闪烁关
    write_cmd(LCD_CLEAR);            // 清屏
    write_cmd(LCD_ENTRY_MODE);       // 地址自动+1，文字不动
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
