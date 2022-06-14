#include <QCoreApplication>
#include <QCommandLineParser>
#include <QString>
#include <iostream>
#include <string>

#include "utils/CS123SceneLoader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addPositionalArgument("filepath", "Path of the scene file.");
    parser.process(a);

    auto positionalArgs = parser.positionalArguments();

    if (positionalArgs.size() != 1) {
        std::cerr << "incorrect parameter input, there should only be one input" << std::endl;
        a.exit(1);
        return 1;
    }

    // Notes:
    // The "CS123SceneLoader" class is copied from my own command-line ray tracer I ported a few weeks ago.
    // Its interface is just for an overview of the whole lab pipeline. We don't have to necessarily follow its format.
    // All other files are from last year's stencil.

    // Also, Zack mentioned something about not using the CS123 prefix and namespace any more, because it seems too creepy lol.
    // Anyway, we will figure this out :).

    QString filepath = positionalArgs[0];
    CS123::CS123SceneMetaData metaData;
    CS123::CS123SceneLoader::load(filepath.toStdString(), metaData);

    // TODO: some testing functions

    return 0;
}
