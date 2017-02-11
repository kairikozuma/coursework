// Task class
import java.time.LocalDateTime;

public class Task {

    // Member variables
    private String mName;
    private int mPriority;
    private LocalDateTime mTime;
    private boolean mComplete;

    // Constructor
    public Task(String name, int priority, String time, boolean complete) {
        mName = name;
        mPriority = priority;
        mTime = LocalDateTime.parse(time);
        mComplete = complete;
    }

    // Private method to return correct priority
    private String getPriority() {
        if (mPriority > 6) {
            return "High";
        } else if (mPriority > 3) {
            return "Medium";
        } else {
            return "Low";
        }
    }

    // Getters
    public String getName() {
        return mName;
    }

    public boolean isComplete() {
        return mComplete;
    }

    // Setters
    public void markComplete(boolean complete) {
        mComplete = complete;
    }

    // toString method
    public String toString() {
        // Indent string for task details
        String indent = "\n        ";

        return "[" + (mComplete ? "-" : " ") + "]"
            + mName + indent
            + "Priority: " + getPriority() + indent
            + "Due: " + mTime + indent
            + "Complete: " + (mComplete ? "Yes" : "No") + "\n";
    }

    public String toDatabaseFormat() {
        String divider = " | ";
        return mName + divider + mPriority + divider + mTime.toString()
                    + divider + mComplete;
    }
}
