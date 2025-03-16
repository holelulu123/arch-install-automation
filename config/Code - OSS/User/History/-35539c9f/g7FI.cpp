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
        resize(400, 300);  // Set window size

        // Timer for periodic update (10 ms interval)
        // QTimer *timer = new QTimer(this);
        // connect(timer, SIGNAL(timeout()), this, SLOT(update())); // Use the SIGNAL and SLOT macros
        // timer->start(10);  // Update every 10 ms
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        // For smooth lines
        painter.setRenderHint(QPainter::SmoothPixmapTransform);  
        
        painter.setPen(Qt::red);  // Set pen color to blue

        // Parameters for the sine wave
        const int amplitude = 50;  // Amplitude of the sine wave
        const int frequency = 2;   // Frequency (number of cycles)
        const int centerY = height() / 2;  // Center Y to draw the wave around

        // Start drawing, QPainterPath is an object that paint at specific X,Y in the QT window
        QPainterPath path;
        bool firstPoint = true;

        int first_point_x;
        int first_point_y;
        for (int x = 0; x < width(); ++x) {
            // Calculate y based on sine function
            double radians = frequency * (x / double(width())) * 2 * M_PI; // Normalize x to [0, 2*pi]
            int y = centerY - amplitude * sin(radians); // Calculate y value for sine wave
            
            if (firstPoint) {
                first_point_x = x;
                first_point_y = y;
                // Tell that painter to start at a certain X,Y
                path.moveTo(first_point_x, first_point_y);  // Start path at the first point
                firstPoint = false;
            } else {
                first_point_x += 20;
                first_point_y += 20;
                std::cout << "Y: " << first_point_y << "| X: " << first_point_x << std::endl;
                
                // Tell the painter to go to the next X,Y to continues paint
                path.lineTo(first_point_x, first_point_y);  // Draw line to next point
                
            }
        }

        // Draw the sine wave
        painter.drawPath(path);
    }
};


int main(int argc, char *argv[]){
    QApplication app(argc, argv);  // Start the Qt application

    SineWaveWindow window;  // Create the window object
    window.show();  // Show the window

    return app.exec();

}
