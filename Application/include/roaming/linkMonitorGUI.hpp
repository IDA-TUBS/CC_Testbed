#include <qt5/QtWidgets/QApplication>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QPushButton>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtGui/QPainter>
#include <qt5/QtCore/QTimer> // Include QTimer header

class ColorWidget : public QWidget
{
    // Q_OBJECT

public:
    enum linkState
    { 
	    DOWN = 0,
        UP = 1,
        ACTIVE = 2,
    };
    
    ColorWidget(QWidget *parent = nullptr) : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        QFont font = painter.font();
        font.setPointSize(24); // Set the font size to 24
        painter.setFont(font);

        painter.drawText(50, 30, "Link 0");
        painter.drawText(210, 30, "Link 1");
        QRect rectA(10, 40, 150, 50);
        QRect rectB(170, 40, 150, 50);

        // Set colors based on the value of _var
        setLinkStatus(painter, rectA, _var1);
        setLinkStatus(painter, rectB, _var2);
        
    }

    void setLinkStatus(QPainter &painter, QRect &rect, linkState link)
    {
        switch(link)
        {
            case ACTIVE:
                painter.fillRect(rect, Qt::green);
                painter.drawText(rect, Qt::AlignCenter, "active");
                break;

            case UP:
                painter.fillRect(rect, orange);
                painter.drawText(rect, Qt::AlignCenter, "up");
                break;

            case DOWN:
            default:
                painter.fillRect(rect, Qt::red);
                painter.drawText(rect, Qt::AlignCenter, "down");
                break;
        }
    }

public slots:
    void toggleColor(linkState link1, linkState link2)
    {
        _var1 = link1;
        _var2 = link2;
        update();  // Trigger a repaint
    }

private:
    linkState _var1 = DOWN;
    linkState _var2 = DOWN;

    QColor orange = QColor(255, 165, 0); // Define custom orange color
};

