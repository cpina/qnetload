# qnetload
## Introduction
Note that this has been quickly done over a weekend: not much quality testing or testing at all has happened yet. The license is GPL v3.

I still (2017) use xnetload and the way that shows the information is extremely useful for my use cases. Sadly xnetload was removed from Debian in 2010:
https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=595195

Thanks R.F. Smith for programming the original xnetload!

I've evaluated many similar packages but I wasn't satisfied with any of them, so I wrote a xnetloda replacement. I've named it qnetload as a tribute to xnetload and because it's implemented using the Qt libraries.

See the screenshots:
xnetload (the original one):

![xnetload](images-for-documentation/xnetload.png)

qnetload (this one):

![qnetload](images-for-documentation/qnetload.png)

## Installation
### Debian and Ubuntu
I've tested it on Ubuntu 17.04 Zesty but it should work with other Ubuntus as well.
On Debian I've tested it on a Debian 8.7 Jessie.

```
sudo apt-get install git cmake qtbase5-dev
git clone https://github.com/cpina/qnetload.git
cd qnetload
mkdir build
cd build
cmake ../src
make
sudo make install
```

Then qnetload should be in your path and can be executed just typing qnetload on the command line.

If it doesn't work for you feel free to contact me (carles@pina.cat) or open a Github bug. Please include the output of the commands.

## TODO
I'd like the user interface and way to use it to be very similar to xnetload. Some possible ideas:

* If no interface name is passed: choose one and let the user change it using the mouse (possibly just clicking on the interface name)
* Save in the settings the latest used interface and use it by default
* Be able to change between kiloBytes and kiloBits (same for the other units)

If you would like something else let me know (create a bug or drop me an email).

Carles Pina i Estany (carles@pina.cat)
