#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define I2C_ADDR 0x3C
#define I2C_DEV "/dev/i2c-2"

#define SH1106_COL_OFFSET 2

int i2c_fd;

void sh1106_send_command(uint8_t cmd)
{
    uint8_t buffer[2] = {0x00, cmd};
    write(i2c_fd, buffer, 2) != 2;
}

void sh1106_send_data(uint8_t *data, size_t len)
{
    uint8_t *buffer = malloc(len + 1);
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, len);
    write(i2c_fd, buffer, len + 1) != (ssize_t)(len + 1);
    free(buffer);
}

void sh1106_init()
{
    sh1106_send_command(0xAE);
    sh1106_send_command(0xD5);
    sh1106_send_command(0x80);
    sh1106_send_command(0xA8);
    sh1106_send_command(0x3F);
    sh1106_send_command(0xD3);
    sh1106_send_command(0x00);
    sh1106_send_command(0x40);
    sh1106_send_command(0xAD);
    sh1106_send_command(0x8B);
    sh1106_send_command(0xA1);
    sh1106_send_command(0xC8);
    sh1106_send_command(0xDA);
    sh1106_send_command(0x12);
    sh1106_send_command(0x81);
    sh1106_send_command(0xFF);
    sh1106_send_command(0xD9);
    sh1106_send_command(0x1F);
    sh1106_send_command(0xDB);
    sh1106_send_command(0x40);
    sh1106_send_command(0xA4);
    sh1106_send_command(0xA6);
    sh1106_send_command(0xAF);
}

void sh1106_set_cursor(uint8_t page, uint8_t col)
{
    col += SH1106_COL_OFFSET;
    sh1106_send_command(0xB0 | (page & 0x07));
    sh1106_send_command(0x00 | (col & 0x0F));
    sh1106_send_command(0x10 | ((col >> 4) & 0x0F));
}

void sh1106_clear()
{
    for (int page = 0; page < 8; ++page)
    {
        sh1106_set_cursor(page, 0);
        uint8_t zero[128] = {0};
        sh1106_send_data(zero, 128);
    }
}

static const uint8_t font5x8[][5] = {
    [' ' - 32] = {0x00, 0x00, 0x00, 0x00, 0x00},
    ['!' - 32] = {0x00, 0x00, 0x5F, 0x00, 0x00}, 
    ['H' - 32] = {0x7F, 0x08, 0x08, 0x08, 0x7F},
    ['e' - 32] = {0x38, 0x54, 0x54, 0x54, 0x18},
    ['l' - 32] = {0x00, 0x41, 0x7F, 0x40, 0x00},
    ['o' - 32] = {0x38, 0x44, 0x44, 0x44, 0x38},
    ['W' - 32] = {0x3F, 0x40, 0x38, 0x40, 0x3F},
    ['r' - 32] = {0x7C, 0x08, 0x04, 0x04, 0x08},
    ['d' - 32] = {0x38, 0x44, 0x44, 0x44, 0x7F},
};

void sh1106_write_text(const char *text, uint8_t page, uint8_t col)
{
    sh1106_set_cursor(page, col);
    while (*text)
    {
        if (*text < 32 || *text > 127)
        {
            text++;
            continue;
        }
        uint8_t buf[6];
        memcpy(buf, font5x8[(uint8_t)(*text - 32)], 5);
        buf[5] = 0x00; // 1 pixel spacing
        sh1106_send_data(buf, 6);
        text++;
    }
}

int main()
{
    i2c_fd = open(I2C_DEV, O_RDWR);
    if (i2c_fd < 0)
    {
        perror("Open I2C");
        return 1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, I2C_ADDR) < 0)
    {
        perror("I2C_SLAVE");
        close(i2c_fd);
        return 1;
    }

    sh1106_init();
    sh1106_clear();
    sh1106_write_text("Hello World", 0, 0);

    close(i2c_fd);
    return 0;
}
