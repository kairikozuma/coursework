import java.math.BigDecimal;

/**
 * Represents Woodwind Instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-15
 */
public abstract class Woodwind extends Instrument {

    // Member variables
    private boolean reedExists;
    private int reedQuality;

    // Values used to determine quality of reed
    private static final int SUPERB_QUALITY = 10;
    private static final int TERRIBLE_QUALITY = 4;
    private static final int ABYSMAL_QUALITY = 0;

    /**
     * Constructs woodwind instrument
     * @param   name of the woodwind instrument
     * @param   sound that the instrument makes
     * @param   price of the instrument
     * @param   serialNumber alphanumeric String of length 6
     * @param   reedExists whether the instrument has a reed
     */
    public Woodwind(String name, String sound,
        BigDecimal price, String serialNumber, boolean reedExists) {
        super(name, sound, Type.WOODWIND, price, serialNumber);
        this.reedExists = reedExists;
        this.reedQuality = SUPERB_QUALITY;
    }

    /**
     * Get whether the instrument has a reed
     * @return boolean of whether the instrument has a reed
     */
    public boolean hasReed() {
        return reedExists;
    }

    /**
     * Get the quality of the reed
     * @return integer quality of reed, -1 if instrument does not have reed
     */
    public int getReedQuality() {
        return reedExists ? reedQuality : -1;
    }

    /**
     * Reset the reed quality to maximum of 10
     * @return boolean of wether instrument has a reed
     */
    public boolean changeReed() {
        if (reedExists) {
            reedQuality = SUPERB_QUALITY;
        }
        return reedExists;
    }

    /**
     * Play the instrument and get the string of the sound and adjust reed
     * @return String representation of sound
     */
    @Override
    public String play() {
        if (reedExists && reedQuality > ABYSMAL_QUALITY) {
            reedQuality--;
        }
        if (reedQuality <= TERRIBLE_QUALITY) {
            return "Squeeeeeeeeak";
        } else {
            return super.play();
        }
    }
}
