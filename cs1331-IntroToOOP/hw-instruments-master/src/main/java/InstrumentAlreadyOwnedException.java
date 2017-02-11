/**
 * Exception: instrument already owned by musician
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
class InstrumentAlreadyOwnedException extends Exception {

    // Member variable
    private String message;

    /**
    * Constructs an InstrumentAlreadyOwnedException
    */
    public InstrumentAlreadyOwnedException() { }

    /**
    * Constructs an InstrumentAlreadyOwnedException with information
    * @param musician attempting to buy instrument
    * @param inst attempted to be bought by musician
    */
    public InstrumentAlreadyOwnedException(Musician musician, Instrument inst) {
        message = "\n" + musician.getName() + " already owns \n" + inst;
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
