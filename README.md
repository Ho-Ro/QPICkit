# QPICkit

![QPICkit](QPICkit.png)

Simple and quick [pk2cmd](https://github.com/cjacker/pk2cmd-minus) front end -
a GUI for programming PIC microcontrollers with the
[PICkit™ 2 Development Programmer/Debugger](https://www.microchip.com/en-us/development-tool/pg164120) and
[PICkit™ 3 In-Circuit Debugger/Programmer](https://www.microchip.com/en-us/development-tool/pg164130) tools.

This combination supports the majority of the PIC family (10F, 12F, 16F, 18F, 24F, 32F, as well as dsPIC30 and dsPIC33)
and many serial EEPROMS (24 and 25 series) - currently over 1500 devices.

The GUI supports also these programmer HW/SW combinations - but only for a limited PIC device range:

- [ArdPicProg](https://github.com/Ho-Ro/ArdPicProg)
DIY programmer hard- and software.
- [PICkit1](https://github.com/Ho-Ro/PICkit1) software for the
[PICkit™ 1 Flash Starter Kit](https://www.microchip.com/en-us/development-tool/dv164101).

Other programmer can easily be added to the file [`programmer.cpp`](programmer.cpp).

## Building

You must have `qt`, `qmake` and `make` installed in your system.
The program was tested with Qt5 and Qt6 on Debian stable
using PICkit1, PICkit2, and ArdPicProg.

run:

```
qmake QPICkit.pro
make all -j4
make clean
```
