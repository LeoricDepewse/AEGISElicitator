#include "dynamiclinkdialog.h"
#include "ui_dynamiclinkdialog.h"

DynamicLinkDialog::DynamicLinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DynamicLinkDialog)
{
    ui->setupUi(this);
}

DynamicLinkDialog::~DynamicLinkDialog()
{
    delete ui;
}
