# gamma
program to change screen brightness (in windows)

some (older) games sometimes mess up the screen's brightness.
so i tried helping myself with this program.

it uses gdi32's Get-/SetDeviceGammaRamp.

#### usage:
```
gamma.exe (default brightness of 50%)
gamma.exe <brightness>
```

#### this equation...
```
v = (t[c][i] / i) - 128; (c = color)
```
