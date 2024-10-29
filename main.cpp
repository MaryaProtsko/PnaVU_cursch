#include <QApplication>
#include "CaptureWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CaptureWindow window;
    window.show();

    return app.exec();
}
