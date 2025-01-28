#include "opentabledialog.h"
#include "ui_opentabledialog.h"

OpenTableDialog::OpenTableDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenTableDialog)
{
    ui->setupUi(this);
}

OpenTableDialog::~OpenTableDialog()
{
    delete ui;
}
