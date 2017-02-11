import java.math.BigDecimal;

/**
 * Represents Brass Instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-15
 */
public abstract class Brass extends Instrument {

    // Member variables
    private int spitLevel;
    private int numValves;

    // Values used to determine amount of spit
    private static final int MAX_SPIT_LEVEL = 10;
    private static final int MED_SPIT_LEVEL = 6;
    private static final int MIN_SPIT_LEVEL = 0;

    /**
     * Constructs brass instrument
     * @param   name of the stringed instrument
     * @param   sound that the instrument makes
     * @param   price of the instrument
     * @param   serialNumber String of 6 alphanumeric characters
     * @param   numValves number of valves
     */
    public Brass(String name, String sound,
        BigDecimal price, String serialNumber, int numValves) {
        super(name, sound, Type.BRASS, price, serialNumber);
        this.spitLevel = MIN_SPIT_LEVEL;
        this.numValves = numValves;
    }

    /**
     * Gets the spit level of the brass instrument
     * @return int spit level of the instrument
     */
    public int getSpitLevel() {
        return spitLevel;
    }

    /**
     * Gets the number of valves on the brass instrument
     * @return int number of valves of instrument
     */
    public int getNumValves() {
        return numValves;
    }

    /**
     * Empties spit valve of the brass instrument, resets spitLevel to 0
     */
    public void emptySpitValve() {
        spitLevel = MIN_SPIT_LEVEL;
    }

    /**
     * Play the instrument and get the string of the sound, depending on
     * spit level
     * @return String representation of sound
     */
    @Override
    public String play() {
        if (spitLevel < MAX_SPIT_LEVEL) {
            spitLevel++;
        }
        if (spitLevel > MED_SPIT_LEVEL) {
            return "Brurururururu";
        } else {
            return super.play();
        }
    }
}
