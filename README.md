# rectangles
Diagonals approach to solving simple coding example from mock Google interview on YouTube

https://www.youtube.com/watch?v=EuPSibuIKIg

## Manifest

### Makefile
Controls building and running
- make all (or [make] alone:  build r3 and r0 executables (r3.exe and r0.exe on Cygwin)
- make go:  run r3 and r0 executables; write outputs to log_r3.txt and log_r0.txt respectively.
- make clean:  remove executables and log files

### rbase.cpp
C++ Source code for counting rectangles that may be formed from N points at integral [X,Y] Cartesian (2D) locations

### log_r3.txt, log_r0.txt
Log files:  STDOUT of runs of rbase.cpp compiled with and without -D__r3__

### scale_times.py
Parse a log file, express cumulative run times as multiples of various combinations of N and logN

### rpy.py
Python script to do the same thing as rbase.cpp; replaced by rbase.cpp

### README.md
This file

