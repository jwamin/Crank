# Crank

![Set](pd-test.png?raw=true "Screenshot")

## Playdate "Hello world" application in C, with Crank and button action.

### Features 

* Draw Patterned Ellipse with crank input
* trigger XOR mirror while A or B button is depressed
* logging
 

### Instructions

https://sdk.play.date/1.10.0/Inside%20Playdate%20with%20C.html#_xcodemake

#### Debug

```
mkdir build
cd build
cmake .. -G "Xcode"
```

#### Release 

```
cd release
cmake -DCMAKE_TOOLCHAIN_FILE=<path to SDK>/C_API/buildsupport/arm.cmake ..
make
```
