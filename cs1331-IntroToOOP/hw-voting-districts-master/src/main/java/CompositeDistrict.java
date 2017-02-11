import java.util.Set;
import java.util.HashSet;
import java.util.Map;
import java.util.HashMap;

/**
 * Class that represents a district containing other districts
 *
 * @author kkozuma3
 * @version 1.0
 */
public class CompositeDistrict extends District {
    private int size;
    private Candidate winner;
    private Set<District> districtSet;
    private Map<Candidate, Integer> candidates;

    /**
     * Construct a CompositeDistrict object
     * @param  name the name of district
     */
    public CompositeDistrict(String name) {
        super(name);
        districtSet = new HashSet<District>();
        candidates = new HashMap<Candidate, Integer>();
    }

    /**
     * Add a district to the CompositeDistrict
     * @param d district to be added
     */
    public void addDistrict(District d) {
        districtSet.add(d);
    }

    /**
     * Get the set of districts contained by this object
     * @return Set of all districts within this object
     */
    public Set<District> getDistricts() {
        return districtSet;
    }

    @Override
    public int getSize() {
        size = 0;
        for (District d : districtSet) {
            size += d.getSize();
        }
        return size;
    }

    @Override
    public Candidate winner() {
        for (District d : districtSet) {
            Candidate c = d.winner();

            // If not in list, add to list
            if (!candidates.containsKey(c)) {
                candidates.put(c, 0);
            }

            // Increment based on choice
            candidates.put(c, candidates.get(c) + d.getSize());
        }

        int max = 0;
        for (Map.Entry<Candidate, Integer> m : candidates.entrySet()) {
            if (m.getValue() > max) {
                winner = m.getKey();
                max = m.getValue();
            }
        }

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
        if (!(o instanceof CompositeDistrict)) {
            return false;
        }
        CompositeDistrict other = (CompositeDistrict) o;
        return this.getName().equals(other.getName());
    }

    @Override
    public int hashCode() {
        return getName().hashCode();
    }

    @Override
    public String toString() {
        return getName() + ": " + winner()
            + " \n" + getSize() + " " + districtSet;
    }
}
