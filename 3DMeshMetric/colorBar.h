#ifndef COLORBAR_H
#define COLORBAR_H

// Qt Libraries
#include <QWidget>
#include <QLinearGradient>
#include <QPalette>
#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QGradientStop>
#include <QGradientStops>

// Other Libraries
#include <iostream>

class colorBar: public QWidget
{
    Q_OBJECT

    public:
        colorBar( QWidget *Qparent = NULL );
        void paintEvent(QPaintEvent *);
        void setSize( QPoint topLeft , QPoint bottomRight );
        void changeCyan( double NewPosition );
        void changeYellow( double NewPosition );
        void changeGreen();
        void initGradientSigned();
        void initGradientAbsolute();
        void updateGradient();

    private:
        QLinearGradient m_Gradient;
        QRect m_Rect;
        QPixmap *m_Background;
        QGradientStop m_Cyan;
        QGradientStop m_Yellow;
        QGradientStop m_Green;
        QGradientStops m_List;
        int m_ItCyan;
        int m_ItYellow;
        int m_ItGreen;
};


#endif
