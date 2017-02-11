---
title: Todo List
---

Due Date: Friday, September 25th at 11:59 PM

[Download Zip](https://github.gatech.edu/cs1331-fall2015/hw-todo-list/archive/master.zip)

## Intro & Setup
Write a todo list and interactive program to use it.

Put all of your code inside `src/main/java` 

## Problem Description
The todo list contains at most 10 goals, each of which is broken into sub-tasks. Some more details:

### Goals...
...are long term desires that might be composed of several tasks. They have:

- name
- at most 10 sub-tasks (see below)
- status (complete or incomplete). This should be dependent on the status of the sub-tasks - once they are completed, the goal is completed.

### Tasks...
...are short-term pieces of work with:

- name
- priority
- due date
- status (complete or incomplete)


### Users
Your todo list program should allow for multiple users. Each user has

- username
- a todo list

### File IO
You have been provided with a file, `users.csv` which contains a list of users. It will look something like:

        tshields3, csimpkins, dlillethun

Each user will have a corresponding file that contains his/her todo list. It might look something like:
        
        Goal Name, Task Name | Priority | Due Date | Status, Task Name | Priority | Due Date | Status, etc...

### Interaction
You should write a command-line program that allows for the following interactions with the todo list:

- Prompts the user for his username
- Loads the user's todo list from a file named `[username].csv`, where `[username]` is the username of the current user. If the file doesn't exist, create one.
- Displays the user's tasks/goals after they log in
- Allows the user to add new tasks or goals. 
    - When adding a task, the user should be able to select an existing goal to apply it to.
- Allows the user to mark tasks as completed.
- Save the todo list to the user's file when they are finished

Example:

        $> What is your name? thomas
        $> Goals:
            Redecorate house - Incomplete
                - Paint room - High Priority - due 2015-09-17 12:00 - Complete
                - Replace carpet - Low Priority - due 2015-09-16 15:00 - Incomplete
            Finish HW - Complete
                - CS HW - Medium Priority - due 2015-09-16 15:00 - Complete
                - English HW  - Medium Priority - due 2015-09-15 23:59 - Complete
        $> Select an option:
            [1] Add Task
            [2] Add Goal
            [3] Quit & Save
         1
        ...

This is just one possible way it could look; you may do it however you like.

## Files
You may name your classes whatever you like, but be sure the class containing the main method is named `TodoList`. I.e, you would always be able to run `java TodoList`

## Checkstyle
You will need to run checkstyle on your submission. For this assignment you may lose up to 15 points maximum, one per checkstyle error. For now, run checkstyle without `sbt`. We hope to push out an update soon that will allow you to run `sbt checkstyle` smoothly.

## Tips
- Java's [LocalDateTime](https://docs.oracle.com/javase/8/docs/api/java/time/LocalDateTime.html) could be useful, though it is not necessary. If you use, it, the `parse` method could be useful. ;)

## Submission

You will need to have [sbt](http://www.scala-sbt.org/download.html) installed to submit your homework. Once you have it installed, submit your assignment by running from the root of your homework directory:

        sbt submit

