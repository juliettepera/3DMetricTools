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
        int convertPosition( double PosInColorBar );

        /* init the type of the color bar
         * signed = 5 points and absolute = 3 points
         */
        void updateGradientSigned(bool Signed);

        /* change the position of the color
         */
        void changeDelta( double DeltaY , double DeltaC );
        void changeCenter( double NewPosition );

        /* set the arrow position
         */
        //void setArrowPositionCenter( double position );

        /* get the arrow position
         */
        //double getArrowPosition();

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

        int m_ItCyan;
        int m_ItYellow;
        int m_ItGreen;

        double m_GreenPos;
        double m_CyanPos;
        double m_YellowPos;

        QGradientStops m_List;

        QPolygon m_ArrowCenter;
        QPolygon m_ArrowCyan;
        QPolygon m_ArrowYellow;

        int m_XLeftLimit;
        int m_XRightLimit;
        int m_YLimit;

};


#endif
