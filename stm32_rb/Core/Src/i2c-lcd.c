/**
Edit by modify: Ngoc Hang
**/

#include "global.h"
#include "main.h"
#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;

#define SLAVE_ADDRESS_LCD (0x21 << 1) // change this according to ur setup


void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);           						// Lấy 4 bit cao của lệnh
	data_l = ((cmd<<4)&0xf0);							// Lấy 4 bit thấp của lệnh và dịch sang vị trí cao
	data_t[0] = data_u|0x0C;  //en=1, rs=0				//-> gửi phần 4 bit cao với EN = 1
	data_t[1] = data_u|0x08;  //en=0, rs=0				//-> xác nhận lệnh đã gửi xong
	data_t[2] = data_l|0x0C;  //en=1, rs=0				//-> gửi phần 4 bit thấp với EN = 1
	data_t[3] = data_l|0x08;  //en=0, rs=0				//-> xác nhận lệnh đã gửi xong


//	data_t[0] = data_u | 0x0D;  // en=1, rs=1
//	data_t[1] = data_u | 0x09;  // en=0, rs=1
//	data_t[2] = data_l | 0x0D;  // en=1, rs=1
//	data_t[3] = data_l | 0x09;  // en=0, rs=1

	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}



/*
  	RS (Register Select): Lựa chọn thanh ghi của LCD (0 = gửi lệnh, 1 = gửi dữ liệu).
	RW (Read/Write): Chọn chế độ đọc hoặc ghi (thường đặt 0 để ghi).
	EN (Enable): Kích hoạt để LCD nhận lệnh/dữ liệu (EN = 1) và xác nhận đã nhận (EN = 0).
	D4-D7: Các chân dữ liệu, vì LCD được giao tiếp trong chế độ 4-bit.

	Thứ tự các bit(binary) từ phải -> trái
	vd: 0x0C (0000 1100 trong nhị phân)
		RS = 0 (bit thứ 0, tức là chế độ lệnh).
		RW = 0 (bit thứ 1, chế độ ghi).
		EN = 1 (bit thứ 2 từ phải sang).

 */

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void) {
	lcd_send_cmd (0x33); /* set 4-bits interface */
	lcd_send_cmd (0x32);
	HAL_Delay(50);
	lcd_send_cmd (0x28); /* start to set LCD function */
	HAL_Delay(50);
	lcd_send_cmd (0x01); /* clear display */
	HAL_Delay(50);
	lcd_send_cmd (0x06); /* set entry mode */
	HAL_Delay(50);
	lcd_send_cmd (0x0c); /* set display to on */
	HAL_Delay(50);
	lcd_send_cmd (0x02); /* move cursor to home and set data address to 0 */
	HAL_Delay(50);
	lcd_send_cmd (0x80);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
}

void lcd_goto_XY (int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1)
	{
		pos_Addr = 0x80 + row - 1 + col;
	}
	else
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	lcd_send_cmd(pos_Addr);
}

//void lcd_goto_XY (int row, int col) {
//    uint8_t pos_Addr;
//    if (row == 1) {
//        pos_Addr = 0x80 + (col - 1); // Lệnh cho dòng 1
//    } else if (row == 2) {
//        pos_Addr = 0xC0 + (col - 1); // Lệnh cho dòng 2
//    }
//    lcd_send_cmd(pos_Addr);
//}






