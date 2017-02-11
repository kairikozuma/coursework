import java.math.BigDecimal;

/**
 * Viola Stringed Instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
public class Viola extends Stringed {

    /**
     * Constructs Viola object
     * @param   price           Price of the instrument
     * @param   serialNumber    Serial number in alphanumeric String of length 6
     */
    public Viola(BigDecimal price, String serialNumber) {
        super("Viola", "Nweeeeeh", price, serialNumber, true, 4);
    }

    /**
     * Get String representation of Viola
     * @return String of Viola with member value details
     */
    @Override
    public String toString() {
        return super.toString()
            + "\n  Number of Strings: " + getNumStrings()
            + "\n  Has a Bow: " + hasBow();
    }
}
