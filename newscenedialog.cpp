#include "newscenedialog.h"

NewSceneDialog::NewSceneDialog()
{
    QGridLayout *layout = new QGridLayout;
    QLabel *heightLabel = new QLabel(tr("Height of scene: "));
    QLabel *widthLabel = new QLabel(tr("Width of scene: "));
    heightEdit = new QLineEdit(tr("300"));
    widthEdit = new QLineEdit(tr("300"));
    addButton = new QDialogButtonBox(QDialogButtonBox::Ok);
    cancelButton = new QDialogButtonBox(QDialogButtonBox::Cancel);
    QIntValidator *val = new QIntValidator(1, 1024, this);
    heightEdit->setValidator(val);
    widthEdit->setValidator(val);
    connect(addButton, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(cancelButton, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(heightLabel, 1, 1);
    layout->addWidget(heightEdit, 1, 2);
    layout->addWidget(widthLabel, 2, 1);
    layout->addWidget(widthEdit, 2, 2);
    layout->addWidget(addButton, 3, 1);
    layout->addWidget(cancelButton, 3, 2);
    setLayout(layout);
}

int NewSceneDialog::getHeight() {
    return heightEdit->text().toInt();
}

int NewSceneDialog::getWidth() {
    return widthEdit->text().toInt();
}
