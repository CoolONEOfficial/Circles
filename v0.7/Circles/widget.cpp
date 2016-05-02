#include "widget.h"
#include "ui_widget.h"

//

#include <QPainter>
#include <QMouseEvent>
#include <time.h>
#include <stdlib.h>
#include <QKeyEvent>
#include <QGraphicsDropShadowEffect>

//

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{   
    ui->setupUi(this);

    // Set Labels Font
    lFont.setFamily("Script MT Bold");
    lFont.setWeight(QFont::Light);
    lFont.setPixelSize(38);

    // Random Color
    R = rand()%255;
    G = rand()%255;
    B = rand()%255;

    for(int f = 0; f<Num; f++)
    {
        mas[0][f] = 0;
        mas[1][f] = 0;
    }

    onBools();

    numCircles = 0;

    srand(time(0));

    timerID = startTimer(ui->horizontalSlider->maximum() - ui->horizontalSlider->value() + 10); // 20 optimal value

    showFullScreen();

    OnColor = QColor(255,255,255);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // Antialiasing
    p.setRenderHints(QPainter::Antialiasing,true);

    // Set Color
    p.setPen(QColor(R,G,B));


    // Set Font
    p.setFont(lFont);

    p.setBrush(QColor(R,G,B));

    // Draw All
    drawBackground(p);
    drawLabels(p);
    setFill(p);
    drawNiceLines(p);
    drawLines(p);
    drawCircles(p);

}

void Widget::changeColor()
{
    if(background)
    {
        // Change R,G,B Downs
        if (R>=255)
            rDown = true;

        if (G>=255)
            gDown = true;

        if (B>=255)
            bDown = true;

        // Change R,G,B Downs
        if (R<=1)
            rDown = false;

        if (G<=1)
            gDown = false;

        if (B<=1)
            bDown = false;

        // Change Color
        if (rDown)
            R--;
         else
            R++;

        if (gDown)
            G--;
         else
            G++;

        if (bDown )
            B--;
         else
            B++;
    }

    //
}

void Widget::keyPressEvent(QKeyEvent *event)
{

    // Key Esc

    if(event->key() == Qt::Key_Escape)
    {
        close();
    }

    // Key 1
    if(event->key() == Qt::Key_1)
    {
        if (circles)
        {
            circles = false;
        }
         else
        {
            circles = true;
        }
    }

    // Key 2
    if(event->key() == Qt::Key_2)
    {
        if (lines)
        {
            lines = false;
        }
         else
        {
            lines = true;
        }
    }

    // Key 3
    if(event->key() == Qt::Key_3)
    {
        if (niceLines)
        {
            niceLines = false;
        }
         else
        {
            niceLines = true;
        }
    }

    // Key Background
    if(event->key() == Qt::Key_B)
    {
        if (background)
        {
            background = false;
        }
         else
        {
            background = true;
        }
    }

    // Key Recolor
    if(event->key() == Qt::Key_R)
    {
        R = rand()%255;;
        G = rand()%255;
        B = rand()%255;
    }

    // Key Fill
    if(event->key() == Qt::Key_F && circles)
    {
        if (fill)
        {
            fill = false;
        }
         else
        {
            fill = true;
        }
    }

    // Key Reverse (Q)
    if(event->key() == Qt::Key_Q)
    {
        if (reverse)
        {
            reverse = false;
        }
         else
        {
            reverse = true;
        }
    }
    //
}

void Widget::drawNiceLines(QPainter &p)
{
    // Nicelines
    if (niceLines)
    {
        for(int f = 4; f<numCircles-2; f++)
        {
            p.drawLine(mas[0][f], mas[1][f],mas[0][numCircles-f+1], mas[1][numCircles-f+1]);
        }
    }

    //
}

void Widget::drawLines(QPainter &p)
{
    // Lines
    if (lines)
    {
        for(int f = 0; f<numCircles-2; f++)
        {
            p.drawLine(mas[0][f], mas[1][f],mas[0][f+1], mas[1][f+1]);
        }
    }

    //
}

void Widget::drawCircles(QPainter &p)
{
    // Circles
    if (circles)
    {
        if (reverse)
        {
            for(int f = numCircles-2; f>=0; f--)
            {
                p.drawEllipse(mas[0][f]-f, mas[1][f]-f, 2*f, 2*f);
            }
        }
        else
        {
            for(int f = 0; f<numCircles-2; f++)
            {
                p.drawEllipse(mas[0][f]-f, mas[1][f]-f, 2*f, 2*f);
            }
        }
    }

    //
}

void Widget::drawBackground(QPainter &p)
{
    // Background
    bColor.setRgb(255-R,255-G,255-B,255);
    p.setBrush(bColor);
    p.drawRect(-1,-1,width()+2,height()+2);

    //
}

