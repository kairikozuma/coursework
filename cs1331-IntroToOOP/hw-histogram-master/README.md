#Histogram - Grade Distribution
Due Wednesday, September 16th at 11:59 PM

##Setup
Download the homework by clicking "Download ZIP" on the right. Submit your homework as noted below with `sbt submit`.

##Introduction
The homework will cover basic Java control structures, arrays, file I/O, and string manipulation.

##Problem Description

You are a Georgia Tech professor teaching a class. It is the end of the semester and you wish to see how your students performed, so you write a Java program that will create a histogram of the grade distribution. You want this histogram program to be able to give you a very detailed view or a very broad view of the grade distribution. To accomplish this, it asks the user how many bins the grades should be split into.

##Solution Description
Put your code in a file named `GradeHistogram.java`

We have provided you with a CSV file that has a list of students and their grades. A CSV file is just a text file with data partitioned by commas and (in this case) newlines. Note that there may be any number of spaces surrounding the comma.

These grades are not sorted but they are bound between 0 and 100 (inclusive). For example, the file may look like:

        Glenn Hollingsworth,91
        Chris Simpkins, 100
        Thomas Shields, 89
        Bob,55
        Alice,   95
        Eve, 87

Use command-line arguments to inform your program of the name of the grades file - see the example below for how to pass the file name in while running the program.

Using an array, you must count the frequency of each grade value in the file and print it to the standard output as a horizontal histogram. For every grade in a particular range, print a pair of brackets.

**You may only read through the file once.**

You must also label the range of each histogram bar and allow the user to indicate what size interval they would like the histogram to be made with. You must allow the user to specify this size interval in each of the following ways:

1. Firstly, the size interval may be specified as an additional command line arg, e.g `java GradeHistogram grades.csv 5`
2. If the second command line arg is not present, your program must ask the user for the bucket size.

If the number of intervals does not divide evenly, the slack should be made up on the *lower* end. See the example output below.

Running the program should look like this:

> **Note:** `$` is the command prompt on Unix.  On Windows it will look like `C:>`

        $ java GradeHistogram grades.csv
        What bucket size would you like?
        >>> 10
        100 - 91 | [][][][][][][][][][][][]
         90 - 81 | [][][][][][][][][][][][][][][][][][][][][][][][][][]
         80 - 71 | [][][][][][][][][][][][][][]
         70 - 61 | [][][][][][][][][][][][][][][][][]
         60 - 51 | [][][][][][][]
         50 - 41 | [][][][]
         40 - 31 | [][][][][][]
         30 - 21 | [][]
         20 - 11 |
         10 -  1 | []
          0 -  0 | []

        $ java GradeHistogram grades.csv 5
        100 - 96 | [][][][][][]
         95 - 91 | [][][][][][]
         90 - 86 | [][][][][][][][][][][][][][][][][]
         85 - 81 | [][][][][][][][][]
         80 - 76 | [][][][][][][][][][][][]
         75 - 71 | [][]
         70 - 66 | [][][][][][][][][][]
         65 - 61 | [][][][][][][]
         60 - 56 | [][][][][]
         55 - 51 | [][]
         50 - 46 | [][][]
         45 - 41 | []
         40 - 36 | [][][]
         35 - 31 | [][][]
         30 - 26 | []
         25 - 21 | []
         20 - 16 |
         15 - 11 |
         10 -  6 | []
          5 -  1 |
          0 -  0 | []


> **Note:** The pipe characters should be aligned and your program must not exclude any subrange between 0 and 100.

##Tips

1. You may assume that you always get valid input.
2. You may assume the text file has valid numbers.
3. 101 is a prime number.
4. An array is a fixed size data structure; you need to know ahead of time how big it needs to be. How do we do this?
5. You can give interpretations to the indices and contents of an array to arrive at creative solutions to problems. Code smart, not hard.
6. Creating a `Scanner` object with a file will throw a checked exception. Don't worry about what this means --- for now, just append `throws Exception` to the end of the `main` method signature wherein the file is opened.

## Submission

You will need to have [sbt](http://www.scala-sbt.org/download.html) installed to submit your homework. Once you have it installed, submit your assignment by running from the root of your homework directory:

        sbt submit

