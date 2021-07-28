#ifndef HEADER_SUBTITLE
#define HEADER_SUBTITLE

#include <fstream>
#include <string>
#include <regex>

#include <QString>
#include <QTime>


class Subtitle
{
    public:
        Subtitle();

        void open(const QString &openFileName);
        QTime getStartTime() const;
        QTime getEndTime() const;

        void generate(QString saveFileName, QTime const& start, QTime const& end);

    private:
        QString fileName;
        QTime startTime;
        QTime endTime;

        std::regex timeRegex;
};

#endif
