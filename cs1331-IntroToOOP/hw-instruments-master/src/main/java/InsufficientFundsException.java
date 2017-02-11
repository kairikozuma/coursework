
import java.text.NumberFormat;
/**
 * Exception: musician does not have enough funds to purchase instrument
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
class InsufficientFundsException extends Exception {

    // Member variable
    private String message;

    /**
    * Constructs an InsufficientFundsException
    */
    public InsufficientFundsException() { }

    /**
    * Constructs an InsufficientFundsException with information
    * @param musician attempting to purchase instrument
    * @param instrument attempted to be bought by musician
    */
    public InsufficientFundsException(Musician musician, Instrument inst) {
        message = "\n" + musician.getName() + " only has "
            + NumberFormat.getCurrencyInstance().format(musician.getFunds())
            + ". Not enough to buy \n" + inst;
    }

    /**
     * Return message of exception
     * @return String message of the exception
     */
    @Override
    public String getMessage() {
        return message;
    }
}
