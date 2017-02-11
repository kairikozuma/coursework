import java.math.BigDecimal;
import java.util.List;
import java.util.ArrayList;

/**
 * Class that tests musician and instrument classes
 * @author Kairi Kozuma
 * @version 1.0 10-03-15
 */
public class MusicStore {

    /**
     * Tests the implementations of musician and instrument objects
     * @param args arguments from command line
     */
    public static void main(String[] args) {
        // Set up instruments in store
        List<Instrument> storeList = getStoreInstruments();

        // Instantiate a musician
        Musician mario = new Musician("Mario", new BigDecimal(1000),
            getMusicianInstruments());
        System.out.println("This is " + mario.getName() + ", a musician: \n");
        System.out.println(mario);

        // Fix problems with each instrument
        System.out.println("\n" + mario.getName() + " fixes his instruments.");
        System.out.println();
        fixInstruments(mario.getInstrumentList());
        System.out.println(mario);

        // Play each instrument in the store
        System.out.println("\nMario plays and checks each instrument"
            + " in the music store.");
        playStoreInstruments(storeList);

        // Attempt to purchase each instrument
        purchaseStoreInstruments(mario, storeList);

        // Create random instrument
        Instrument tromboneB = new Trombone(new BigDecimal(4000), "KLKL90");

        // Try to sell random instrument
        try {
            System.out.println("\n" + mario.getName()
                + " tries to sell: \n" + tromboneB);
            mario.sell(tromboneB);
        } catch (InstrumentNotOwnedException e) {
            System.out.println(e.getMessage());
        } catch (CannotSellLastInstrumentException e) {
            System.out.println(e.getMessage());
        }

        // Attempt to sell every instrument
        sellAllInstruments(mario);
    }

    /**
     * Generate a list of instruments for the musician
     * @return List<Instrument> for the musician
     */
    private static List<Instrument> getMusicianInstruments() {
        // Instantiate 2 instruments that the musician owns
        Clarinet clarinet = new Clarinet(new BigDecimal(120), "0AJF18");
        Trombone trombone = new Trombone(new BigDecimal(250), "DF893D");
        Viola viola = new Viola(new BigDecimal(250), "VX3350");
        viola.loseBow();
        Timpani timpani = new Timpani(new BigDecimal(250), "KLJ0022");
        timpani.loseSticks();

        // Add instruments to list
        List<Instrument> mList = new ArrayList<Instrument>();
        mList.add(clarinet);
        mList.add(trombone);
        mList.add(viola);
        mList.add(timpani);

        // Play each instrument 8 times
        for (Instrument inst : mList) {
            for (int i = 0; i < 8; i++) {
                inst.play();
            }
        }

        // Return list
        return mList;
    }

    /**
     * Generate a list of instruments for the music store
     * @return List<Instrument> for the music store
     */
    private static List<Instrument> getStoreInstruments() {
        // Instantiate 8 instruments, 2 of each type
        Clarinet clarinetA = new Clarinet(new BigDecimal(120), "0AJF18");
        Trombone tromboneA = new Trombone(new BigDecimal(250), "DF993D");
        Viola violaA = new Viola(new BigDecimal(400), "VX9931");
        Timpani timpaniA = new Timpani(new BigDecimal(600), "KLI992");

        // Add instruments to list
        List<Instrument> mList = new ArrayList<Instrument>();
        mList.add(clarinetA);
        mList.add(tromboneA);
        mList.add(violaA);
        mList.add(timpaniA);

        // Return list
        return mList;
    }

    /**
     * Tests each instrument
     * @param instList list containing instruments to be tested
     */
    private static void playStoreInstruments(List<Instrument> instList) {
        for (Instrument inst : instList) {
            System.out.println(inst);
        }
    }

    /**
     * Fixes each instrument in the store
     * @param instList list containing instruments to be repaired
     */
    private static void fixInstruments(List<Instrument> instList) {
        for (Instrument inst : instList) {
            if (inst instanceof Woodwind) {
                ((Woodwind) inst).changeReed();
            } else if (inst instanceof Brass) {
                ((Brass) inst).emptySpitValve();
            } else if (inst instanceof Stringed) {
                ((Stringed) inst).obtainBow();
            } else if (inst instanceof Percussion) {
                ((Percussion) inst).obtainSticks();
            }
        }
    }

    /**
     * Have the musician try to purchase all instruments in instList
     * @param  musician that tries to purchase all instruments
     * @param  instList list of instrumnets possible to buy
     */
    private static void purchaseStoreInstruments(Musician musician,
        List<Instrument> instList) {
        int i = 0;
        while (i < instList.size()) {
            try {
                Instrument inst = instList.get(i);
                System.out.println("\n" + musician.getName()
                    + " tries to buy: \n" + inst);
                musician.purchase(inst);
                instList.remove(i);
                System.out.println("\n\n" + musician);
            } catch (InstrumentAlreadyOwnedException e) {
                System.out.println(e.getMessage());
                i++;
            } catch (InsufficientFundsException e) {
                System.out.println(e.getMessage());
                i++;
            }
        }
    }

    /**
     * Have the musician try to sell all instruments
     * @param  musician that tries to sell all instruments
     */
    private static void sellAllInstruments(Musician musician) {
        int i = 0;
        while (i < musician.getInstrumentList().size()) {
            try {
                Instrument inst = musician.getInstrumentList().get(i);
                System.out.println("\n" + musician.getName()
                    + " tries to sell: \n" + inst);
                musician.sell(inst);
                System.out.println("\n\n" + musician);
            } catch (InstrumentNotOwnedException e) {
                System.out.println(e.getMessage());
                i++;
            } catch (CannotSellLastInstrumentException e) {
                System.out.println(e.getMessage());
                i++;
            }
        }
    }
}
