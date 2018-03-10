#include "overview.h"
#include "ui_overview.h"
#include "dynamiclinkdialog.h"

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
}

Overview::~Overview()
{
    delete ui;
}

void Overview::on_actionDynamic_Linker_triggered()
{
    //Create DynLink Window
    auto window = new DynamicLinkDialog();
    window->show();
}
