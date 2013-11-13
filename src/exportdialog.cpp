/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Sebastian Ruml
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
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
