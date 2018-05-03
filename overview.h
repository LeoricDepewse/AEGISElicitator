#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSpacerItem>
#include "databasehandler.h"
#include "dataform.h"

namespace Ui {
class Overview;
}

class Overview : public QMainWindow
{
    Q_OBJECT

public:
    explicit Overview(QWidget *parent = 0);
    ~Overview();

private slots:
    void on_actionDynamic_Linker_triggered();

    void on_actionNew_Database_triggered();

    void on_actionOpen_Database_triggered();

    void on_typeCombo_currentIndexChanged(const QString &arg1);

    void createObject(QJsonObject object);

    void on_listCombo_currentIndexChanged(const QString &arg1);

    void on_objectList_currentTextChanged(const QString &currentText);

private:
    Ui::Overview *ui;
    DatabaseHandler* handler;
    QStringList databases;
    bool dbsListed = false;
    DataForm *createForm, *editForm;
//    std::vector<QObject*> formcache;
};

#endif // OVERVIEW_H
