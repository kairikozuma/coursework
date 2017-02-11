import java.util.Collection;
import java.text.DecimalFormat;

/**
* Class that represents a movie
*
* @version 1.0
* @author Kairi Kozuma
*/
public class Movie {
    // Member variables
    private String name;
    private double rating;
    private int year;
    private Collection<Genre> genres;

    /**
     * Constructs new Movie object
     * @param name the title of the movie
     * @param rating the decimal rating of the movie
     * @param year the year the movie was released
     * @param genres the list of genres of the movie
     */
    public Movie(String name, double rating, int year,
        Collection<Genre> genres) {
        this.name = name;
        this.rating = rating;
        this.year = year;
        this.genres = genres;
    }

    /**
     * Whether object equals this instance
     * @param o Object being compared to this instance
     * @return  whether object equals this instance
     */
    @Override
    public boolean equals(Object o) {
        // Check if null or not of same Type
        if (o == null || !(o instanceof Movie)) {
            return false;
        }

        // Check self identity
        if (this == o) {
            return true;
        }

        Movie myMovie = (Movie) o;
        return name.equals(myMovie.getName())
            && (Math.abs(rating - myMovie.getRating()) < 1e-12)
            && (year == myMovie.getRating())
            && genres.equals(myMovie.getGenres());
    }

    /**
     * Get the name of the movie
     * @return name of the movie
     */
    public String getName() {
        return name;
    }

    /**
     * Get the decimal rating of the movie
     * @return rating of the movie
     */
    public double getRating() {
        return rating;
    }

    /**
     * Get the year the movie was released
     * @return year of the movie release
     */
    public int getYear() {
        return year;
    }

    /**
     * Get the collection of genres of the movie
     * @return colelction of genres of the movie
     */
    public Collection<Genre> getGenres() {
        return genres;
    }

    /**
     * Get the string representation of the movie
     * @return String of the movie, with the name, year, and rating
     */
    @Override
    public String toString() {
        DecimalFormat format = new DecimalFormat("0.##");
        return String.format("%s [%d] - %s", name, year, format.format(rating));
    }

    /**
     * Calculate the hashcode for the movie
     * @return the calculated hashcode
     */
    @Override
    public int hashCode() {
        return name.hashCode()
            + (int) (rating * 17) + year * 23 + genres.hashCode();
    }
}
