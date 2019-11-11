# rectangles
Measuring performance of a diagonals-based approach to solving simple coding example from mock Google interview on YouTube

https://www.youtube.com/watch?v=EuPSibuIKIg

TL;DR:  the performance seems to be somewhere between O(N\*\*2) and O(N\*\*2 logN)


        N   T(Time),us       T/N**2  T/N**2/logN       T/N**3  T/N**3/logN
     ----   ----------   ----------  -----------   ----------  -----------
       14,          83,  4.2347e-01,  1.1122e-01,  3.0248e-02,  7.9446e-03
       29,         362,  4.3044e-01,  8.8605e-02,  1.4843e-02,  3.0553e-03
       50,        1120,  4.4800e-01,  7.9378e-02,  8.9600e-03,  1.5876e-03
       80,        2890,  4.5156e-01,  7.1428e-02,  5.6445e-03,  8.9285e-04
      122,        6855,  4.6056e-01,  6.6452e-02,  3.7751e-03,  5.4469e-04
      180,       15139,  4.6725e-01,  6.2368e-02,  2.5959e-03,  3.4649e-04
      262,       32779,  4.7752e-01,  5.9442e-02,  1.8226e-03,  2.2688e-04
      376,       73348,  5.1882e-01,  6.0648e-02,  1.3798e-03,  1.6130e-04
      536,      164241,  5.7168e-01,  6.3057e-02,  1.0666e-03,  1.1764e-04
      760,      354423,  6.1361e-01,  6.4119e-02,  8.0739e-04,  8.4368e-05
     1074,      701721,  6.0835e-01,  6.0420e-02,  5.6644e-04,  5.6257e-05
     1513,     1446951,  6.3209e-01,  5.9838e-02,  4.1777e-04,  3.9550e-05
     2128,     2942976,  6.4990e-01,  5.8786e-02,  3.0540e-04,  2.7625e-05
     2989,     5992711,  6.7077e-01,  5.8098e-02,  2.2441e-04,  1.9437e-05
     4194,    11990833,  6.8170e-01,  5.6647e-02,  1.6254e-04,  1.3507e-05
     5881,    23038153,  6.6611e-01,  5.3196e-02,  1.1326e-04,  9.0454e-06
     8243,    46074197,  6.7809e-01,  5.2125e-02,  8.2262e-05,  6.3235e-06
    11550,    97135707,  7.2814e-01,  5.3954e-02,  6.3042e-05,  4.6713e-06

    N.B. The range of XY coordinates was N/2, and there were 10-30 rectangles
         identified per N over all N.


## Caveats
- The entire approach was taken from several comments on the original youtube video; it is not at all original.
- There has been no attempt made to validate the algorithm, but duck debuggin and eyeballing the results suggest it may be correct.
- The focus here was on a minimal implementation that provides some idea of the performance of the diagonal algorithm.
- The code uses random numbers to generate test cases; in an actual application the algorithm would be placed in a separate subroutine
- The key design of the unordered map and set limits the XY coordinates to about 15 bits (0-32k), and even before that, performance drops off because of paging (on my system, at least) for XY coordinates beyond around 12k.
- The times are for my system (Win10/Cygwin), my CPU, my memory etc.  I tweaked a few parameters of the test space to try to ensure that no paging occured; before that I had several runs that were very slow and had noticeable disk useage (order 10^8 MB/s, as monitored by Task Manager), especially cases with large N values; I assumed that disk usage was due to paging.


## Manifest

### Makefile
Controls building and running
- make [all] (default):  build r3 and r0 executables (r3.exe and r0.exe on Cygwin)
- make go:  run r3 and r0 executables; write outputs to log_r3.txt and log_r0.txt respectively.
- make log_results:  calculates ratios of time to N**2, N**2*logN, etc.
- make clean:  remove executables and log files

### baser.cpp
C++ Source code for counting rectangles that may be formed from N points at integral [X,Y] Cartesian (2D) locations

### log_r3.txt, log_r0.txt
Log files:  STDOUT of runs of baser.cpp compiled with and without -D__r3__

### scale_times.py
Parse a log file, express cumulative run times as multiples of various combinations of N and logN

### rpy.py
Python script to do the same thing as baser.cpp; replaced by baser.cpp

### README.md
This file

