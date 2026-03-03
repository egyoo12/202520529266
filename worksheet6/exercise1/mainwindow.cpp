#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    // create/allocate the ModelList
    this->partList = new ModelPartList("PartsList");

    //link it to the treeview in the gui
    ui->treeView->setModel(this->partList);

    //manually create a model tree
    ModelPart *rootItem = this->partList->getRootItem();

    //add three top level items
    for (int i = 0; i <3; i++)
        {
            //create strings for both data columns
            QString name = QString("TopLevel %1").arg(i);
            QString visible("true");

            //create child item
            ModelPart *childItem = new ModelPart({name, visible});

            //append to tree top-level
            rootItem->appendChild(childItem);

            //add five sub-items
            for (int j = 0; j < 5; j++)
            {
                QString name = QString("Item %1,%2").arg(i).arg(j);
                QString visible("true");

                ModelPart *childChildItem = new ModelPart({name, visible});

                //append to parent
                childItem->appendChild(childChildItem);
            }
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1()
{
    emit statusUpdateMessage("button 1 has been clicked!", 2000);
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage("button 2 has been clicked!", 2000);
}

void MainWindow::handleTreeClicked(const QModelIndex& index)
{
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString name = selectedPart->data(0).toString();
    emit statusUpdateMessage("selected: " + name, 2000);
}

void MainWindow::on_actionOpen_File_triggered()
{
    //adding code to test if the action is working
    emit statusUpdateMessage(QString("Open File action triggered."), 2000);
}

