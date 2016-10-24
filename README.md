# TestSuite
This repo contains a set of functions which are generic but useful as templates for common tasks such as timer, random number generator, etc., as a starting point for writing test cases in a complicated system.

The initial motivation of this repo is from EpochManager project which is also in my personal git repository, where a lot of common patterns such as thread timing, integer hashing, and so on, are being repeated. In addition, I noticed that in my other projects there are also similar problems with repeated testing framework code everywhere, and therefore I just collected some of the most commonly used classes and functions into one file "test_suite.h" and then use it as a template for writing test cases.

class Timer
===========
Timer class that provides high precision timing.

Usage:
// This creates and starts timer
Timer timer{true};
...
double duration = timer.Stop();

