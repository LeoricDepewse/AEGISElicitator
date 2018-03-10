#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QMainWindow>

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

private:
    Ui::Overview *ui;
};

#endif // OVERVIEW_H
