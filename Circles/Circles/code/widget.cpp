#include "widget.h"
#include "ui_widget.h"

//

#include <QPainter>
#include <QMouseEvent>
#include <time.h>
#include <stdlib.h>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFontDatabase>

//

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    // Random Color
    R = rand()%255;
    G = rand()%255;
    B = rand()%255;

    for(int f = 0; f<Num; f++)
    {
        mas[0][f] = 0;
        mas[1][f] = 0;
    }

    loadAll();

    numCircles = 0;

    srand(time(0));

    ui->setupUi(this);

    timerID = startTimer(ui->horizontalSlider->maximum() - ui->horizontalSlider->value() + 10); // 20 optimal value

    OnColor = QColor(255,255,255);

    showFullScreen();

    setFontAll();

    settings = true;
    openSettings();
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
    if(ui->checkBox_background->isChecked())
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
}

void Widget::drawNiceLines(QPainter &p)
{
    // Nicelines
    if (ui->checkBox_nicelines->isChecked())
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
    if (ui->checkBox_lines->isChecked())
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
    if (ui->checkBox_circles->isChecked())
    {
        if (ui->checkBox_reverce->isChecked())
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
    if(!settings)
    {
        // Version
        p.setFont(font);
        p.setPen(QColor(0,0,0,150));
        p.drawText(QRect(0,0,width(),p.font().pixelSize()),Qt::AlignRight,
                   "Circles v0.8 ");

        p.drawImage(QRect(width()-p.font().pixelSize()-10,p.font().pixelSize()+10,p.font().pixelSize(),p.font().pixelSize()),settingsIcon);
    }
}

void Widget::setFill(QPainter &p)
{
    // Fill
    if (!ui->checkBox_fill->isChecked() & ui->checkBox_circles->isChecked())
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

void Widget::openSettings()
{
    // Change visibility
    ui->checkBox_background->setVisible(!ui->checkBox_background->isVisible());
    ui->checkBox_circles->setVisible(!ui->checkBox_circles->isVisible());
    ui->checkBox_fill->setVisible(!ui->checkBox_fill->isVisible());
    ui->checkBox_lines->setVisible(!ui->checkBox_lines->isVisible());
    ui->checkBox_nicelines->setVisible(!ui->checkBox_nicelines->isVisible());
    ui->checkBox_reverce->setVisible(!ui->checkBox_reverce->isVisible());
    ui->horizontalSlider->setVisible(!ui->horizontalSlider->isVisible());
    ui->label->setVisible(!ui->label->isVisible());
    ui->pushButton->setVisible(!ui->pushButton->isVisible());
    ui->pushButton_back->setVisible(!ui->pushButton_back->isVisible());
    settings = !settings;
}

void Widget::loadAll()
{
    // Load All

    // Images
    settingsIcon.load(":/images/settings.png");

    // Fonts
    QString path = ":/fonts/font.ttf";

    // Set path
    int fontId = QFontDatabase::addApplicationFont(path);

    if (fontId != -1)  // No load Fail?
    {
        font.setFamily(QFontDatabase::applicationFontFamilies(fontId).first());
    } else
    {
        QMessageBox::information(this, "Error load font", path);
    }

    font.setPixelSize(30);
}

void Widget::setFontAll()
{
    ui->checkBox_background->setFont(font);
    ui->checkBox_circles->setFont(font);
    ui->checkBox_fill->setFont(font);
    ui->checkBox_lines->setFont(font);
    ui->checkBox_nicelines->setFont(font);
    ui->checkBox_reverce->setFont(font);
    ui->label->setFont(font);
    ui->pushButton->setFont(font);
    ui->pushButton_back->setFont(font);
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

    if(ui->checkBox_background->isChecked())
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

void Widget::on_pushButton_clicked()
{
    // Recolor
    R = rand()%255;
    G = rand()%255;
    B = rand()%255;
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    // Open
    if(event->x()>width()-70 &&
            event->y()<110 &&
            event->y()>40)
    openSettings();
}

void Widget::on_pushButton_back_clicked()
{
    // Back
    openSettings();
    ui->pushButton_back->setVisible(false);
}
