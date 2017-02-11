/**
 * Represents a candidate
 *
 * @author kkozuma3
 * @version 1.0
 */
public class Candidate {
    private String name;

    /**
     * Construct a candidate object
     * @param  name candidate name
     */
    public Candidate(String name) {
        this.name = name;
    }

    /**
     * Get the name for the candidate
     * @return name of candidate
     */
    public String getName() {
        return name;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null) {
            return false;
        }
        if (!(o instanceof Candidate)) {
            return false;
        }
        Candidate other = (Candidate) o;
        return this.getName().equals(other.getName());
    }

    @Override
    public int hashCode() {
        return getName().hashCode();
    }

    @Override
    public String toString() {
        return name;
    }
}
