# README
FreeRTOS (tasks) using STM32CubeIDE
This project is an embedded system designed to control 3 LED to blink using a STM32 microcontroller board.

# Hardware Requirements
The following components are required to build the project:

STM32F446 Nucleao-64 board

# Software Requirements
STM32CubeIDE 1.11.2

# setting

I Rename the DefaultTask to RedLED

AND two more tasks named GreenLED and BlueLED

Change Priority to osPriorityNormal for all tasks

Set PA0, PA1 and PA4 GPIO_Output
