#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include <QApplication>
#include <QtWidgets>
#include "Subtitle.hpp"


class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow(int argc, char *argv[]);

    public slots:
        void openFile();
        void saveFile();
        void loadFile(const QString &file);
        void generate();
        void shiftLastTime(QTime const time=QTime()) const;

    private:
        // Input and output files
        QLineEdit *srcFile;
        QLabel *destFile;
        bool destModified;

        // Time display
        QLabel *firstTimeLabel;
        QLabel *lastTimeLabel;
        QTime *firstTime;
        QTime *lastTime;

        // Time edit
        QTimeEdit *editFirstSub;
        QCheckBox *moveLastSub;
        QTimeEdit *editLastSub;

        // Subtitle object
        Subtitle subEditor;

        void updateTime() const;
};

#endif
