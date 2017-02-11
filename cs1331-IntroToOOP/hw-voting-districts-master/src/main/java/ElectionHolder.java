import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Collection;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * Class that pareses three text files to conduct an election
 *
 * @author kkozuma3
 * @version 1.0
 */
public class ElectionHolder {

    private Map<String, Voter> voters;
    private Map<String, District> allDistricts;
    private Map<String, CompositeDistrict> topLevelDistricts;
    private CompositeDistrict topDistrict;

    private String cmpDistFile;
    private String locDistFile;
    private String voterFile;

    private Candidate winner;

    /**
     * Construct a new ElectionHolder object
     * @param   cmpDistFile file name containing the composite districts
     * @param   locDistFile file name continaing the local districts
     * @param   voterFile file name containing the voters
     */
    public ElectionHolder(
        String cmpDistFile, String locDistFile, String voterFile) {
        this.cmpDistFile = cmpDistFile;
        this.locDistFile = locDistFile;
        this.voterFile = voterFile;

        topDistrict = new CompositeDistrict("Top Level District");

        initialize();
    }

    /**
     * Get all districts in the election
     * @return Collection of districts in election
     */
    public Collection<District> getAllDistricts() {
        return allDistricts.values();
    }

    /**
     * Get all local districts in the election
     * @return List of local districts
     */
    public List<LocalDistrict> getLocalDistricts() {
        Collection<District> allDistricts = getAllDistricts();
        List<LocalDistrict> allLocalDist = new ArrayList<LocalDistrict>();
        for (District d : allDistricts) {
            if (d instanceof LocalDistrict) {
                LocalDistrict ld = (LocalDistrict) d;
                allLocalDist.add(ld);
            }
        }
        return allLocalDist;
    }

    /**
     * Initialize the ElectionHolder by parsing from the file
     */
    private void initialize() {
        parseVoterFile();
        parseLocalDistrictFile();
        parseCompositeDistrictFile();
        for (District d : topLevelDistricts.values()) {
            topDistrict.addDistrict(d);
        }
    }

    /**
     * Returns the winner of the election
     * @return Candidate winner of the election
     */
    public Candidate winner() {
        return topDistrict.winner();
    }

    /**
     * Parse voters from the voter file
     */
    private void parseVoterFile() {
        voters = new HashMap<String, Voter>();
        try {
            Scanner fileScanner = new Scanner(new File(voterFile));
            while (fileScanner.hasNextLine()) {
                Voter v = parseVoter(fileScanner.nextLine());
                voters.put(v.getName(), v);
            }
        } catch (FileNotFoundException e) {
            System.out.println(e);
        }
    }

    /**
     * Parse a single voter
     * @param  voterString String from which to extract a voter
     * @return             Voter parsed from String
     */
    private Voter parseVoter(String voterString) {
        String[] tokens = voterString.split("[:,]");
        if (tokens.length != 4) {
            System.out.println("Invalid voter format");
            return null;
        }
        List<Candidate> voteList = new ArrayList<Candidate>(3);
        voteList.add(new Candidate(tokens[1].trim()));
        voteList.add(new Candidate(tokens[2].trim()));
        voteList.add(new Candidate(tokens[3].trim()));

        return new Voter(tokens[0], voteList);
    }

    /**
     * Parse local districts from a file
     */
    private void parseLocalDistrictFile() {
        allDistricts = new HashMap<String, District>();

        try {
            Scanner fileScanner = new Scanner(new File(locDistFile));
            while (fileScanner.hasNextLine()) {
                District localD = parseLocalDistrict(fileScanner.nextLine());
                allDistricts.put(localD.getName(), localD);
            }
        } catch (FileNotFoundException e) {
            System.out.println(e);
        }
    }

    /**
     * Parse a local district from a line of string
     * @param  localDistrictString String to parse
     * @return                     LocalDistrict parsed from String
     */
    private LocalDistrict parseLocalDistrict(String localDistrictString) {

        String[] tokens = localDistrictString.split("[:,]");

        List<Voter> voterList = new ArrayList<Voter>(tokens.length - 1);
        for (int i = 1; i < tokens.length; i++) {
            voterList.add(voters.get(tokens[i].trim()));
        }

        return new LocalDistrict(tokens[0], voterList);
    }

    /**
     * Parse composite districts from a file
     */
    private void parseCompositeDistrictFile() {

        topLevelDistricts = new HashMap<String, CompositeDistrict>();

        try {
            Scanner fileScanner = new Scanner(new File(cmpDistFile));

            // Parse lines
            while (fileScanner.hasNextLine()) {
                parseCompositeDistrict(fileScanner.nextLine());
            }

        } catch (FileNotFoundException e) {
            System.out.println(e);
        }
    }

    /**
     * Parse a single composite district from a line of String
     * @param compositeDistrictString String from which to extract a composite
     *                                district
     */
    private void parseCompositeDistrict(String compositeDistrictString) {

        if (compositeDistrictString.contains(":")) {
            String[] tokens = compositeDistrictString.split("[:,]");
            String name = tokens[0].trim();
            // If not a top level district, create district with subdistricts
            CompositeDistrict topLevelD = topLevelDistricts.get(name);
            District district = allDistricts.get(name);

            // Not a top level district
            if (topLevelD == null) {
                if (district == null) {
                    district = new CompositeDistrict(name);
                    allDistricts.put(name, district);
                }
                for (int i = 1; i < tokens.length; i++) {
                    ((CompositeDistrict) district)
                        .addDistrict(allDistricts.get(tokens[i].trim()));
                }
            } else {
                // Add all districts
                for (int i = 1; i < tokens.length; i++) {
                    topLevelD.addDistrict(allDistricts.get(tokens[i].trim()));
                }
            }
        } else {
            String[] tokens = compositeDistrictString.split("[,]");
            for (String t : tokens) {
                String name = t.trim();
                District topLevelD = new CompositeDistrict(name);
                allDistricts.put(name, topLevelD);
                topLevelDistricts.put(name, (CompositeDistrict) topLevelD);
            }
        }
    }
}
