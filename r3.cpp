#include <bits/stdc++.h>
#include <set>
#include <math.h>
#include <sys/time.h>
#include <random>
#include <unistd.h>
using namespace std;

size_t Diag(unsigned int x, unsigned int y, unsigned int L) {
  return ((size_t)x) | (((size_t)y)<<16) | (((size_t)L)<<32);
}

typedef unordered_set<size_t> USTDiag;
typedef USTDiag::iterator USTDiagIT;

typedef unordered_set<unsigned int> USTuint;
typedef USTuint::iterator USTuintIT;

int main(int argc, char** argv) {

# define USEC (gettimeofday(&mytv,NULL) , ((mytv.tv_sec*1000000L) + mytv.tv_usec))
  struct timeval mytv;
  unsigned long usecstart;
  default_random_engine generator((unsigned) USEC);

  double dNmax = (1<<14)/1.4;

  for (unsigned int N = 14; N < dNmax; N = ((N*140) / 100) + 10) {
    unsigned int twoN = N << 1;
    unsigned int edge = N;
    //unsigned int edge = ceil(sqrt(N)*1.25);
    uniform_int_distribution<unsigned int> distrib(0L,edge);

    unsigned long cumtime = 0;
    unsigned long deltime;
    unsigned int cumcount = 0;

    USTuint ustuint;
    USTDiag ust_diag(N*N);

    for (unsigned int pass=0; pass<4; ++pass) {

      unsigned int xys[N*2];
      unsigned int* pxy = xys;
      unsigned int* pxyo;
      unsigned int* pxysend = xys + twoN;
      unsigned int x;
      unsigned int y;
      unsigned int xo;
      unsigned int yo;
      unsigned int tmpx;
      unsigned int tmpy;
      ustuint.clear();
      while(pxy<pxysend) {
        tmpx = distrib(generator);
        tmpy = distrib(generator);
        pair<USTuintIT,bool> insert_result = ustuint.insert(tmpx | (tmpy<<16));
        if (!insert_result.second) continue;
        *(pxy++) = tmpx;
        *(pxy++) = tmpy;
      }

      unsigned int dx;
      unsigned int dy;
      unsigned int count = 0;

      ust_diag.clear();
      usecstart = USEC;

      for (pxy=xys; pxy<pxysend; ) {
        x = *(pxy++);
        y = *(pxy++);
        if ( !((pxy-xys) % 2000) ) { cerr << '.'; cerr.flush(); }
        for (pxyo=pxy; pxyo<pxysend; ) {
          xo = *(pxyo++);
          yo = *(pxyo++);
          dx = x > xo ? (x - xo) : (xo - x);
          dy = y > yo ? (y - yo) : (yo - y);
          pair<USTDiagIT,bool> ib = ust_diag.insert(Diag(x+xo,y+yo,(dx*dx)+(dy*dy)));
          if (!ib.second) ++count;
        } // for (pxyo=pxy; pxy<pxysend; )
      } // for (pxy=xys; pxy<pxysend; )

      deltime = (USEC - usecstart);
      cumtime += deltime;
      cumcount += count;

      cout << pass
           << "," << N
           << "," << ustuint.size()
           << "," << count
           << "," << deltime
           << "=pass,N,ustuint.size(),count,deltime\n";
      cout.flush();

    } // for (unsigned int pass=0; pass<5; ++pass)

    cout << N
         << "," << (N*N)
         << "," << edge
         << "," << cumtime
         << "=N,N*N,edge,cumtime\n";
    cout.flush();

  } // for (unsigned int N = 14; N < dNmax; N = ((N*140) / 100) + 10)

  return 0;
}
