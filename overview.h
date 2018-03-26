#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "databasehandler.h"

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

    void on_commitNew_clicked();

private:
    Ui::Overview *ui;
    DatabaseHandler* handler;
    QStringList databases;
    bool dbsListed = false;
    std::vector<QObject*> formcache;
};

#endif // OVERVIEW_H
