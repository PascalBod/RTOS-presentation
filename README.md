# An introduction to the use of an RTOS for Machine Learning

## Overview

This repository contains material providing an introduction to the use of an RTOS (Real-Time Operating System) on a microcontroller for developing Machine Learning applications:
* Some introductory material about hardware and software
* What an RTOS is and what it provides
* Some practical exercises, using the EFR32MG24 as target

This presentation is based on the [course](https://github.com/PascalBod/IMTAtlantique-2024) taught at [IMT Atlantique](https://www.imt-atlantique.fr/en) and is currently in construction, being adapted to another target board than the one used in the IMT Atlantique course. Additionally, some parts will be enriched. Revisited chapters will be provided one by one, after the already available introductory part.

This presentation DOES NOT provide any information about Machine Learning topic.

## Prerequisites for the practical exercises

### Software

The provided exercises will rely on the environment development presented [here](https://github.com/PascalBod/lm-efr32-simplicityStudio): Silicon Labs Simplicity Studio installed in a Linux Mint Xfce Virtual Machine.

Using a virtual machine allows to isolate all specific configuration relating to Simplicity Studio from your other projects. A drawback is that the host computer must be powerful enough and must have more memory.

The above preconfigured virtual machine will be made available. It will be an x86_64 version.

### Hardware

The exercises will use the following hardware:
* [EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview)

### Software development competencies

* Basic knowledge of *git* - [git user manual](https://git-scm.com/docs/user-manual)
* Basic knowledge of *GitHub* - [About GitHub and Git](https://docs.github.com/en/get-started/start-your-journey/about-github-and-git)
* Basic knowledge of *Linux* (knowing the most common commands...) - [An Introduction to Linux Basics, from DigitalOcean](https://www.digitalocean.com/community/tutorials/an-introduction-to-linux-basics)
* Basic knowledge of *VirtualBox* (knowing how to create a virtual machine...) - [VirtualBox end-user documentation](https://www.virtualbox.org/wiki/End-user_documentation)
* Good knowledge of one programming language

C language is used, for the exercises. [This Standford Computer Science Education document](http://cslibrary.stanford.edu/101/EssentialC.pdf) provides a good presentation of the language. 

### What to do before going through the presentation

1. Install *Simplicity Studio* on your computer. It's a good thing to install it in a virtual machine as presented in the *Software* section above. This way, you can reproduce the exact same environment than the presented one. You can also directly install  Simplicity Studio in your usual environment, but in this case, you are on your own.
2. Get an EFR32xG24 Dev Kit (see the link provided in the *Hardware* section above.
3. Ensure that you can build and program the sample application, as presented in the *lm-efr32-simplicityStudio* repository, introduced in the *Software* section above.

## Presentation

The presentation is at this URL: https://pascalbod.github.io/RTOS-presentation. 

Source code of the presentation is in the `docs` directory. 

The presentation is licensed under the  [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-nc-sa/4.0/).