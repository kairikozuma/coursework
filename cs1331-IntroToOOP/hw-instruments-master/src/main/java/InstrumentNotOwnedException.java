/**
 * Exception: instrument not owned by the musician
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
class InstrumentNotOwnedException extends Exception {

    // Member variable
    private String message;

    /**
    * Constructs an InstrumentNotOwnedException
    */
    public InstrumentNotOwnedException() { }

    /**
    * Constructs an InstrumentNotOwnedException with information
    * @param musician attempting to buy instrument
    * @param inst attempted to be bought by musician
    */
    public InstrumentNotOwnedException(Musician musician, Instrument inst) {
        message = "\n" + musician.getName() + " does not own \n" + inst;
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
