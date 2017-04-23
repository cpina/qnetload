# qnetload
## Introduction
Note that this has been quickly done over a weekend: not much quality testing or testing at all has happened yet. The license is GPL v3.

I still (2017) use xnetload and the way that shows the information is extremely useful for my use cases. Sadly xnetload was removed from Debian in 2010:
https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=595195

Thanks R.F. Smith for programming the original xnetload!

I've evaluated many similar packages but I wasn't satisfied with any of them, so I started writing a xnetload replacement. I've named qnetload as a tribute to xnetload and because it's implemented using the Qt libraries.

See the screenshots:
xnetload (the original one):

![xnetload](images-for-documentation/xnetload.png)

qnetload (this one):

![qnetload](images-for-documentation/qnetload.png)

## Installation
I've only tested it on a Debian Jessie (8.7).

Pre-requisites: Qt5 and cmake need to be installed.

This can be done on a Debian/Ubuntu system:
```
apt-get install cmake libqt5gui5 libqt5core5a
```

This works for me:
```
git clone https://github.com/cpina/qnetload.git
cd qnetload
mkdir build
cd build
cmake ../src
make
make install
```

Then to execute it:
```
qnetload wlan0
```
(or another interface instead of wlan0)


If it doesn't work for you feel free to contact me (carles@pina.cat)

## TODO
Many things can be done. My main idea is that the user interface should be the same or extremely similar and the way of working (invoking with the interface, etc.). Some possible ideas:
* If no interface is passed: choose one and let the user change it using the mouse (possibly just clicking on the interface?)
* Save in the settings the latest used interface and use it by default
* Be able to change between kiloBytes and kiloBits (same for the other units)

Carles Pina i Estany (carles@pina.cat)
