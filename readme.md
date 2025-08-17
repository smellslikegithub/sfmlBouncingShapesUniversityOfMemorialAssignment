# Assignment

## Some prerequisites
- Cannot use multiple files such as .hpp/header files etc. All the code must be in a single main.cpp file. Kind of counter intuitive but probably easier to grade since there is one fiel to deal with.
- Linux users need to have cmake for building the project
- Avoid using OOP and keep things simple.

## Assignment
User populates a config.txt file that contains space seperated tokens
Acceptable Tokens: [Window, Font, Circle, Rectangle]
I added a 'LsdAcidHighMode' token for fun. Not part of the assignment but what the heck! A liitle anarchy hurst no one! :D
- Line 1 and line 2 should always contain 
  - Window Width Height
  - Font fontpath fontsize fontcolor(rgb)
- Rest of the lines will have shapes like Circle/Rectangle
  - Rectangle label(text inside the shape) xPos yPos x:velocity y:velocity red green blue width height
  - Circle label(text inside the shape), xPos, yPos x:velocity y:velocity red green blue radius


Example of the config.txt

```js
Window 1920 1080
Font assets/fonts/MondayFeelings.ttf 20 128 128 255
LsdAcidHighMode 0
Circle CGreen 100 100 -0.3 0.2 0 255 100 50
Circle CBlue 200 200 0.2 0.4 0 0 255 100
Circle CPurple 300 300 -0.11 0.22 255 0 255 75
Rectangle RRed 200 200 0.2 0.1 255 0 0 500 100
Rectangle RGrey 300 250 -0.5 0.76 100 100 100 50 100
Rectangle RTeal 25 100 -0.22 -0.11 0 255 255 250 250