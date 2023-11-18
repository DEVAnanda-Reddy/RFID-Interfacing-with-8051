#include <reg51.h>
#include <string.h>

sbit rs = P2^0;
sbit en = P2^1;
sfr ldata = 0x90; // Port 1

unsigned char a[100];

void delay(unsigned int i);
void lcd_cmd(unsigned char a);
void lcd_data(unsigned char b);
void lcd_init(void);
void lcd_str(unsigned char *str);
void clear(void);

int main() {
    TMOD = 0x20;    // Timer1 mode 2 - auto-reload mode
    TH1 = 0xFD;     // 9600 baud rate
    SCON = 0x50;    // 8-bit data, 1 start bit, 1 stop bit
    TR1 = 1;

    lcd_init();
    lcd_str("   WELCOME TO   ");
    lcd_cmd(0xc0);
    lcd_str("   MY PROJECT   ");

    delay(65000);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("   MMIT Mini   ");
    lcd_cmd(0xc0);
    lcd_str("   project   ");
    delay(65000);

    while (1) {
        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_str("   Waiting For    ");
        lcd_cmd(0xc0);
        lcd_str("   RFID Tag   ");

        int i;
        for (i = 0; i < 10; i++) {
            char temp;
            while (RI == 0); // Wait for 1 char received
            RI = 0;
            temp = SBUF;
            a[i] = temp;
        }

        a[10] = '\0';

        if (strcmp("10003B0CA7", a) == 0) {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_str(" EMPLOYEE NO : 1");
            lcd_cmd(0xc0);
            lcd_str(a);
            delay(65000);
            clear();
        } else if (strcmp("10003B0CAE", a) == 0) {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_str(" EMPLOYEE NO : 2");
            lcd_cmd(0xc0);
            lcd_str(a);
            delay(65000);
            clear();
        } else {
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_str(" INVALID TAG");
            lcd_cmd(0xc0);
            lcd_str(a);
            delay(65000);
            clear();
        }
    }
}

void lcd_init() {
    lcd_cmd(0x38);
    lcd_cmd(0x0c);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void delay(unsigned int i) {
    unsigned int j;
    for (j = 0; j < i; j++);
}

void lcd_cmd(unsigned char a) {
    rs = 0;      // Command mode
    ldata = a;
    en = 1;
    delay(5);
    en = 0;
    delay(5);
}

void lcd_data(unsigned char b) {
    rs = 1;      // Data mode
    ldata = b;
    en = 1;
    delay(5);
    en = 0;
    delay(5);
}

void lcd_str(unsigned char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void clear(void) {
    int i;
    for (i = 0; i < 100; i++) {
        a[i] = '\0';
    }
}
