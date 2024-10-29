// AccessCodeDialog.h
#ifndef ACCESSCODEDIALOG_H
#define ACCESSCODEDIALOG_H

#include <QDialog>

namespace Ui {
class AccessCodeDialog;
}

class AccessCodeDialog : public QDialog {
    Q_OBJECT

public:
    explicit AccessCodeDialog(QWidget *parent = nullptr);
    ~AccessCodeDialog();

    bool isAccessCodeValid() const;

private slots:
    void onConfirmClicked();

private:
    Ui::AccessCodeDialog *ui;
    bool accessCodeValid;
};

#endif // ACCESSCODEDIALOG_H
