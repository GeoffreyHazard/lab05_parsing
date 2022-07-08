#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glwidget.h"
#include "parser/sceneparser.h"

#include <QFileDialog>
#include <QMessageBox>


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
        return;
    }

    SceneMetaData metaData;
    bool success = SceneParser::parse(file.toStdString(), metaData);
    if (!success) {
        QMessageBox::critical(this, "Error", "Parse XML fail");
        return;
    }

    // load the scene
    ui->glwidget->loadScene(metaData);
}

