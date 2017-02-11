/**
 * Exception: musician only has one instrument left to sell
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
class CannotSellLastInstrumentException extends Exception {

    // Member variable
    private String message;

    /**
    * Constructs an CannotSellLastInstrumentException
    */
    public CannotSellLastInstrumentException()  { }

    /**
    * Constructs an CannotSellLastInstrumentException with information
    * @param musician trying to sell last instrument
    */
    public CannotSellLastInstrumentException(Musician musician) {
        message = "\n" + musician.getName() + " only has one instrument left!";
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
