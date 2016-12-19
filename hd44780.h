#ifndef HD44780_H
#define HD44780_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************
  Opens /dev/i2c-? FD, sets ioctrl to I2C_SLAVE
************************************************/
int lcd_i2cSetup(int i2cbus, int i2cDevAddr);

/***********************************************
  Initialize the display [Sets 4 bit operation,
  8 bit function, display ON & cursor blinking
************************************************/
void lcd_init_HD44780(int *fd);

/***********************************************
    Send nibble in 4 bit mode to HD44780
        RS = data bit 0 (register select)
        RW = data bit 1 (Read(1)/Write{0}) (leave 0)
        EN = data bit 2 (enable)
        BL = data bit 3 (backlight)
        D4 = data bit 4
        D5 = data bit 5
        D6 = data bit 6
        D7 = data bit 7

    EX: Write a 'w'
    lcd_write_nibble(1, 0b0101); //upper nibble
    lcd_write_nibble(1, 0b0111); //lower nibble
************************************************/
void lcd_write_nibble(int *fd, char rs, char data);

/***********************************************
  Clears display
************************************************/
void lcd_clear_display(int *fd);

/***********************************************
  Sets curser to home position
************************************************/
void lcd_return_home(int *fd);

/**********************************************
  Mostly complete character mapping for HD44780
  (minus yen, accent, and left/right arrows)
**********************************************/
void lcd_writeChar(int *fd, char c);

/**********************************************
  Moves the cursor to specified position
  (0-based,L-R)
**********************************************/
void lcd_goto(int *fd, char position);

#ifdef __cplusplus
}
#endif

#endif // HD44780_H
