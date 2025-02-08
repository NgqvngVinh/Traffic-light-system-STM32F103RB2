---

<h1 align="center">Traffic Light Control Application System on STM32F103RB2</h1>




This repository hosts a traffic light control application designed for an intersection with adjustable timing in both **automatic** and **manual** modes. The system utilizes mechanical buttons with software debouncing to manage the three phases—red, amber, and green. Developed using the **STM32F103RB2**, it implements a Finite State Machine (FSM) to manage various modes. The system has been validated on real hardware, specifically on an STM32F103RB2 board, and features an LCD 16x2 display for real-time information along with debounced mechanical buttons for mode selection and timing adjustments. This project is an enhanced version of the **Traffic-light-STM32F103C6** project.


## Table of Contents
1. [Project Overview](#project-overview)  
2. [Features](#features)  
3. [Hardware Requirements](#hardware-requirements)  
4. [Languages and Tools](#Languages-and-Tools)  
5. [Project Structure](#project-structure)  
6. [Mode Descriptions](#mode-descriptions)  
7. [How to Build and Run](#how-to-build-and-run)  
8. [Implementing on Hardware](#Implementing-on-Hardware)  


---

## 1. Project Overview

This project simulates a **traffic light system** at a crossroads with the following characteristics:

- **12 LEDs** (4 red, 4 amber, 4 green) corresponding to two directions of traffic.
- **4 seven-segment displays** (two per road) to show countdown time and mode number.
- **3 buttons**:
  1. **Mode Selection**: Cycles through 4 modes  
  2. **Time Increase**: Increments the time duration in modification modes
  3. **Time Set**: Confirms and stores the chosen time.

An **FSM (Finite State Machine)** is used to manage the modes, transitions, and LED blinking behavior. The design also includes software-based button debouncing and a flexible timer configuration to ensure stable performance.

### Main Goals

1. **Normal Traffic Light Operation** (Mode 1).  
2. **On-the-Fly Modification** of Red, Amber and Green Durations (Modes 2, 3, 4).  
3. **Stable Timer and Debouncing**: Changing timer interrupt intervals without affecting the main application logic.

---

## 2. Features

- **Operating Modes (4 total)**:
  1. **Normal Mode**: Standard traffic light sequence  
  2. **Modify Red Duration**  
  3. **Modify Amber Duration**  
  4. **Modify Green Duration**  

- **Real-Time Display** on LCD 16x2 using **I2C protocol** showing:
  - Current countdown time for each color.
  - Current mode indication during editing.

- **Blinking LEDs** in modification modes (2 Hz) to indicate the active editing state and also which color is being edited.

- **Button Debouncing** using software debouncing method for all 3 push buttons to ensure stable input readings.

- **Adjustable Timer Interrupt** (10 ms default) that can be configured to 1 ms or 100 ms without impacting the overall system behavior.

---

## 3. Hardware Requirements

- **STM32F103RB2 board**
- **12 LEDs**: 4 red, 4 amber, 4 green  
- **16x2 I2C LCD Displays** (common anode or cathode, depending on your design)  
- **3 Push Buttons** for:
  1. Mode Selection: to select modes
  2. Time Increase: to modify the time for each color LED on the fly
  3. Time Set: to set the chosen value

---

## 4. Languages and Tools

### Languages

<div align="center">
  <a href="https://en.wikipedia.org/wiki/C_(programming_language)">
    <img src="https://upload.wikimedia.org/wikipedia/commons/1/18/C_Programming_Language.svg" alt="C Programming Logo" width="50">
  </a>
  <a href="https://en.wikipedia.org/wiki/Assembly_language" target="_blank" style="margin-left: 10px;">
    <img src="https://img.icons8.com/color/100/assembly.png" alt="Assembly Language Logo" width="60">
  </a>
</div>

### Tools

- **STM32CubeIDE**: [STM32CubeIDE Download](https://www.st.com/en/development-tools/stm32cubeide.html)  
- **STM32CubeProgrammer**: [STM32CubeProgrammer Download](https://www.st.com/en/development-tools/stm32cubeprog.html)  

- **Proteus Simulation Software**: [Proteus Information](https://www.labcenter.com/)

Ensure your development environment is configured for STM32 microcontrollers.

---

## 5. Project Structure

```
.
├── stm32_rb/                          # Main source directory for the STM32RB project
│   ├── .settings/                     # Project settings and preferences
│   ├── Core/                          # Core source code and headers
│   │   ├── Inc/                       # Header files
│   │   │   ├── button.h              
│   │   │   ├── fsm_automatic.h       
│   │   │   ├── fsm_manual.h          
│   │   │   ├── global.h              
│   │   │   ├── i2c-lcd.h             
│   │   │   ├── led_7_seg.h             # This file is not used
│   │   │   ├── led_states.h          
│   │   │   ├── main.h          
│   │   │   ├── software_timer.h      
│   │   │   └── ...             
│   │   └── Src/                      # Source code files
│   │       ├── button.c              # Button reading and debouncing logic
│   │       ├── fsm_automatic.c       # FSM implementation for normal mode
│   │       ├── fsm_manual.c          # FSM implementation for the whole system (Mode 1-4)
│   │       ├── global.c              # Global variables and timing settings
│   │       ├── i2c-lcd.c             # LCD display implementation
│   │       ├── led_7_seg.c           # This file is not used
│   │       ├── led_states.c          # Implement all the states of 12 LEDs
│   │       ├── main.c                # Main loop integrating FSM 
│   │       ├── software_timer.c      # Timer configuration and interrupt 
│   │       └──...                 
│       └── Startup
│   ├── Debug/                         # Debug build output (hex file in here)
│   ├── Drivers/                       # STM32 drivers and libraries
│   ├── STM32F103RBTX_FLASH.ld         # Linker script for STM32F103RBT6
│   ├── STM32_RB_Traffic_light_sys.ioc # STM32CubeMX configuration file
│   ├── .cproject                      # Eclipse project configuration
│   ├── .mxproject                     # STM32CubeMX project metadata
│   ├── .project                       # Eclipse project metadata
│   
|        
├── .project
├── README.md                            # You're reading this file
├── Traffic-light-system-STM32F103RB2-Demo.gif

```

### Notable Files

- **main.c**: Initializes hardware, put fsm logic in main loop.  
- **fsm.c / fsm.h**: Contains the FSM logic for mode control.  
- **button.c / button.h**: Button reading (press, hold) and software debouncing.  
- **i2c-lcd.c / i2c-lcd.h**: I2C protocol functions to display mode and time on the 16x2 LCD.  
- **timer.c / timer.h**: Timer setup for periodic interrupts (default 10 ms).

---

## 6. Mode Descriptions

1. **Mode 1 - Normal Mode**  
   - Traffic lights operate normally with fixed durations for Red, Amber, and Green. 
   - 7-segment displays show the remaining time for the current LED color in each direction.

2. **Mode 2 - Modify Red LEDs**  
   - Red LEDs blink at **2 Hz** to indicate we are editing their time duration.  
   - Use the **second button** to increase the time (1–99 seconds).  
   - Press the **third button** to confirm and save the new time.  
   - The 7-segment displays show the **mode number** and the **current time** for Red.

3. **Mode 3 - Modify Amber LEDs**  
   - Similar behavior to Mode 2, but for the Amber LED duration.

4. **Mode 4 - Modify Green LEDs**  
   - Similar behavior to Mode 2, but for the Green LED duration.

Press the **first button** at any time to cycle through modes (1 → 2 → 3 → 4 → 1 → ...). Initially, the system runs with 5 seconds for Red, 3 seconds for Amber, and 2 seconds for Green. After that, when it returns to mode 1, the system runs with the new setting time or the same time if the user does not modify anything.

---

## 7. How to Build and Run

1. **Clone the Repository**  
   ```bash
   git clone https://github.com/NgqvngVinh/Traffic-light-system-STM32F103RB2.git
   cd Traffic-light-system-STM32F103RB2
   ```

2. **Open Project** in your preferred STM32 IDE (recommend: **STM32CubeIDE**).

3. **Check Pin Assignments**: Ensure the pin assignments in the `.ioc` file (STM32_RB_Traffic_light_sys.ioc) match your actual hardware connections (LEDs, buttons, LCD).

4. **Build the Project**  
   - Compile the code (Ctrl + B) and generate the `.hex` file.
   - If the `.hex` file is not generated, go to C/C++ Build > Settings > MCU Post Build Outputs and enable the Intel Hex file generation.

5. **Proteus Simulation** (Optional)
   - Open Proteus and configure the necessary hardware components for the simulation.
   - Double-click on the MCU component and load the compiled firmware (`.hex` file) into the virtual MCU. 
   - Run the simulation to verify functionality.
   - **Verify Operation**  
         - Upon start or reset, the system begins in **Mode 1 (Normal)**.  
         - Press **Button 1** to cycle modes.  
         - In Modes 2–4, use **Button 2** to increment time, then press **Button 3** to set it.

     


---

## 8. Implementing on Hardware
Here is the demo of the traffic light system, showing real-time operation including mode transitions, time adjustments, and LED control.
You can watch the full demo video here: [VIDEO](https://drive.google.com/file/d/1-h3BXGX-usQDUE6SucHsm1V2tB6qQ1rZ/view)



![Demo](./Traffic-light-system-STM32F103RB2-Demo.gif)

---

**Thank you for checking out this project!**  
If you have any questions or suggestions, feel free to open an issue or submit a pull request.

# Author
<div align="center">
<a href="https://github.com/NgqvngVinh" target="_blank">
  <img src="https://github.com/NgqvngVinh.png" alt="Avatar của tôi" width="60" style="border-radius: 50%;">
</a>
</div>

