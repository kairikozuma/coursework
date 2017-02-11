import java.math.BigDecimal;
import java.text.NumberFormat;
import java.util.List;

/**
 * Musician object that has arraylist of instruments
 * @author Kairi Kozuma
 * @version 1.0 10-03-2015
 */
public class Musician {

    // Member variables
    private String name;
    private BigDecimal funds;
    private List<Instrument> instrumentList;

    /**
     * Constructs musician object
     * @param name             Name of musician
     * @param funds            Initial funds of musician
     * @param instrumentList   Initial instrument owned by musician
     */
    public Musician(String name, BigDecimal funds,
        List<Instrument> instrumentList) {
        this.name = name;
        this.funds = funds;
        this.instrumentList = instrumentList;
    }

    /**
     * Gets the name of the musician
     * @return String name of the musician
     */
    public String getName() {
        return name;
    }

    /**
     * Gets the funds available to the musician
     * @return BigDecimal funds of the musician
     */
    public BigDecimal getFunds() {
        return funds;
    }

    /**
     * Gets the instruments owned by musician
     * @return List of instruments
     */
    public List<Instrument> getInstrumentList() {
        return instrumentList;
    }

    /**
     * Purchase instrument for musician and add to list
     * @param inst Instrument the musician is attempting to purchase
     * @throws InsufficientFundsException if not enough funds to purchase
     * @throws InstrumentAlreadyOwnedException if buying instrument owned
     */
    public void purchase(Instrument inst)
        throws InsufficientFundsException, InstrumentAlreadyOwnedException {
        if (instrumentList.contains(inst)) {
            throw new InstrumentAlreadyOwnedException(this, inst);
        } else if (funds.compareTo(inst.getPrice()) >= 0) {
            funds = funds.subtract(inst.getPrice());
            instrumentList.add(inst);
        } else {
            throw new InsufficientFundsException(this, inst);
        }
    }

    /**
     * Sell an instrument from the musician and remove it from list
     * @param inst Instrument the musician is attempting to sell
     * @throws CannotSellLastInstrumentException if musician only has one
     *         instrument left
     * @throws InstrumentNotOwnedException if musician does not own instrument
     * @return Instrument sold from musician
     */
    public Instrument sell(Instrument inst)
        throws InstrumentNotOwnedException, CannotSellLastInstrumentException {
        if (instrumentList.size() == 1) {
            throw new CannotSellLastInstrumentException(this);
        } else if (instrumentList.contains(inst)) {
            instrumentList.remove(inst);
            funds = funds.add(inst.getPrice());
            return inst;
        } else {
            throw new InstrumentNotOwnedException(this, inst);
        }
    }

    /**
     * Get string representation of the musician
     * @return String with musician's name and list of instruments owned
     */
    @Override
    public String toString() {
        String myStr = "Name: " + name + "\n  Funds: "
            + NumberFormat.getCurrencyInstance().format(funds)
            + "\n  Instruments: \n";
        for (Instrument inst : instrumentList) {
            myStr += inst;
        }
        return myStr;
    }
}
