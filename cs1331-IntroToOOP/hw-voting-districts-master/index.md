---
title: Voting Districts
---

Due: Friday, December 4th at 11:59 PM

[Download Zip](https://github.gatech.edu/cs1331-fall2015/hw-voting-districts/archive/master.zip)

## Intro
In this assignment you will be writing code to represent voting districts, calculate the winner of an election, and write an implementation of mergesort to sort the voting districts by their chosen candidate.

## Problem Description
### Voters
Each voter gets three votes representing his or her first, second, and third choices. The first choice vote is worth 3 points, the second choice vote is worth 2 points, and the third choice vote is worth 1.

### Districts
A district represents a geographical area. It can either a big area that's broken down into smaller areas (like a state broken down into counties) or a small area (like a county) that consists of a bunch of voters.
The bigger kind of district is called a composite district since it is composed of other, smaller districts, though these smaller districts could still be composite districts (for example, a state contains counties which contain cities, etc.).

The smaller kind of district is called a local district. Local districts are composed of a collection of voters.

Each district gets a single, weighted vote. The vote's weight is determined by the district's size, which is determined by the sum of the sizes of its sub-districts for a composite district or the number of voters for a local district.

The vote for a local district is calculated by taking the candidate with the highest score from the voters. The vote for a composite district is calculated by taking the candidate with the highest score from its sub-districts.

### Example
District A is composed of districts B and C. District B is composed of districts D and E.

District D is composed of 100 voters.
50 voters choose candidate 1 as their first choice, 20 choose him as their second choice, and 15 as their third choice, so his score is: `3*50 + 20*2 + 15*1 = 205`.

30 voters choose candidate 2 as their first choice, 40 choose him as their second choice, and 40 as their third choice, so his score is: `3*30 + 40*2 + 40*1 = 210`.

20 voters choose candidate 3 as their first choice, 40 choose him as their second choice, and 45 as their third choice, so his score is: `3*20 + 40*2 + 45*1 = 195`.

Thus, the vote for District D is Candidate 2, with a score of 100.

If District E is composed of 50 voters and votes for Candidate 1, then District B votes for Candidate 2, since he scored higher, with a score of 150 (size of D + size of E)

District C votes for Candidate 3 and has a size of 300.

District A then has Candidate 3 with a score of 300, and Candidate 2 with score of 150 to choose from, so Candidate 3 wins the election.

There are dummy data files in `src/main/resources` that you may use to test if you wish.
- `districts.txt`
    The first line contains the main districts. Each line after that represents a district and which districts it is composed of.
- `local_districts.txt` contains the local districts. Each line represents a local district and the voters it is composed of.
- `voters.txt`contains the voters. Each line represents a voter and his or her first, second, and third choices.

> **Tip**: When parsing, make sure to parse the voters first since you need that data when constructing local districts, and parse local districts before the other districts, etc.

As a sanity check, Tony Stark should be the winner with a score of 25 due to his victory in Indiana with a score of 19. Darth Vader should be the runner up with a score of 6 from Georgia.
## Sorting
In some testing class, create a list of districts and mergesort them by their candidate. You must write the code to sort it yourself, without using `Collections.sort`. You must write mergesort, not another kind of sorting.

To calculate the candidate with the maximum score, you will likely have to sort a list of candidates. You must use your custom mergesort implementation for this.

This can simply be a helper method in the same class that you test your voting districts code.

## Tips
- When calculating the candidate with the maximum score, Java's `Map` will likely be useful.`Map` has an inner class called `Map.Entry` which represents an entry in the map and contains both the key and the value. This is useful if you want to sort a map's keys by their corresponding values. You can get the entries from a map with the `entrySet()` method.


## Extra Credit
How much this worth will be announced soon.

Write a JavaFX GUI that draws the districts as maps. Voters should be represented as dots, and the size of the district should correspond to the size of the shapes drawn in the GUI. Use a different color for each candidate and display a key showing which candidate is what color. Display the overall winner as a separate text field.

## Checkstyle and Javadocs:
The checkstyle cap for this assignment is 100 points.

For the assignment you will be writing Javadocs. The cap for this assignment is 10 points. Please read the [description](http://cs1331.org/javadoc) on the course website for a walkthrough of how to write javadocs: http://cs1331.org/javadoc

You can run checkstyle (both for checkstyle and javadocs) by running:

        sbt checkstyle


## Submission
As always, place your code in `src/main/java`

You will need to have [sbt](http://www.scala-sbt.org/download.html) installed to submit your homework. Once you have it installed, submit your assignment by running from the root of your homework directory:

        sbt submit
