---
title: Netflix
---
Due: November 6th, 2015 at 11:59 PM

[Download Zip](https://github.gatech.edu/cs1331-fall2015/hw-netflix/archive/master.zip)

## Intro
This homework will cover Collections and Collections Algortihms.

## Problem Description:

You and your roommates own one TV, and share a common collection of movies. However, all of you like different types of movies, and have different ways of arranging them.

Use your knowledge of Collections and Collections Algorithms to write some Java classes which take everyone's preferences, and create a custom Netflix service.

## Solution Description:
**Put your code in ``src/main/java``, as always!**

### (Partially) Provided Files:
#### ``MySet``
`MySet` is the class that implements `Set`. The Shell is provided.

#### ``MySetTester``
This class allows you to test your implementation of `MySet`.

#### ``Genre``
The `Genre` class is an enum of different genres a movie can have. For example, Action, Adventure, Animation, Comedy, Crime, Drama, Horror, Romance, Sci-Fi, Thriller.

#### ``PopulateNetflix``
This class populates your apartment's collection of movies. You will not need to modify this.

#### ``Netflix``
This is a driver class that will suggest movies for the apartment. Follow the instructions in the comments to add a `Map<String, List<Movie>>` representing a mapping from roommate name to their watchlist. Add the following roommates and watch lists to the map:

- "Taylor": Likes Comedy movies displayed alphabetically
- "George": - Likes Action or Adventure movies, sorted by rating (highest first)
- "Sarah":  - Likes movies from the 80s and 90s

### Classes to Create:

#### ``MySet``

To start off, you will need something to store _collections_ of items. You will use the given shell for MySet, that implements the Set interface using a generic type. You must provide all of the methods that the `Set` interface requires. For a detailed description of what each of these methods is supposed to do, check the [Java API page](https://docs.oracle.com/javase/8/docs/api/java/util/Set.html) for the Set interface.

To help check to make sure your implementation is correct, we have provided the driver class MySetTester.java. You are not to modify this file - it should run correctly when provided with your MySet class.

#### ``Movie``

This class describes a Movie on Netflix. Each movie should have -

* Name - `String`
* Rating - `double`
* Year Released - `int`
* Multiple Genres : You've been given a Genre enum which contains individual genres a movie could have. However, on Netflix, a movie can have multiple genres (how else would Rom-Coms exist?). You should be able to store a _collection_ of genres. The constructor (and this field) should be able to accept your implementation from the first part of the hw, and also generalized ones provided by Java.

Provide the general `hashCode()`, `equals()` and `toString()` methods.

You should be able to arrange and group movies based on multiple parameters - alphabetically, by rating, by year range, or by genre. Use your knowledge of Collections in Java to lend this functionality to the Movie class.

#### ``Library``
This class represents a personal Netflix library with a _collection_ of movies. It should use the List collection you learned about to store these. It should have the following methods that allow for easy organization of the movies:

* `listAlphabetically`
* `moviesWithGenre` - Groups by a particular Genre
* `listByYearRange` - takes in a starting (inclusive) and ending (exclusive) year
* `listByLowestRating`
* `listByHighestRating`

### Testing
You have been given the ``Netflix`` driver class that will automatically populate the movies for you. It will use your implementation of all classes to output the queue of movies that will be played in the apartment. 

#### Sample Output -
```
$   java Netflix
Welcome to your very own Netflix service!

Now, who's watching tonight?
1: Taylor
2: George
3: Sarah
4: Close Netflix...
1
Welcome, Taylor! We're preparing your queue. Please wait...
How many movies would you like to watch? (1 - 10)
5

Here you go!

Aladdin (1992) - 8.0
Back to the Future (1985) - 8.5
Finding Nemo (2003) - 8.2
Meet the Parents (2000) - 7.0
Mr. and Mrs. Smith (2005) - 6.5
```

## Checkstyle and Javadocs:
The checkstyle cap for this assignment is 40 points.

For the assignment you will be writing Javadocs. The cap for this assignment is 10 points. Please read the [description](http://cs1331.org/javadoc) on the course website for a walkthrough of how to write javadocs: http://cs1331.org/javadoc

You can run checkstyle (both for checkstyle and javadocs) by running:

        sbt checkstyle

## Submission

You will need to have [sbt](http://www.scala-sbt.org/download.html) installed to submit your homework. Once you have it installed, submit your assignment by running from the root of your homework directory:

        sbt submit
