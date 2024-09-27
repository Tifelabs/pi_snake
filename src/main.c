#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <ncurses.h>
#include <unistd.h>  // For usleep
#include <time.h>

#define LCD_ADDR 0x27   // I2C address for your LCD, change this if necessary
#define LCD_CHR  1      // Mode - Sending data
#define LCD_CMD  0      // Mode - Sending command
#define LCD_BACKLIGHT 0x08  // On
#define ENABLE  0b00000100  // Enable bit

// LCD Parameters
#define LCD_WIDTH 16    // Maximum characters per line
#define LCD_LINE_1 0x80 // LCD RAM address for the 1st line
#define LCD_LINE_2 0xC0 // LCD RAM address for the 2nd line

int fd;  // I2C file descriptor

// Prototypes
void lcdSendByte(int bits, int mode);
void lcdInit();
void lcdClear();
void lcdPosition(int row, int col);
void lcdPuts(const char *str);
void lcdToggleEnable(int bits);

enum direction {UP, DOWN, LEFT, RIGHT};

typedef struct {
    int x, y;
} Point;

Point snake[32];
int snakeLength = 1;
Point food;
enum direction dir = RIGHT;

void initNcurses() {
    initscr();
    timeout(100);  // Non-blocking input every 100 ms
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
}

void moveSnake() {
    for (int i = snakeLength - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    switch (dir) {
        case UP:    snake[0].y = (snake[0].y - 1 + 2) % 2; break;
        case DOWN:  snake[0].y = (snake[0].y + 1) % 2; break;
        case LEFT:  snake[0].x = (snake[0].x - 1 + 16) % 16; break;
        case RIGHT: snake[0].x = (snake[0].x + 1) % 16; break;
    }
}

void generateFood() {
    int overlap;
    do {
        overlap = 0;
        food.x = rand() % 16;
        food.y = rand() % 2;

        for (int i = 0; i < snakeLength; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                overlap = 1;
                break;
            }
        }
    } while (overlap);
}

void checkFood() {
    if (snake[0].x == food.x && snake[0].y == food.y) {
        snakeLength++;
        generateFood();
    }
}

void render() {
    lcdClear();
    char display[17];

    for (int row = 0; row < 2; row++) {
        for (int i = 0; i < 16; i++) display[i] = ' ';
        display[16] = '\0';

        for (int i = 0; i < snakeLength; i++) {
            if (snake[i].y == row) {
                display[snake[i].x] = (i == 0) ? 'O' : 'o';
            }
        }

        if (food.y == row) {
            display[food.x] = '*';
        }

        lcdPosition(row, 0);
        lcdPuts(display);
    }
}

void handleInput() {
    int ch = getch();
    switch (ch) {
        case KEY_UP:    if (dir != DOWN) dir = UP; break;
        case KEY_DOWN:  if (dir != UP) dir = DOWN; break;
        case KEY_LEFT:  if (dir != RIGHT) dir = LEFT; break;
        case KEY_RIGHT: if (dir != LEFT) dir = RIGHT; break;
        case 'q':       endwin(); exit(0);
    }
}

int main() {
    srand(time(NULL));  // Seed the random number generator
    wiringPiSetup();

    fd = wiringPiI2CSetup(LCD_ADDR);  // I2C setup for the LCD
    if (fd == -1) {
        printf("Failed to init I2C communication.\n");
        return -1;
    }

    lcdInit();

    snake[0].x = 8;
    snake[0].y = 1;
    generateFood();  // Initial food generation

    initNcurses();

    while (1) {
        handleInput();
        moveSnake();
        checkFood();
        render();
        usleep(200000);  // Slow down the game loop (200ms)
    }

    endwin();
    return 0;
}

// LCD Functions
void lcdSendByte(int bits, int mode) {
    int bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    int bits_low  = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    wiringPiI2CWrite(fd, bits_high);
    lcdToggleEnable(bits_high);

    wiringPiI2CWrite(fd, bits_low);
    lcdToggleEnable(bits_low);
}

void lcdToggleEnable(int bits) {
    usleep(500);
    wiringPiI2CWrite(fd, (bits | ENABLE));
    usleep(500);
    wiringPiI2CWrite(fd, (bits & ~ENABLE));
    usleep(500);
}

void lcdInit() {
    lcdSendByte(0x33, LCD_CMD);  // Initialize
    lcdSendByte(0x32, LCD_CMD);  // Initialize
    lcdSendByte(0x06, LCD_CMD);  // Cursor move direction
    lcdSendByte(0x0C, LCD_CMD);  // Turn cursor off
    lcdSendByte(0x28, LCD_CMD);  // 2 line display
    lcdSendByte(0x01, LCD_CMD);  // Clear display
    usleep(500);
}

void lcdClear() {
    lcdSendByte(0x01, LCD_CMD);
    usleep(500);
}

void lcdPosition(int row, int col) {
    int addr = (row == 0) ? LCD_LINE_1 : LCD_LINE_2;
    lcdSendByte(addr + col, LCD_CMD);
}

void lcdPuts(const char *str) {
    while (*str) {
        lcdSendByte(*str++, LCD_CHR);
    }
}
