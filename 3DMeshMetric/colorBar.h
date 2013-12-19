/* 3DMeshMetric is a visualization tool used to measures and display
 * surface-to-surface distance between two triangle meshes.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors : Juliette Pera, Francois Budin, Beatriz Paniagua
 * Web site : http://www.nitrc.org/projects/meshmetric3d/
 *
 * Reference : MeshValmet, Validation Metric for Meshes
 * http://www.nitrc.org/projects/meshvalmet/
 *
 * This program also uses Qt libraries
 * http://www.qt-project.org/
 */

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
