
#ifndef frameDisplayWidget_h
#define frameDisplayWidget_h

#include <qt5/QtGui/QImage>
#include <qt5/QtWidgets/QLabel>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtGui/QPainter>
#include <qt5/QtCore/QTimer> 

#include <opencv2/opencv.hpp>

#include <roaming/subscriber.hpp>


QImage matToQImage(const cv::Mat& mat) {
    if (mat.channels() == 3) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
    } else if (mat.channels() == 1) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    }
    return QImage();
}

class frameDisplayWidget : public QWidget {
    // Q_OBJECT

public:

    static constexpr int DEFAULT_FRAME_WIDTH = 300;
    static constexpr int DEFAULT_FRAME_HEIGHT = 200;
    static constexpr int DEFAULT_INTERVAL = 10; //  update frame display every 10 milliseconds

    explicit frameDisplayWidget(
        int width = DEFAULT_FRAME_WIDTH, 
        int height = DEFAULT_FRAME_HEIGHT,
        int interval = DEFAULT_INTERVAL,
        QWidget *parent = nullptr
    ): 
        QWidget(parent) 
    {
        setWindowTitle("Frame Display");

        // Initialize GUI elements
        imageLabel = new QLabel();
        imageLabel->setFixedSize(width, height); // Default size, can be modified later

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(imageLabel);
        setLayout(layout);

        // Initialize timers
        frameTimer = new QTimer(this);

        connect(frameTimer, &QTimer::timeout, this, &frameDisplayWidget::updateFrame);

        frameTimer->start(10);  
    }

    void setSubscriber(Subscriber *sub) { mySub = sub; }

private slots:
    void updateFrame() {
        if (!mySub) return;

        cv::Mat frame = mySub->get_frame();
        if (!frame.empty()) {
            QImage img = matToQImage(frame);
            imageLabel->setPixmap(QPixmap::fromImage(img).scaled(imageLabel->size(), Qt::KeepAspectRatio));
        }
    }

private:
    QLabel *imageLabel;
    QTimer *frameTimer;

    Subscriber *mySub = nullptr;
};

#endif //frameDisplayWidget_HPP