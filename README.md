# Convert a black and white PNG image into Jack code
For example ![alt text](data/cactus.png) is converted into 

```java
method void draw(int x, int y) {
    do Screen.drawRectangle(x+18,y,x+27,y+91);
    do Screen.drawRectangle(x+16,y+2,x+17,y+91);
    do Screen.drawRectangle(x+28,y+2,x+29,y+91);
    do Screen.drawRectangle(x+38,y+20,x+43,y+53);
    do Screen.drawRectangle(x+36,y+22,x+37,y+59);
    do Screen.drawRectangle(x+44,y+22,x+45,y+51);
    do Screen.drawRectangle(x+2,y+24,x+7,y+57);
    do Screen.drawRectangle(x,y+26,x+1,y+55);
    do Screen.drawRectangle(x+8,y+26,x+9,y+61);
    do Screen.drawRectangle(x+10,y+52,x+15,y+61);
    do Screen.drawRectangle(x+30,y+52,x+35,y+59);
    do Screen.drawRectangle(x+38,y+54,x+41,y+55);
    do Screen.drawRectangle(x+38,y+56,x+39,y+57);
    do Screen.drawRectangle(x+4,y+58,x+7,y+59);
    do Screen.drawRectangle(x+6,y+60,x+7,y+61);
    do Screen.drawRectangle(x+12,y+90,x+15,y+91);
    return;
}  
```

Algorithm will group individual pixels into rectangles and lines. There is a flag to disable this

## Instalation and run


1) To install run the next command. By default, it will copy the binary to `~/bin`. 
You can change this in [CmakeLists.txt](CMakeLists.txt) by changing `install(TARGETS ${PROJECT_NAME} DESTINATION ~/bin)` 
```shell
make install
```
2) Run the utility. For example 
```shell
jack-image-converter data/cactus.png
```
Type `jack-image-converter --help` for all available options

## Dependencies:
By default, this utility will copy the output to clipboard. To use this you would probably need to install some OS package(-s).
More info on https://github.com/dacap/clip?tab=readme-ov-file#platform-specific-details

You can use `-o` flag so  that this utility will write to standard output.

