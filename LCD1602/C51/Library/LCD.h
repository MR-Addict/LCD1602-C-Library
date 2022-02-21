#define LCD_Port P0

uint8_t DISPLAYTYPE = 0x38;
uint8_t DISPLAYCOMMAND = 0x0c;
uint8_t DATAADDRESS = 0x06;
uint8_t DISPLAYCLEAR = 0x01;

sbit  LCD_E = P1^5;
sbit  LCD_RW = P1^1;
sbit  LCD_RS = P1^0;

void LCD_Ready(){
	uint8_t status;
	//������
	LCD_Port = 0xff;
	LCD_RS = 0;
	LCD_RW = 1;
	do{
		LCD_E = 1;
		status = LCD_Port;
		LCD_E = 0;
	}while(status & 0x80);	//�����λΪ1ʱ��ʾ����æ
}

void LCD_Write_CMD(uint8_t cmd){
	LCD_Ready();
	//����д�������
	LCD_RS=0;
	LCD_RW=0;
	LCD_Port=cmd;	//ָ��
	LCD_E=1;	//��������д����
	LCD_E=0;	//�ر�����д����
}

void LCD_Write_Data(uint8_t Data){
	//����д���ݲ���
	LCD_Ready();
	LCD_RS=1;
	LCD_RW=0;
	LCD_Port=Data;	//����
	LCD_E=1;	//��������д����
	LCD_E=0;	//�ر�����д����
}

void LCD_Init(){
	LCD_Write_CMD(DISPLAYTYPE);	//16*2��ʾ��5*7����8λ���ݽӿ�
	LCD_Write_CMD(DISPLAYCOMMAND);	//��ʾ���������ر�
	LCD_Write_CMD(DATAADDRESS);	//���ֲ�������ַ���Զ�+1
	LCD_Write_CMD(DISPLAYCLEAR);	//����
}

void LCD_Clear(){
	LCD_Write_CMD(DISPLAYCLEAR);	//����
}

void LCD_Cursor(){
	DISPLAYCOMMAND = DISPLAYCOMMAND|0x02;	//��λ���������ȡ��
	LCD_Write_CMD(DISPLAYCOMMAND);	//�򿪹��
}

void LCD_No_Cursor(){
	DISPLAYCOMMAND = DISPLAYCOMMAND & 0x02;	//��λ���������ȡ��
	LCD_Write_CMD(DISPLAYCOMMAND);	//�رչ��
}
void LCD_Blink(){
	DISPLAYCOMMAND = DISPLAYCOMMAND|0x01;	//��λ���������ȡ��
	LCD_Write_CMD(DISPLAYCOMMAND);	//�����˸
}
void LCD_No_Blink(){
	DISPLAYCOMMAND = DISPLAYCOMMAND & 0x01;	//��λ���������ȡ��
	LCD_Write_CMD(DISPLAYCOMMAND);	//�ⲻ��˸
}
	
void LCD_Set_Cursor(uint8_t y,uint8_t x){
	uint8_t address;
	if(y==0)address = 0x00 + x;	//��һ���ַ���ַ��0x00��ʼ
	else if(y == 1)address = 0x40 + x;	//�ڶ����ַ���ַ��0x40��ʼ
	LCD_Write_CMD(address | 0x80);	//����RAM��ַ
}

void LCD_Print_Char(uint8_t Data){
  LCD_Write_Data(Data);
}

void LCD_Print_Str(uint8_t *str){
	while(*str != '\0'){
		LCD_Write_Data(*str++);
	}
}

void LCD_Custom_Char(uint8_t Num,uint8_t Data[]){
	uint8_t i;
  LCD_Write_CMD(0x40+8*(Num-1));   // CGRAM��ַ��Ϊ��ʼλ�ô�
  for(i=0;i<8;i++) {
     LCD_Write_Data(Data[i]);	// ������д������
  }
}

void LCD_Print_Custom_Char(uint8_t y,uint8_t x,uint8_t Num){
	LCD_Set_Cursor(y,x);
  LCD_Write_Data(0x00+Num-1);
}