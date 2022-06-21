#include "VideoSurface.h"
#include <QtWidgets>
#include <QtMultimedia/QAbstractVideoSurface.h>
#include <QtMultimedia/QVideoSurfaceFormat.h>
#include <iostream>


VideoSurface::VideoSurface(QObject* parent)
    : QAbstractVideoSurface(parent)
    , imageFormat(QImage::Format_Invalid)
{
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(
    QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
            << QVideoFrame::Format_RGB32
            << QVideoFrame::Format_ARGB32
            << QVideoFrame::Format_ARGB32_Premultiplied
            << QVideoFrame::Format_RGB565
            << QVideoFrame::Format_RGB555;
    }
    else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool VideoSurface::isFormatSupported(const QVideoSurfaceFormat& format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
        && !size.isEmpty()
        && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool VideoSurface::start(const QVideoSurfaceFormat& format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        QAbstractVideoSurface::start(format);
        return true;
    }
    else {
        return false;
    }
}

bool VideoSurface::present(const QVideoFrame& frame)
{
    currentFrame = frame;
    if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        QImage image(
            currentFrame.bits(),
            currentFrame.width(),
            currentFrame.height(),
            currentFrame.bytesPerLine(),
            imageFormat);

        // 여기서 image 객체 가지고 image processing 하면 됨

        imageCaptured = QPixmap::fromImage(image.copy(image.rect()));
        currentFrame.unmap();
        emit imageProcesssed(imageCaptured);
    }
    return true;
}