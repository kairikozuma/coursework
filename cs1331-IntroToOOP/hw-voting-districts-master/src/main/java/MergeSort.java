import java.util.List;
import java.util.ArrayList;

/**
 * Class that has a merge sort method to sort a list of objects
 *
 * @author kkozuma3
 * @version 1.0
 */
public class MergeSort {

    /**
     * Test merge sort with local districts
     * @param args command line arguments
     */
    public static void main(String[] args) {
        // List<Integer> intC = Arrays.asList(0,5,-2,-5,2,5,4,0,9,-2,-4,8,-5,7);
        // System.out.println(intC);
        // System.out.println(sort(intC));

        ElectionHolder mElectionHolder = null;
        if (args.length == 0) {
            mElectionHolder = new ElectionHolder(
                "../resources/districts.txt",
                "../resources/local_districts.txt",
                "../resources/voters.txt"
                );
        } else if (args.length == 3) {
            mElectionHolder = new ElectionHolder(
                args[0],
                args[1],
                args[2]
            );
        }
        if (mElectionHolder != null) {
            // Extract local districts from list
            List<LocalDistrict> allLD = mElectionHolder.getLocalDistricts();

            System.out.println(allLD);

            // Sort the points
            System.out.println(MergeSort.sort(allLD));
        }
    }

    /**
     * Merge sort method
     * @param  list of comparable of type T
     * @param  <T> comparable type
     * @return      sorted list
     */
    public static <T extends Comparable<T>> List<T> sort(List<T> list) {
        return mergeSort(list.subList(0, list.size() / 2),
            list.subList(list.size() / 2, list.size()));
    }

    private static <T extends Comparable<T>> List<T>
        mergeSort(List<T> listBegin, List<T> listEnd) {
        if (listBegin.size() > 1) {
            listBegin = mergeSort(listBegin.subList(0, listBegin.size() / 2),
                listBegin.subList(listBegin.size() / 2, listBegin.size()));
        }
        if (listEnd.size() > 1) {
            listEnd = mergeSort(listEnd.subList(0, listEnd.size() / 2),
                listEnd.subList(listEnd.size() / 2, listEnd.size()));
        }
        return merge(listBegin, listEnd);
    }

    private static <T extends Comparable<T>> List<T>
        merge(List<T> listA, List<T> listB) {

        List<T> myList = new ArrayList<T>();
        int indexA = 0;
        int indexB = 0;
        T currA = listA.get(indexA);
        T currB = listB.get(indexB);

        while (indexA < listA.size() || indexB < listB.size()) {
            if (indexA < listA.size() && indexB >= listB.size()) {
                myList.add(currA);
                if (++indexA < listA.size()) {
                    currA = listA.get(indexA);
                }
            }

            if (indexA >= listA.size() && indexB < listB.size()) {
                myList.add(currB);
                if (++indexB < listB.size()) {
                    currB = listB.get(indexB);
                }
            }

            if (indexA < listA.size() && indexB < listB.size()) {
                int cmp = currA.compareTo(currB);
                if (cmp <= 0) {
                    myList.add(currA);
                    if (++indexA < listA.size()) {
                        currA = listA.get(indexA);
                    }
                } else {
                    myList.add(currB);
                    if (++indexB < listB.size()) {
                        currB = listB.get(indexB);
                    }
                }
            }
        }

        return myList;
    }
}
