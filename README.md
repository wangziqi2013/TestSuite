# TestSuite
This repo contains a set of functions which are generic but useful as templates for common tasks such as timer, random number generator, etc., as a starting point for writing test cases in a complicated system.

The initial motivation of this repo is from EpochManager project which is also in my personal git repository, where a lot of common patterns such as thread timing, integer hashing, and so on, are being repeated. In addition, I noticed that in my other projects there are also similar problems with repeated testing framework code everywhere, and therefore I just collected some of the most commonly used classes and functions into one file "test_suite.h" and then use it as a template for writing test cases.

class Timer
===========
Timer class that provides high precision timing. Usually for thread timing

```c
// This creates and starts timer. If false is passed then it does not start
// automatically, and requires call to Start() to start it.
// Multiple calls to Start() only record starting time for the last call
Timer timer{true};
...
double duration = timer.Stop();
```

class Random
============
Random class generates random number in a given constant interval. Node that the interval is specified as part of the template argument, limiting its usage since it does not accept run-time variables. Also note that the interval given is half-open, i.e. [lower, upper).

```c
// Integer type, low
Random<uint64_t, 0UL, 100UL> r{};

// Either call Get() explicitly or use operator()
uint64_t t1 = r.Get();
uint64_t t2 = r();
```
 
class SimpleInt64Random
=======================
SimpleInt64Random implements a simple integer hashing function applied to uint64_t. It could be used as a hash function directly by calling operator() and providing a seed and salt, or be used as a pseudo-random number generator. The seed and salt value determines the hash number returned, and could be used to distinguish between different test cases (e.g. in a loop for i = 1 to 10, valeu i could be used as part of the salt, making each iterator different for i).

Similar to class Random, this class only accepts upper bound and lower bound as template arguments.

class Argv
==========
Argv analyzes command line arguments passed through argc and argv, and stores key-value pairs in a map and values without keys inside a vector. Caller could choose to interpret a value as either raw string or integer type, depending on the semantics of the argument.

Usage not provided here, since there are many valid inpits. For more details please refer to the class documentation.
