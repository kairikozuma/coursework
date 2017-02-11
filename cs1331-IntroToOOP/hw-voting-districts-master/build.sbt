onLoadMessage := ""
showSuccess := false
com.etsy.sbt.Checkstyle.CheckstyleTasks.checkstyleConfig := file("cs1331-checkstyle.xml")

lazy val filesToSubmit = settingKey[Seq[String]]("Files to submit, test, and grade.")
lazy val collabs = settingKey[Seq[String]]("Users to add as collaborators.")

lazy val root = (project in file(".")).
  settings(
    com.etsy.sbt.Checkstyle.checkstyleSettings).
  settings(

    // The name of the assignment goes here. Homeworks should be prefixed with
    // hw-, while programming quizzes should be prefixed with pq-
    name := "hw-voting-districts",
    organization := "cs1331",

    // The list of people to add to the student's repository as collaborators.
    // Typically these are the users who will fork the student's submission,
    // so they should have admin rights for the grading organization.
    // Must be a Seq of String
    collabs := Seq("tshields3", "thartman3", "kcartledge3"),

    // The specific files to submit, can be a directory.
    // Must be a Seq of String
    filesToSubmit := Seq("src/main/java"),
    version := "1.0",
    checkstyle := checkstyleImpl.value,
    Keys.commands += org.cs1331.gitsubmitter.GitSubmitterPlugin.submitCommand
  )
