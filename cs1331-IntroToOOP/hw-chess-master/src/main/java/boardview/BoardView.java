package boardview;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.Optional;
import gamecontrol.GameController;
import gamecontrol.GameState;
import gamecontrol.NetworkedChessController;
import gamecontrol.AIChessController;
import gamecontrol.ChessState;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.geometry.HPos;
import javafx.geometry.Pos;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Priority;
import javafx.scene.text.Text;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ButtonType;
import javafx.scene.paint.Color;
import model.Move;
import model.Piece;
import model.PieceType;
import model.Position;
import model.Side;
import model.IllegalMoveException;

/**
 * A class for a view for a chess board. This class must have a reference
 * to a GameController for chess playing chess
 * @author Gustavo
 * @version 1.0
 */
public class BoardView {

    /* You may add more instance data if you need it */
    protected GameController controller;
    private GridPane gridPane;
    private VBox root;
    private HBox boardInfo;
    private Tile[][] tiles;
    private Text sideStatus;
    private Text state;
    private boolean isRotated;
    private Tile firstTile;
    private Move lastMove;

    /**
     * Construct a BoardView with an instance of a GameController
     * @param controller The controller for the chess game
     */
    public BoardView(GameController controller) {
        this(controller, new Text(), new Text());
    }

    /**
     * Construct a BoardView with an instance of a GameController
     * and a couple of Text object for displaying info to the user
     * @param controller The controller for the chess game
     * @param state A Text object used to display state to the user
     * @param sideStatus A Text object used to display whose turn it is
     */
    public BoardView(GameController controller, Text state, Text sideStatus) {
        this.controller = controller;
        this.state = state;
        this.state.setText(ChessState.START.toString());
        this.sideStatus = sideStatus;
        this.sideStatus.setText("Turn: "
            + controller.getCurrentSide().toString());
        tiles = new Tile[8][8];
        gridPane = new GridPane();
        root = new VBox();
        boardInfo = new HBox();
        boardInfo.setPadding(new Insets(15, 12, 15, 12));
        boardInfo.setSpacing(10);
        boardInfo.setAlignment(Pos.CENTER);
        boardInfo.getChildren().addAll(sideStatus, state);
        root.getChildren().addAll(gridPane, boardInfo);
        gridPane.setStyle("-fx-background-color : goldenrod;");
        reset(controller);
        firstTile = null;
        lastMove = null;
        controller.beginTurn();
    }

    /**
     * Listener for clicks on a tile
     *
     * @param tile The tile attached to this listener
     * @return The event handler for all tiles.
     */
    private EventHandler<? super MouseEvent> tileListener(Tile tile) {
        return event -> {
            if (controller instanceof NetworkedChessController
                    && controller.getCurrentSide()
                    != ((NetworkedChessController) controller).getLocalSide()) {
                //not your turn!
                return;
            }

            // Don't change the code above this :)
            if (firstTile != null) {
                secondClick(tile);
            } else {
                firstClick(tile);
            }
        };
    }

