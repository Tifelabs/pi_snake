# 🐍 Raspberry Pi Snake Game with 16x2 LCD and Keyboard Control

Welcome to the **Snake Game** built for the **Raspberry Pi**, displayed on a **16x2 LCD** and controlled with your **keyboard**. This retro-style game brings the classic snake game to your Pi while working with a minimalistic display.

## 🎮 Game Overview

In this project, we control the movement of a snake using the arrow keys on a keyboard. The snake grows when it eats the food, and the game wraps around the edges of the 16x2 LCD screen. The goal is to grow the longest snake possible.

---

## 🖥️ Features

- **16x2 LCD Display**: Play the game on a retro-style LCD screen.
- **Keyboard Control**: Move the snake using the arrow keys.
- **Snake Growth**: Snake grows as it eats food.
- **Food Generation**: Randomly generated food on the screen.
- **Edge Wrapping**: The snake wraps around the screen when it hits the edge.

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
