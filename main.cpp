#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Notes:
    // The "CS123SceneLoader" class is copied from my own command-line ray tracer I ported a few weeks ago.
    // Its interface is just for an overview of the whole lab pipeline. We don't have to necessarily follow its format.
    // All other files are from last year's stencil.

    // Also, Zack mentioned something about not using the CS123 prefix and namespace any more, because it seems too creepy lol.
    // Anyway, we will figure this out :).

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
