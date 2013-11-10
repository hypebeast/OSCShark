#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>

#include "exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *lay1 = new QHBoxLayout();
    QHBoxLayout *lay2 = new QHBoxLayout();
    lay2->addStretch();
    mainLayout->addLayout(lay1);
    mainLayout->addStretch();
    mainLayout->addLayout(lay2);

    cbTimestamps = new QCheckBox("Export Timestamps");
    lay1->addWidget(cbTimestamps);
    cbTimestamps->setChecked(true);

    bCancel = new QPushButton("Cancel");
    lay2->addWidget(bCancel);
    bOk = new QPushButton("Ok");
    lay2->addWidget(bOk);

    this->setWindowTitle("Export OSC Messages to a CSV file");
    this->setLayout(mainLayout);
    this->setFixedSize(350, 120);

    connect(bOk, SIGNAL(clicked()), this, SLOT(onOkClicked()));
    connect(bCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(cbTimestamps, SIGNAL(stateChanged(int)), this, SLOT(onExportTimestampsChecked(int)));
}

void ExportDialog::onOkClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export to"),
                                                    QDir::homePath(), tr("All files (*.*)"));

    if (!fileName.isEmpty() && !fileName.isNull()) {
        fileNameExport = fileName;
    }
    else
        fileNameExport = "";

    accept();
}

void ExportDialog::onCancelClicked()
{
    reject();
}

void ExportDialog::onExportTimestampsChecked(int state)
{
    exportTimestamp = state ? true : false;
}
