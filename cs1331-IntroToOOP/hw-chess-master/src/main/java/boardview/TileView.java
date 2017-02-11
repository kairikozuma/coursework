package boardview;

import javafx.scene.paint.Color;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.GridPane;
import javafx.geometry.HPos;
import javafx.scene.control.Label;
import javafx.scene.Node;
import javafx.scene.text.Font;
import javafx.scene.layout.Region;
import model.Position;

/**
 * View class for a tile on a chess board
 * A tile should be able to display a chess piece
 * as well as highlight itself during the game.
 *
 * @author Kairi Kozuma
 * @version 1.0
 */
public class TileView implements Tile {

    private static final int TILE_HEIGHT = 60;
    private static final int TILE_WIDTH = 70;

    // Member variables
    private String symbol;
    private Position position;
    private StackPane stackPane;
    private Region colorFilter;
    private Label symbolLabel;

    /**
     * Creates a TileView with a specified position
     * @param p position of the tile
     */
    public TileView(Position p) {
        // Initiliaze variables
        position = p;
        stackPane = new StackPane();
        stackPane.setPrefSize(TILE_WIDTH, TILE_HEIGHT);
        colorFilter = new Region();
        symbol = "";
        symbolLabel = new Label("");
        symbolLabel.setFont(new Font("Cambria", TILE_HEIGHT));
        GridPane.setHalignment(symbolLabel, HPos.CENTER);

        // Set checkerboard pattern
        if ((p.getRow() + p.getCol()) % 2 == 0) {
            stackPane.setStyle("-fx-background-color : white;");
        } else {
            stackPane.setStyle("-fx-background-color : grey;");
        }
        stackPane.getChildren().addAll(colorFilter, symbolLabel);
    }


    @Override
    public Position getPosition() {
        return position;
    }

    @Override
    public Node getRootNode() {
        return stackPane;
    }

    @Override
    public void setSymbol(String symbol) {
        this.symbol = symbol;
        symbolLabel.setText(symbol);
    }

    @Override
    public String getSymbol() {
        return symbol;
    }

    @Override
    public void highlight(Color color) {
        if (color.equals(Color.TRANSPARENT)) {
            colorFilter.setStyle("-fx-background-color : #00000000;");
        } else {
            colorFilter.setStyle("-fx-background-color : #"
                + color.toString().substring(2, 8) + "5E;");
        }
    }


    @Override
    public void clear() {
        symbol = "";
    }
}
