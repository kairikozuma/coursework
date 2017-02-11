package boardview;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import gamecontrol.AIChessController;
import gamecontrol.ChessController;
import gamecontrol.GameController;
import gamecontrol.NetworkedChessController;
import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;


/**
 * Main class for the chess application
 * Sets up the top level of the GUI
 * @author Gustavo
 * @version 1.0
 */
public class ChessFX extends Application {

    private GameController controller;
    private BoardView board;
    private HBox buttonRow;
    private HBox networkBar;
    private Button btnReset2P;
    private Button btnResetCmp;
    private Button btnHost;
    private Button btnJoin;
    private Text ipAddrText;
    private TextField ipAddrInput;
    private VBox root;

    @Override
    public void start(Stage primaryStage) {
        // 2 Player by default
        controller = new ChessController();
        controller.startGame();

        board = new BoardView(controller);

        // Set up button bar with reset buttons
        btnReset2P = new Button();
        btnResetCmp = new Button();
        buttonRow = new HBox();

        // Set the correct controller for reset button
        btnReset2P.setText("Reset");
        btnReset2P.setOnAction(event -> {
                board.reset(new ChessController());
            }
        );

        // Set the AI controller for the Play Computer button
        btnResetCmp.setText("Play Computer");
        btnResetCmp.setOnAction(event -> {
                board.reset(new AIChessController());
            }
        );
        buttonRow.setPadding(new Insets(15, 12, 15, 12));
        buttonRow.setSpacing(10);
        buttonRow.getChildren()
            .addAll(btnReset2P, btnResetCmp);

        // Set up network bar
        btnHost = new Button();
        btnJoin = new Button();
        try {
            ipAddrText = new Text(InetAddress.getLocalHost().toString());
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        ipAddrInput = new TextField();
        networkBar = new HBox();

        // Set default NetworkedChessController for hosting
        btnHost.setText("Host");
        btnHost.setOnAction(event -> {
                board.reset(new NetworkedChessController());
            }
        );

        // Set NetworkedChessController with ip address string for joining
        ipAddrInput.setPromptText("a.b.c.x");
        btnJoin.setText("Join");
        btnJoin.setOnAction(event -> {
                try {
                    board.reset(new NetworkedChessController(
                        InetAddress.getByName(ipAddrInput.getText().toString())
                    ));
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                }
            }
        );
        networkBar.setPadding(new Insets(15, 12, 15, 12));
        networkBar.setSpacing(10);
        networkBar.getChildren()
            .addAll(btnHost, ipAddrText, ipAddrInput, btnJoin);

        // Add all views to root view
        root = new VBox();
        root.getChildren().addAll(board.getView(), buttonRow, networkBar);

        Scene scene = new Scene(root);

        primaryStage.setTitle("CS 1331 Chess");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private EventHandler<? super MouseEvent> makeHostListener() {
        return event -> {
            board.reset(new NetworkedChessController());
        };
    }

    private EventHandler<? super MouseEvent> makeJoinListener(TextField input) {
        return event -> {
            try {
                InetAddress addr = InetAddress.getByName(input.getText());
                GameController newController
                    = new NetworkedChessController(addr);
                board.reset(newController);
            } catch (IOException e) {
                e.printStackTrace();
            }
        };
    }

    /**
     * Main method that starts JavaFX program
     * @param args arguments for launch
     */
    public static void main(String[] args) {
        launch(args);
    }
}
