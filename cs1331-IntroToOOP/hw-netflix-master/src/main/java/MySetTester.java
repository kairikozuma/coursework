/**
 * This class lets you test MySet
 *
 * @author Aaron Friesen
 * @version 1.0
 */
public class MySetTester {

    /**
     * Tests MySet
     * @param args String[] of arguments
     */
    public static void main(String[] args) {
        MySet<String> strings = new MySet<String>();
        strings.add("Hey!");
        strings.add("Hey!");
        strings.add("Hey!");
        strings.add("Hey!");
        strings.add("Hey!");
        strings.add("Listen!");
        strings.add("Sorry, I couldn't resist.");
        strings.add("Sorry, I couldn't resist.");
        strings.add("(you know you would if you could)");

        System.out.println("Testing add:\n");
        System.out.println("Your size: " + strings.size()
                        + ", contains(Listen): " + strings.contains("Listen!"));
        System.out.println("Exp. size: 4, contains(Listen): true\n");

        MySet<String> moreStrings = new MySet<String>();
        moreStrings.add("Sorry, I couldn't resist.");
        moreStrings.add("(you know you would if you could)");

        strings.removeAll(moreStrings);
        System.out.println("Testing remove and removeAll:\n");
        System.out.println("Your size: " + strings.size()
                           + ", contains(Sorry): "
                           + strings.contains("Sorry, I couldn't resist."));
        System.out.println("Exp. size: 2, contains(Sorry): false\n");


        MySet<Integer> ints = new MySet<Integer>();
        for (int i = 0; i < 100; i++) {
            ints.add(i);
        }

        System.out.println("Your size: " + ints.size());
        System.out.println("Exp. size: 100\n");

        for (int i = 0; i < 100; i += 2) {
            ints.remove(i);
        }

        System.out.println("Your size: " + ints.size());
        System.out.println("Exp. size: 50\n");

        MySet<Integer> zeroThroughNine = new MySet<Integer>();

        for (int i = 0; i < 10; i++) {
            zeroThroughNine.add(i);
        }

        ints.retainAll(zeroThroughNine);
        System.out.println("ints should now only retain odd numbers"
                            + " 0 through 10\n");

        System.out.println("Testing your iterator:\n");

        for (Integer i : ints) {
            System.out.println(i);
        }

        System.out.println("\nExpected: \n\n1 \n3 \n5 \n7 \n9\n");

        System.out.println("Yours:");
        for (String s : strings) {
            System.out.println(s);
        }
        System.out.println("\nExpected: \nHey! \nListen!");

        strings.clear();
        System.out.println("\nClearing your set...\n");
        System.out.println("Your set is empty: " + strings.isEmpty());
        System.out.println("Exp. set is empty: true");
    }
}
