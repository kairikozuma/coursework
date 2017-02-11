// Kairi Kozuma

// Homework 1 - CS 1331
import java.util.Scanner;
import java.io.File;

public class GradeHistogram {

    public static void main(String[] args) throws Exception {

        // Get the number of buckets
        int bucketSize = args.length == 1 ? 0 : Integer.parseInt(args[1]);

        // If number is not provided, get it from the command line
        if (bucketSize == 0) {
            System.out.print("What bucket size would you like?\n>>> ");
            Scanner mScanner = new Scanner(System.in);
            bucketSize = mScanner.nextInt();
        }

        // Create array of buckets
        boolean hasRemainder = bucketSize != 0 && ((101 % bucketSize) != 0);
        int[] buckets = new int[101 / bucketSize + (hasRemainder ? 1 : 0)];

        // Create scanner from file
        String fileName = args[0];

        //Scanner mScanner = new Scanner(System.in);
        Scanner mFile = new Scanner(new File(fileName));

        while (mFile.hasNext()) {
            int grade = parseLine(mFile.nextLine());
            int loc = buckets.length;
            int lowBoundary = 101;
            // Decrease loc index until lowBoundary is below grade
            while (grade < lowBoundary && lowBoundary > 0) {
                lowBoundary -= bucketSize;
                loc--;
            }
            buckets[loc]++;
        }

        printBucket(buckets, bucketSize);
    }

    // Helper method to obtain integer grade from line
    private static int parseLine(String mLine) {
        String[] lineData = mLine.split(",");
        return Integer.parseInt(lineData[1].trim());
    }

    // Helper method to print function
    private static void printBucket(int[] buckets, int bucketSize) {
        int max = 100;
        // Obtain high and low values
        int high = max;
        int low = max - bucketSize + 1;
        for (int k = buckets.length - 1; k >= 0; k--) {
            System.out.printf("%3d -%3d | ", high, low);
            for (int i = 0; i < buckets[k]; i++) {
                System.out.print("[]");
            }
            System.out.println();
            // Subtract bucket size from high and low
            high -= bucketSize;
            // Low cannot go past 0
            low = bucketSize > low ? 0 : low - bucketSize;
        }
    }
}
