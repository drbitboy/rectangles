/* Given a subset of integral {X,Y} coordinates on a Cartesian grid,
   count rectangles composed of groups of four points at rectangle
   vertices using points from that subset.

   Algorithm:  calculate potential diagonals' parameters, midpoint and
               length*, from all possible sets of {{X1,Y1},{X2,Y2}}
               pairs of points in the subset.  If there are M potential
               diagonals with the same parameters, then there are
               M(M-1)/2 rectangles that may be created from those
               M diagonals.

   * Use sum of diagonal endpoints {X1+X2,Y1+Y2} as a proxy for
     the midpoint {(X1+X2)/2,(Y1+Y2)/2}; use square of length of
     diagonal via Pythagorean Theorem ((X1-X2)**2 + (Y1-Y2)**2) as a
     proxy for the length (SQRT((X1-X2)**2 + (Y1-Y2)**2)).

 */
#include <bits/stdc++.h>
#include <set>
#include <math.h>
#include <sys/time.h>
#include <random>
using namespace std;

#define __Rn_USE_DEFAULT__

////////////////////////////////////////////////////////////////////////
#   ifdef __r3__
    size_t Diag(unsigned int x, unsigned int y, unsigned int L) {
      return ((size_t)x) | (((size_t)y)<<16) | (((size_t)L)<<32);
    }

    typedef unordered_set<size_t> USTDiag;
#   undef __Rn_USE_DEFAULT__
#   endif
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
#   ifdef __Rn_USE_DEFAULT__
    class Diag {
    public:
      unsigned int midx; unsigned int midy; unsigned int lensq;
      Diag() : midx(-1), midy(-1), lensq(-1) {}
      Diag(unsigned int _x, unsigned int _y, unsigned int _lsq) : midx(_x), midy(_y), lensq(_lsq) {}
    };
    struct DiagHash {
    public:
      size_t operator()(const Diag& diag) const {
      hash<size_t> hash_fn;
        return hash_fn(((size_t)diag.midx) | (((size_t)diag.midy) << 16) | (((size_t)diag.lensq) << 32));
        //return diag.midx ^ diag.midy ^ diag.lensq
            //     ^ (diag.midx+diag.midy-diag.lensq);
      }
    };
    struct DiagEq {
    public:
      size_t operator()(const Diag& diag1, const Diag& diag2) const {
#   define F(M) if (diag1.M!=diag2.M) return false
        F(midx); F(midy); F(lensq); return true;
      }
    };
    typedef unordered_set<Diag, DiagHash, DiagEq> USTDiag;
#   endif
////////////////////////////////////////////////////////////////////////


typedef USTDiag::iterator USTDiagIT;

typedef unordered_set<unsigned int> USTuint;
typedef USTuint::iterator USTuintIT;

int main(int argc, char** argv) {

# define USEC (gettimeofday(&mytv,NULL) , ((mytv.tv_sec*1000000L) + mytv.tv_usec))
  struct timeval mytv;
  unsigned long usecstart;
  default_random_engine generator((unsigned) USEC);

  //double dNmax = (1<<14);
  double dNmax = 11999;

  for (unsigned int N = 14; N < dNmax; N = ((N*140) / 100) + 10) {
    unsigned int twoN = N << 1;
    unsigned int edge = N;
    //unsigned int edge = ceil(sqrt(N)*1.25);
    uniform_int_distribution<unsigned int> distrib(0L,edge);

    unsigned long cumtime = 0;
    unsigned long deltime;
    unsigned int cumcount = 0;

    USTuint ustuint(N*10);
    USTDiag ust_diag(N*N*2);

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
      unsigned int collisions = 0;
      ustuint.clear();
      while(pxy<pxysend) {
        tmpx = distrib(generator);
        tmpy = distrib(generator);
        pair<USTuintIT,bool> insert_result = ustuint.insert(tmpx | (tmpy<<16));
        if (!insert_result.second) {
          ++collisions;
          continue;
        }
        *(pxy++) = tmpx;
        *(pxy++) = tmpy;
      }

      unsigned int ustuint_size = ustuint.size();
      unsigned int buckets = ustuint.bucket_count();
      ustuint.clear();

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
          // Need to fix this to increase by M-1 for Mth matching diagonal
        } // for (pxyo=pxy; pxy<pxysend; )
      } // for (pxy=xys; pxy<pxysend; )

      deltime = (USEC - usecstart);
      cumtime += deltime;
      cumcount += count;

      cout << pass
           << "," << N
           << "," << ustuint_size
           << "," << collisions
           << "," << buckets
           << "," << count
           << "," << deltime
           << "=pass,N,ustuint_size,collisions,buckets,count,deltime\n";
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
