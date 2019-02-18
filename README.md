# cppbench
An easy to use C++ header only benchmark inspired by Perl's "Benchmark" module.

# Sample
The file sample.cc shows a simple use case for the benchmark - comparing the runtimes
of various string to double conversion functions.

To build the sample, simply invoke ``make``, then run it like this:

    $ ./sample 
                 runtime    strtod      atof      stod    sscanf
        strtod    253762        --     5.88%    10.84%    42.45%
          atof    269620    -6.25%        --     5.27%    38.85%
          stod    284604   -12.15%    -5.56%        --    35.45%
        sscanf    440928   -73.76%   -63.54%   -54.93%        --

    runtimes in milliseconds:
                 runtime    strtod      atof      stod    sscanf
        strtod       253        --     5.88%    10.84%    42.45%
          atof       269    -6.25%        --     5.27%    38.85%
          stod       284   -12.15%    -5.56%        --    35.45%
        sscanf       440   -73.76%   -63.54%   -54.93%        --

    runtimes with min, max, mean, variance and standard deviation

    nanoseconds
                 runtime       min       max       avg       var       dev
        strtod 253762917    244.00  12777.00    253.76   9247.29     96.16
          atof 269620479    243.00  37206.00    269.62  29361.12    171.35
          stod 284604985    254.00  38179.00    284.60   7676.04     87.61
        sscanf 440928764    414.00  16635.00    440.93  25874.05    160.85

    microseconds
                 runtime       min       max       avg       var       dev
        strtod    253762     0.244    12.777     0.254     9.247     0.096
          atof    269620     0.243    37.206     0.270    29.361     0.171
          stod    284604     0.254    38.179     0.285     7.676     0.088
        sscanf    440928     0.414    16.635     0.441    25.874     0.161

    milliseconds
                 runtime       min       max       avg       var       dev
        strtod       253   0.00024   0.01278   0.00025   0.00925   0.00010
          atof       269   0.00024   0.03721   0.00027   0.02936   0.00017
          stod       284   0.00025   0.03818   0.00028   0.00768   0.00009
        sscanf       440   0.00041   0.01664   0.00044   0.02587   0.00016

    successfully wrote runtimes to sample-*.txt files


From the output matrix one can see that strtod is fastest, followed by atof, stod and sscanf.
Positive percentages mean faster, negative ones mean slower, ``--`` means no comparison (same test).

Beginning with the first output line from left to right, strtod is 5.88% faster than atof,
10.84% faster than stod and 42.45% faster than sscanf.
Second line, atof is 6.25% slower than strtod, 5.27% faster than stod and 38.85% faster than sscanf, etc
until the last line, where sscanf is the slowest of all.

The runtimes of the tests are printed with decreasing time resolutions, including the minimum, maximum and
average runtimes along with the variance and standard distribution of the tests.

Finally a set of text files is written for post-processing, containing the runtime
results in the same resolution as chrono::high_resolution_clock - two files are
written for each test result, one contains the runtime, min, max, avg, var and dev data,
while the other contains the distribution of runtimes.
