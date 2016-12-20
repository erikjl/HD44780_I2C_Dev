/***********************************************

http://www.protostack.com/blog/2010/03/character-lcd-displays-part-1/
https://dawes.wordpress.com/2010/01/05/hd44780-instruction-set/
http://www.dinceraydin.com/lcd/commands.htm



************************************************/

#include "hd44780.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>  //I2C_SLAVE
#include <sys/ioctl.h>
#include <fcntl.h>          //open()
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>         //usleep()

static char _lcd_buffer[1];
static const char _EN   = 0b00000100; //enable
static const char _BL   = 0b00001000; //backlight
static const char _COL2 = 0b00100000;
static const char _COL3 = 0b00110000;
static const char _COL4 = 0b01000000;
static const char _COL5 = 0b01010000;
static const char _COL6 = 0b01100000;
static const char _COL7 = 0b01110000;
static const char _ROW0 = 0b00000000;
static const char _ROW1 = 0b00010000;
static const char _ROW2 = 0b00100000;
static const char _ROW3 = 0b00110000;
static const char _ROW4 = 0b01000000;
static const char _ROW5 = 0b01010000;
static const char _ROW6 = 0b01100000;
static const char _ROW7 = 0b01110000;
static const char _ROW8 = 0b10000000;
static const char _ROW9 = 0b10010000;
static const char _ROW10 = 0b10100000;
static const char _ROW11 = 0b10110000;
static const char _ROW12 = 0b11000000;
static const char _ROW13 = 0b11010000;
static const char _ROW14 = 0b11100000;
static const char _ROW15 = 0b11110000;

/***********************************************
  Opens /dev/i2c-? FD, sets ioctrl to I2C_SLAVE
************************************************/
int lcd_i2cSetup(int i2cbus, int i2cDevAddr)
{
    int fd;
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", i2cbus);
    fd = open(filename, O_RDWR);

    if (fd < 0)
    {
        printf("Could not open I2C Bus '%d'! [%s]\r\n", i2cbus, strerror(errno));
        exit(1);
    }

    printf("I2C bus %d opened...\r\n", i2cbus);

    /* Binds FD to device address */
    if (ioctl(fd, I2C_SLAVE, i2cDevAddr) < 0)
    {
        printf("Failed to acquire I2C bus access and/or talk to slave '%#2x' [%s]\r\n", i2cDevAddr, strerror(errno));
        exit(1);
    }

    usleep(2000);
    return fd;
}

