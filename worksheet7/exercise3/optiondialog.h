#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPart.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    void loadFromPart(ModelPart* part);
    void applyToPart(ModelPart* part);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
