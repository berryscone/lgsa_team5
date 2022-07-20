#pragma once
#include <QDialog>

#include "ui_VehicleDetailDialog.h"
#include "Model/VehicleDetail.h"


QT_BEGIN_NAMESPACE
namespace Ui { class VehicleDetailDialogClass; };
QT_END_NAMESPACE

class VehicleDetailDialog : public QDialog
{
    Q_OBJECT

public:
    VehicleDetailDialog(QWidget* parent);
    void ShowDetail(const VehicleDetail& vehicleDetail);

private:
    Ui::VehicleDetailDialogClass* ui;
};
