#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <cmath>  // For sine wave
#include <iostream>

class SineWaveWindow : public QWidget {
public:
    SineWaveWindow() {
        setWindowTitle("Sine Wave");
        resize(800, 900);  // Set window size

        // Timer for periodic update (10 ms interval)
        // QTimer *timer = new QTimer(this);
        // connect(timer, SIGNAL(timeout()), this, SLOT(update())); // Use the SIGNAL and SLOT macros
        // timer->start(40);  // Update every 10 ms
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        // For smooth lines
        painter.setRenderHint(QPainter::SmoothPixmapTransform);  
        
        // The pen color 
        painter.setPen(Qt::red);  // Set pen color to blue

        const int centerY = height() / 2;  // Center Y to draw the wave around

        // Start drawing, QPainterPath is an object that paint at specific X,Y in the QT window
        QPainterPath path;
        bool firstPoint = true;

        // Because we show signal in the time domain we want to iritate on the Width, like 
        // signal in math, going from x=0 to x=width() (the limit of the QT window)
        for (int x = 0; x < width(); x+=2) {
            painter.drawPoint(x, centerY);
        }

        refresh_window();
    }
    private: 
        int amplitude = 0;
        int get_amp(){
            return amplitude;
        }
        void set_amp(int amp){
            amplitude = amp;
        }
        
        void refresh_window(){
            // Refresh the screen for new data to be displayed
            update();
        }

};


int main(int argc, char *argv[]){
    QApplication app(argc, argv);  // Start the Qt application

    SineWaveWindow window;  // Create the window object
    window.show();  // Show the window

    return app.exec();

}
