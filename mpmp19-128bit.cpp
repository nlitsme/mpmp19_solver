/*

   This tool scans for solutions to the mpmp19 problem:

   find values 'n' for which: sum(p[i]^2, i=1..n) % n == 0

   This version will work up to about n = 10^12, above that
   the sum will no longer fit in a 128-bit value.
   
Dependencies:
 * cpputils from: https://github.com/nlitsme/cpputils
 * primesieve from: https://github.com/kimwalisch/primesieve

Usage:
 Without arguments: start scan at 0.
 With arguments: pass the 'n', 'p', and 'sum' from a previous session to
 start the scan at 'n'.
 The tool will output the current state when you press Ctrl-C,
 pressing ctrl-c three times will stop the scan.
 The scan will stop when a 128-bit integer overflow is detected in the sum.


c++ -o mpmp19-128bit -O3 mpmp19-128bit.cpp -std=c++17 -I cpputils -I primesieve/include primesieve/build/libprimesieve.a -lgmp -lpthread

 */
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include <primesieve.hpp>
#include <cpputils/formatter.h>

typedef unsigned __int128 BigInt;

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
uint64_t scan64(char *s)
{
    return strtoull(s, 0, 0);
}
BigInt scan128(char *s)
{
    BigInt result = 0;
    while (*s)
    {
        char digit = *s++;
        if (digit < '0' && digit > '9')
            throw "invalid digit";
        result *= 10;
        result += digit - '0';
    }
    return result;
}
bool safe_add_to(BigInt &accu, BigInt value)
{
    BigInt sum = accu + value;
    if (sum < accu)
        return false;
    accu = sum;
    return true;
}

void report(const char *tag, double deltat, uint64_t n, uint64_t p, BigInt sum)
{
    print("%s: (%12.6f sec)  (n,p,sum): %12d  %12d %d\n", tag, deltat, n, p, sum);
}

int main(int argc, char**argv)
{
  uint64_t p = 0;
  BigInt sum = 0;
  uint64_t n = 0;

  signal(SIGINT, handlectrlc);

  double tstart = getclock();
  primesieve::iterator it;
  double tinit = getclock();

  if (argc==1) {
      print("Starting at 0\n");
  }
  else if (argc==4) {
      n = scan64(argv[1]);
      p = scan64(argv[2]);
      sum = scan128(argv[3]);

      it.skipto(p+1);

      double tskip = getclock();
      print("%.6f sec in init, %.6f sec in skip\n", tinit-tstart, tskip-tinit);
      report("start", tskip-tinit, n, p, sum);
  }
  else {
      print("Usage: mpmp19 [n p sum]\n");
      print("   scan A111441 prime square sums\n");
      print("   optionally pass a previous state as your arguments\n");
      print("   press ctrl-c for the current state\n");
      print("   press ctrl-c 3 times within 5 sec to stop\n");
      return 0;
  }

  double tprev = getclock();

  while (!stopflag)
  {
    uint64_t previous_p = p;
    p = it.next_prime();

    if (!safe_add_to(sum, BigInt(p)*BigInt(p)))
    {
        double tnow = getclock();
        print("while adding %d: ", p);
        report("overflow", tnow-tprev, n, previous_p, sum);
        break;
    }
    n += 1;

    if (sum % n == 0) {
        double tnow = getclock();
        report("found", tnow-tprev, n, p, sum);
        tprev = tnow;
    }
    if (infoflag) {
      double tnow = getclock();
      report("current", tnow-tprev, n, p, sum);
      infoflag = false;
    }
  }
  if (stopflag) {
      double tnow = getclock();
      report("stopped", tnow-tprev, n, p, sum);
  }

  return 0;
}
