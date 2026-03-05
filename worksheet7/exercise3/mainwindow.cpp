#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <vtkNew.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    ui->treeView->addAction(ui->actionItem_Options);

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

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
    OptionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
        {
            emit statusUpdateMessage(QString("Dialog accepted."), 2000);
        }
    else
        {
            emit statusUpdateMessage(QString("Dialog rejected."), 2000);
        }
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
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("OpenFile"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));

    if (!fileName.isEmpty())
    {
        emit statusUpdateMessage("Selected: " + fileName, 2000);
    }

    if (fileName.isEmpty())
        return;

    //obtains the selected item in the tree
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
    {
        emit statusUpdateMessage("No tree item selected!", 2000);
        return;
    }

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if(!part)
    {
        emit statusUpdateMessage("Invalid selection.", 2000);
        return;
    }

    QString baseName = QFileInfo(fileName).fileName();

    QList<QVariant> itemData;
    itemData << baseName << "Yes";

    QModelIndex childIndex = partList->appendChild(index, itemData);

    ModelPart* newPart = static_cast<ModelPart*>(childIndex.internalPointer());

    newPart->loadSTL(fileName);
    updateRender();
    resetCamera();

    emit statusUpdateMessage("Loaded STL: " + baseName, 2000);
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    OptionDialog dialog(this);

    dialog.loadFromPart(part);

    if (dialog.exec() == QDialog::Accepted)
    {
        //save dialog values
        dialog.applyToPart(part);

        //send success message
        emit statusUpdateMessage("Saved: " + part->data(0).toString(), 3000);
    }

}

void MainWindow::resetCamera()
{
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
    renderWindow->Render();
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto* part = static_cast<ModelPart*>(index.internalPointer());
        if (part)
        {
            auto a = part->getActor();
            if (a) renderer->AddActor(a);
        }
    }

    if(!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
        return;

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; ++i)
    {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
