#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define I2C_ADDR 0x3C
#define I2C_DEV  "/dev/i2c-1"

// SH1106 có RAM 132 col, màn hình 128 col => offset 2
#define SH1106_COL_OFFSET 2

int i2c_fd;

void sh1106_send_command(uint8_t cmd) {
    uint8_t buffer[2] = {0x00, cmd};
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("write command");
    }
}

void sh1106_send_data(uint8_t *data, size_t len) {
    uint8_t *buffer = malloc(len + 1);
    if (!buffer) return;
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, len);
    if (write(i2c_fd, buffer, len + 1) != (ssize_t)(len + 1)) {
        perror("write data");
    }
    free(buffer);
}

void sh1106_init() {
    sh1106_send_command(0xAE); // Display OFF

    sh1106_send_command(0xD5); // Set display clock divide
    sh1106_send_command(0x80);

    sh1106_send_command(0xA8); // Set multiplex ratio
    sh1106_send_command(0x3F); // 64 rows

    sh1106_send_command(0xD3); // Set display offset
    sh1106_send_command(0x00);

    sh1106_send_command(0x40); // Set display start line = 0

    sh1106_send_command(0xAD); // Charge pump (SH1106 specific)
    sh1106_send_command(0x8B); // Enable internal VCC

    sh1106_send_command(0xA1); // Segment remap (col 127 = SEG0)
    sh1106_send_command(0xC8); // COM scan direction remapped

    sh1106_send_command(0xDA); // Set COM pins
    sh1106_send_command(0x12);

    sh1106_send_command(0x81); // Set contrast
    sh1106_send_command(0xFF);

    sh1106_send_command(0xD9); // Set pre-charge period
    sh1106_send_command(0x1F);

    sh1106_send_command(0xDB); // Set VCOM deselect level
    sh1106_send_command(0x40);

    sh1106_send_command(0xA4); // Resume RAM content display
    sh1106_send_command(0xA6); // Normal display (not inverted)

    sh1106_send_command(0xAF); // Display ON
}

// Set cursor: page (0-7), col (0-127)
void sh1106_set_cursor(uint8_t page, uint8_t col) {
    col += SH1106_COL_OFFSET; // offset bắt buộc với SH1106
    sh1106_send_command(0xB0 | (page & 0x07));           // page address
    sh1106_send_command(0x00 | (col & 0x0F));            // low nibble col
    sh1106_send_command(0x10 | ((col >> 4) & 0x0F));     // high nibble col
}

void sh1106_clear() {
    for (int page = 0; page < 8; ++page) {
        sh1106_set_cursor(page, 0);
        uint8_t zero[128] = {0};
        sh1106_send_data(zero, 128);
    }
}

// Font 5x8 cho các ký tự cần dùng
static const uint8_t font5x8[][5] = {
    [' ' - 32] = {0x00, 0x00, 0x00, 0x00, 0x00},
    ['!' - 32] = {0x5F, 0x00, 0x00, 0x00, 0x00},
    ['H' - 32] = {0x7F, 0x08, 0x08, 0x08, 0x7F},
    ['e' - 32] = {0x3C, 0x4A, 0x4A, 0x4A, 0x30},
    ['l' - 32] = {0x00, 0x41, 0x7F, 0x40, 0x00},
    ['o' - 32] = {0x38, 0x44, 0x44, 0x44, 0x38},
    ['W' - 32] = {0x7C, 0x02, 0x0C, 0x02, 0x7C},
    ['r' - 32] = {0x7C, 0x08, 0x04, 0x04, 0x08},
    ['d' - 32] = {0x38, 0x44, 0x44, 0x44, 0x7F},
};

void sh1106_write_text(const char *text, uint8_t page, uint8_t col) {
    sh1106_set_cursor(page, col);
    while (*text) {
        if (*text < 32 || *text > 127) { text++; continue; }
        uint8_t buf[6];
        memcpy(buf, font5x8[(uint8_t)(*text - 32)], 5);
        buf[5] = 0x00; // 1 pixel spacing
        sh1106_send_data(buf, 6);
        text++;
    }
}

int main() {
    i2c_fd = open(I2C_DEV, O_RDWR);
    if (i2c_fd < 0) { perror("Open I2C"); return 1; }

    if (ioctl(i2c_fd, I2C_SLAVE, I2C_ADDR) < 0) {
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