    /**
     * Perform the first click functions, like displaying
     * which are the valid moves for the piece you clicked.
     * @param tile The TileView that was clicked
     */
    private void firstClick(Tile tile) {
        if (tile.getSymbol().isEmpty() || !(controller.getCurrentSide()
            .equals(controller.getPieceAt(tile.getPosition()).getSide()))) {
            return;
        }

        // Set selected tile
        firstTile = tile;

        // Get valid moves for piece in tile
        Set<Move> validMoves = controller
            .getMovesForPieceAt(tile.getPosition());

        // Highlight current tile yellow
        tile.highlight(Color.YELLOW);

        // Highlight all valid moves green, red if capture
        for (Move m : validMoves) {
            Tile t = getTileAt(m.getDestination());
            if (!controller.moveResultsInCapture(m)) {
                t.highlight(Color.GREEN);
            } else {
                t.highlight(Color.RED);
            }
        }
    }
    /**
     * Perform the second click functions, like
     * sending moves to the controller but also
     * checking that the user clicked on a valid position.
     * If they click on the same piece they clicked on for the first click
     * then you should reset to click state back to the first click and clear
     * the highlighting effected on the board.
     *
     * @param tile the TileView at which the second click occurred
     */
    private void secondClick(Tile tile) {
        // Get valid moves for piece in tile
        Set<Move> validMoves = controller
            .getMovesForPieceAt(firstTile.getPosition());

        // If same tile selected
        if (firstTile.equals(tile)) {

            // Clear highlights
            tile.highlight(Color.TRANSPARENT);
            for (Move m : validMoves) {
                getTileAt(m.getDestination()).highlight(Color.TRANSPARENT);
            }

            // Keep last move as yellow highlight
            if (!(controller instanceof AIChessController
                || controller instanceof NetworkedChessController)) {
                if (lastMove != null) {
                    getTileAt(lastMove.getStart()).highlight(Color.YELLOW);
                    getTileAt(lastMove.getDestination())
                        .highlight(Color.YELLOW);
                }
            }
            // Reset selected tile
            firstTile = null;
        } else {
            Position destination = tile.getPosition();
            Move moveMade = null;
            Iterator<Move> moveIterator = validMoves.iterator();
            while (moveIterator.hasNext() && (moveMade == null)) {
                Move aMove = moveIterator.next();
                if (destination.equals(aMove.getDestination())) {
                    moveMade = aMove;
                }
            }
            if (moveMade != null) {
                firstTile = null;
                // TODO : list of positions
                List<Position> capturedPos = new ArrayList<Position>();
                capturedPos.add(moveMade.getDestination());
                try {
                    lastMove = moveMade;
                    controller.makeMove(moveMade);
                    updateView(moveMade, capturedPos);
                    controller.endTurn();
                    controller.beginTurn();
                } catch (IllegalMoveException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * This method should be called any time a move is made on the back end.
     * It should update the tiles' highlighting and symbols to reflect the
     * change in the board state.
     *
     * @param moveMade the move to show on the view
     * @param capturedPositions a list of positions where pieces were captured
     *
     */
    public void updateView(Move moveMade, List<Position> capturedPositions) {
        String symbol = controller
            .getSymbolForPieceAt(moveMade.getDestination());
        Tile startTile = getTileAt(moveMade.getStart());
        Tile endTile = getTileAt(moveMade.getDestination());
        startTile.setSymbol("");
        endTile.setSymbol(symbol);
        for (Tile[] tileArray : tiles) {
            for (Tile t : tileArray) {
                t.highlight(Color.TRANSPARENT);
            }
        }
        startTile.highlight(Color.YELLOW);
        endTile.highlight(Color.YELLOW);
    }

    /**
     * Asks the user which PieceType they want to promote to
     * (suggest using Alert). Then it returns the Piecetype user selected.
     *
     * @return  the PieceType that the user wants to promote their piece to
     */
    private PieceType handlePromotion() {
        List<PieceType> pieceList = controller.getPromotionTypes();

        Alert alert = new Alert(AlertType.CONFIRMATION);
        alert.setTitle("Promote");
        alert.setHeaderText("Promote a piece");
        alert.setContentText("Promote this piece to: ");

        List<ButtonType> btnArray = new ArrayList<ButtonType>(pieceList.size());

        for (PieceType pt : pieceList) {
            btnArray.add(new ButtonType(pt.toString()));
        }
        alert.getButtonTypes().setAll(btnArray);

        Optional<ButtonType> result = alert.showAndWait();
        return pieceList.get(btnArray.indexOf(result.get()));
    }

    /**
     * Handles a change in the GameState (ie someone in check or stalemate).
     * If the game is over, it should open an Alert and ask to keep
     * playing or exit.
     *
     * @param s The new Game State
     */
    public void handleGameStateChange(GameState s) {
        state.setText(s.toString());
        if (s.isGameOver()) {
            Alert alert = new Alert(AlertType.CONFIRMATION);
            alert.setTitle("Game Over");
            alert.setHeaderText("Result: " + s);
            alert.setContentText("Continue playing or exit?");

            ButtonType btnCont = new ButtonType("Continue");
            ButtonType btnExit = new ButtonType("Exit");
            alert.getButtonTypes().setAll(btnCont, btnExit);

            Optional<ButtonType> result = alert.showAndWait();
            if (result.get() == btnCont) {
                // Continue
                reset(controller.getNewInstance());
            } else if (result.get() == btnExit) {
                // Exit
                Platform.exit();
            } else {
                // Closed dialog
                Platform.exit();
            }
        }
    }

    /**
     * Updates UI that depends upon which Side's turn it is
     *
     * @param s The new Side whose turn it currently is
     */
    public void handleSideChange(Side s) {
        sideStatus.setText("Turn: " + s.toString());
    }

    /**
     * Resets this BoardView with a new controller.
     * This moves the chess pieces back to their original configuration
     * and calls startGame() at the end of the method
     * @param newController The new controller for this BoardView
     */
    public void reset(GameController newController) {
        lastMove = null;
        if (controller instanceof NetworkedChessController) {
            ((NetworkedChessController) controller).close();
        }
        controller = newController;
        isRotated = false;
        if (controller instanceof NetworkedChessController) {
            Side mySide
                = ((NetworkedChessController) controller).getLocalSide();
            if (mySide == Side.BLACK) {
                isRotated = true;
            }
        }

        state.setText(ChessState.START.toString());
        sideStatus.setText("Turn: " + controller.getCurrentSide().toString());

        // controller event handlers
        // We must force all of these to run on the UI thread
        controller.addMoveListener(
                (Move move, List<Position> capturePositions) ->
                Platform.runLater(
                    () -> updateView(move, capturePositions)));

        controller.addCurrentSideListener(
                (Side side) -> Platform.runLater(
                    () -> handleSideChange(side)));

        controller.addGameStateChangeListener(
                (GameState state) -> Platform.runLater(
                    () -> handleGameStateChange(state)));

        controller.setPromotionListener(() -> handlePromotion());


        addPieces();
        controller.startGame();
        if (isRotated) {
            setBoardRotation(180);
        } else {
            setBoardRotation(0);
        }
    }

    /**
     * Initializes the gridPane object with the pieces from the GameController.
     * This method should only be called once before starting the game.
     */
    private void addPieces() {
        gridPane.getChildren().clear();
        Map<Piece, Position> pieces = controller.getAllActivePiecesPositions();
        /* Add the tiles */
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Tile tile = new TileView(new Position(row, col));
                gridPane.add(tile.getRootNode(),
                        1 + tile.getPosition().getCol(),
                        1 + tile.getPosition().getRow());
                GridPane.setHgrow(tile.getRootNode(), Priority.ALWAYS);
                GridPane.setVgrow(tile.getRootNode(), Priority.ALWAYS);
                getTiles()[row][col] = tile;
                tile.getRootNode().setOnMouseClicked(
                        tileListener(tile));
                tile.clear();
                tile.setSymbol("");
            }
        }
        /* Add the pieces */
        for (Piece p : pieces.keySet()) {
            Position placeAt = pieces.get(p);
            getTileAt(placeAt).setSymbol(p.getType().getSymbol(p.getSide()));
        }
        /* Add the coordinates around the perimeter */
        for (int i = 1; i <= 8; i++) {
            Text coord1 = new Text((char) (64 + i) + "");
            GridPane.setHalignment(coord1, HPos.CENTER);
            gridPane.add(coord1, i, 0);

            Text coord2 = new Text((char) (64 + i) + "");
            GridPane.setHalignment(coord2, HPos.CENTER);
            gridPane.add(coord2, i, 9);

            Text coord3 = new Text(9 - i + "");
            GridPane.setHalignment(coord3, HPos.CENTER);
            gridPane.add(coord3, 0, i);

            Text coord4 = new Text(9 - i + "");
            GridPane.setHalignment(coord4, HPos.CENTER);
            gridPane.add(coord4, 9, i);
        }
    }

    private void setBoardRotation(int degrees) {
        gridPane.setRotate(degrees);
        for (Node n : gridPane.getChildren()) {
            n.setRotate(degrees);
        }
    }

    /**
     * Gets the view to add to the scene graph
     * @return A pane that is the node for the chess board
     */
    public Pane getView() {
        return root;
    }

    /**
     * Gets the tiles that belong to this board view
     * @return A 2d array of TileView objects
     */
    public Tile[][] getTiles() {
        return tiles;
    }

    private Tile getTileAt(int row, int col) {
        return getTiles()[row][col];
    }

    private Tile getTileAt(Position p) {
        return getTileAt(p.getRow(), p.getCol());
    }

}
