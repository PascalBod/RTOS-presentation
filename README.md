# An introduction to the use of an RTOS for Machine Learning

## Overview

This repository contains material providing an introduction to the use of an RTOS on a microcontroller for developing Machine Learning applications:
* Some introductory material about hardware and software
* What an RTOS is and what it provides
* Some practical exercises, using the EFR32BG24 as target

This presentation is based on the [course](https://github.com/PascalBod/IMTAtlantique-2024) taught at [IMT Atlantique](https://www.imt-atlantique.fr/en) and is currently in construction, being adapted to another target board than the one used in the IMT Atlantique course. Additionally, some parts will be enriched. Revisited chapters will be provided one by one, after the already available introductory part.

This presentation DOES NOT provide any information about Machine Learning topic.

## Prerequisites for the practical exercises

### Software

The provided exercises will rely on the environment development presented [here](https://github.com/PascalBod/lm-efr32-simplicityStudio): Silicon Labs Simplicity Studio installed in a Linux Mint Xfce Virtual Machine.

Using a virtual machine allows to isolate all specific configuration relating to Espressif-IDE from your other projects. A drawback is that the host computer must be powerful enough and must have more memory.

Apple MacOS or Microsoft Windows can be used to install and run Simplicity Studio. But things are easier and more reliable with Linux.

### Hardware

The exercises will use the following hardware:
* [EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview)

### What to do before going through the presentation

1. Install *Simplicity Studio* on your computer. It's a good thing to install it in a virtual machine as presented in the *Software* section above. This way, you can reproduce the exact same environment than the presented one. You can also directly install  Simplicity Studio in your usual environment, but in this case, you are on your own.
2. Get an EFR32xG24 Dev Kit (see the link provided in the *Hardware* section above.
3. Ensure that you can build and program the sample application, as presented in the *lm-efr32-simplicityStudio* repository, introduced in the *Software* section above.

## Presentation

The presentation is at this URL: https://pascalbod.github.io/RTOS-presentation. 

Source code of the presentation is in the `docs` directory. 

The presentation is licensed under the  [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-nc-sa/4.0/).