# An introduction to the use of an RTOS for Machine Learning

## Overview

This repository contains material providing an introduction to the use of an RTOS on a microcontroller for developing Machine Learning applications:
* Some introductory material about hardware and software
* What an RTOS is and what it provides
* Some practical exercises, using the ESP32-C6 as target

This presentation is based on the [course](https://github.com/PascalBod/IMTAtlantique-2024) taught at [IMT Atlantique](https://www.imt-atlantique.fr/en) and is currently in construction, being adapted to another target board than the one used in the IMT Atlantique course. Additionally, some parts will be enriched. Revisited chapters will be provided one by one, after the already available introductory part.

This presentation DOES NOT provide any information about Machine Learning topic.

## Prerequisites for the practical exercises

### Software

The exercises which will be provided will rely on the environment development presented [here](https://github.com/PascalBod/lm-esp32-eclipse): Espressif-IDE installed in a Linux Mint Xfce Virtual Machine.

Using a virtual machine allows to isolate all specific configuration relating to Espressif-IDE from your other projects. A drawback is that the host computer must be powerful enough and must have more memory.

Apple MacOS or Microsoft Windows can be used to install and run Espressif-IDE. But things are easier and more reliable with Linux.

### Hardware

The exercises will use the following hardware:
* [ESP32-C6-DevKitM-1](https://docs.espressif.com/projects/espressif-esp-dev-kits/en/latest/esp32c6/esp32-c6-devkitm-1/user_guide.html)
* One USB C to USB A cable, or USB C to USB C, depending on your computer
* One breadboard
* One temperature, humidity and pressure sensor - interface: I2C and SPI
* Two LEDs
* Jumper wires
* Resistors

A detailed list will be provided soon, with links to online shops where to buy.

## Presentation

The presentation is at this URL: https://pascalbod.github.io/RTOS-presentation. 

Source code of the presentation is in the `docs` directory. 

The presentation is licensed under the  [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-nc-sa/4.0/).
