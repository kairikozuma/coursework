import java.util.ArrayList;
import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.io.File;

/**
* Represents the entire selection of Netflix movies.
*
* @author  Jayanth & Srijan
* @version 1.0
*/
public class PopulateNetflix {
    private ArrayList<Movie> population;
    private Random rand;

    /**
    * Creates a Netflix population
    * @throws Exception propagated to caller
    */
    public PopulateNetflix() throws Exception {
        rand = new Random();
        int val = rand.nextInt(100);
        population = new ArrayList<Movie>(30);
        if (val < 50) {
            populate1();
        } else if (val >= 50) {
            populate2();
        }
    }

    /**
    * Returns a population of all the netflix movies.
    * @return population of netflix movies
    */
    public ArrayList<Movie> getNetflixPopulation() {
        return population;
    }

    /**
    * MovieLibrary v1
    */
    private void populate1() throws Exception {
        Scanner s = new Scanner(new File("netflix.txt"));
        while (s.hasNext()) {
            String line = s.nextLine();
            String[] args = line.split(",");
            int year = Integer.parseInt(args[0].trim());
            String name = args[1].trim();
            String[] genres = args[2].split("/");
            double rating = Double.parseDouble(args[3]);
            HashSet<Genre> m = new HashSet<Genre>();
            for (int x = 0; x < genres.length; x++) {
                if (genres[x].equals("Action")) {
                    m.add(Genre.ACTION);
                } else if (genres[x].equals("Drama")) {
                    m.add(Genre.DRAMA);
                } else if (genres[x].equals("Adventure")) {
                    m.add(Genre.ADVENTURE);
                } else if (genres[x].equals("Comedy")) {
                    m.add(Genre.COMEDY);
                } else if (genres[x].equals("Horror")) {
                    m.add(Genre.HORROR);
                } else if (genres[x].equals("Sci-fi")) {
                    m.add(Genre.SCIFI);
                } else if (genres[x].equals("Thriller")) {
                    m.add(Genre.THRILLER);
                } else if (genres[x].equals("Romance")) {
                    m.add(Genre.ROMANCE);
                }
            }
            population.add(new Movie(name, rating, year, m));
        }
    }

    /**
    * MovieLibrary v2
    */
    private void populate2() {
        population.add(new Movie("Aladdin", 8.0, 1992,
            genreAdder(Genre.ADVENTURE, Genre.ANIMATION,
                Genre.COMEDY, Genre.ROMANCE)));
        population.add(new Movie("Toy Story", 8.3, 1995,
            genreAdder(Genre.ADVENTURE, Genre.ANIMATION, Genre.COMEDY)));
        population.add(new Movie("The Lion King", 8.5, 1994,
            genreAdder(Genre.ADVENTURE, Genre.ANIMATION, Genre.DRAMA)));
        population.add(new Movie("Beauty and the Beast", 8.0, 1991,
            genreAdder(Genre.ANIMATION, Genre.ROMANCE)));
        population.add(new Movie("The Imitation Game", 8.0, 2014,
            genreAdder(Genre.DRAMA, Genre.THRILLER)));
        population.add(new Movie("The Shawshank Redemption", 9.3, 1994,
            genreAdder(Genre.DRAMA)));
        population.add(new Movie("Insidious Chapter 3", 6.2, 2015,
            genreAdder(Genre.HORROR, Genre.DRAMA, Genre.THRILLER)));
        population.add(new Movie("Back to the Future", 8.5, 1985,
            genreAdder(Genre.ADVENTURE, Genre.COMEDY, Genre.SCIFI)));
        population.add(new Movie("Schindler's List", 8.9, 1993,
            genreAdder(Genre.DRAMA)));
        population.add(new Movie("The Lord of the Rings: "
            + "The Return of the King", 8.9, 2003,
            genreAdder(Genre.ADVENTURE, Genre.ACTION)));
        population.add(new Movie("Inception", 8.8, 2010,
            genreAdder(Genre.ACTION, Genre.SCIFI)));
        population.add(new Movie("Finding Nemo", 8.2, 2003,
            genreAdder(Genre.ANIMATION, Genre.ADVENTURE, Genre.COMEDY)));
        population.add(new Movie("Savior", 7.4, 1998,
            genreAdder(Genre.DRAMA, Genre.THRILLER)));
        population.add(new Movie("The Devil's Own", 6.1, 1997,
            genreAdder(Genre.ACTION, Genre.DRAMA, Genre.THRILLER)));
        population.add(new Movie("Sweet November", 6.7, 2001,
            genreAdder(Genre.DRAMA, Genre.ROMANCE)));
        population.add(new Movie("Sleepless in Seattle", 6.8, 1993,
            genreAdder(Genre.COMEDY, Genre.DRAMA, Genre.ROMANCE)));
        population.add(new Movie("A Beautiful Mind", 8.2, 2001,
            genreAdder(Genre.DRAMA)));
        population.add(new Movie("The Social Network", 7.8, 2010,
            genreAdder(Genre.DRAMA)));
        population.add(new Movie("Windtalkers", 6.0, 2002,
            genreAdder(Genre.ACTION, Genre.DRAMA)));
        population.add(new Movie("Mr. and Mrs. Smith", 6.5, 2005,
            genreAdder(Genre.ACTION, Genre.COMEDY, Genre.DRAMA)));
        population.add(new Movie("The Family Man", 6.7, 2000,
            genreAdder(Genre.COMEDY, Genre.DRAMA)));
        population.add(new Movie("Meet the Parents", 7.0, 2000,
            genreAdder(Genre.COMEDY)));
        population.add(new Movie("When Harry Met Sally", 7.6, 1989,
            genreAdder(Genre.COMEDY, Genre.DRAMA, Genre.ROMANCE)));
        population.add(new Movie("Salt", 6.4, 2010,
            genreAdder(Genre.ACTION, Genre.ADVENTURE, Genre.THRILLER)));
        population.add(new Movie("Top Gun", 6.9, 1986,
            genreAdder(Genre.ACTION, Genre.DRAMA, Genre.ROMANCE)));
        population.add(new Movie("Iron Man", 7.9, 2008,
            genreAdder(Genre.ACTION, Genre.ADVENTURE, Genre.SCIFI)));
        population.add(new Movie("X-Men", 7.4, 2000,
            genreAdder(Genre.ACTION, Genre.ADVENTURE, Genre.SCIFI)));
        population.add(new Movie("Titanic", 7.7, 1997,
            genreAdder(Genre.DRAMA, Genre.ROMANCE)));
        population.add(new Movie("Avatar", 7.9, 2009,
            genreAdder(Genre.ACTION, Genre.ADVENTURE, Genre.SCIFI)));
        population.add(new Movie("Snatch", 8.3, 2000,
            genreAdder(Genre.COMEDY)));
    }

    /**
    * NOTE:
    * Change all mentions of MySet<> in this method to HashSet<>
    * if you cannot make MySet<> work
    */
    private static MySet<Genre> genreAdder(Genre...genres) {
        MySet<Genre> genreSet = new MySet<Genre>();
        for (Genre g : genres) {
            genreSet.add(g);
        }
        return genreSet;
    }
}
