//Adding Neccessary Librabries
#include  <stdio.h>

//This library really isnt available for sudo install on the pi
#include <wiringPi.h> //What I did was to git clone the repo and cd Build the library
#include <wiringPiI2C.h>
#include <unistd.h>