#ifndef UPLINKWIDGET_h
#define UPLINKWIDGET_h

#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtGui/QPainter>
#include <qt5/QtCore/QTimer> 

#include <opencv2/opencv.hpp>

#include <cc_linkmonitor/mobility_management/connectionManager.hpp>
#include <cc_linkmonitor/mobility_management/linkMonitor.hpp>

#include <roaming/colorWidget.hpp>
#include <roaming/frameDisplayWidget.hpp>
#include <roaming/linkMonitorWidget.hpp>


class uplinkWidget : public QWidget {
    // Q_OBJECT

public:

    explicit uplinkWidget(
        int width, 
        int height,
        QWidget *parent = nullptr
    ): 
        QWidget(parent) 
    {
        
        linkMonitor = new LinkMonitorWidget();
        frameDisplay = new frameDisplayWidget(width, height);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(linkMonitor);
        layout->addWidget(frameDisplay);
        setLayout(layout);
    }

    void setSubscriber(Subscriber *sub) { frameDisplay->setSubscriber(sub); }
    void setLinkReader(cc_linkmonitor::linkReader *reader) { linkMonitor->setLinkReader(reader); }
    void setConnectionManager(cc_linkmonitor::ConnectionManager *manager) { linkMonitor->setConnectionManager(manager); }

private:


private:

    LinkMonitorWidget *linkMonitor;
    frameDisplayWidget *frameDisplay;
};


#endif // COMBINEDGUI_h