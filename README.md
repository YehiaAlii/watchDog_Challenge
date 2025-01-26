# WatchDog Challenge

## Overview
This project demonstrates the use of a watchdog timer on an Arduino to handle system resets and recover from hangs. It includes EEPROM state management and LED indicators.

## Features
- **Watchdog Timer**: Automatically resets the Arduino if it hangs.
- **EEPROM State Management**: Saves and retrieves execution state to/from EEPROM.
- **LED Indicators**: Uses two LEDs to indicate the current state.

## Components
- Arduino Uno
- Two LEDs (Red and Blue)
- Connecting wires

## Circuit Diagram
![arduino_watchdog_setup png](https://github.com/user-attachments/assets/1d7a94cf-aef6-4903-a018-4a29fff344d2)

## Setup
1. Connect the components as shown in the circuit diagram.
2. Upload the code to your Arduino.

## Code Explanation
- **Watchdog Timer**: Configured to reset the system if it hangs.
- **EEPROM**: Stores the execution state and interrupt flag.
- **LEDs**: Indicate the current execution state.

## Usage
1. Power the Arduino.
2. Observe the LED behavior.
3. The system will reset if it hangs, and the state will be restored from EEPROM.
