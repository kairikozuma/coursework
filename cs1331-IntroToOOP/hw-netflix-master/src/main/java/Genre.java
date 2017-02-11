/**
 * Enumeration of Genres
 * Grouping based on their position in the enum
 *
 * @author Jayanth & Srijan
 * @version 1.0
 */
public enum Genre {

    ACTION, ADVENTURE, ANIMATION, COMEDY, DRAMA, HORROR, SCIFI,
        THRILLER, ROMANCE;

    private int value;

    Genre() {
        value = 1 + ordinal();
    }

    /**
     * Returns the value of a particular genre
     * @return Genre's value
     */
    public int getValue() {
        return value;
    }
}
