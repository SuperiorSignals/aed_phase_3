#ifndef CHECKFORM_H
#define CHECKFORM_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class CheckForm;
}

class CheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit CheckForm(QWidget *parent = 0);
    ~CheckForm();
    void completeText(std::string input, int checkBoxNumber);
    void removeCheckBox(int checkBoxCount);
    bool getIsChecked(int number); // Index ranges from 1 to 4

private slots:

private:
    Ui::CheckForm *ui;
    QCheckBox *getCheckBox(int number); // Index ranges from 1 to 4
};

#endif // CHECKFORM_H
