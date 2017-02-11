import java.util.Map;
import java.util.HashMap;
import java.util.List;

/**
 * Class that represents a local district
 *
 * @author kkozuma3
 * @version 1.0
 */
public class LocalDistrict extends District
    implements Comparable<LocalDistrict> {
    private int size;
    private List<Voter> voters;
    private Map<Candidate, Integer> candidates;

    private Candidate winner;

    /**
     * Constuct a local district
     * @param   name the name of the local district
     * @param   voters list of voters in the local district
     */
    public LocalDistrict(String name, List<Voter> voters) {
        super(name);
        this.voters = voters;
        this.size = voters.size();

        candidates = new HashMap<Candidate, Integer>();

        for (Voter v : voters) {
            // Choice multiplier
            int i = 3;

            for (Candidate c : v.getVote()) {
                // If not in list, add to list
                if (!candidates.containsKey(c)) {
                    candidates.put(c, 0);
                }

                // Increment based on choice
                candidates.put(c, candidates.get(c) + i);

                // Decrement multiplier
                i--;
            }
        }

        int max = 0;
        for (Map.Entry<Candidate, Integer> m : candidates.entrySet()) {
            if (m.getValue() > max) {
                winner = m.getKey();
                max = m.getValue();
            }
        }
    }

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public Candidate winner() {
        return winner;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null) {
            return false;
        }
        if (!(o instanceof LocalDistrict)) {
            return false;
        }
        LocalDistrict other = (LocalDistrict) o;
        return (this.getName().equals(other.getName())
            && size == other.getSize());
    }

    @Override
    public int hashCode() {
        return getName().hashCode() + size * 31;
    }

    @Override
    public String toString() {
        return getName() + ": " + winner + " " + getSize() + " \n";
    }

    @Override
    public int compareTo(LocalDistrict ld) {
        return this.winner().getName().compareTo(ld.winner().getName());
    }
}
