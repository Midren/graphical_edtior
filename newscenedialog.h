#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QValidator>
#include <QIntValidator>
#include <QLabel>

#ifndef NEWSCENEDIALOG_H
#define NEWSCENEDIALOG_H


class NewSceneDialog : public QDialog
{
    Q_OBJECT
public:
    NewSceneDialog();
    int getHeight();
    int getWidth();
private:
    QLineEdit *heightEdit;
    QLineEdit *widthEdit;
    QDialogButtonBox *addButton;
    QDialogButtonBox *cancelButton;
};

#endif // NEWSCENEDIALOG_H
