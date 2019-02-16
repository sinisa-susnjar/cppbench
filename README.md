# cppbench
An easy to use C++ header only benchmark inspired by Perl's "Benchmark" module.

# Sample
The sample.cc shows a simple use case for the benchmark - compare the runtimes
of various string to double conversion functions.

To build the sample, simply invoke ``make``, then run it like this:

    $ ./sample 
                 runtime    strtod      stod      atof    sscanf
        strtod    200340        --     1.27%     2.70%    38.29%
          stod    202920    -1.29%        --     1.45%    37.50%
          atof    205902    -2.78%    -1.47%        --    36.58%
        sscanf    324654   -62.05%   -59.99%   -57.67%        --

    runtimes in milliseconds:
                 runtime    strtod      stod      atof    sscanf
        strtod       200        --     1.27%     2.70%    38.29%
          stod       202    -1.29%        --     1.45%    37.50%
          atof       205    -2.78%    -1.47%        --    36.58%
        sscanf       324   -62.05%   -59.99%   -57.67%        --

From the output matrix one can see that strtod is fastest, followed by stod, atof and sscanf.
Positive percentages mean faster, negative ones mean slower, ``--`` means no comparison (same test).

Beginning with the first output line from left to right, strtod is 1.27% faster than stod,
2.70% faster than atof and 38.29% faster than sscanf.
Second line, stod is 1.29% slower than strtod, 1.45% faster than atof and 37.50% faster than sscanf, etc
until the last line, where sscanf is the slowest of all.

