#pragma once

#include "IMsgHandler.h"
#include "model/VehicleInfoModel.h"

#include <QImage>

class VehicleInfoMsgHandler :
        public IMsgHandler,
        public IVehicleInfoListener
{
    Q_OBJECT

public :
    VehicleInfoMsgHandler();
    ~VehicleInfoMsgHandler() override;

    // IMsgHandler interface
    void OnStartHandler() override;
    void OnStopHandler() override;

    // IVehicleInfoListener interface
    void OnVehicleInfoUpdated(const std::string &info) override;

signals :
    void UpdateLaptopAppUi(QString info);
};
