#include "VehicleDetailDialog.h"


VehicleDetailDialog::VehicleDetailDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::VehicleDetailDialogClass())
{
	ui->setupUi(this);
}

void VehicleDetailDialog::ShowDetail(const VehicleDetail& vehicleDetail) {
	QPixmap image = QPixmap::fromImage(vehicleDetail.image);
	image = image.scaledToHeight(120);
	ui->label_image->setPixmap(image);
	if (vehicleDetail.status != VehicleDetail::Status::Normal) {
		ui->label_image->setStyleSheet("QLabel{ border: 2px solid red; }");
	}
	else {
		ui->label_image->setStyleSheet("");
	}
	ui->line_number->setText(vehicleDetail.number);
	ui->line_reason->setText(vehicleDetail.rawStatus);
	ui->line_make->setText(vehicleDetail.make);
	ui->line_model->setText(vehicleDetail.model);
	ui->line_color->setText(vehicleDetail.color);
	ui->line_owner->setText(vehicleDetail.owner);
	ui->text_address->setText(vehicleDetail.address);
	show();
}