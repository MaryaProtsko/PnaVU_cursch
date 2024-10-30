#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui {
class mainWidget;
}
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();

    QString getDay();

private slots:
    void on_pushButton_clicked();

    void on_pushButtonBackInfo_clicked();

    void on_pushButtonRegistrationMenu_clicked();

    void on_pushButtonRegist_clicked();

    void on_pushButtonBackMain_clicked();

    void on_pushButtonAuthorization_clicked();

    void on_radioButtonClient_clicked();

    void on_radioButtonEmployee_clicked();

    void on_pushButtonEntering_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::mainWidget *ui;
};
#endif // MAINWIDGET_H
