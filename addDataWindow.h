//
// Created by Daniil on 28.10.2024.
//

#ifndef QTPROJECT_ADDDATAWINDOW_H
#define QTPROJECT_ADDDATAWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

class AddDataWindow : public QDialog {
private:
    QLineEdit *nameEdit{};
    QLineEdit *loginEdit{};
    QLineEdit *passwordEdit{};
public:
    explicit AddDataWindow(QWidget* parent = nullptr);

    QString getName() const;
    QString getLogin() const;
    QString getPassword() const;
};


#endif //QTPROJECT_ADDDATAWINDOW_H
