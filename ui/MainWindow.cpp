#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GLWidget.h"
#include "parser/SceneParser.h"

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

    RenderData renderData;
    bool success = SceneParser::parse(file.toStdString(), renderData);
    if (!success) {
        QMessageBox::critical(this, "Error", "Parse XML fail");
        return;
    }

    // load the scene
    ui->glwidget->loadScene(renderData);
}

