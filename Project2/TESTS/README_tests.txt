


Quick start guide:

    Copy GridWorld.h into directory with all of these files.

    Run ./batch (from same directory)

       [You might need to change the permissions on the script:

               chmod +x batch
       ]

    Running ./batch should produce a directory _TEST_RESULTS 
      containing log files for all of the tests and a file score_summary

    It will also report to the terminal a grand-total.

    There are tests for both correctness and runtime.

    Runtime tests are based on running two problem instances:

        one of size N and
        one of size 2N

          (where N is specified in tutil.c).

        The ratio of these two runtimes is evaluated.  For example,
        if the overall runtime for the test should be nlogn, then
        the ratio should just be a little more than 2.0.  The tests
        give quite a bit of headroom (with the threshold being 2.5
        for the nlogn case.

        (If the implementation is really quadratic, the ratio will
        be about 4.0...)

    Note:  there are 14 distinct executables.  If an executable
      crashes, there will almost certainly not be an entry in 
      the score_summary file.  Look at the log file.

      You may notice that there are no test programs t5.cpp and t6.cpp.
      This is not an error -- they do not exist (well, they did, but
      were removed...).

      The maximum possible points for these tests is 145

FILES:

_test.h : 

	included by actual test programs t*.cpp

	contains macros for running tests and tabulating scores.

	also contains several utility functions for building test cases
	without relying on student code.

	also contains "calibration" code for calculating reasonable 
	elapsed time limits for certain operations.



_util.h:

  also included by test programs.

  includes functions for constructing and modifying lists.

    If you want to modify to run on differing problem sizes, 
    you must follow this rule.


test programs:


scripts:

  batch

	   compiles all test programs and runs time one-by-one.
     Ideally, you should just be able to type ./batch and results
     will be collected.


makefile:  makefile specifically for compiling test suite.

LOG FILES:  log files are produced in a subdirectory _TEST_RESULTS

_TEST_RESULTS contents

    make.log contains output of compilation process (if compilation
     fails, hopefully you will find some info here).

	  complete logs of each test program in tX.log

	  file crashes:  if a test does not even complete, this file
		should tell you.

	  file score_summary:  this file is the result of extracting 
		the score results from the log files on a per-function
		basis.   Includes total.






