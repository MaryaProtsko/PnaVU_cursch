#include "ui/mainwindow.h"

#include <QApplication>

#include "globalinstances.h"
#include "components/logger.h"

int main(int argc, char *argv[]) {
    GlobalInstances({new Logger()});

    QApplication a(argc, argv); GlobalInstances::getLogger()->log_msg("Create QApplication");
    MainWindow w; GlobalInstances::getLogger()->log_msg("Create MainWindow");
    w.show(); GlobalInstances::getLogger()->log_msg("Show MainWindow");
    int app_return_code = a.exec(); GlobalInstances::getLogger()->log_msg("QApplication exited");

    GlobalInstances::delete_instances();
    return app_return_code;

    return 0;
}
