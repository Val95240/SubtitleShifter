#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window(argc, argv);
    window.show();

    return app.exec();
}
