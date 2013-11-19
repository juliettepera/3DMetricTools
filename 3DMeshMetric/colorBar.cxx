#include "colorBar.h"

colorBar::colorBar( QWidget *Qparent ):QWidget( Qparent )
{
    m_Rect = QRect( QPoint( 0 , 0 ) , QPoint( 10 , 10 ) );

    m_Cyan.first = 0.4;
    m_Cyan.second = Qt::cyan;
    m_ItCyan = 0;

    m_Yellow.first = 0.6;
    m_Yellow.second = Qt::yellow;
    m_ItYellow = 0;

    m_Green.first = 0.5;
    m_Green.second = Qt::green;
    m_ItGreen = 0;
}


//*************************************************************************************************
void colorBar::setSize( QPoint topLeft , QPoint bottomRight )
{
    m_Rect = QRect( topLeft , bottomRight );
    m_Background = new QPixmap( m_Rect.size() );
}


//*************************************************************************************************
void colorBar::initGradientSigned()
{
    m_List.clear();

    m_List.push_back( QGradientStop( 0 , Qt::blue ) );
    m_List.push_back( m_Cyan );
    m_List.push_back( m_Green );
    m_List.push_back( m_Yellow );
    m_List.push_back( QGradientStop( 1 , Qt::red ) );

    m_Gradient.setStops( m_List );

    m_ItCyan = 1; m_ItGreen = 2 ; m_ItYellow = 3;
}

void colorBar::initGradientAbsolute()
{
    m_List.clear();

    m_List.push_back( QGradientStop( 0 , Qt::green ) );
    m_List.push_back( m_Yellow );
    m_List.push_back( QGradientStop( 1 , Qt::red ) );

    m_Gradient.setStops( m_List );

    m_ItYellow = 1;
}


//*************************************************************************************************
void colorBar::changeCyan( double NewPosition )
{
    m_Cyan.first = NewPosition;
    m_List.replace( m_ItCyan , m_Cyan );
}

void colorBar::changeYellow( double NewPosition )
{
   m_Yellow.first = NewPosition;
   m_List.replace( m_ItYellow , m_Yellow );
}

void colorBar::changeGreen()
{
    double NewPosition = (m_Yellow.first + m_Cyan.first)/2.0;
    m_Green.first = NewPosition;
    m_List.replace( m_ItGreen , m_Green );
}


//*************************************************************************************************
void colorBar::updateGradient()
{
    m_Gradient.setStops( m_List );
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

   painter.end();
}
