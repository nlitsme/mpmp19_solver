/*
This tool scans sums of various prime powers.

  sum( p[i]^e, i=1..n ) % n == 0   for all e and n.


cpputils from: https://github.com/nlitsme/cpputils
primesieve from: https://github.com/kimwalisch/primesieve

c++ -o expscan -O3 expscan.cpp -std=c++17 -I cpputils -I primesieve/include primesieve/build/libprimesieve.a -lgmp -lpthread

*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include <primesieve.hpp>
#include "formatter.h"

#include <gmpxx.h>

inline std::ostream& operator<<(std::ostream& os, const mpz_class& num)
{
    int base = 10;
    int flags = os.flags() & os.basefield;

    if (flags == os.hex) base = 16;
    else if (flags == os.dec) base = 10;
    else if (flags == os.oct) base = 8;

    return os << num.get_str(base);
}
double getclock()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 0.000001*tv.tv_usec;
}

double firstpress = 0;
int npresses = 0;
bool stopflag = false;
bool infoflag = false;


void handlectrlc(int sig)
{
  signal(SIGINT, handlectrlc);

  infoflag = true;

  double tnow = getclock();
  if (tnow - firstpress < 5.0) {
      npresses += 1;
      if (npresses==3) {
          stopflag = true;
      }
  }
  else {
      firstpress = tnow;
      npresses = 1;
  }
}

void report(const char *tag, double deltat, uint64_t n, uint64_t p, int exp, const mpz_class& sum)
{
    print("%s: (%12.6f sec)  (n,p,sum): %3d %12d  %12d\n", tag, deltat, exp, n, p);
}

int main(int argc, char**argv)
{
  int N = argc>1 ? strtol(argv[1], 0, 0) : 32;

  uint64_t p = 0;
  std::vector<mpz_class> sums(N);
  uint64_t n = 0;

  signal(SIGINT, handlectrlc);

  primesieve::iterator it;

  double tprev = getclock();

  while (!stopflag)
  {
    p = it.next_prime();

    n += 1;
    double tnow = getclock();
    mpz_class x = p;
    int i = 1;
    for (auto& s : sums) {
        s += x;
        x *= p;

        if (n>1 && (s % n == 0)) {
            report("found", tnow-tprev, n, p, i, s);
            tprev = tnow;
        }
        i++;
    }


    if (infoflag) {
      double tnow = getclock();
      report("current", tnow-tprev, n, p, -1, 0);
      infoflag = false;
    }
  }
  if (stopflag) {
      double tnow = getclock();
      report("stopped", tnow-tprev, n, p, -1, 0);
  }

  return 0;
}

