---
title: hw-chess 
---
**Due:** November 20th, 2015 at 11:59 PM

[Download Zip](https://github.gatech.edu/cs1331-fall2015/hw-chess/archive/master.zip)

## Before You Begin
1. You need at least jdk 1.8.0_40 to do this assignment. You can get the latest jdk [here](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html). You can check your java version with `java -version` and `javac -version`. You can follow the [instructions](http://cs1331.org/install-java.html) on the course website for help re-setting up java.
2. Read over the instructions carefully! There's a LOT here, but don't be overwhelmed. Start by getting something to show up, then move on to add buttons, before worrying about wiring up all the functionality. 
3. As mentioned, there's a LOT here. You have two weeks for this assignment: **start early!!**

### Assignment
You've been given the back end for a [chess](https://en.wikipedia.org/wiki/Chess) application, and your task is to build a UI for it using javafx.

#### How to Work With the Back End
- ***All your interaction with the game logic and state will be done through an interface called `GameController`***
- The basic turn execution depends mostly on 4 methods declared in the `GameController` interface:
  1. `void beginTurn()`: Generates all possible moves for every `Piece` on the `Side` whose turn it is
    - If the result of move generation is empty, the game enters a game over state
    - There are a few options for game over states, and which one is chosen depends on the state at the start of the turn
      - If the current `Side` is in check when `beginTurn()` is called and then has no moves, that side has been checkmated
      - Otherwise, if they did not start the turn in check, the result is a stalemate
  2. `Set<Move> getMovesForPieceAt(Position p)`: If a local user clicks on a tile at a certain position, it is your job to call this method, so that you can highlight valid moves for the piece at that position
    - This is described in more detail below when discussing the UI itself
  3. `void makeMove(Move m)`: After a user then chooses one of the possible `Move`s, you should call this method to execute the move on the logical `Board` itself
  4. `void endTurn()`: Switches the currently active `Side` and evaluates whether or not the new current `Side` is in check
    - This determination of check will affect `beginTurn()` as described above, since whether or not a `Side` begins a turn in check affects the endgame result should this be the final turn
    - So typically, after telling the `GameController` to make a legal move, you'll want to call `endTurn()` and then `beginTurn()` in sequence
    - think of it like ending the current turn, and beginning the next one
- Chess is a two player game, so we've provided a few different controller implementations corresponding to different opponent types (ie local human/pass the laptop to a friend/not-friend, local computer/ai, remote human). Changes between opponents must be accomplished by swapping out the `GameController` implementation used. (hint, see `BoardView::reset`)
  1. `ChessController`: This is the base implementation which allows for a local game between two human players
  2. `AIChessController`: This implementation allows you to play locally against your computer
      - Your computer is not very good, and will just choose moves at random
      - if you want to get more sophisticated with the move selection algorithm, go for it, but it is certainly not required
  3. `NetworkedChessController`: This will allow you to play against another human over the same local network (ie GTwifi) by obtaining their IP address. 
    - If you don't know what an IP address is, that's okay. Think of it like a postal address for the internet.
    - All of the networking code has been written for you. You're  only responsible for providing UI for hosting a game, or for entering an IP address and joining a game.
      - If you HOST, you will play as white
      - If you JOIN, you play as black
    - Additionally, the execution of a turn will be slightly different if your game is networked
      - Instead of making moves for both sides locally (either by a human, or a computer player), you will only ever make moves for one `Side`
      - The other `Side`'s moves will come in from the network when they are made, and your view will be updated accordingly assuming you correctly subscribe to the events we'll discuss below.

-----------------------------------------------------------------------------------------------------------
### UI Programmning
#### Classes

There's a lot of room for you to stretch your creativity here, but at the very least, we'd like to see the following 3 classes:

1. `TileView implements Tile`
  - This class represents the visual representation of a single square on the board.
  - It should have a symbol representation of a piece that occupies this square (hint: see `model.chess.ChessPiece.ChessPieceType`). If the square is unoccupied, the symbol should be empty String.
    - This symbol should be tied to a UI element (hint: `javafx.scene.control.Label`)
  - It should also support color highlighting, so that when someone interacts with the board, spaces can be highlighted to show different information as detailed below
  - Finally, it must contain an instance of a `javafx.scene.Node` to which you can add actual UI objects as children and attach a mouse clicked listener so that the board knows what to do when a particular tile is clicked (hint: `javafx.scene.layout.StackPane`)
2. `BoardView`
  - This class is a visual representation of the logical game board.
  - It should hold an 8x8 `Tile[][]` as well as a `javafx.scene.layout.GridPane` to hold the `Node`s that back each `Tile`
  - The `BoardView` is responsible for setting up the `TileView`s with the proper symbols at the start of the game, and providing a way to reset to the original state at any time
    - ***we have done this step for you by providing the constructor and a reset method***
    - You will just need to be sure to call reset from the appropriate place(s).
  - It is also responsible for determining what happens when a tile is clicked
  - It should also thereby control which tiles get highlighted, and when. We would like to see the following:
    1. When a `Piece` is first clicked, highlight its possible moves, and use a different color to indicate capture moves
    2. If a `Piece` is deselected by clicking on it, remove the highlights made 
    3. When a `Move` is completed successfully, highlight that move some other color so that the opponent can see the last `Move` that was made. You'll also need to clear the highlighting for the old last move
  - Finally, there are several events in the `GameController` which your `BoardView` must subscribe to, described below in the section titled "Events"
    - ***we have already registered empty methods to these events in reset, so you will be responsible for implementing those handler methods in `BoardView`, and not the actual registration***
3. `ChessFX`
  - This class will be the entry point for execution (ie contains the main method) that ties everything together
  - It is responsible for instantiating the `BoardView` and the `GameController`, and should also provide the following additional UI elements
    - A `Text` representing the current state of the game
    - A `Text` telling you whose turn it is
    - A `Button` for resetting to just a normal local game between 2 humans
    - A `Button` for resetting to a game against the computer
    - A `Button` for hosting a networked gamea
    - A `Text` telling you your public IP address - you can get this by calling `InetAddress.getLocalHost().toString()`
    - A `TextField` for entering an IP address
    - A `Button` for joining a game with the IP address entered
      - This button should be disabled until an IP address has been entered
  - We won't be looking for anything else, but if you're looking for an extra challenge, try to add something that will tell you the history of `Move`s made in the game so far

#### Demo
[Here](https://youtu.be/wsm834SVXwE) is a short demo video showing what your interactions should look like.

#### Events
UI programming largely follows the [event driven](https://en.wikipedia.org/wiki/Event-driven_programming) paradigm. At a high-level, there are 'events' that can occur that other parts of the program would like to be notified about. Traditional events involve how your UI handles user input (ie mouse clicked, key pressed, etc - you will be handling mouse clicked events). 

The mechanism by which someone is notified of an event is by registering a callback or listener method. Then when the event occurs, the code that caused the event will also call any callback methods that were listening for that event.

In addition to getting user input, events can also be used more generally, and are very useful for updating UI when certain things happen to the state of the game. For example, when the `GameController` performs a `Move` on the `Board` at your request, the UI needs to be notified so that it can modify tiles on the view to reflect that the move has been made. We'd like you to handle these events in the following way:

1. Listen for changes in the current playing `Side` so that you can update the UI element that tells you whose turn it is
2. Listen for changes in the `GameState`so that you can update the UI element that tells you the current game state
  - Additionally, if the new state is game over, you should use a `javafx.scene.control.Alert` with options to play again or quit
  - [this](http://code.makery.ch/blog/javafx-dialogs-official/) is a useful resource about `Alert`s
3. Listen for moves being made so that you can update your tiles to reflect the move (use `addMoveListener` for this). Listeners for this event will take in a `Move` representing a move that was made on the back end, and a `List<Position>` representing a list of the `Position`s of any pieces that were captured as the result of this move.
4. When a pawn reaches the enemy's back rank, it gets [promoted](https://en.wikipedia.org/wiki/Promotion_(chess)) to another type of piece. Your UI needs to listen for a promotion event, and somehow provide a way to get the type of piece the player wants to promote the pawn to. I'd suggest opening an `Alert` with the user's options and return their choice to the controller.

***again, we have already registered empty handlers to each of these events, so all you'll need to do is implement these handler methods. We hold you responsible for anything marked TODO***

### Compiling & Running
There are two ways to compile & run the code in this assignment. Be aware that until you start making changes in `ChessFX` running the code will do nothing.

1. `sbt compile` and `sbt run`
2.  Manually:
   1.  Create the folders "target/classes" with `mkdir -p target/classes` on Linux/Mac or `mkdir target\classes` on Windows.
   2.  `javac -encoding utf8 -d target/classes/ -cp src/main/java/ src/main/java/boardview/ChessFX.java` to compile
   3.  `java -cp target/classes boardview.ChessFX` to run
