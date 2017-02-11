// Goal class

public class Goal {

    // Static variables
    private static final int MAX_TASKS = 10;

    // Member variables
    private String mName;
    private boolean mComplete;
    private Task[] mTaskList = new Task[MAX_TASKS];
    private boolean mFull;
    private int numTask;

    // Constructor
    public Goal(String name) {
        mName = name;
        mComplete = false;
        mFull = false;
        numTask = 0;
    }

    // Adds new task
    public boolean addTask(Task task) {

        // If task list is full, return false
        if (isFull()) {
            return false;
        }

        // Add the task to the next open slot
        mTaskList[numTask] = task;

        // Increment next slot number
        numTask++;

        // Set full to true if numTask exceeds size
        mFull = numTask >= MAX_TASKS;

        // Set complete if all tasks complete
        mComplete = allTasksComplete();

        return true;
    }

    // Getters
    public String getName() {
        return mName;
    }

    public boolean isComplete() {
        return mComplete;
    }

    public boolean isFull() {
        return mFull;
    }

    public Task[] getTaskList() {
        return mTaskList;
    }

    public int getNumTask() {
        return numTask;
    }

    // toString method
    public String toString() {
        // Set complete if all tasks complete
        mComplete = allTasksComplete();

        String str =  "[" + (mComplete ? "-" : " ") + "]"
            + mName + ": "
            + (mComplete ? "complete\n" : "incomplete\n");
        int i = 0;
        while (i < numTask) {
            str += "   " + mTaskList[i].toString();
            i++;
        }
        return str;
    }

    public String toStringSelection() {
        mComplete = allTasksComplete();
        String  str =  "[" + (mComplete ? "-" : " ") + "]"
            + mName + ": "
            + (mComplete ? "complete\n" : "incomplete\n");
        int i = 0;
        while (i < numTask) {
            str += "[" + (i + 1) + "]" + mTaskList[i].toString();
            i++;
        }
        return str;
    }

    // Check if all tasks are complete
    private boolean allTasksComplete() {
        // If no tasks exist, return false
        if (numTask == 0) {
            return false;
        }

        int i = 0;
        while (i < numTask) {
            if (!mTaskList[i].isComplete()) {
                return false;
            }
            i++;
        }
        return true;
    }

    // Convert to database form
    public String toDatabaseFormat() {
        String mDatabaseString = mName;
        int i = 0;
        while (i < numTask) {
            mDatabaseString += (", " + mTaskList[i].toDatabaseFormat());
            i++;
        }
        return mDatabaseString;
    }

    // Determine if index is valid for array
    public boolean isValidIndex(int index) {
        return index >= 0 && index < numTask;
    }
}
