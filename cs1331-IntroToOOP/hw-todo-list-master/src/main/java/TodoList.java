// Kairi Kozuma

// TodoList class that asks for username
// Shows corresponding goals and tasks

// Import statements
import java.util.Scanner;
import java.io.File;
import java.io.PrintStream;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class TodoList {

    // Static variables
    private static int maxSize = 10;
    private static Goal[] mGoalList = new Goal[maxSize];
    private static int numGoals = 0;

    private static String mName;

    private static File mUserFile;

    // Main method
    public static void main(String[] args) throws Exception {

        // Prompt for username
        System.out.print("What is your name?\n>>");
        Scanner mScanner = new Scanner(System.in);
        mName = mScanner.nextLine();

        // If username is invalid, exit program
        if (!checkUser(mName)) {
            return;
        }

        // If username is valid, launch todo list
        // Initialize goal list
        System.out.printf("Welcome %s\n\n", mName);
        mUserFile = new File(mName + ".csv");
        loadFromDatabase(mName);

        // If input is not 3, continue loop
        int choice = 0;
        boolean finished = false;
        do {
            printGoalsWithTasks();
            printCommandList();
            choice = mScanner.nextInt();

            if (choice == 0) {
                finished = true;
                saveToDatabase();
            } else if (choice == 1) {
                addTask();
            } else if (choice == 2) {
                addGoal();
            } else if (choice == 3) {
                markTaskComplete();
            } else {
                System.out.println("Invalid input!");
            }
        } while (!finished);

        System.out.println(mName + " has logged off");

        saveToDatabase();
    }

    // Helper functions

    // Checks if user is valid
    private static boolean checkUser(String name) throws Exception {
        String fileName = "users.csv";
        Scanner mUserListScanner = new Scanner(new File(fileName));
        String currentName;
        while (mUserListScanner.hasNext()) {
            currentName = (mUserListScanner.next()).replace(",", "").trim();
            if (currentName.equals(name)) {
                return true;
            }
        }

        // Register new user if needed
        System.out.println(name + " was not found in the database.");
        System.out.println("Do you want to register " + name
            + " as new user? (y/n)");

        // Check if user wants to create new user
        Scanner mScanner = new Scanner(System.in);
        boolean valid = false;
        String answer = "";

        while (!valid) {
            answer = mScanner.nextLine().toLowerCase();
            valid = answer.equals("y") || answer.equals("n");
            if (!valid) {
                System.out.println("Please type (y/n)");
            }
        }

        // Reset scanner and scan line
        mUserListScanner = new Scanner(new File(fileName));
        String mLine = mUserListScanner.nextLine();

        // Save new user to database if yes
        if (answer.equals("y")) {
            PrintStream outFile = new PrintStream(new File(fileName));
            outFile.println(mLine + ", " + name);
            return true;
        } else {
            // return false if no
            return false;
        }
    }

    // Load goals into the static array of goals
    private static void loadFromDatabase(String username) throws Exception {
        if (mUserFile.exists()) {
            Scanner mUserFileScanner = new Scanner(mUserFile);
            int i = 0;
            while (mUserFileScanner.hasNextLine()) {
                addGoalToArray(parseGoal(mUserFileScanner.nextLine()));
            }
        } else {
            System.out.print("No database found. Creating new file.\n\n");
        }
    }

    // Parses line to create new goal object with tasks
    private static Goal parseGoal(String line) {
        // Split the line by commas
        String[] goalData = line.split(",");

        // Create new goal
        Goal mGoal = new Goal(goalData[0]);

        // Start at the second token
        int i = 1;

        // Iterate through and add tasks for goal
        while (i < goalData.length) {
            mGoal.addTask(parseTask(goalData[i]));
            i++;
        }

        return mGoal;
    }

    // Parse task
    private static Task parseTask(String line) {
        // Split with "|" and parse the data
        String[] taskData = line.split("[|]");

        // Parse details and convert to correct type
        String name = taskData[0].trim();
        int priority = Integer.parseInt(taskData[1].trim());
        String dateString = taskData[2].trim();
        boolean complete = Boolean.parseBoolean(taskData[3].trim());

        //return new Task with details
        return new Task(name, priority, dateString, complete);
    }

    // Print goals
    private static void printGoals() {
        int i = 0;
        while (i < numGoals) {
            System.out.println("[" + (i + 1) + "]" + mGoalList[i].getName());
            i++;
        }
    }

    // Print goals and tasks
    private static void printGoalsWithTasks() {
        int i = 0;
        while (i < numGoals) {
            System.out.println(mGoalList[i]);
            i++;
        }
    }

    // Print command list
    private static void printCommandList() {
        System.out.print("[1] Add Task\n"
            + "[2] Add Goal\n"
            + "[3] Mark Task Complete\n\n"
            + "[0] Quit & Save\n\n>>"
        );
    }

    // Add new task
    private static boolean addTask() {
        // Instantiate new scanner
        Scanner mScanner = new Scanner(System.in);

        // Goal the task will be added to
        Goal mGoal;

        // Select a goal to add the task
        int choice = 0;
        boolean finished = false;
        do {
            System.out.print("\nSelect goal to add task:\n\n");
            printGoals();
            System.out.print("\n[0] Exit\n\n");
            System.out.print(">>");

            choice = mScanner.nextInt();
            if (choice > 0 && choice <= numGoals) {
                mGoal = mGoalList[choice - 1];
                if (mGoal.isFull()) {
                    System.out.print("The goal chosen is full.\n");
                } else {
                    finished = true;
                }
            } else if (choice == 0) {
                return false;
            } else {
                System.out.println("Invalid choice!");
            }

        } while (!finished);

        System.out.print("\nNew Task\n");
        System.out.print("  - Name: Title of Task\n");
        System.out.print("  - Priority: 1 to 10\n");
        System.out.print("  - Date format: MM/dd/YYYY HH:mm\n");

        mScanner.nextLine();

        // Get name
        System.out.print("  Name: ");
        String name = mScanner.nextLine();

        // Get priority
        System.out.print("  Priority: ");
        int priority = -1;
        boolean validPriority = false;
        while (!validPriority) {
            priority = mScanner.nextInt();
            validPriority = priority > 0 && priority <= 10;
            if (!validPriority) {
                System.out.println("Please enter number between 1 - 10\n");
            }
        }
        mScanner.nextLine();

        // Get due date and time
        System.out.print("  Due: ");
        String dateString = mScanner.nextLine();
        DateTimeFormatter formatter =
            DateTimeFormatter.ofPattern("MM/dd/yyyy HH:mm");
        String date = LocalDateTime.parse(dateString, formatter).toString();

        mGoalList[choice - 1].addTask(new Task(name, priority, date, false));

        return true;
    }

    // Add new goal
    private static void addGoal() {
        // Instantiate new scanner
        Scanner mScanner = new Scanner(System.in);

        System.out.print("\nNew Goal\n");

        // Get name
        System.out.print("  Name: ");
        String name = mScanner.nextLine();

        addGoalToArray(new Goal(name));
    }

    // Add new goal and allow for resizable array
    private static void addGoalToArray(Goal goal) {
        // If number of goals is within array limitations, add it to array
        // Otherwise, copy to new larger array
        if (numGoals >= maxSize) {
            maxSize += 10;
            Goal[] mNewList = new Goal[maxSize];
            int i = 0;
            while (i < numGoals) {
                mNewList[i] = mGoalList[i];
                i++;
            }
            mGoalList = mNewList;
        }

        mGoalList[numGoals] = goal;
        numGoals++;
    }

    // Save to database
    private static void saveToDatabase() throws Exception {
        PrintStream outFile = new PrintStream(mUserFile);
        int i = 0;
        while (i < numGoals) {
            outFile.println(mGoalList[i].toDatabaseFormat());
            i++;
        }
        outFile.close();
    }

    // Mark task complete
    private static void markTaskComplete() {
        // Instantiate new scanner
        Scanner mScanner = new Scanner(System.in);

        // Goal the task will be added to
        Goal mGoal;

        // Select a goal to add the task
        boolean finishedGoalSelection = false;
        do {
            System.out.print("\nSelect goal to mark task complete:\n\n");
            printGoals();
            System.out.print("\n[0] Exit\n\n>>");

            int choiceGoal = mScanner.nextInt();

            if (choiceGoal > 0 && choiceGoal <= numGoals) {
                mGoal = mGoalList[choiceGoal - 1];

                boolean finishedTaskSelection = false;
                do {
                    System.out.print("\nSelect task to mark complete\n\n");

                    System.out.println(mGoal.toStringSelection());

                    System.out.print("\n[0] Exit\n\n>>");

                    int choiceTask = mScanner.nextInt();
                    if (mGoal.isValidIndex(choiceTask - 1)) {
                        mGoal.getTaskList()[choiceTask - 1].markComplete(true);
                    } else if (choiceTask == 0) {
                        finishedTaskSelection = true;
                    } else {
                        System.out.println("Invalid choice!");
                    }

                } while (!finishedTaskSelection);
            } else if (choiceGoal == 0) {
                finishedGoalSelection = true;
            }

        } while (!finishedGoalSelection);
    }
}
