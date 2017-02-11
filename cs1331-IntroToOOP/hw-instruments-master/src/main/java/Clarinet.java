import java.math.BigDecimal;

/**
 * Clarinet object that extends Woodwind
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
public class Clarinet extends Woodwind {

    /**
     * Constructs clarinet object
     * @param   price           Price of the instrument
     * @param   serialNumber    Serial number in alphanumeric String of length 6
     */
    public Clarinet(BigDecimal price, String serialNumber) {
        super("Clarinet", "Puuuuuuu", price, serialNumber, true);
    }

    /**
     * Get String representation of clarinet
     * @return String of clarinet with member value details
     */
    @Override
    public String toString() {
        return super.toString()
            + "\n  Reed Instrument: " + hasReed()
            + (hasReed() ? "\n  Reed Quality: " + getReedQuality() : "");
    }
}
