import java.math.BigDecimal;

/**
 * Represents Percussion Instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-15
 */
public abstract class Percussion extends Instrument {

    // Member variables
    private boolean sticksExist;
    private int numPieces;

    /**
     * Constructs brass instrument
     * @param   name of the stringed instrument
     * @param   sound that the instrument makes
     * @param   price of the instrument
     * @param   serialNumber alphanumeric String of length 6
     * @param   sticksExist whether sticks exist initially
     * @param   numPieces of the instrument
     */
    public Percussion(String name, String sound, BigDecimal price,
        String serialNumber, boolean sticksExist, int numPieces) {
        super(name, sound, Type.PERCUSSION, price, serialNumber);
        this.sticksExist = sticksExist;
        this.numPieces = numPieces;
    }

    /**
     * Whether percussion instrument has sticks
     * @return boolean of whether percussion has sticks
     */
    public boolean hasSticks() {
        return sticksExist;
    }

    /**
     * Gets number of pieces of the percussion instrument
     * @return int numPieces of that instrument
     */
    public int getNumPieces() {
        return numPieces;
    }

    /**
     * Lose the sticks for percussion instrument
     */
    public void loseSticks() {
        sticksExist = false;
    }

    /**
     * Obtain new sticks for the percussion instrument
     */
    public void obtainSticks() {
        sticksExist = true;
    }

    /**
     * Play the instrument and get the string of the sound
     * @return String representation of sound
     */
    @Override
    public String play() {
        if (!sticksExist) {
            return "Tap tap tap";
        } else {
            return super.play();
        }
    }
}
