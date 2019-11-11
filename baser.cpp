/* Goal:  analyze performance of rectangle-finding algorithm

   Approach:  run algorithm with exponentially increasing problem
              sizes; measure how long it takes at each size

   Rectangles:  given a subset of integral {X,Y} coordinates on a
                Cartesian grid, count rectangles comprising four points
                at rectangle vertices using points from that subset.

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

#include <set>
#include <math.h>
#include <random>
#include <sys/time.h>
#include <unordered_map>
#include <bits/stdc++.h>
#include <bits/stl_pair.h>

using namespace std;

#define __Rn_USE_DEFAULT__

////////////////////////////////////////////////////////////////////////
// Two schemes for hashing a diagonal for lookup.
// N.B. both models use diagonal parameter proxies for midpoint & length
//
// 1) Midpoint proxy comprises two values:  midx = X1+X2; midy = Y1+Y2
// 1.1) This proxy is twice actual midpoint, but still unique
// 1.2) Midpoint proxies are limited to 16-bit unsigned integers here,
//      to allow this scheme to work
//
// 2) Length proxy comprises one value:  lsq = (X1-X2)**2 + (Y1-Y2**2)
// 2.1) This proxy uses Pythagorean Theorem to calculate the square 
//      of the length, but still unique
// 2.2) Length proxy is limited to 32-bit unsigned integer here, to
//      allow this scheme to work
//
// 3) Midpoint proxies are combined into low 32 bits (X | (Y<<16)) of
//    64-bit size_t; length proxy goes into high 32 bits (L<<32) of
//    64-bit size_t
// 3.1) size_t is 64-bit unsigned integer
//
// 4) Items (1.2), (2.2), and (3) above constrain the X1, X2, Y1, Y2
//    values to 15 bits, but in practice we stop between 13 and 14 bits,
//    because beyond that the runs take too long.
//
////////////////////////////////////////////////////////////////////////

static hash<size_t> hash_fn;

////////////////////////////////////////////////////////////////////////
// Scheme 1:  Pass {{X1,Y1},{X2,Y2}} pairs to function Diag(...), Diag()
//            calculates midx, midy and length-squared, then combines
//            them to make a hash, then uses that hash a key into 
//            unordered_map<size_t,int> to keep track of counts of
//            matching diagonals
//
// N.B. This scheme is used only if macro __r3__ is defined e.g.
//
//        g++ -D__r3__ ...
//
////////////////////////////////////////////////////////////////////////

#   ifdef __r3__

    // Function to calculate size_t hash from input {(x1,y1),x2,y2)}

    size_t Diag(unsigned int x1, unsigned int y1
               ,unsigned int x2, unsigned int y2
               ) {
      unsigned int midx = x1 + x2;
      unsigned int midy = y1 + y2;
      unsigned int dx = x1 > x2 ? (x1 - x2) : (x2 - x1);
      unsigned int dy = y1 > y2 ? (y1 - y2) : (y2 - y1);
      unsigned int lensq = (dx*dx) + (dy*dy);
      //return hash_fn(((size_t)midx) | (((size_t)midy)<<16) | (((size_t)lensq)<<32));
      return ((size_t)midx) | (((size_t)midy)<<16) | (((size_t)lensq)<<32);
    }

    // Unordered map uses size_t as key, int as mapped type (count of
    // matching diagonals)

    typedef unordered_map<size_t,int> UMPDiag;
    typedef size_t Diag_key_type;

    // Clear default macro

#   undef __Rn_USE_DEFAULT__
#   endif
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Scheme 2:  Create Diag class, which calculates and keeps Midpoint and
//            Length proxies internally
//
// N.B. This scheme is used only if no other macro __r?__ is defined, so
//      macros __Rn_USE_DEFAULT__ is left defined
////////////////////////////////////////////////////////////////////////

#   ifdef __Rn_USE_DEFAULT__

    // Class Diag (diagonal) to calculate midpoint and length proxies
    // from input {(x1,y1),x2,y2)}

    class Diag {
    public:
      unsigned int midx; unsigned int midy; unsigned int lensq;
      //Diag() : midx(-1), midy(-1), lensq(-1) {}

      Diag(unsigned int x1, unsigned int y1
          ,unsigned int x2, unsigned int y2
          ) {
        midx = x1 + x2;
        midy = y1 + y2;
        unsigned int dx = x1 > x2 ? (x1 - x2) : (x2 - x1);
        unsigned int dy = y1 > y2 ? (y1 - y2) : (y2 - y1);
        lensq = (dx*dx) + (dy*dy);
      }
    };

    // Hash function for using Diag instance as key to unordered_map

    struct DiagHash {
    public:
      size_t operator()(const Diag& diag) const {
        //return hash_fn(((size_t)diag.midx) | (((size_t)diag.midy) << 16) | (((size_t)diag.lensq) << 32));
        return ((size_t)diag.midx) | (((size_t)diag.midy) << 16) | (((size_t)diag.lensq) << 32);
      }
    };

    // Predicate equal_to function for using Diag instance as key to
    // unordered_map

    struct DiagEq {
    public:
      size_t operator()(const Diag& diag1, const Diag& diag2) const {
#       define F(M) if (diag1.M!=diag2.M) return false
        F(midx); F(midy); F(lensq); return true;
      }
    };
    typedef unordered_map<Diag,int,DiagHash,DiagEq> UMPDiag;
    typedef Diag Diag_key_type;

    ostream& operator<<(ostream& out, Diag& d) {
      out << "Diag(" << d.midx
          << "," << d.midy
          << "," << d.lensq
          << ")";
      return out;
    }

#   endif
////////////////////////////////////////////////////////////////////////

typedef UMPDiag::iterator UMPDiagIT;

// Unordered set of <unsigned int> values, to ensure random (X,Y) values
// have no duplicates

typedef unordered_set<unsigned int> USTuint;
typedef USTuint::iterator USTuintIT;


////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

  // Time measurement

# define USEC (gettimeofday(&mytv,NULL) , ((mytv.tv_sec*1000000L) + mytv.tv_usec))
  struct timeval mytv;
  unsigned long usecstart;

  // Seed random generator with microseconds

  default_random_engine generator((unsigned) USEC);

  // Exponential progression of N by a factor of roughly 1.4;
  // N will roughly double every two steps

  //double dNmax = 1<<13;
  double dNmax = 12000;
  for (unsigned int N = 14; N < dNmax; N = ((N*140) / 100) + 10) {

    // Get twice N for length into XY pairs
    unsigned int twoN = N << 1;

    // Set edge as range of X & Y values from random number distribution
    // 1) edge = N >> 1 yields about 20 rectangles for each N
    // 2) edge = N yields 0-3 rectangles
    // 3) edge = sqrt(N)*1.25 yields many, overflowing the 32-bit count

    unsigned int edge = N >> 1;
    //unsigned int edge = ceil(sqrt(N)*1.25);
    //unsigned int edge = N;

    // Distribute random XY integer coordinates between zero and edge

    uniform_int_distribution<unsigned int> distrib(0L,edge);

    unsigned long cumtime = 0;   // Accumulating time for N
    unsigned int cumcount = 0;   // Accumulating rectangle count for N
    unsigned long deltime;       // Delta-time per pass

    USTuint ustuint(5*N);    // Set of ints holding XY coordinate pairs;
                             // initial bucket count = twoN

    UMPDiag ump_diag(2*N*N);     // Map from diag to matching diag count

    for (unsigned int pass=0; pass<4; ++pass) {   // Make 4 passes per N

      unsigned int xys[twoN];               // 2N XY coordinates
      unsigned int* pxy = xys;              // Pointer into xys
      unsigned int* pxyo;                   // Other ointer to xys
      unsigned int* pxysend = xys + twoN;   // End of XY coordinatesk
      unsigned int x;                       // X value, outer loop
      unsigned int y;                       // Y value, outer loop
      unsigned int xo;                      // Other X value, outer loop
      unsigned int yo;                      // Other Y value, outer loop
      unsigned int tmpx;                  // Temporary generated X value
      unsigned int tmpy;                  // Temporary generated Y value
      unsigned int collisions = 0;        // Collisions of generator
      ustuint.clear();                    // Empty set of X,Y pairs

      while(pxy<pxysend) {               // Loop until N pairs generated

        tmpx = distrib(generator);       // Generate X
        tmpy = distrib(generator);       // Generate Y

        pair<USTuintIT,bool>             // Try to insert that X,Y pair
        insert_result
        = ustuint.insert(tmpx | (tmpy<<16)); // ... as combined uint

        if (!insert_result.second) {     // If insert failed, then there
          ++collisions;                  // is already an identical XY
          continue;                      // pair, so continue in loop
        }

        *(pxy++) = tmpx;          // To here, insert succeeded, so store
        *(pxy++) = tmpy;          // XY pair and increment pointers

      } // while(pxy<pxysend)  // Loop until N pairs generated

      // Save state of unordered_set<unsigned int>, then clear it
      ustuint.clear();

      //unsigned int dx;
      //unsigned int dy;

      unsigned int count = 0;       // Initialize count to zero
      ump_diag.clear();             // Clear map of diagonals
      usecstart = USEC;             // Start timer

      for (pxy=xys; pxy<pxysend; ) {  // Outer loop
        x = *(pxy++);                 // Get XY pair
        y = *(pxy++);

        if ( !((pxy-xys) % 2000) ) { cerr << '.'; cerr.flush(); }   // Progress

        for (pxyo=pxy; pxyo<pxysend; ) {  // Inner loop over other XYs
                                          //  at & after incremented pxy

          xo = *(pxyo++);                 // Get another XY pair
          yo = *(pxyo++);

          // For this set of {{x,y},{xo,yo}} XY pairs, generate the
          // corresponding diagonal Diag(x,y,xo,yo), first
          // 1) Increase count by number of earlier diagonal matches,
          // 2) then lastly increment the number of diagonal matches
          //
          // N.B. if there is no entry in unordered_map ump_diag, then
          //      the value zero will be assigned to count, after which
          //      the value will be incremented to one.

          count += ump_diag[Diag(x,y,xo,yo)]++;

        } // for (pxyo=pxy; pxy<pxysend; )
      } // for (pxy=xys; pxy<pxysend; )

      
      deltime = (USEC - usecstart);   // Calculate time for this pass
      cumtime += deltime;             // Accumulate time for N
      cumcount += count;              // Accumulate counts for N

      cout << pass                    // Output pass results to STDOUT
           << "," << N
           << "," << collisions
           << "," << count
           << "," << deltime
           << "=pass,N,collisions,count,deltime\n";
      cout.flush();

    } // for (unsigned int pass=0; pass<5; ++pass)

    cout << N                         // Output accumulated results from
         << "," << (N*N)              // N passes to STDOUT
         << "," << edge
         << "," << cumtime
         << "=N,N*N,edge,cumtime\n";
    cout.flush();

  } // for (unsigned int N = 14; N < dNmax; N = ((N*140) / 100) + 10)

  return 0;
}
