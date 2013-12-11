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
#include <QPolygon>
#include <QPainterPath>


// Other Libraries
#include <iostream>
#include <math.h>

class colorBar: public QWidget
{
    Q_OBJECT

    public:
        colorBar( QWidget *Qparent = NULL );

        /* Set the size of the color bar
         */
        void setSize(QPoint topLeft , QPoint bottomRight );

        /* Convert the position in the colorbar to the actual position on the widget
         */
        int convertPosition( double PosInColorBar );

        /* update the color bar
         * signed = 5 points and absolute = 3 points
         */
        void updateGradientSigned( double DeltaC, double DeltaY , double Center );
        void updateGradientAbsolute( double DeltaY );

        /* paint the color bar and arrow
         */
        void paintEvent(QPaintEvent *);

    private:
        QRect m_Rect;
        QLinearGradient m_Gradient;
        QPixmap *m_Background;

        QGradientStop m_Cyan;
        QGradientStop m_Yellow;
        QGradientStop m_Green;

        double m_GreenPos;
        double m_CyanPos;
        double m_YellowPos;

        QGradientStops m_ListSigned;
        QGradientStops m_ListAbsolute;

        QPolygon m_ArrowCenter;
        QPolygon m_ArrowCyan;
        QPolygon m_ArrowYellow;

        int m_XLeftLimit;
        int m_XRightLimit;
        int m_YLimit;

};


#endif
