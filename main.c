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

    char test[]  = "Testing the HD44780 library!";
    lcd_write_string(&fd, 0, test, 28);
    usleep(5000000);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    /* column 2 */
    printf("Comumn 2\r\n");
    lcd_write_char(&fd, ' ');
    lcd_write_char(&fd, '!');
    lcd_write_char(&fd, '"');
    lcd_write_char(&fd, '#');
    lcd_write_char(&fd, '$');
    lcd_write_char(&fd, '%');
    lcd_write_char(&fd, '&');
    lcd_write_char(&fd, '\'');
    lcd_write_char(&fd, '(');
    lcd_write_char(&fd, ')');
    lcd_write_char(&fd, '*');
    lcd_write_char(&fd, '+');
    lcd_write_char(&fd, ',');
    lcd_write_char(&fd, '-');
    lcd_write_char(&fd, '.');
    lcd_write_char(&fd, '/');

    lcd_goto(&fd,64);

    /* column 3 */
    printf("Comumn 3\r\n");
    lcd_write_char(&fd, '@');
    lcd_write_char(&fd, 'A');
    lcd_write_char(&fd, 'B');
    lcd_write_char(&fd, 'C');
    lcd_write_char(&fd, 'D');
    lcd_write_char(&fd, 'E');
    lcd_write_char(&fd, 'F');
    lcd_write_char(&fd, 'G');
    lcd_write_char(&fd, 'H');
    lcd_write_char(&fd, 'I');
    lcd_write_char(&fd, 'J');
    lcd_write_char(&fd, 'K');
    lcd_write_char(&fd, 'L');
    lcd_write_char(&fd, 'M');
    lcd_write_char(&fd, 'N');
    lcd_write_char(&fd, 'O');
    usleep(3000000);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    /* column 4 */
    printf("Comumn 4\r\n");
    lcd_write_char(&fd, '0');
    lcd_write_char(&fd, '1');
    lcd_write_char(&fd, '2');
    lcd_write_char(&fd, '3');
    lcd_write_char(&fd, '4');
    lcd_write_char(&fd, '5');
    lcd_write_char(&fd, '6');
    lcd_write_char(&fd, '7');
    lcd_write_char(&fd, '8');
    lcd_write_char(&fd, '9');
    lcd_write_char(&fd, ':');
    lcd_write_char(&fd, ';');
    lcd_write_char(&fd, '<');
    lcd_write_char(&fd, '=');
    lcd_write_char(&fd, '>');
    lcd_write_char(&fd, '?');

    lcd_goto(&fd,64);

    /* column 5 */
    printf("Comumn 5\r\n");
    lcd_write_char(&fd, 'P');
    lcd_write_char(&fd, 'Q');
    lcd_write_char(&fd, 'R');
    lcd_write_char(&fd, 'S');
    lcd_write_char(&fd, 'T');
    lcd_write_char(&fd, 'U');
    lcd_write_char(&fd, 'V');
    lcd_write_char(&fd, 'W');
    lcd_write_char(&fd, 'X');
    lcd_write_char(&fd, 'Y');
    lcd_write_char(&fd, 'Z');
    lcd_write_char(&fd, '[');
    lcd_write_char(&fd, ']');
    lcd_write_char(&fd, '^');
    lcd_write_char(&fd, '_');
    usleep(3000000);
    lcd_clear_display(&fd);
    lcd_return_home(&fd);

    /* column 6 */
    printf("Comumn 6\r\n");
    lcd_write_char(&fd, 'a');
    lcd_write_char(&fd, 'b');
    lcd_write_char(&fd, 'c');
    lcd_write_char(&fd, 'd');
    lcd_write_char(&fd, 'e');
    lcd_write_char(&fd, 'f');
    lcd_write_char(&fd, 'g');
    lcd_write_char(&fd, 'h');
    lcd_write_char(&fd, 'i');
    lcd_write_char(&fd, 'j');
    lcd_write_char(&fd, 'k');
    lcd_write_char(&fd, 'l');
    lcd_write_char(&fd, 'm');
    lcd_write_char(&fd, 'n');
    lcd_write_char(&fd, 'o');

    lcd_goto(&fd,64);

    /* column 7 */
    printf("Comumn 7\r\n");
    lcd_write_char(&fd, 'p');
    lcd_write_char(&fd, 'q');
    lcd_write_char(&fd, 'r');
    lcd_write_char(&fd, 's');
    lcd_write_char(&fd, 't');
    lcd_write_char(&fd, 'u');
    lcd_write_char(&fd, 'v');
    lcd_write_char(&fd, 'w');
    lcd_write_char(&fd, 'x');
    lcd_write_char(&fd, 'y');
    lcd_write_char(&fd, 'z');
    lcd_write_char(&fd, '{');
    lcd_write_char(&fd, '|');
    lcd_write_char(&fd, '}');
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
