#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>

class ExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExportDialog(QWidget *parent = 0);
    QString fileNameExport;
    bool exportTimestamp;

private:
    QCheckBox *cbTimestamps;
    QPushButton *bOk;
    QPushButton *bCancel;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onExportTimestampsChecked(int state);
};

#endif // EXPORTDIALOG_H
