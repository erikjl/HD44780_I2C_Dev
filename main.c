#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h> //usleep()
#include "hd44780.h"

int main(int argc, char** argv)
{
    int i2cbus = 1;
    //int i2cDevAddr = 0x27;
    int i2cDevAddr = 0x3f;
    int fd = lcd_i2cSetup(i2cbus, i2cDevAddr);
    lcd_init_HD44780(&fd);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    //test 1st line
    for(int i = 0; i < 16; i++)
    {
        lcd_goto(&fd,i);
        printf("   %d\r\n", i);
        usleep(500000);
    }
    //test second line
    for(int i = 64; i < 80; i++)
    {
        lcd_goto(&fd,i);
        printf("   %d\r\n", i);
        usleep(500000);
    }

    lcd_return_home(&fd);

    /* column 2 */
    printf("Comumn 2\r\n");
    lcd_writeChar(&fd, ' ');
    lcd_writeChar(&fd, '!');
    lcd_writeChar(&fd, '"');
    lcd_writeChar(&fd, '#');
    lcd_writeChar(&fd, '$');
    lcd_writeChar(&fd, '%');
    lcd_writeChar(&fd, '&');
    lcd_writeChar(&fd, '\'');
    lcd_writeChar(&fd, '(');
    lcd_writeChar(&fd, ')');
    lcd_writeChar(&fd, '*');
    lcd_writeChar(&fd, '+');
    lcd_writeChar(&fd, ',');
    lcd_writeChar(&fd, '-');
    lcd_writeChar(&fd, '.');
    lcd_writeChar(&fd, '/');

    lcd_goto(&fd,64);

    /* column 3 */
    printf("Comumn 3\r\n");
    lcd_writeChar(&fd, '@');
    lcd_writeChar(&fd, 'A');
    lcd_writeChar(&fd, 'B');
    lcd_writeChar(&fd, 'C');
    lcd_writeChar(&fd, 'D');
    lcd_writeChar(&fd, 'E');
    lcd_writeChar(&fd, 'F');
    lcd_writeChar(&fd, 'G');
    lcd_writeChar(&fd, 'H');
    lcd_writeChar(&fd, 'I');
    lcd_writeChar(&fd, 'J');
    lcd_writeChar(&fd, 'K');
    lcd_writeChar(&fd, 'L');
    lcd_writeChar(&fd, 'M');
    lcd_writeChar(&fd, 'N');
    lcd_writeChar(&fd, 'O');
    usleep(3000000);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    /* column 4 */
    printf("Comumn 4\r\n");
    lcd_writeChar(&fd, '0');
    lcd_writeChar(&fd, '1');
    lcd_writeChar(&fd, '2');
    lcd_writeChar(&fd, '3');
    lcd_writeChar(&fd, '4');
    lcd_writeChar(&fd, '5');
    lcd_writeChar(&fd, '6');
    lcd_writeChar(&fd, '7');
    lcd_writeChar(&fd, '8');
    lcd_writeChar(&fd, '9');
    lcd_writeChar(&fd, ':');
    lcd_writeChar(&fd, ';');
    lcd_writeChar(&fd, '<');
    lcd_writeChar(&fd, '=');
    lcd_writeChar(&fd, '>');
    lcd_writeChar(&fd, '?');

    lcd_goto(&fd,64);

    /* column 5 */
    printf("Comumn 5\r\n");
    lcd_writeChar(&fd, 'P');
    lcd_writeChar(&fd, 'Q');
    lcd_writeChar(&fd, 'R');
    lcd_writeChar(&fd, 'S');
    lcd_writeChar(&fd, 'T');
    lcd_writeChar(&fd, 'U');
    lcd_writeChar(&fd, 'V');
    lcd_writeChar(&fd, 'W');
    lcd_writeChar(&fd, 'X');
    lcd_writeChar(&fd, 'Y');
    lcd_writeChar(&fd, 'Z');
    lcd_writeChar(&fd, '[');
    lcd_writeChar(&fd, ']');
    lcd_writeChar(&fd, '^');
    lcd_writeChar(&fd, '_');
    usleep(3000000);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    /* column 6 */
    printf("Comumn 6\r\n");
    lcd_writeChar(&fd, 'a');
    lcd_writeChar(&fd, 'b');
    lcd_writeChar(&fd, 'c');
    lcd_writeChar(&fd, 'd');
    lcd_writeChar(&fd, 'e');
    lcd_writeChar(&fd, 'f');
    lcd_writeChar(&fd, 'g');
    lcd_writeChar(&fd, 'h');
    lcd_writeChar(&fd, 'i');
    lcd_writeChar(&fd, 'j');
    lcd_writeChar(&fd, 'k');
    lcd_writeChar(&fd, 'l');
    lcd_writeChar(&fd, 'm');
    lcd_writeChar(&fd, 'n');
    lcd_writeChar(&fd, 'o');

    lcd_goto(&fd,64);

    /* column 7 */
    printf("Comumn 7\r\n");
    lcd_writeChar(&fd, 'p');
    lcd_writeChar(&fd, 'q');
    lcd_writeChar(&fd, 'r');
    lcd_writeChar(&fd, 's');
    lcd_writeChar(&fd, 't');
    lcd_writeChar(&fd, 'u');
    lcd_writeChar(&fd, 'v');
    lcd_writeChar(&fd, 'w');
    lcd_writeChar(&fd, 'x');
    lcd_writeChar(&fd, 'y');
    lcd_writeChar(&fd, 'z');
    lcd_writeChar(&fd, '{');
    lcd_writeChar(&fd, '|');
    lcd_writeChar(&fd, '}');
    usleep(3000000);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    /*****************************************************************
    *   This code will allow interactive entering of nibbles
    *****************************************************************/
    /*
    while(1)
    {
        printf("->");
        fflush(stdout);
        char line[8];
        fgets(line, 8, stdin);

        char rs = line[0] & 1;
        char nibble =
            ((line[4] & 1) << 4) |
            ((line[3] & 1) << 5) |
            ((line[2] & 1) << 6) |
            ((line[1] & 1) << 7);


        printf("RS: %d DECIMAL:  %d\r\n", rs, nibble);
        lcd_write_nibble(&fd, rs, nibble | _BL);
    }
    */

    /*****************************************************************
    *   This code toggles each I2C bit high for 3 seconds
        (allows for mapping of i2c bits to hd44780 pins)
    *****************************************************************/
    /*
    int i = 0;
    while(1)
    {
        if(i == 8)
            i=0;

        buffer[0] = 1 << i++;
        printf("Failed to write\r\n");
        printf("Bit: %d\r\n", i-1);
        usleep(3000000);
    }
    */

    return 0;
}
