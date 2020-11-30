"""

This tool summarizes the output of 'expscan' to produce something like this:

  17 -> (6249)  16k+{15}
  19 -> (5555)  18k+{2}
  23 -> (4545)  22k+{1, 13}
  25 -> (4999)  20k+{3, 5, 7, 11, 15, 19}

Indicating for instance that:

  sum( p[i]^(18k+2), i=1..19 ) % 19 == 0    for k >= 0

"""
from collections import defaultdict
import re

def readxout(filename):
    with open(filename, "r") as fh:
        for line in fh:
            m = re.search(r':\s+(\d+)\s+(\d+)', line)
            if m:
                yield int(m.group(1)), int(m.group(2))

def findpattern(n, exps):
    diffs = [b-a for a, b in zip(exps, exps[1:])]
    if len(diffs)==1:
        print("%4d ?" % n)
        return

    patternlength = None
    patternsum = 0
    nrepeats = 0
    for l in range(1, len(diffs)):
        e = (len(diffs)//l) * l
        if diffs[l:e] + diffs[:l] == diffs[:e]:
            patternlength = l
            patternsum = sum(diffs[:l])
            nrepeats = e//l
            break
    if patternsum:
        print("%4d -> (%3d)  %dk+{%s}" % (n, nrepeats, patternsum, ", ".join(map(str,exps[:patternlength]))))
    else:
        print("%4d ?" % n)

def main():
    byn = defaultdict(set)

    import sys
    for fn in sys.argv[1:]:
        for e, n in readxout(fn):
            byn[n].add(e)

    for n, exps in sorted(byn.items()):
        findpattern(n, sorted(exps))


if __name__ == '__main__':
    main()
