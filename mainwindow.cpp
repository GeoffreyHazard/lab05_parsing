#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glwidget.h"
#include "utils/CS123SceneLoader.h"

#include <QFileDialog>
#include <QMessageBox>

using namespace CS123;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileOpen() {
    QString file = QFileDialog::getOpenFileName(this, QString(), "./scenefiles");
        if (file.isNull()) {
            return;
        }

        if (!file.endsWith(".xml")) {
            QMessageBox::warning(this, "Error", "Unsupported file format");
        } else {
            CS123SceneMetaData metaData;
            bool success = CS123SceneLoader::load(file.toStdString(), metaData);
            if (!success) {
                QMessageBox::critical(this, "Error", "Parse XML fail");
                return;
            }

            // load the scene
            ui->glwidget->loadScene(metaData);
        }
}

