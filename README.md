This program uses BLEKeyboard.h to send ALT+CTRL+[a-z] codes to PC to open any shortcut you create. 
Exammple:  Create a Web site shortcut.  Open shortcut and select properties.  Selet Shortcut tab.  Edit shortcut by typing a 
letter that is related to the program or web site.
During build use ESP32 vesion 2.0.17 or later.  Version 3.x.x will not work anymore in Arduino IDE

Edit line 35 int Action[] = {0x61, 0x63, 0x64, 0x66, 0x67, 0x6c, 0x79, 0x70, 0x71};  // [A, C, D, F]  [G, L, Y, P]  [N, M, ?, ?] [?, ?, ?, A-C-D]
0x61 = A, 0x63 = C ... etc
The program will sent a ALT+CTRL+A to PC