/***********************************************
  Initialize the display [Sets 4 bit operation,
  8 bit function, display ON & cursor blinking
************************************************/
void lcd_init_HD44780(int *fd)
{

    usleep(5000);
    lcd_write_nibble(fd, 0, 0b00100000 | _BL);    //Set to 4 bit operation */
    usleep(5000);

    /*lcd_write_nibble(fd, 0, 0b00100000 | _BL);    //Function set
    lcd_write_nibble(fd, 0, 0b10000000 | _BL); */


    lcd_write_nibble(fd, 0, 0b00100000 | _BL);    //Function set
    lcd_write_nibble(fd, 0, 0b10000000 | _BL);

    lcd_write_nibble(fd, 0, 0b00000000 | _BL);    //Display ON, Cursor On, Cursor Blinking
    lcd_write_nibble(fd, 0, 0b11110000 | _BL);

    lcd_write_nibble(fd, 0, 0b00000000 | _BL);    //Entry mode, increment cursor position, no display shift
    lcd_write_nibble(fd, 0, 0b01100000 | _BL);
}

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
void lcd_write_nibble(int *fd, char rs, char data)
{
    /*rs doesnt need to be shifted as its bit 0 */
    _lcd_buffer[0] = data | rs | _EN;
    write(*fd, _lcd_buffer, 1);
    usleep(80);
    _lcd_buffer[0] = data | rs  ;
    write(*fd, _lcd_buffer, 1);
    usleep(80);
}
/***********************************************
  Clears display
************************************************/
void lcd_clear_display(int *fd)
{
    lcd_write_nibble(fd, 0, 0b00000000 | _BL);    //clear display
    lcd_write_nibble(fd, 0, 0b00010000 | _BL);
    usleep(1600);
}
/***********************************************
  Sets curser to home position
************************************************/
void lcd_return_home(int *fd)
{
    lcd_write_nibble(fd, 0, 0b00000000 | _BL);    //clear display
    lcd_write_nibble(fd, 0, 0b00100000 | _BL);
     usleep(1600);
}
/**********************************************
  Mostly complete character mapping for HD44780
  (minus yen, accent, and left/right arrows)
**********************************************/
void lcd_writeChar(int *fd, char c)
{
    /* TODO: reorganize into sequential ASCII values for compiler optimization?
    See: http://embeddedgurus.com/stack-overflow/2010/04/efficient-c-tip-12-be-wary-of-switch-statements/ */

    switch(c)
    {
    /*COLUMN 2 */
        case ' ':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW0 | _BL);
            break;
        case '!':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW1 | _BL);
            break;
        case '"':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW2 | _BL);
            break;
        case '#':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW3 | _BL);
            break;
        case '$':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW4 | _BL);
            break;
        case '%':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW5 | _BL);
            break;
        case '&':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW6 | _BL);
            break;
        case '\'':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW7 | _BL);
            break;
        case '(':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW8 | _BL);
            break;
        case ')':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW9 | _BL);
            break;
        case '*':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW10 | _BL);
            break;
        case '+':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW11 | _BL);
            break;
        case ',':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW12 | _BL);
            break;
        case '-':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW13 | _BL);
            break;
        case '.':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW14 | _BL);
            break;
        case '/':
            lcd_write_nibble(fd, 1, _COL2 | _BL);
            lcd_write_nibble(fd, 1, _ROW15 | _BL);
            break;
    /* COLUMN 3 */
        case '0':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW0 | _BL);
            break;
        case '1':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW1 | _BL);
            break;
        case '2':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW2 | _BL);
            break;
        case '3':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW3 | _BL);
            break;
        case '4':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW4 | _BL);
            break;
        case '5':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW5 | _BL);
            break;
        case '6':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW6 | _BL);
            break;
        case '7':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW7 | _BL);
            break;
        case '8':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW8 | _BL);
            break;
        case '9':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW9 | _BL);
            break;
        case ':':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW10 | _BL);
            break;
        case ';':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW11 | _BL);
            break;
        case '<':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW12 | _BL);
            break;
        case '=':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW13 | _BL);
            break;
        case '>':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW14 | _BL);
            break;
        case '?':
            lcd_write_nibble(fd, 1, _COL3 | _BL);
            lcd_write_nibble(fd, 1, _ROW15 | _BL);
            break;
    /* COLUMN 4 */
        case '@':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW0 | _BL);
            break;
        case 'A':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW1 | _BL);
            break;
        case 'B':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW2 | _BL);
            break;
        case 'C':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW3 | _BL);
            break;
        case 'D':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW4 | _BL);
            break;
        case 'E':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW5 | _BL);
            break;
        case 'F':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW6 | _BL);
            break;
        case 'G':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW7 | _BL);
            break;
        case 'H':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW8 | _BL);
            break;
        case 'I':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW9 | _BL);
            break;
        case 'J':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW10 | _BL);
            break;
        case 'K':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW11 | _BL);
            break;
        case 'L':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW12 | _BL);
            break;
        case 'M':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW13 | _BL);
            break;
        case 'N':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW14 | _BL);
            break;
        case 'O':
            lcd_write_nibble(fd, 1, _COL4 | _BL);
            lcd_write_nibble(fd, 1, _ROW15 | _BL);
            break;
    /* COLUMN 5 */
        case 'P':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW0 | _BL);
            break;
        case 'Q':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW1 | _BL);
            break;
        case 'R':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW2 | _BL);
            break;
        case 'S':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW3 | _BL);
            break;
        case 'T':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW4 | _BL);
            break;
        case 'U':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW5 | _BL);
            break;
        case 'V':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW6 | _BL);
            break;
        case 'W':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW7 | _BL);
            break;
        case 'X':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW8 | _BL);
            break;
        case 'Y':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW9 | _BL);
            break;
        case 'Z':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW10 | _BL);
            break;
        case '[':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW11 | _BL);
            break;
        /*case '': YEN?
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW12 | _BL);
            break; */
        case ']':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW13 | _BL);
            break;
        case '^':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW14 | _BL);
            break;
        case '_':
            lcd_write_nibble(fd, 1, _COL5 | _BL);
            lcd_write_nibble(fd, 1, _ROW15 | _BL);
            break;
    /* COLUMN 6 */
        case 'a':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW1 | _BL);
            break;
        case 'b':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW2 | _BL);
            break;
        case 'c':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW3 | _BL);
            break;
        case 'd':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW4 | _BL);
            break;
        case 'e':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW5 | _BL);
            break;
        case 'f':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW6 | _BL);
            break;
        case 'g':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW7 | _BL);
            break;
        case 'h':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW8 | _BL);
            break;
        case 'i':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW9 | _BL);
            break;
        case 'j':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW10 | _BL);
            break;
        case 'k':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW11 | _BL);
            break;
        case 'l':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW12 | _BL);
            break;
        case 'm':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW13 | _BL);
            break;
        case 'n':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW14 | _BL);
            break;
        case 'o':
            lcd_write_nibble(fd, 1, _COL6 | _BL);
            lcd_write_nibble(fd, 1, _ROW15 | _BL);
            break;
    /* COLUMN 7 */
        case 'p':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW0 | _BL);
            break;
        case 'q':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW1 | _BL);
            break;
        case 'r':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW2 | _BL);
            break;
        case 's':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW3 | _BL);
            break;
        case 't':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW4 | _BL);
            break;
        case 'u':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW5 | _BL);
            break;
        case 'v':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW6 | _BL);
            break;
        case 'w':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW7 | _BL);
            break;
        case 'x':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW8 | _BL);
            break;
        case 'y':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW9 | _BL);
            break;
        case 'z':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW10 | _BL);
            break;
        case '{':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW11 | _BL);
            break;
        case '|':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW12 | _BL);
            break;
        case '}':
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW13 | _BL);
            break;
        /*case 'o': right arrow?
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW14 | _BL);
            break;
        case 'o': left arrow?
            lcd_write_nibble(fd, 1, _COL7 | _BL);
            lcd_write_nibble(fd, 1, _ROW15 | _BL);
            break; */
        default:
            printf("lcd_writeChar(): unmapped char: %c\r\n", c);
            break;
    }

}
/**********************************************
  Moves the cursor to specified position
  (0-based,L-R)
**********************************************/
void lcd_goto(int *fd, char position)
{
    position += 0x80;
    char upper = 0b11110000 & position;
    char lower = (0b00001111 & position) << 4;
    lcd_write_nibble(fd, 0, upper | _BL);
    lcd_write_nibble(fd, 0, lower | _BL);
    usleep(60);
}


