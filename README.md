# wg-gui
Linux client for WireGuard.

## Description
wg-gui is a GUI WireGuard client. It acts as a wrapper over wg-quick and simplifies configuration tasks.

## Requirements
* wg-quick
* qt5

## Build
```
qmake -o Makefile wg-gui.pro
make -j8
```

