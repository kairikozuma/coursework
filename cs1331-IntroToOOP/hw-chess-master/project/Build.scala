import sbt._
import sbt.Keys._
import com.etsy.sbt._

object Cs1331Build extends Build{

  lazy val checkstyle = taskKey[Unit]("Run checkstyle on solution.")

  lazy val checkstyleImpl = Def.task {
    (Checkstyle.CheckstyleTasks.checkstyle in Compile).result.value
    val stylePoints = checkstylePoints
  }

  def checkstylePoints = {
    val checkstyleReport = xml.XML.loadFile("target/checkstyle-report.xml")
    val files = (checkstyleReport \ "file")
    var errorCount = 0
    val report = new StringBuilder
    report.append("\nStyle errors:\n")
    for (file <- files) {
      val name = (file \ "@name").text
      // TODO: this regex will fail on unix files with \ in them
      val fileName = name.split("[\\/]").last
      if (fileName endsWith ".java") {
        val errors = (file \ "error")
        errorCount += errors.length
        if (errors.length > 0)
          report.append(scala.Console.YELLOW + s"$fileName:\n" + scala.Console.RESET)
        for (error <- errors) {
          val msg = (error \ "@message").text
          val line = (error \ "@line").text
          report.append(s"- Line $line: $msg\n")
        }
      }
    }
    report.append(if (errorCount == 0) scala.Console.GREEN + "No style errors!\n" + scala.Console.RESET else scala.Console.RED + s"${errorCount} Total style errors\n" + scala.Console.RESET)
    println(report.toString)
    errorCount
  }
}
