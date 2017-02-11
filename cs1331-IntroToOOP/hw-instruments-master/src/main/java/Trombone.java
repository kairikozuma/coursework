import java.math.BigDecimal;

/**
 * Trombone object that extends Brass
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
public class Trombone extends Brass {

    /**
     * Constructs Trombone object
     * @param   price           Price of the instrument
     * @param   serialNumber    Serial number in alphanumeric String of length 6
     */
    public Trombone(BigDecimal price, String serialNumber) {
        super("Trombone", "Bwaaaaaah", price, serialNumber, 3);
    }

    /**
     * Get String representation of trombone
     * @return String of trombone with member value details
     */
    @Override
    public String toString() {
        return super.toString()
            + "\n  Spit Level: " + getSpitLevel()
            + "\n  Number of Valves: " + getNumValves();
    }
}
