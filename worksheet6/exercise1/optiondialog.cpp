#include "optiondialog.h"
#include "ui_optiondialog.h"
#include "ModelPart.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::loadFromPart(ModelPart* part)
{
    ui->lineEditName->setText(part->data(0).toString());

    // if you store visible in column 1 as "true"/"false" string:
    ui->checkBoxIsVisible->setChecked(part->data(1).toString() == "true");

    // if you have getters for colour (or store them somewhere):
    ui->redValue->setValue(part->getColourR());
    ui->greenValue->setValue(part->getColourG());
    ui->blueValue->setValue(part->getColourB());
}

void OptionDialog::applyToPart(ModelPart* part)
{
    part->set(0, ui->lineEditName->text());

    part->set(1, ui->checkBoxIsVisible->isChecked() ? "true" : "false");

    part->setColour(
        static_cast<unsigned char>(ui->redValue->value()),
        static_cast<unsigned char>(ui->greenValue->value()),
        static_cast<unsigned char>(ui->blueValue->value())
        );
}
