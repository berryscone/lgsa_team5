#pragma once
#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QVideoFrame>
#include <QImage>
#include <QRect>
#include <QLabel>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoSurface(QObject* parent = 0);

Q_SIGNALS:
    void imageProcesssed(QPixmap pix);

private:
    QImage::Format imageFormat;
    QPixmap imageCaptured;
    QVideoFrame currentFrame;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    bool isFormatSupported(const QVideoSurfaceFormat& format) const;

    bool start(const QVideoSurfaceFormat& format);

    bool present(const QVideoFrame& frame);
};
