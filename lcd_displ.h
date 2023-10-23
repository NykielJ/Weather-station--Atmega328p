#define set_lcd_d4 PORTD|=(1<<4)
#define clr_lcd_d4 PORTD&=~(1<<4)
#define ddr_d4 DDRD|=(1<<4)           // tak jak set_lcd

#define set_lcd_d5 PORTD|=(1<<5)
#define clr_lcd_d5 PORTD&=~(1<<5)
#define ddr_d5 DDRD|=(1<<5)

#define set_lcd_d6 PORTD|=(1<<6)
#define clr_lcd_d6 PORTD&=~(1<<6)
#define ddr_d6 DDRD|=(1<<6)

#define set_lcd_d7 PORTD|=(1<<7)
#define clr_lcd_d7 PORTD&=~(1<<7)
#define ddr_d7 DDRD|=(1<<7)

#define set_lcd_en PORTD|=(1<<3)
#define clr_lcd_en PORTD&=~(1<<3)
#define ddr_en DDRD|=(1<<3)

#define set_lcd_rs PORTD|=(1<<2)
#define clr_lcd_rs PORTD&=~(1<<2)
#define ddr_rs DDRD|=(1<<2)

// podswietlenie , jesli uzywane
//#define set_lcd_light PORTB|=(1<<3)
//#define clr_lcd_light PORTB&=~(1<<3)
//#define ddr_light DDRB|=(1<<3)




void lcd_init(void);
void lcd_send_data(unsigned char b);
void lcd_send_com(unsigned char b);
void lcd_send_4(unsigned char b);
void lcd_send_8(unsigned char b);
void lcd_control( unsigned char on, unsigned char cur, unsigned char blink);
void lcd_clear(void);
void lcd_home(void);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_swrite( char *s);
void lcd_iwrite(int i);
int lcd_printf( char *format, ... );


