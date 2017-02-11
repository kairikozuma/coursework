lazy val filesToSubmit = settingKey[Seq[String]]("Tuple of files to submit.")

lazy val root = (project in file(".")).
  settings(
    // The name of the assignment goes here. Homeworks should be prefixed with
    // hw-, while programming quizzes should be prefixed with pq-
    name := "hw-todo-list",
    // In a particular semester's fork of this repo, organization should be
    // changed to match the github.gatech.edu user name of the user under
    // whose name the repo will be forked.
    organization := "tshields3",
    // Must be a tuple of String, i.e., comma-delimited list of Strings
    // enclosed in parentheses. Examples: "src/main/java", "Class.java"
    filesToSubmit := Seq("src/main/java"),
    version := "1.0",
    Keys.commands += org.cs1331.gitsubmitter.GitSubmitterPlugin.submitCommand
  )
