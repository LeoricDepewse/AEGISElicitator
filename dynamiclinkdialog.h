#ifndef DYNAMICLINKDIALOG_H
#define DYNAMICLINKDIALOG_H

#include <QDialog>

namespace Ui {
class DynamicLinkDialog;
}

class DynamicLinkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DynamicLinkDialog(QWidget *parent = 0);
    ~DynamicLinkDialog();

private:
    Ui::DynamicLinkDialog *ui;
};

#endif // DYNAMICLINKDIALOG_H
