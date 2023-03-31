# Interfacing-External-EEPROM-Storage-With-PIC16F877A-Microcontroller-Using-SPI-Communication-Protocol

Contributors: [Gayathri R]( https://github.com/Gayathri-R7 ),  [Rasiga T D]( https://github.com/RASIGATD ), Nandita S




## Overview

In this project, the PIC microcontroller is interfaced to load and retrieve data from external memory. The user-defined data is provided as the input via the source code. To execute the code, the MPLAB IDE is used along with the XC8 compiler. The code is dumped into the PIC microcontroller by employing the PIC ISP software. The programmed PIC writes the data into the EEPROM chip via the SPI protocol. Then, the data present in the EEPROM chip is read by the PIC microcontroller and displayed by making use of a 16x2 LCD.

The data and program memory available in a PIC is limited. To optimize the working of the PIC microcontroller, various kinds of external memory like Flash memory, EEPROM are used. The communication of the PIC microcontroller with other memory devices is achieved via different protocols.
Non-volatile storage is a type of computer memory that can retain the stored information even after power is removed. A classic example of non-volatile memory is ROM. An EEPROM is a more advanced version of ROM, which is used to store relatively small amounts of data by allowing individual bytes to be erased and reprogrammed. While programming the PIC microcontroller to interface it with external peripherals, a wired communication protocol is chosen that effectively facilitates internal communication. I2C (Inter-Integrated circuit) protocol or the SPI (serial peripheral interface) protocol is commonly used for this purpose.

## Components Used 

| S.No      |  Name  | Specification |
| ----------- | ----------- | ------------- |
| 1           | Microcontroller       |PIC16F877A
| 2           | EEPROM       | FM93C66A
| 3           | LCD / 7 Segment LED display       |LM016L
| 4           | Potentiometer       |10 k Ohms 
| 5           | Connecting wires       |-


## Scope for Extending our work

 - This project can further be extended, by interfacing different sensors, from which the acquired data can be transmitted and displayed utilizing the SPI protocol. Data acquired from external sensors such as Temperature or Pressure sensors can be stored in the external EEPROM and can be accessed when need be. 

- Data written in the EEPROM chip can be displayed on the LCD Screen in marquee format

- A Thin Film Transistor LCD Screen can be integrated instead of the character-based LCD for reading image files.

- This project can be extended further to ARM Microcontrollers for high-speed data paths, lower latency, and better battery life.
