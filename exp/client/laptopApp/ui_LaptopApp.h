/********************************************************************************
** Form generated from reading UI file 'LaptopApp.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAPTOPAPP_H
#define UI_LAPTOPAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LaptopApp
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QGraphicsView *playbackView;
    QLabel *playbackTextTitle;
    QFrame *frame_2;
    QListWidget *recentPlatesListView;
    QLabel *playbackTextTitle_2;
    QFrame *frame_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *openButton;
    QPushButton *toggleButton;
    QFrame *frame_4;
    QLabel *vehicleInfo;
    QLabel *playbackTextTitle_3;
    QFrame *line;
    QLabel *label_2;
    QFrame *frame_5;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_4;
    QFrame *frame_6;
    QLabel *debugInfo;

    void setupUi(QMainWindow *LaptopApp)
    {
        if (LaptopApp->objectName().isEmpty())
            LaptopApp->setObjectName(QString::fromUtf8("LaptopApp"));
        LaptopApp->resize(798, 600);
        centralwidget = new QWidget(LaptopApp);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(290, 80, 501, 391));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(1);
        playbackView = new QGraphicsView(frame);
        playbackView->setObjectName(QString::fromUtf8("playbackView"));
        playbackView->setGeometry(QRect(11, 9, 480, 371));
        playbackTextTitle = new QLabel(centralwidget);
        playbackTextTitle->setObjectName(QString::fromUtf8("playbackTextTitle"));
        playbackTextTitle->setGeometry(QRect(480, 40, 131, 41));
        QFont font;
        font.setPointSize(14);
        playbackTextTitle->setFont(font);
        playbackTextTitle->setScaledContents(false);
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 80, 261, 211));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Raised);
        recentPlatesListView = new QListWidget(frame_2);
        recentPlatesListView->setObjectName(QString::fromUtf8("recentPlatesListView"));
        recentPlatesListView->setGeometry(QRect(10, 10, 241, 191));
        recentPlatesListView->setFrameShape(QFrame::Box);
        recentPlatesListView->setLineWidth(2);
        playbackTextTitle_2 = new QLabel(centralwidget);
        playbackTextTitle_2->setObjectName(QString::fromUtf8("playbackTextTitle_2"));
        playbackTextTitle_2->setGeometry(QRect(80, 40, 131, 41));
        playbackTextTitle_2->setFont(font);
        playbackTextTitle_2->setScaledContents(false);
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(290, 540, 501, 51));
        frame_3->setFrameShape(QFrame::Box);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(300, 550, 481, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        openButton = new QPushButton(horizontalLayoutWidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        horizontalLayout->addWidget(openButton);

        toggleButton = new QPushButton(horizontalLayoutWidget);
        toggleButton->setObjectName(QString::fromUtf8("toggleButton"));
        toggleButton->setCheckable(true);

        horizontalLayout->addWidget(toggleButton);

        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(10, 370, 261, 221));
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        vehicleInfo = new QLabel(frame_4);
        vehicleInfo->setObjectName(QString::fromUtf8("vehicleInfo"));
        vehicleInfo->setGeometry(QRect(20, 20, 221, 191));
        QFont font1;
        font1.setPointSize(11);
        vehicleInfo->setFont(font1);
        playbackTextTitle_3 = new QLabel(centralwidget);
        playbackTextTitle_3->setObjectName(QString::fromUtf8("playbackTextTitle_3"));
        playbackTextTitle_3->setGeometry(QRect(80, 330, 131, 41));
        playbackTextTitle_3->setFont(font);
        playbackTextTitle_3->setScaledContents(false);
        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 310, 261, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 10, 91, 21));
        frame_5 = new QFrame(centralwidget);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(10, 7, 261, 30));
        frame_5->setFrameShape(QFrame::Box);
        frame_5->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(230, 5, 21, 20));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/assets/images/greenButton.png")));
        label_3->setScaledContents(true);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(746, 6, 48, 16));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(740, 30, 31, 31));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/assets/images/powerButton.png")));
        label_4->setScaledContents(true);
        frame_6 = new QFrame(centralwidget);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(290, 480, 501, 41));
        frame_6->setFrameShape(QFrame::Box);
        frame_6->setFrameShadow(QFrame::Raised);
        debugInfo = new QLabel(frame_6);
        debugInfo->setObjectName(QString::fromUtf8("debugInfo"));
        debugInfo->setGeometry(QRect(10, 10, 481, 21));
        LaptopApp->setCentralWidget(centralwidget);

        retranslateUi(LaptopApp);

        QMetaObject::connectSlotsByName(LaptopApp);
    } // setupUi

    void retranslateUi(QMainWindow *LaptopApp)
    {
        LaptopApp->setWindowTitle(QCoreApplication::translate("LaptopApp", "LaptopApp", nullptr));
        playbackTextTitle->setText(QCoreApplication::translate("LaptopApp", "PlayBack View", nullptr));
        playbackTextTitle_2->setText(QCoreApplication::translate("LaptopApp", "Recent Plates", nullptr));
        openButton->setText(QCoreApplication::translate("LaptopApp", "Open", nullptr));
        toggleButton->setText(QCoreApplication::translate("LaptopApp", "Pause", nullptr));
        vehicleInfo->setText(QCoreApplication::translate("LaptopApp", "No license plate information recognized yet", nullptr));
        playbackTextTitle_3->setText(QCoreApplication::translate("LaptopApp", "Vehicle Info", nullptr));
        label_2->setText(QCoreApplication::translate("LaptopApp", "Network Status", nullptr));
        label_3->setText(QString());
        label_5->setText(QCoreApplication::translate("LaptopApp", "Exit", nullptr));
        label_4->setText(QString());
        debugInfo->setText(QCoreApplication::translate("LaptopApp", "DebugInfo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LaptopApp: public Ui_LaptopApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAPTOPAPP_H
