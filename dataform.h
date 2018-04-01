#ifndef DATAFORM_H
#define DATAFORM_H
#include <QtCore/QDebug>
#include <typeinfo>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QSpinBox>
#include <QInputDialog>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include "dataobject.h"
#include "objectpayloads.h"

class DataForm : public QFrame
{
    Q_OBJECT
public:
    explicit DataForm(QWidget *parent = 0);
    DataObject *dataModel() const;
    void setDataModel(DataObject *dataModel);

private:
    DataObject* _dataModel;
    std::vector<QWidget*> _formFields;
    QPushButton* _submitButton;
signals:
    void on_submit(QJsonObject object);
public slots:
    void submit();
};

#endif // DATAFORM_H
