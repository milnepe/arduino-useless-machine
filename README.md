Arduino Useless Machine
=======================

0.1 Release

Copywrite 2016 Peter Milne

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or 
(at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program.  If not, see [www.gnu.org/licenses](<http://www.gnu.org/licenses/>).

Code & schematics for building useless machine that appears to switch itself off.
This build has the following features:
* Low power sleep mode - will remain "ON" for several months when powered by 4 AAA cells
* Separate Lid and Arm servos
* MOSFET switch for reduced power consumption
* Reset button
* Debugging LED
* 10 different Lid / Arm movement sequences

Once powered up by 4 AAA 1.2V rechargeable batteries, the box enters sleep mode. Turning switch on
wakes up the processor using ISR, this signals the MOSFET to open and then servos can activate to given sequence.
The Lid servo opens the box lid and the Arm servo turns switch off, the processor then goes back to sleep.  

Inspired by Arvid Mortensen's brilliant machine
[www.lamja.com](http://www.lamja.com/?p=451)

Power saving from code by Nick Gammon
[www.gammon.com.au/power](http://www.gammon.com.au/power)

