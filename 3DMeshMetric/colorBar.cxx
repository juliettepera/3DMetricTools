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

#include "colorBar.h"

colorBar::colorBar( QWidget *Qparent ):QWidget( Qparent )
{
    m_Rect = QRect( QPoint( 0 , 0 ) , QPoint( 10 , 10 ) );

    // position rect
    m_XLeftLimit = 0;
    m_XRightLimit = 300;
    m_YLimit = 30;

    // position gradient
    m_CyanPos = 0.4;
    m_Cyan.first = m_CyanPos;
    m_Cyan.second = Qt::cyan;

    m_YellowPos = 0.6;
    m_Yellow.first = m_YellowPos;
    m_Yellow.second = Qt::yellow;

    m_GreenPos = 0.5;
    m_Green.first = m_GreenPos;
    m_Green.second = Qt::green;

    // init gradient signed
    m_ListSigned.push_back( QGradientStop( 0 , Qt::blue ) );
    m_ListSigned.push_back( m_Cyan );
    m_ListSigned.push_back( m_Green );
    m_ListSigned.push_back( m_Yellow );
    m_ListSigned.push_back( QGradientStop( 1 , Qt::red ) );

    // init gradient absolute
    m_ListAbsolute.push_back( QGradientStop( 0 , Qt::green ) );
    m_ListAbsolute.push_back( m_Yellow );
    m_ListAbsolute.push_back( QGradientStop( 1 , Qt::red ) );

    m_Gradient.setStops( m_ListSigned );


}


//*************************************************************************************************
void colorBar::setSize( QPoint topLeft , QPoint bottomRight )
{
    m_XLeftLimit = topLeft.x() + 10;
    m_XRightLimit = bottomRight.x() - 10;
    m_YLimit = bottomRight.y() - 10;

    topLeft.setX( m_XLeftLimit );
    bottomRight.setX( m_XRightLimit );
    bottomRight.setY( m_YLimit );

    m_Rect = QRect( topLeft , bottomRight );
    m_Background = new QPixmap( m_Rect.size() );

    // init arrow
    m_ArrowCenter << QPoint( m_XLeftLimit , m_YLimit );
    m_ArrowCenter << QPoint( m_XLeftLimit - 7 , m_YLimit + 10 );
    m_ArrowCenter << QPoint( m_XLeftLimit + 7 , m_YLimit + 10 );

    m_ArrowCyan << QPoint( m_XLeftLimit , m_YLimit );
    m_ArrowCyan << QPoint( m_XLeftLimit - 7 , m_YLimit + 10 );
    m_ArrowCyan << QPoint( m_XLeftLimit + 7 , m_YLimit + 10 );

    m_ArrowYellow << QPoint( m_XLeftLimit , m_YLimit );
    m_ArrowYellow << QPoint( m_XLeftLimit - 7 , m_YLimit + 10 );
    m_ArrowYellow << QPoint( m_XLeftLimit + 7 , m_YLimit + 10 );

    // translate to good position
    m_ArrowCenter.translate( convertPosition( m_GreenPos ) , 0 );
    m_ArrowCyan.translate( convertPosition( m_CyanPos ) , 0 );
    m_ArrowYellow.translate( convertPosition( m_YellowPos ) , 0 );

    update();
}


//*************************************************************************************************
int colorBar::convertPosition( double PosInColorBar )
{
    int PosInRect = 0;
    PosInRect = ( m_XRightLimit - m_XLeftLimit )*PosInColorBar;
    return PosInRect;
}


//*************************************************************************************************
void colorBar::updateGradientSigned(double DeltaC , double DeltaY , double Center )
{
    // moove center arrow
    int offset = convertPosition( Center ) - convertPosition( m_GreenPos );
    m_ArrowCenter.translate( offset , 0 );

    // replace green position
    m_Green.first = Center;
    m_ListSigned.replace( 2 , m_Green );
    m_GreenPos = Center;

    // moove cyan arrow
    int offsetC = convertPosition( DeltaC ) - convertPosition( m_CyanPos );
    m_ArrowCyan.translate( offsetC , 0 );

    // replace cyan position
    m_Cyan.first = DeltaC;
    m_ListSigned.replace( 1 , m_Cyan );
    m_CyanPos = DeltaC;

    // moove yellow arrow
    int offsetY = convertPosition( DeltaY ) - convertPosition( m_YellowPos );
    m_ArrowYellow.translate( offsetY , 0 );

    // replace arrow position
    m_Yellow.first = DeltaY;
    m_ListSigned.replace( 3 , m_Yellow );
    m_YellowPos = DeltaY;

    // set the new list
    m_Gradient.setStops( m_ListSigned );

    update();
}


void colorBar::updateGradientAbsolute( double DeltaY )
{
    int offsetC = convertPosition( 0 ) - convertPosition( m_CyanPos );
    m_ArrowCyan.translate( offsetC , 0 );
    m_CyanPos = 0;

    int offsetG = convertPosition( 0 ) - convertPosition( m_GreenPos );
    m_ArrowCenter.translate( offsetG , 0 );
    m_GreenPos = 0;

    // moove yellow arrow
    int offsetY = convertPosition( DeltaY ) - convertPosition( m_YellowPos );
    m_ArrowYellow.translate( offsetY , 0 );

    // replace arrow position
    m_Yellow.first = DeltaY;
    m_ListAbsolute.replace( 1 , m_Yellow );
    m_YellowPos = DeltaY;

    // set the new list
    m_Gradient.setStops( m_ListAbsolute );

    update();
}


//*************************************************************************************************
void colorBar::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   painter.eraseRect( m_Rect );

   m_Background = new QPixmap( m_Rect.size() );

   m_Gradient.setStart(m_Rect.topLeft() );
   m_Gradient.setFinalStop( m_Rect.bottomRight() );

   painter.drawPixmap( m_Rect, *m_Background, m_Rect );
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
   painter.setRenderHint(QPainter::Antialiasing);

   QBrush brush( m_Gradient );

   painter.fillRect(m_Rect, brush);
   painter.drawRect(m_Rect);

   QPen pen;
   pen.setColor( Qt::black );
   painter.setPen( pen );

   brush.setStyle( Qt::SolidPattern );
   brush.setColor( Qt::cyan );
   QPainterPath pathc;
   pathc.addPolygon( m_ArrowCyan );
   painter.drawPolygon( m_ArrowCyan );
   painter.fillPath( pathc , brush );

   brush.setColor( Qt::green );
   QPainterPath pathg;
   pathg.addPolygon( m_ArrowCenter );
   painter.drawPolygon( m_ArrowCenter );
   painter.fillPath( pathg , brush );

   brush.setColor( Qt::yellow );
   QPainterPath pathy;
   pathy.addPolygon( m_ArrowYellow );
   painter.drawPolygon( m_ArrowYellow );
   painter.fillPath( pathy , brush );

   painter.end();
}


