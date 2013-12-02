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

    m_ItCyan = 1; m_ItGreen = 2 ; m_ItYellow = 3;

    // init gradient
    m_List.push_back( QGradientStop( 0 , Qt::blue ) );
    m_List.push_back( m_Cyan );
    m_List.push_back( m_Green );
    m_List.push_back( m_Yellow );
    m_List.push_back( QGradientStop( 1 , Qt::red ) );

    m_Gradient.setStops( m_List );


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
void colorBar::updateGradientSigned( bool Signed )
{
    m_List.clear();

    if( Signed == true )
    {
        m_ItCyan = 1; m_ItGreen = 2 ; m_ItYellow = 3;

        m_List.push_back( QGradientStop( 0 , Qt::blue ) );
        m_List.push_back( m_Cyan );
        m_List.push_back( m_Green );
    }
    else
    {
        m_ItYellow = 1;

        m_List.push_back( QGradientStop( 0 , Qt::green ) );
    }

    m_List.push_back( m_Yellow );
    m_List.push_back( QGradientStop( 1 , Qt::red ) );

    m_Gradient.setStops( m_List );

    update();
}


//*************************************************************************************************
void colorBar::changeDelta( double DeltaY , double DeltaC )
{
    m_Cyan.first = DeltaC;
    m_List.replace( m_ItCyan , m_Cyan );

    int offsetC = convertPosition( DeltaC ) - convertPosition( m_CyanPos );
    m_ArrowCyan.translate( offsetC , 0 );
    m_CyanPos = DeltaC;

    m_Yellow.first = DeltaY;
    m_List.replace( m_ItYellow , m_Yellow );

    int offsetY = convertPosition( DeltaY ) - convertPosition( m_YellowPos );
    m_ArrowYellow.translate( offsetY , 0 );
    m_YellowPos = DeltaY;

    update();
}

void colorBar::changeCenter( double NewPosition )
{
    int offset = convertPosition( NewPosition ) - convertPosition( m_GreenPos );
    m_ArrowCenter.translate( offset , 0 );

    m_Green.first = NewPosition;
    m_List.replace( m_ItGreen , m_Green );
    m_GreenPos = NewPosition;

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

   brush.setColor( Qt::green );
   brush.setStyle( Qt::SolidPattern );

   QPainterPath pathg;
   pathg.addPolygon( m_ArrowCenter );
   painter.drawPolygon( m_ArrowCenter );
   painter.fillPath( pathg , brush );

   brush.setColor( Qt::cyan );
   QPainterPath pathc;
   pathc.addPolygon( m_ArrowCyan );
   painter.drawPolygon( m_ArrowCyan );
   painter.fillPath( pathc , brush );

   brush.setColor( Qt::yellow );
   QPainterPath pathy;
   pathy.addPolygon( m_ArrowYellow );
   painter.drawPolygon( m_ArrowYellow );
   painter.fillPath( pathy , brush );

   painter.end();
}


