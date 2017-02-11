import java.math.BigDecimal;
import java.text.NumberFormat;

/**
 * Musical instrument abstract object
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
public abstract class Instrument {

    // Member variables
    private String name;
    private String sound;
    private Type type;
    private BigDecimal price;
    private String serialNumber;

    /**
     * Type of instrument enumerated type
     * @author Kairi Kozuma
     * @version 1.0 10-03-2015
     */
    public enum Type {
        STRINGED("Stringed"),
        BRASS("Brass"),
        WOODWIND("Woodwind"),
        PERCUSSION("Percussion");

        private String type;

        Type(String type) {
            this.type = type;
        }

        @Override
        public String toString() {
            return type;
        }
    }

    /**
     * Constructs an Instrument object
     * @param   name            Name of the instrument
     * @param   sound           Sound the instrument makes
     * @param   type            Type of instrument
     * @param   price           Price of the instrument
     * @param   serialNumber    Serial number in alphanumeric String of length 6
     */
    public Instrument(String name, String sound, Type type,
        BigDecimal price, String serialNumber) {
        this.name = name;
        this.sound = sound;
        this.type = type;
        this.price = price;
        this.serialNumber = serialNumber;
    }

    /**
     * Gets the name of the instrument
     * @return String name of instrument
     */
    public String getName() {
        return name;
    }

    /**
     * Gets the price of the instrument
     * @return BigDecimal price of instrument
     */
    public BigDecimal getPrice() {
        return price;
    }

    /**
     * Gets the serial number of the instrument
     * @return String serial number of instrument
     */
    public String getSerialNumber() {
        return serialNumber;
    }

    /**
     * Gets the sound the instrument makes
     * @return String sound of instrument
     */
    public String play() {
        return sound;
    }

    /**
     * Gets enum type of instrument
     * @return Type of instrument
     */
    public Type getType() {
        return type;
    }

    /**
     * Checks if serial numbers of two instruments are equal
     * @return boolean of whether serial numbers were equal
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Instrument) {
            return serialNumber.equals(((Instrument) obj).getSerialNumber());
        } else {
            return false;
        }
    }

    /**
     * Get int hashcode evaluated using the serialNumber
     * @return int of hashcode
     */
    @Override
    public int hashCode() {
        return serialNumber.hashCode();
    }

    /**
     * Get String representation of the instrument
     * @return String of instrument with member value details
     */
    @Override
    public String toString() {
        return "\n[" + name + "]"
            + "\n  Type: " + type
            + "\n  Sound: " + play()
            + "\n  Price: "
                + NumberFormat.getCurrencyInstance().format(getPrice())
            + "\n  Serial Number: " + getSerialNumber();
    }
}
