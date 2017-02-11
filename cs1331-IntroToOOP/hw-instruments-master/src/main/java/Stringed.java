import java.math.BigDecimal;

/**
 * Represents Stringed Instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-15
 */
public abstract class Stringed extends Instrument {

    // Member variables
    private boolean bowExists;
    private int numStrings;

    /**
     * Constructs stringed instrument
     * @param   name of the stringed instrument
     * @param   sound that the instrument makes
     * @param   price of the instrument
     * @param   serialNumber alphanumeric String of length 6
     * @param   bowExists whether the bow exists
     * @param   numStrings number of strings of instrument
     */
    public Stringed(String name, String sound, BigDecimal price,
        String serialNumber, boolean bowExists, int numStrings) {
        super(name, sound, Type.STRINGED, price, serialNumber);
        this.bowExists = bowExists;
        this.numStrings = numStrings;
    }

    /**
     * Wether stringed instrument has a bow
     * @return boolean of whether the instrument has a bow
     */
    public boolean hasBow() {
        return bowExists;
    }

    /**
     * Get number of strings of the stringed instrument
     * @return int of number of strings of instrument
     */
    public int getNumStrings() {
        return numStrings;
    }

    /**
     * Lose the bow for the stringed instrument
     */
    public void loseBow() {
        bowExists = false;
    }

    /**
     * Obtain a bow for the stringed instrument
     */
    public void obtainBow() {
        bowExists = true;
    }

    /**
     * Play the instrument and get the string of the sound
     * @return String representation of sound
     */
    @Override
    public String play() {
        if (!bowExists) {
            return "Pluck pluck pluck";
        } else {
            return super.play();
        }
    }
}
