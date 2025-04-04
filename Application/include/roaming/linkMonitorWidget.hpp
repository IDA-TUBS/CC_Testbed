#ifndef LinkMonitorWidget_h
#define LinkMonitorWidget_h

#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtGui/QPainter>
#include <qt5/QtCore/QTimer> 

#include <opencv2/opencv.hpp>

#include <cc_linkmonitor/mobility_management/connectionManager.hpp>
#include <cc_linkmonitor/mobility_management/linkMonitor.hpp>

#include <roaming/colorWidget.hpp>
#include <roaming/subscriber.hpp>


class LinkMonitorWidget : public QWidget {
    // Q_OBJECT

public:

    static constexpr int DEFAULT_WIDTH = 350;
    static constexpr int DEFAULT_HEIGHT = 120;
    static constexpr int DEFAULT_INTERVAL = 1000; //  update link status every 1000 milliseconds (1 second)

    explicit LinkMonitorWidget(
        int width = DEFAULT_WIDTH, 
        int height = DEFAULT_HEIGHT, 
        int interval = DEFAULT_INTERVAL,
        QWidget *parent = nullptr
    ): 
        QWidget(parent) 
    {
        setWindowTitle("Link Monitor");

        // Initialize GUI elements
        colorWidget = new ColorWidget();
        colorWidget->setMinimumSize(350, 120);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(colorWidget);
        setLayout(layout);

        // Initialize timers
        linkTimer = new QTimer(this);

        connect(linkTimer, &QTimer::timeout, this, &LinkMonitorWidget::updateLinkStatus);

        linkTimer->start(interval);
    }

    void setLinkReader(cc_linkmonitor::linkReader *reader) { linkReader = reader; }
    void setConnectionManager(cc_linkmonitor::ConnectionManager *manager) { connManager = manager; }

private slots:
    void updateLinkStatus() {
        if (!linkReader || !connManager) return;

        static ColorWidget::linkState link0 = ColorWidget::linkState::DOWN;
        static ColorWidget::linkState link1 = ColorWidget::linkState::DOWN;

        std::vector<std::string> connections = linkReader->getActiveLinks();
        cc_linkmonitor::ConnectionManager::dataplane_pair active_link = connManager->get_active_pair();

        link0 = ColorWidget::linkState::DOWN;
        link1 = ColorWidget::linkState::DOWN;

        for (const auto& str : connections) {
            if (str.find("102") != std::string::npos) {
                link0 = (str == active_link.first) ? ColorWidget::linkState::ACTIVE : ColorWidget::linkState::UP;
            }
            if (str.find("103") != std::string::npos) {
                link1 = (str == active_link.first) ? ColorWidget::linkState::ACTIVE : ColorWidget::linkState::UP;
            }
        }

        colorWidget->toggleColor(link0, link1);
    }

private:
    ColorWidget *colorWidget;
    QTimer *linkTimer;

    cc_linkmonitor::linkReader *linkReader = nullptr;
    cc_linkmonitor::ConnectionManager *connManager = nullptr;
};

#endif // LinkMonitorWidget_HPP