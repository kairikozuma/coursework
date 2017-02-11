package model.chess;

import java.util.HashSet;
import java.util.Set;

import junit.framework.TestCase;

import model.Move;
import model.Piece;
import model.Position;
import model.Side;

import org.junit.Assert;
import org.junit.Rule;
import org.junit.Test;
import org.junit.Before;
import org.junit.rules.Timeout;

import model.Piece;
import model.Position;
import model.Move;
import model.Side;
import model.Board;
import model.chess.ChessBoard;

public class PawnTest extends TestCase {
    private Piece whitePawn;
    private Piece blackPawn;
    private Board chessboard;


    @Rule
    public Timeout timeout = Timeout.millis(200);

    @Before
    public void setUp() {
        chessboard = new ChessBoard();
        whitePawn = new Pawn(Side.WHITE);
        blackPawn = new Pawn(Side.BLACK);
    }

    @Test
    public void testMoves() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Set<Move> moves = whitePawn.generateMoves(new Position(i, j));
                assertNotNull(moves);
                validateMoves(moves, -1, 6);

                moves = blackPawn.generateMoves(new Position(i, j));
                assertNotNull(moves);
                validateMoves(moves, 1, 1);
            }
        }
    }

    @Test
    public void centralPositionWithoutEnemyTest() {

        Position startPos = new Position(4, 4);

        Set<Move> validMoves = new HashSet<>();
        // There is only one place this pawn can go
        validMoves.add(new Move(startPos, new Position(3, 4)));


        Set<Move> generated = whitePawn.generateMoves(startPos);
        compareValidAndGenerated(validMoves, generated);

    }

    @Test
    public void centralPositionWithEnemyTest() {

        Position whiteStart = new Position(4, 4);
        Position blackStart = new Position(3, 3);

        Set<Move> whiteValidMoves = new HashSet<>();
        Set<Move> blackValidMoves = new HashSet<>();

        // Can move forward or capture
        whiteValidMoves.add(new Move(whiteStart, new Position(3, 4)));
        whiteValidMoves.add(new Move(whiteStart, new Position(3, 3)));

        blackValidMoves.add(new Move(blackStart, new Position(4, 3)));
        blackValidMoves.add(new Move(blackStart, new Position(4, 4)));

        Set<Move> generated = whitePawn.generateMoves(whiteStart);
        compareValidAndGenerated(whiteValidMoves, generated);

        generated = blackPawn.generateMoves(blackStart);
        compareValidAndGenerated(blackValidMoves, generated);

    }

    @Test
    public void startingPositionWithoutEnemyTest() {

        Position startPos = new Position(6, 4);

        Set<Move> validMoves = new HashSet<>();
        // Can move one or two spaces to start
        validMoves.add(new Move(startPos, new Position(5, 4)));
        validMoves.add(new Move(startPos, new Position(4, 4)));


        Set<Move> generated = whitePawn.generateMoves(startPos);
        compareValidAndGenerated(validMoves, generated);

    }

    @Test
    public void startingPositionWithEnemyTest() {

        Position whiteStart = new Position(6, 4);
        Position blackStart = new Position(5, 3);

        Set<Move> whiteValidMoves = new HashSet<>();
        Set<Move> blackValidMoves = new HashSet<>();

        // Can move forward 1, 2, or capture
        whiteValidMoves.add(new Move(whiteStart, new Position(5, 4)));
        whiteValidMoves.add(new Move(whiteStart, new Position(5, 3)));
        whiteValidMoves.add(new Move(whiteStart, new Position(4, 4)));

        blackValidMoves.add(new Move(blackStart, new Position(6, 3)));
        blackValidMoves.add(new Move(blackStart, new Position(6, 4)));

        Set<Move> generated = whitePawn.generateMoves(whiteStart);
        compareValidAndGenerated(whiteValidMoves, generated);

        generated = blackPawn.generateMoves(blackStart);
        compareValidAndGenerated(blackValidMoves, generated);

    }

    public void compareValidAndGenerated(Set<Move> valid, Set<Move> generated) {
        assertFalse("Generated Pawn moves are fewer than actual",
            generated.size() < valid.size());
        assertFalse("Generated Pawn moves are greater than actual",
            generated.size() > valid.size());

        for (Move m: generated) {
            assertTrue(generateErrorMessage("Illegal move generated.", m),
                valid.contains(m));
        }

        assertEquals("Generated and Valid moves are not the same", generated, valid);
    }

    public void validateMoves(Set<Move> moves, int dy, int yInit) {
        for (Move m : moves) {

            int startCol = m.getStart().getCol();
            int startRow = m.getStart().getRow();

            int destCol = m.getDestination().getCol();
            int destRow = m.getDestination().getRow();

            boolean boundsTest =
                destCol >= 0
                && destCol < 8
                && destRow >= 0
                && destRow < 8;

            boolean legalityTest =
                (Math.abs(destCol - startCol) == 1 || destCol == startCol)
                && (destRow - startRow == dy
                    || ( (destRow - startRow) == (2 * dy)
                        && startRow == yInit
                        && destCol == startCol ));

            assertTrue(
                generateErrorMessage("Out of bounds move generated.", m),
                boundsTest);

            assertTrue(
                generateErrorMessage("Illegal move generated.", m),
                legalityTest);
        }
    }

    public String generateErrorMessage(String msg, Move m) {
        int startCol = m.getStart().getCol();
        int startRow = m.getStart().getRow();

        int destCol = m.getDestination().getCol();
        int destRow = m.getDestination().getRow();

        StringBuilder sb = new StringBuilder();
        sb.append("Error: ");
        sb.append(msg);
        sb.append("\n");
        sb.append(m.toString());

        return sb.toString();
    }
}
