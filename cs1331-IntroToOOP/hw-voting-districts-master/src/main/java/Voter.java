import java.util.List;

/**
 * Class that represents a voter
 *
 * @author kkozuma3
 * @version 1.0
 */
public class Voter {
    private String name;
    private List<Candidate> voteList;

    /**
     * Construct a Voter
     * @param  name the name of the voter
     */
    public Voter(String name) {
        this.name = name;
    }

    /**
     * Construct a Voter with a list of candidates to vote for
     * @param   name the name of the voter
     * @param   voteList list of Candidates the voter voted for
     */
    public Voter(String name, List<Candidate> voteList) {
        if (voteList.size() != 3) {
            System.out.println("Vote size for candidate must be three");
        }
        this.name = name;
        this.voteList = voteList;
    }

    /**
     * Get the list of candidates voted for
     * @return list of candidates voted for
     */
    public List<Candidate> getVote() {
        return voteList;
    }

    /**
     * Get the name of the voter
     * @return name of the voter
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
        if (!(o instanceof Voter)) {
            return false;
        }

        Voter other = (Voter) o;
        return this.getName().equals(other.getName());
    }

    @Override
    public int hashCode() {
        return name.hashCode() + voteList.hashCode();
    }

    @Override
    public String toString() {
        return name + " " + voteList;
    }
}
