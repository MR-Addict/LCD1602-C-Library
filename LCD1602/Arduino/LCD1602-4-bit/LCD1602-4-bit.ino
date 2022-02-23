#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_SURSOR_SHIFT 0x18
#define LCD_FUNCTION_SET 0x28
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

uint8_t LCD_DISPLAY_CONTROL = 0x0C;

const uint8_t EN = 8, RS = 9;
uint8_t data_pins[] = {7, 6, 5, 4};

const char* message = "Hello world!";
uint8_t bell[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};

void send_pins(uint8_t);
void write_cmd(uint8_t);
void write_data(uint8_t);
void lcd_init();
void lcd_clear();
void lcd_home();
void display_off();
void display_on();
void cursor_off();
void cursor_on();
void blink_off();
void blink_on();
void scroll_left();
void scroll_right();
void set_cursor();
void print(const char);
void print(const char*);
void creat_char(uint8_t, uint8_t*);

void setup() {
    lcd_init();
    creat_char(0, bell);
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

// 发送数据到引脚
void send_pins(uint8_t dat) {
    for (int i = 0; i < 4; i++) {
        digitalWrite(data_pins[i], (dat >> i) & 0x01);
    }
    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);
}

// 写命令
void write_cmd(uint8_t cmd) {
    // 命令RS=0，RW=0
    digitalWrite(RS, LOW);
    // 高四位
    send_pins(cmd >> 4);
    // 低四位
    send_pins(cmd);
}

// 写数据到DDRAM
void write_data(uint8_t dat) {
    // 数据RS=1，RW=0
    digitalWrite(RS, HIGH);
    // 高四位
    send_pins(dat >> 4);
    // 低四位
    send_pins(dat);
}

// LCD初始化
void lcd_init() {
    for (uint8_t i = 0; i < 4; i++) {
        pinMode(data_pins[i], OUTPUT);
    }
    pinMode(EN, OUTPUT);
    pinMode(RS, OUTPUT);

    delay(15);
    for (uint8_t i = 0; i < 3; i++) {
        send_pins(0x03);
        delayMicroseconds(4500);
    }
    send_pins(0x02);  // 设置4总线模式

    write_cmd(LCD_FUNCTION_SET);     // 4总线，2行显示区域，5x8字体
    write_cmd(LCD_DISPLAY_CONTROL);  // 显示开，光标关，闪烁关
    lcd_clear();                     // 清屏
    write_cmd(LCD_ENTRY_MODE);       // 地址自动+1，文字不动
}

// 清空屏幕
void lcd_clear() {
    write_cmd(LCD_CLEAR);
    delayMicroseconds(2000);  // 该指令需要花较长时间
}

// 光标以及光标指针回到初始状态
void lcd_home() {
    write_cmd(LCD_HOME);
    delayMicroseconds(2000);  // 该指令需要花较长时间
}

// 关闭屏幕
void display_off() {
    LCD_DISPLAY_CONTROL &= 0x04;
    write_cmd(LCD_DISPLAY_CONTROL);
}

// 打开屏幕
void display_on() {
    LCD_DISPLAY_CONTROL |= 0x04;
    write_cmd(LCD_DISPLAY_CONTROL);
}

// 关闭光标
void cursor_off() {
    LCD_DISPLAY_CONTROL &= ~0x02;
    write_cmd(LCD_DISPLAY_CONTROL);
}

// 打开光标
void cursor_on() {
    LCD_DISPLAY_CONTROL |= 0x02;
    write_cmd(LCD_DISPLAY_CONTROL);
}

// 关闭光标闪烁
void blink_off() {
    LCD_DISPLAY_CONTROL &= ~0x01;
    write_cmd(LCD_DISPLAY_CONTROL);
}

// 打开光标闪烁
void blink_on() {
    LCD_DISPLAY_CONTROL |= 0x01;
    write_cmd(LCD_DISPLAY_CONTROL);
}

// 向左滚动一列
void scroll_left() {
    write_cmd(LCD_SURSOR_SHIFT);
}

// 向右滚动一列
void scroll_right() {
    write_cmd(LCD_SURSOR_SHIFT | 0x04);
}

// 设置光标位置
void set_cursor(uint8_t x, uint8_t y) {
    uint8_t addr;
    if (y == 0)
        addr = 0x00 + x;  // 第一行字符地址从 0x00开始
    else
        addr = 0x40 + x;  // 第二行字符地址从 0x40开始
    write_cmd(addr | LCD_SET_DDRAM_ADDR);  // 设置DDRAM地址
}

// 打印字符
void print(const char cha) {
    write_data(cha);
}

// 打印字符串
void print(const char* str) {
    while (*str) {
        write_data(*str++);  // 先取指针的值，再将指针自增
    }
}

// 自定义一个新的CGRAM字符
void creat_char(uint8_t num, uint8_t* bit_map) {
    // 设置CGRAM地址
    write_cmd(LCD_SET_CGRAM_ADDR + 8 * num);
    for (uint8_t i = 0; i < 8; i++) {
        write_data(bit_map[i]);
    }
}