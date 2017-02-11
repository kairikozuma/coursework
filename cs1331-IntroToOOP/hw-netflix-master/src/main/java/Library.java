import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.function.Predicate;
import java.util.stream.Collectors;

/**
* Class that represents a library of Movie classes
*
* @version 1.0
* @author Kairi Kozuma
*/
public class Library {

    private List<Movie> movieLibrary;

    /**
     * Create a library object
     * @param  mLibrary of movies to be added to the library
     */
    public Library(List<Movie> mLibrary) {
        movieLibrary = mLibrary;
    }

    /**
     * Return a new list of movies that is sorted alphabetically
     * @return list of movies sorted alphabetically
     */
    public List<Movie> listAlphabetically() {
        List<Movie> myMovieLibrary = new ArrayList<Movie>(movieLibrary);
        myMovieLibrary.sort(Comparator.comparing(Movie::getName));
        return myMovieLibrary;
    }

    /**
     * Return a list of movies that has at least one of the specified genres
     * @param  genreList list of genres to filter the movie list
     * @return     list of movies with specified genres
     */
    public List<Movie> moviesWithGenre(Genre ... genreList) {
        List<Movie> myMovieLibrary = new ArrayList<Movie>(movieLibrary);
        Predicate<Movie> genreTest = (Movie movie) -> {
            boolean passesTest = false;
            for (Genre g : genreList) {
                if (movie.getGenres().contains(g)) {
                    passesTest = true;
                }
            }
            return passesTest;
        };
        myMovieLibrary = myMovieLibrary.stream().filter(genreTest)
                .collect(Collectors.toList());
        return myMovieLibrary;
    }

    /**
     * Get list of movies within certain range of years
     * @param   startingYear inclusive starting year for the range
     * @param   endingYear   exclusive ending year for the range
     * @return  list of movies within a certain range of years
     */
    public List<Movie> listByYearRange(int startingYear, int endingYear) {
        List<Movie> myMovieLibrary = new ArrayList<Movie>(movieLibrary);
        myMovieLibrary = myMovieLibrary.stream().filter((Movie movie)
                -> movie.getYear() >= startingYear
                && movie.getYear() < endingYear)
                .collect(Collectors.toList());
        return myMovieLibrary;
    }

    /**
     * Get list of movies ordered by lowest rating
     * @return list of movies ordered by lowest rating
     */
    public List<Movie> listByLowestRating() {
        List<Movie> myMovieLibrary = new ArrayList<Movie>(movieLibrary);
        myMovieLibrary.sort(Comparator.comparing(Movie::getRating));
        return myMovieLibrary;
    }

    /**
     * Get list of movies ordered by highest rating
     * @return list of movies ordered by highest rating
     */
    public List<Movie> listByHighestRating() {
        List<Movie> myMovieLibrary = new ArrayList<Movie>(movieLibrary);
        myMovieLibrary.sort(Comparator.comparing(Movie::getRating).reversed());
        return myMovieLibrary;
    }
}
