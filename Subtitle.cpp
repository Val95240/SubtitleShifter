#include "Subtitle.hpp"

Subtitle::Subtitle()
{
    startTime = QTime();
    endTime = QTime();

    timeRegex = "(\\d{2}:){1,2}(\\d{2},\\d{3}) --> (\\d{2}:){1,2}(\\d{2},\\d{3})";
}

void Subtitle::open(const QString &openFileName)
{
    fileName = openFileName;

    std::ifstream file(fileName.toStdString(), std::ios::in);

    if (file)
    {
        bool match;
        std::string line;
        std::string firstLine = "", lastLine = "";
        while (std::getline(file, line))
        {
            if (line[-1] == '\0')
                line.pop_back();

            match = std::regex_match(line, timeRegex);
            if (match)
            {
                if (firstLine == "")
                    firstLine = line;
                lastLine = line;
            }
        }

        QString qfirstLine = QString::fromStdString(firstLine);
        startTime = QTime::fromString(qfirstLine.left(qfirstLine.indexOf(' ')), "hh:mm:ss,zzz");

        QString qlastLine = QString::fromStdString(lastLine);
        endTime = QTime::fromString(qlastLine.left(qlastLine.indexOf(' ')), "hh:mm:ss,zzz");

        file.close();

    } else {
        startTime.setHMS(0, 0, 0, 0);
        endTime.setHMS(0, 0, 0, 0);
    }
}


void Subtitle::generate(QString saveFileName, QTime const& start, QTime const& end)
{
    std::ifstream openFile(fileName.toStdString(), std::ios::in);
    std::ofstream saveFile(saveFileName.toStdString(), std::ios::out | std::ios::trunc);

    double et = endTime.msecsSinceStartOfDay();
    double st = startTime.msecsSinceStartOfDay();
    double net = end.msecsSinceStartOfDay();
    double nst = start.msecsSinceStartOfDay();

    double a = (net - nst) / (et - st);
    double b = nst - a * st;

    if (openFile && saveFile)
    {
        bool match;
        std::string line;
        QString qline;
        QTime time;
        int msecs;

        while (std::getline(openFile, line))
        {
            if (line[-1] == '\0')
                line.pop_back();

            match = std::regex_match(line, timeRegex);
            if (match)
            {
                qline = QString::fromStdString(line);

                msecs = QTime::fromString(qline.left(qline.indexOf(' ')), "hh:mm:ss,zzz").msecsSinceStartOfDay();
                time.setHMS(0, 0, 0, 0);
                time = time.addMSecs(a * msecs + b);
                saveFile << time.toString("hh:mm:ss,zzz").toStdString();

                saveFile << " --> ";

                msecs = QTime::fromString(qline.mid(qline.lastIndexOf(' ')+1), "hh:mm:ss,zzz").msecsSinceStartOfDay();
                time.setHMS(0, 0, 0, 0);
                time = time.addMSecs(a * msecs + b);
                saveFile << time.toString("hh:mm:ss,zzz").toStdString();

                saveFile << std::endl;

            } else {
                saveFile << line << std::endl;
            }
        }
        openFile.close();
        saveFile.close();
    }
}

QTime Subtitle::getStartTime() const
{
    return startTime;
}

QTime Subtitle::getEndTime() const
{
    return endTime;
}
