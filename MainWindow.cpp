#include "MainWindow.hpp"

MainWindow::MainWindow(int argc, char *argv[])
{
    // File Settings Group
    QLabel *srcLabel = new QLabel("File : ");
    srcFile = new QLineEdit;
    QPushButton *openFileButton = new QPushButton("Br&owse...");

    QLabel *destLabel = new QLabel("Dest : ");
    destFile = new QLabel;
    destModified = false;
    QPushButton *saveFileButton = new QPushButton("Brow&se...");

    QGridLayout *fileSettingsGrid = new QGridLayout;
    fileSettingsGrid->addWidget(srcLabel, 0, 0);
    fileSettingsGrid->addWidget(srcFile, 0, 1);
    fileSettingsGrid->addWidget(openFileButton, 0, 2);
    fileSettingsGrid->addWidget(destLabel, 1, 0);
    fileSettingsGrid->addWidget(destFile, 1, 1);
    fileSettingsGrid->addWidget(saveFileButton, 1, 2);

    QGroupBox *fileSettingsGroup = new QGroupBox("File settings");
    fileSettingsGroup->setLayout(fileSettingsGrid);

    // Subtitle Group
    firstTime = new QTime(0, 0, 0, 0);
    lastTime = new QTime(0, 0, 0, 0);

    firstTimeLabel = new QLabel("00:00:00.000");
    lastTimeLabel = new QLabel("00:00:00.000");

    QLabel *newFirstTimeLabel = new QLabel("New first subtitle time : ");
    editFirstSub = new QTimeEdit;
    editFirstSub->setDisplayFormat("hh:mm:ss.zzz");

    moveLastSub = new QCheckBox("&New last subtitle time : ");
    moveLastSub->setTristate(false);
    editLastSub = new QTimeEdit;
    editLastSub->setDisplayFormat("hh:mm:ss.zzz");
    editLastSub->setEnabled(false);

    QGridLayout *subtitleGrid = new QGridLayout;
    subtitleGrid->addWidget(new QLabel("Time first subtitle : "), 0, 0);
    subtitleGrid->addWidget(firstTimeLabel, 0, 1);
    subtitleGrid->addWidget(new QLabel("Time last subtitle : "), 1, 0);
    subtitleGrid->addWidget(lastTimeLabel, 1, 1);
    subtitleGrid->addWidget(newFirstTimeLabel, 2, 0);
    subtitleGrid->addWidget(editFirstSub, 2, 1);
    subtitleGrid->addWidget(moveLastSub, 3, 0);
    subtitleGrid->addWidget(editLastSub, 3, 1);

    QGroupBox *subtitleGroup = new QGroupBox("Subtitles edit");
    subtitleGroup->setLayout(subtitleGrid);

    // Buttons
    QPushButton *generate = new QPushButton("&Generate");
    QPushButton *exit = new QPushButton("E&xit");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignRight);

    buttonLayout->addWidget(generate);
    buttonLayout->addWidget(exit);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileSettingsGroup);
    mainLayout->addWidget(subtitleGroup);
    mainLayout->addLayout(buttonLayout);

    updateTime();

    setLayout(mainLayout);
    setWindowTitle("Subtitle shifter");
    resize(500, 250);

    // Connect signals and slots
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(saveFileButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(srcFile, SIGNAL(textChanged(const QString)), this, SLOT(loadFile(const QString)));

    connect(moveLastSub, SIGNAL(clicked()), this, SLOT(shiftLastTime()));
    connect(editFirstSub, SIGNAL(timeChanged(const QTime)), this, SLOT(shiftLastTime(const QTime)));

    connect(generate, SIGNAL(clicked()), this, SLOT(generate()));
    connect(exit, SIGNAL(clicked()), qApp, SLOT(quit()));

    if (argc >= 2)
        srcFile->setText(QString(argv[1]));

    if (argc >= 3)
    {
        destFile->setText(QString(argv[2]));
        destModified = true;
    }
}

void MainWindow::updateTime() const
{
    firstTimeLabel->setText(firstTime->toString("hh:mm:ss.zzz"));
    lastTimeLabel->setText(lastTime->toString("hh:mm:ss.zzz"));

    editFirstSub->setTime(*firstTime);
    editLastSub->setTime(*lastTime);
}

void MainWindow::shiftLastTime(QTime const time) const
{
    editLastSub->setEnabled(moveLastSub->isChecked());
    if (!moveLastSub->isChecked())
    {
        QTime addition = subEditor.getEndTime();
        addition = addition.addMSecs(editFirstSub->time().msecsSinceStartOfDay());
        addition = addition.addMSecs(-1*subEditor.getStartTime().msecsSinceStartOfDay());
        editLastSub->setTime(addition);
    }
}

void MainWindow::openFile()
{
    srcFile->setText(QFileDialog::getOpenFileName(this, "Open a file", "", "Subtitles (*.srt)"));
}

void MainWindow::saveFile()
{
    destFile->setText(QFileDialog::getSaveFileName(this, "Save a file", "", "Subtitles (*.srt)"));
    destModified = true;
}

void MainWindow::loadFile(const QString &file)
{
    if (!destModified)
    {
        int index = file.lastIndexOf('/');
        QString modifiedFile = "";

        if (index != -1)
            modifiedFile = file.left(++index);
        modifiedFile += "shift_" + file.mid(index);

        destFile->setText(modifiedFile);
    }

    subEditor.open(file);
    *firstTime = subEditor.getStartTime();
    *lastTime = subEditor.getEndTime();
    updateTime();
}

void MainWindow::generate()
{
    subEditor.generate(destFile->text(), editFirstSub->time(), editLastSub->time());
    qApp->quit();
}
