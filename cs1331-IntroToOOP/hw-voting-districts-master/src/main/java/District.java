/**
 * Class that represents a district
 *
 * @author kkozuma3
 * @version 1.0
 */
public abstract class District {
    private String name;

    /**
     * Construct a District object
     * @param  name the name of the district
     */
    public District(String name) {
        this.name = name;
    }

    /**
     * Get the name of the district
     * @return name of the district
     */
    public String getName() {
        return name;
    }

    /**
     * Get the size of the district
     * @return size of the district
     */
    public abstract int getSize();

    /**
     * Get the Candidate winner of the district
     * @return Candidate winner of the district
     */
    public abstract Candidate winner();
}
