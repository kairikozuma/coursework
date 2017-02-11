---
title: Instruments homework
---

[Download Zip](https://github.gatech.edu/cs1331-fall2015/hw-instruments/archive/master.zip)

## Intro
This homework will cover Inheritance, Polymorphism, and Exceptions

## Problem Description:

You are a Musician. What do musicians play? Instruments! You will be writing code that takes extensive advantage of Polymorphism by writing a few different classes concerning instruments. Then, you will write a class that defines how you buy and sell these instruments and the rules that go along with that. Finally, you will write a driver class that tests all of your beautifully musical code so that everyone can hear how great of a musician you are.

## Solution Description:

**Put your code in ``src/main/java``, as always!**

For this homework, you will be writing 15 classes. Don't fret! Not all of them will be long.

### ``Instrument``

To start with, you will need to write code which will represent an instrument. Instruments have a **price** and a unique **serial number**. 

Each ``Instrument`` will need to have ways to retrieve the values of its price and serial number properties. It will also have a method called ``play()``, which will return a ``String`` that represents the sound of the current ``Instrument``. Be creative here; you can have fun with this.

Two instruments are considered equal if they have the same serial number.

> Think: should ``Instrument`` be abstract? If so, which (if any) of its methods should be abstract?

### Types of Instruments

Following this, you will need to write four kinds of instruments: 

- ``Stringed``
- ``Brass``
- ``Woodwind``
- ``Percussion``

> **Think**: Should these classes be abstract or not? How do they fit in to the hierarchy?

Each of these classes should introduce a new property which is up to you to decide what it will be. Think about the differences between the classes (``Stringed``, ``Brass``, etc) and also think about "Is-A" and how that applies to your choosing of a property. Also make sure that you provide a way to retrieve the value of your new property.

### Specific Instruments

Next, you will write a class that Is-A ``Stringed``, a class that ``Is-A Brass``,a class that Is-A ``Woodwind``, and a class that Is-A ``Percussion``.

For instance: 
- a violin is a stringed instrument.
- a trumpet Is-A brass instrument.
- a flute Is-A woodwind instrument.
- a snare drum Is-A percussion instrument.

Feel free to use different instruments for these classes. Just make sure that whatever you choose actually is a proper variant. A trombone is not a stringed instrument. You get the idea.

Each of these classes should implement ``toString()``, which will return a ``String`` that contains all of the properties that apply to the current class. Make sure you use all of them.

### Musician

Next, you will write code to represent a ``Musician``. The ``Musician`` is the player of all of your glorious instruments, and has:

- some number of instruments
- a name,
- funds

Make sure you have a way of retrieving these values from musicians.

Musicians can:
- ``purchase`` an instrument, thereby adding the instrument to his or her collection.
- ``sell`` an instrument, thereby removing the instrument from his or her collection.

> **Think**: How do these operations affect the Musician's funds?


### Exceptions

But wait! Things can go wrong! *EXCEPTIONS!*

There are rules to how the instruments can be purchased and sold. Here are the rules:

1. A musician cannot buy an instrument if he does not have enough funds.
2. A musician cannot buy the same instrument twice. This means that if he were to try and buy an instrument that he already had, he would not be able to do so. This does **not** mean he can't buy two of the same instrument. 
3. A musician cannot sell his last instrument. What would he be without anything to make beautiful music? He always has to have at least one. Always. At all times.
4. A musician cannot sell an instrument that he does not already own.

Each of these rules corresponds to an Exception. You will need to write an exception for each rule, and throw the corresponding exception whenever a rule has been violated.

For example, if there were a fifth rule (there is not, this is for the sake of example) where the musician always needed to have more than 0 in his funds, I would write an ``Exception``  called ``NeedsAtLeastFiftyBucksException`` and throw it whenever an operation would cause the Musician's funds to drop below that amount.

Make sure the Exceptions you write are informative and let the user know
exactly what went wrong.

### Testing

And finally! One last class!

You need to write a ``MusicStore`` class that tests all of your code. This is pretty open-ended.
If you wrote a method, call it to make sure it works! If you wrote an Exception, make sure it gets thrown when it should. Leave nothing un-tested.

Try to save yourself some time here! Don't instantiate something just to call its ``toString()`` method; do more with it, that way you don't spend all of your time instantiating objects left and right. As always, work smart, not hard.


## Tips:
- We've intentionally left out the types of the properties we require you to have. You can use whatever you like as long as it makes sense and is consistent.
- As an example, the output for calling the `play()` method might be something like... toodaloodalooo (this is a flute, beautiful right?)
- `serialNumber` can be formatted however you would like. Again, consistency.
- Remember constructors!
- The API should not be super necessary for this assignment, though it may prove useful for Exceptions.
- Think hard about where you should implement `play()`
- Try not to confuse `String` (as in the Java class `java.lang.String` that represents the type `String`) and `Stringed` (the Java class you are writing that represents an `Instrument` with physical strings

## Checkstyle and (NEW) Javadocs:
The checkstyle cap for this assignment is 40 points. 

For the assignment you will be writing Javadocs. The cap for this assignment is 0 points, in future assignments it will be higher. Please read the [description](http://cs1331.org/javadoc) on the course website for a walkthrough of how to write javadocs: http://cs1331.org/javadoc

You can run checkstyle (both for checkstyle and javadocs) by running:
        
        sbt checkstyle

## Submission

You will need to have [sbt](http://www.scala-sbt.org/download.html) installed to submit your homework. Once you have it installed, submit your assignment by running from the root of your homework directory:

        sbt submit
