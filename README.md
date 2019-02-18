# cppbench
An easy to use C++ header only benchmark inspired by Perl's "Benchmark" module.

# Sample
The file sample.cc shows a simple use case for the benchmark - comparing the runtimes
of various string to double conversion functions.

To build the sample, simply invoke ``make``, then run it like this:

    $ ./sample 
                 runtime    strtod      stod      atof    sscanf
        strtod    257732        --     1.46%     3.70%    41.18%
          stod    261564    -1.49%        --     2.27%    40.31%
          atof    267635    -3.84%    -2.32%        --    38.92%
        sscanf    438187   -70.02%   -67.53%   -63.73%        --

    runtimes in milliseconds:
                 runtime    strtod      stod      atof    sscanf
        strtod       257        --     1.46%     3.70%    41.18%
          stod       261    -1.49%        --     2.27%    40.31%
          atof       267    -3.84%    -2.32%        --    38.92%
        sscanf       438   -70.02%   -67.53%   -63.73%        --

    runtimes with min, max, mean, variance and standard deviation

    nanoseconds
                 runtime       min       max       avg       var       dev
        strtod 257732742    244.00  18283.00    257.73  23230.36    152.42
          stod 261564178    248.00  40466.00    261.56  18360.70    135.50
          atof 267635218    244.00  39090.00    267.64  51913.26    227.84
        sscanf 438187575    421.00  17760.00    438.19  54119.65    232.64

    microseconds
                 runtime       min       max       avg       var       dev
        strtod    257732     0.244    18.283     0.258     0.023     0.152
          stod    261564     0.248    40.466     0.262     0.018     0.136
          atof    267635     0.244    39.090     0.268     0.052     0.228
        sscanf    438187     0.421    17.760     0.438     0.054     0.233

    milliseconds
                 runtime       min       max       avg       var       dev
        strtod       257   0.00024   0.01828   0.00026   0.00000   0.00015
          stod       261   0.00025   0.04047   0.00026   0.00000   0.00014
          atof       267   0.00024   0.03909   0.00027   0.00000   0.00023
        sscanf       438   0.00042   0.01776   0.00044   0.00000   0.00023

    successfully wrote runtimes to sample-*.txt files

From the output matrix one can see that strtod is fastest, followed by stod, atof and sscanf.
Positive percentages mean faster, negative ones mean slower, ``--`` means no comparison (same test).

Beginning with the first output line from left to right, strtod is 1.46% faster than stod,
3.70% faster than atof and 41.18% faster than sscanf.
Second line, stod is 1.49% slower than strtod, 2.27% faster than atof and 40.31% faster than sscanf, etc
until the last line, where sscanf is the slowest of all.

The runtimes of the tests are printed with decreasing time resolutions, including the minimum, maximum and
average runtimes along with the variance and standard deviation of the tests.

Finally a set of text files is written for post-processing, containing the runtime
results in the same resolution as chrono::high_resolution_clock - two files are
written for each test result, one contains the runtime, min, max, avg, var and dev data,
while the other contains the distribution of runtimes.
