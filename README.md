# 🐍 Pi Snake Game with 16x2 LCD 

Hi guys, I'm currently working on a **Snake Game** built with the use of **Raspberry Pi4**, **16x2 LCD Screen** and some **Jumper Wires**.
You should be able to control the game with your **keyboard** to have the real experience.

## 🎮 Game Details

Just some regular snake game style we do play on **Nokia 3310** hahhaaa, you should be able control the movement of a snake using the arrow keys on a keyboard. The snake grows when it eats the food, and the game wraps around the edges of the 16x2 LCD screen. The goal is to grow the longest snake possible.

---

## 🖥️ Features

- **16x2 LCD Display**: Play the game on a retro-style LCD screen.
- **Keyboard Control**: Move the snake using the arrow keys.
- **Snake Growth**: Snake grows as it eats food.
- **Food Generation**: Randomly generated food on the screen.
- **Edge Wrapping**: The snake wraps around the screen when it hits the edge.

## 🤖 Hardware
![Took kit](./image/hardward.PNG) | 🎮 Use arrow keys to control! |

---

## 📷 Demo

| Snake Gameplay     | Keyboard Controls          |
| ------------------ | -------------------------- |
| ![Snake on LCD](./images/snake_lcd_demo.png) | 🎮 Use arrow keys to control! |

---

## 🛠️ Installation and Setup

Follow these steps to get the game running on your Raspberry Pi.

### 1. Prerequisites

Ensure your Raspberry Pi has the following libraries installed:

```bash
sudo apt-get install wiringpi libncurses5-dev
# pi_snake
