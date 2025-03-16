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
        // timer->start(10);  // Update every 10 ms
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        // For smooth lines
        painter.setRenderHint(QPainter::SmoothPixmapTransform);  
        
        // The pen color 
        painter.setPen(Qt::red);  // Set pen color to blue

        // Parameters for the sine wave
        int amplitude = 50;  // Amplitude of the sine wave
        const int frequency = 2;   // Frequency (number of cycles)
        const int centerY = height() / 2;  // Center Y to draw the wave around

        // Start drawing, QPainterPath is an object that paint at specific X,Y in the QT window
        QPainterPath path;
        bool firstPoint = true;

        // Because we show signal in the time domain we want to iritate on the Width, like 
        // signal in math, going from x=0 to x=width() (the limit of the QT window)
        for (int x = 0; x < width(); ++x) {
            // Calculate y based on sine function
            double radians = frequency * (x / double(width())) * 2 * M_PI; // Normalize x to [0, 2*pi]
            int y = centerY - amplitude * sin(radians); // Calculate y value for sine wave
            std::cout << "Y: " << y << std::endl;
            // This if for setting initially the path starting X,Y through the command MoveTo
            if (firstPoint) {
                // Tell that painter to start at a certain X,Y
                path.moveTo(x, y);  // Start path at the first point
                firstPoint = false;
            // This else for moving the X,Y paint through the sinus wave, Throught LineTo function.
            } 
            else {
                
                // Tell the painter to go to the next X,Y to continues paint
                path.lineTo(x, y);  // Draw line to next point
                
            }
        }

        // Draw the sine wave
        painter.drawPath(path);
        refresh_window();
    }
    private: 
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
