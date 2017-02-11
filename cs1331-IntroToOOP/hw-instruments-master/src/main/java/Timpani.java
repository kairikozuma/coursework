import java.math.BigDecimal;

/**
 * Timpani Stringed Instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
public class Timpani extends Percussion {

    /**
     * Constructs Timpani object
     * @param   price           Price of the instrument
     * @param   serialNumber    Serial number in alphanumeric String of length 6
     */
    public Timpani(BigDecimal price, String serialNumber) {
        super("Timpani", "Dun dun dun", price, serialNumber, true, 3);
    }

    /**
     * Get String representation of Timpani
     * @return String of Timpani with member value details
     */
    @Override
    public String toString() {
        return super.toString()
            + "\n  Number of pieces: " + getNumPieces()
            + "\n  Has sticks: " + hasSticks();
    }
}
