#include <stdio.h>
#include <wiringPiI2C.h> 

//What I did was to clone the repo and cd build
#include <wiringPi.h>// This lib wont sudo install on the pi.
#include <unistd.h>

// I2C address of the LCD (usually 0x27 or 0x3F)
#define LCD_ADDR 0x27

// Commands for the LCD
#define LCD_CHR  1  // This is for Sending data
#define LCD_CMD  0  // Sending command

// LCD Line addresses
#define LCD_LINE_1  0x80 // LCD RAM address for the 1st line
#define LCD_LINE_2  0xC0 // LCD RAM address for the 2nd line

// Enable bit and Backlight control
#define LCD_ENABLE  0b00000100  // Enable bit
#define LCD_BACKLIGHT  0x08     // Backlight on

// Timing constants
#define E_DELAY 500

int fd; // File descriptor for I2C communication

// Function prototypes
void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);
void lcd_clear(void);
void lcd_string(const char* message, int line);

int main() {
    // Initialize wiringPi I2C and set up the LCD
    fd = wiringPiI2CSetup(LCD_ADDR);

    // Initialize the LCD
    lcd_init();

    // Display text on the LCD
    lcd_string("Hello, World!", LCD_LINE_1);
    lcd_string("Snake on Pi!", LCD_LINE_2);

    // Keep the display on
    sleep(10);

    // Clear the LCD before exiting
    lcd_clear();

    return 0;
}

// Initialize the LCD display
void lcd_init() {
    lcd_byte(0x33, LCD_CMD); // Initialize
    lcd_byte(0x32, LCD_CMD); // Initialize
    lcd_byte(0x06, LCD_CMD); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD); // Turn cursor off
    lcd_byte(0x28, LCD_CMD); // 2 line display
    lcd_byte(0x01, LCD_CMD); // Clear display
    usleep(E_DELAY * 2000);
}

// Send byte to the data pins
void lcd_byte(int bits, int mode) {
    int bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    int bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    // High bits
    wiringPiI2CWrite(fd, bits_high);
    lcd_toggle_enable(bits_high);

    // Low bits
    wiringPiI2CWrite(fd, bits_low);
    lcd_toggle_enable(bits_low);
}

// Toggle enable pin
void lcd_toggle_enable(int bits) {
    usleep(E_DELAY);
    wiringPiI2CWrite(fd, (bits | LCD_ENABLE));
    usleep(E_DELAY);
    wiringPiI2CWrite(fd, (bits & ~LCD_ENABLE));
    usleep(E_DELAY);
}

// Clear the display
void lcd_clear() {
    lcd_byte(0x01, LCD_CMD);
    usleep(E_DELAY * 2000);
}

// Display a string on the LCD
void lcd_string(const char* message, int line) {
    lcd_byte(line, LCD_CMD);
    while (*message) {
        lcd_byte(*message++, LCD_CHR);
    }
}
