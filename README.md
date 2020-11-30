mpmp19
======

In this document:
 * my mpmp19 answer
 * an interesting pattern to the other prime powers
 * the libraries I used for quickly generating all primenumbers in order.


Context
=======

This is an answer to the 19th and last puzzle of the COVID-19 season of MattParkerMathsPuzzles.

See http://think-maths.co.uk/19challenge and https://www.youtube.com/watch?v=tBXGIXEV7tI .


My answer
=========

I quickly found that the relevant OEIS sequence is: A111441, 
Of course I overestimated the amount of work needed to improve this list,
and immediately submitted the largest entry from A111441: a(12) =  51283502951.

Sure enough, the next morning I found that youtube user Balping had calculated a(13),
and had updated A111441 accordingly.

So my answer is certainly no longer the largest one submitted.


no test
=======

Unfortunately I did not find a way to validate answers. So in theory people
could just submit a very large number, and you would have no way of telling
wheter it was indeed a correct answer.

One test you could do, if you have both the 'n', and the 'sum':

   (sum - n) % 12 == 11

If that is false, then you would be sure the 'n' and 'sum' are not a valid combintion.

This is because:

    for all primes > 3: p % 6  is either 1 or -1

        -> p^2 = (6*k+1)^2 = 36*k^2+12*k+1 = 12*(3*k+1) + 1
        or p^2 = (6*k-1)^2 = 36*k^2-12*k+1 = 12*(3*k+1) + 1

    so  (p^2) % 12 == 1  for p>3

    --> sum(p[i]^2, i<n) % 12 = (2^2+3^3 + n-2) % 12  = (n+11) % 12



more power(s)
=============

It is interesting to note that sum of the first 19 prime powers is divisible by
19 again when raising the primes to the 20th power, and again with primes
raised to the 38th power.

    2^2 + 3^2 + 5^2 + ... + 67^2  is divisible by 19
    2^20 + 3^20 + 5^20 + ... + 67^20  is divisible by 19
    2^38 + 3^38 + 5^38 + ... + 67^38  is divisible by 19

One can extrapolate this to:

    sum(p[i]^(18*k+2), i=1..19) is divisible by 19 for all k >= 0

Using some python code to investigate this pattern, I found the following:


|   n | power    |
| ---:| -------------------- |
|  17 |     16\*k+{15}                         |
|  19 |     18\*k+{2}                          |
|  23 |     22\*k+{1, 13}                      |
|  25 |     20\*k+{3, 5, 7, 11, 15, 19}        |
|  31 |     30\*k+{15}                         |
|  37 |     36\*k+{2, 12, 18, 24}              |
|  41 |     40\*k+{28}                         |
|  43 |     42\*k+{8, 31}                      |
|  47 |     46\*k+{28, 31}                     |
|  53 |     52\*k+{1}                          |
|  59 |     58\*k+{11}                         |
|  61 |     60\*k+{30}                         |
|  79 |     78\*k+{58}                         |
|  83 |     82\*k+{46}                         |
|  89 |     88\*k+{69}                         |
|  97 |     96\*k+{62}                         |

You may notice that the values before `k` are the euler totient function values.


The general form of the sum can be written like this:

    sum(p[i]^(phi(n)*k+x), i=1..n) % n == 0

Because of euler's theorem:  a^phi(n)==1 (mod n)
This becomes:

    sum(p[i]^x, i=1..n) % n == 0

So the 'somenumber times k' part of the observed behaviour can be explained.