void Widget::drawLabels(QPainter &p)
{
    drawLabel(p, QRect(p.font().pixelSize()*0.3,0,width(),p.font().pixelSize()),
              "Circles - 1", circles, Qt::AlignLeft);

    drawLabel(p, QRect(p.font().pixelSize()*0.3,p.font().pixelSize()*1.1,width(),p.font().pixelSize()),
              "Lines - 2", lines, Qt::AlignLeft);

    drawLabel(p, QRect(p.font().pixelSize()*0.3,p.font().pixelSize()*1.1*2,width(),p.font().pixelSize()),
              "Nicelines - 3", niceLines, Qt::AlignLeft);


    // Recolor

    p.drawText(QRect(p.font().pixelSize()*0.3,p.font().pixelSize()*1.1*3,width(),p.font().pixelSize()),Qt::AlignLeft,"Color - R");

    drawLabel(p, QRect(p.font().pixelSize()*0.3,p.font().pixelSize()*1.1*4,width(),p.font().pixelSize()),
              "Background - B", background, Qt::AlignLeft);

    drawLabel(p, QRect(p.font().pixelSize()*0.3,p.font().pixelSize()*1.1*5,width(),p.font().pixelSize()),
              "Fill - F", fill, Qt::AlignLeft);

    drawLabel(p, QRect(p.font().pixelSize()*0.3,p.font().pixelSize()*1.1*6,width(),p.font().pixelSize()),
              "Reverse - Q", reverse, Qt::AlignLeft);

    p.setPen(QColor(R,G,B));

    // Time
    p.drawText(ui->horizontalSlider->x() + (ui->horizontalSlider->width () / 2)-40,
               ui->horizontalSlider->y()-10,
               "Time");

    // Version
    p.setPen(QColor(R,G,B));
    p.drawText(QRect(0,0,width(),p.font().pixelSize()),Qt::AlignRight,
               "Circles! v0.7 ");

    // Down
    p.setPen(QColor(R,G,255));
    p.drawText(QRect(ui->horizontalSlider->x(),
               ui->horizontalSlider->y()-p.font().pixelSize(),width(),p.font().pixelSize()),Qt::AlignLeft,
               "Down");

    // Up
    p.setPen(QColor(255,G,B));
    p.drawText(QRect(ui->horizontalSlider->x(),
                     ui->horizontalSlider->y()-p.font().pixelSize(),width(),p.font().pixelSize()+10),Qt::AlignRight,
               "Up  ");
    p.setPen(QColor(R,G,B));
}

void Widget::setFill(QPainter &p)
{
    // Fill
    if (!fill & circles)
    {
        fColor.setRgb(0,0,0,0);
        p.setBrush(fColor);
    }

    //
}

void Widget::drawLabel(QPainter &p, QRect rect, QString label, bool val, Qt::Alignment align)
{
    // Fill
    if(val)
    {
        p.setPen(QColor(R,G,B));
    }
    else
    {
        p.setPen(OnColor);
    }
    p.drawText(rect, align, label);

    p.setPen(QColor(R,G,B));
}

void Widget::resizeEvent(QResizeEvent *)
{
    // Sliders
    ui->horizontalSlider->resize(width()-20,
                                 20);

    ui->horizontalSlider->move(width () - ui->horizontalSlider->width()-10,
                               height() - 20);

    //
}

void Widget::onBools()
{
    // On all Bools
    lines = true;
    circles = true;
    niceLines = true;
    background = true;
    fill = true;

    //
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    // Change Mas
    for(int f = Num-2; f>=0; f--)
    {
        mas[0][f+1] = mas[0][f];
        mas[1][f+1] = mas[1][f];
    }

    mas[0][0] = event->x();
    mas[1][0] = event->y();

    // Change numCircles

    if( numCircles>Num-1 )
    {
        numCircles = Num;
    }

        numCircles++;

    update();

    //
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    // Right Button
    if( event->button() & Qt::RightButton )
    {
        ui->horizontalSlider->setValue(ui->horizontalSlider->value() + ui->horizontalSlider->singleStep());
    }

    // Middle Button
    if( event->button() & Qt::MiddleButton )
    {
        ui->horizontalSlider->setValue((ui->horizontalSlider->maximum()+ui->horizontalSlider->minimum())/2);
    }

    // Left Button
    if( event->button() & Qt::LeftButton )
    {
        ui->horizontalSlider->setValue(ui->horizontalSlider->value() - ui->horizontalSlider->singleStep());
    }

    //
}

void Widget::timerEvent(QTimerEvent *)
{
    // Alpha Blend Value
    if(windowOpacity() < 1)
    {
        setWindowOpacity(windowOpacity()+0.1);
    }

    // Change Mas
    for(int f = Num-2; f>=0; f--)
    {
        mas[0][f+1] = mas[0][f];
        mas[1][f+1] = mas[1][f];
    }

    // Change numCircles
    numCircles--;
    if( numCircles <1 )
    {
        numCircles = 1;
    }

    if(background)
    {
        changeColor();
    }

    update();

    //
}


void Widget::on_horizontalSlider_valueChanged(int value)
{
    // Set Speed/Time
    killTimer(timerID);
    timerID = startTimer(ui->horizontalSlider->maximum()-value+10);

    //
}
