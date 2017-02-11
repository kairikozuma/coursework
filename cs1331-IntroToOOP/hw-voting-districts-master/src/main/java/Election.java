/**
 * Class that represents an election taking place
 *
 * @author kkozuma3
 * @version 1.0
 */
public class Election {

    /**
     * Class that executes an election
     * @param args command line arguments for election
     *             districts.txt
     *             local_districts.txt
     *             voters.txt
     */
    public static void main(String[] args) {
        ElectionHolder mElectionHolder = null;
        if (args.length == 0) {
            mElectionHolder = new ElectionHolder(
                "../resources/districts.txt",
                "../resources/local_districts.txt",
                "../resources/voters.txt"
                );
        } else if (args.length == 3) {
            mElectionHolder = new ElectionHolder(
                args[0],
                args[1],
                args[2]
            );
        }
        if (mElectionHolder != null) {
            System.out.println(mElectionHolder.winner());
        }
    }
}
