#include "doubleClick.h"
 
MouseInteractorStyleDoubleClick::MouseInteractorStyleDoubleClick()
{
    m_NumberOfClicks = 0;

    m_ClickedPosition[0] = 0;
    m_ClickedPosition[1] = 0;
    m_PickedPosition[0] = 0;
    m_PickedPosition[1] = 0;
    m_PickedPosition[2] = 0;

    m_IdPoint = -1;

    m_Renderer = vtkSmartPointer <vtkRenderer>::New();
    m_Polydata = vtkSmartPointer <vtkPolyData>::New();
}

void MouseInteractorStyleDoubleClick::SetRenderer( vtkSmartPointer<vtkRenderer> Renderer )
{
    m_Renderer = Renderer;
}

void MouseInteractorStyleDoubleClick::SetPolydata( vtkSmartPointer <vtkPolyData> Polydata)
{
    m_Polydata = Polydata;
}

vtkIdType MouseInteractorStyleDoubleClick::GetIdMatch()
{
    return m_IdPoint;
}

 
void MouseInteractorStyleDoubleClick::OnLeftButtonDown()
{
   m_NumberOfClicks++;

   if( m_NumberOfClicks == 2)
   {
      m_NumberOfClicks = 0;

      int pickPosition[2];
      GetInteractor()->GetEventPosition( pickPosition );
      m_ClickedPosition[0] = pickPosition[0];
      m_ClickedPosition[1] = pickPosition[1];

      GetInteractor()->GetPicker()->Pick( m_ClickedPosition[0] , m_ClickedPosition[1] , 0 , m_Renderer );
      GetInteractor()->GetPicker()->GetPickPosition( m_PickedPosition );

      //std::cout << "Picked position : ( " << m_PickedPosition[0] << " , " << m_PickedPosition[1] << " , " << m_PickedPosition[2] << " )" << std::endl;

      std::vector <vtkIdType> IdPicked;
      double delta = 0.5;

      for( vtkIdType i = 0 ; i < m_Polydata->GetNumberOfPoints() ; i++ )
      {
        double Point[3];
        m_Polydata -> GetPoints() -> GetPoint( i , Point );

        double delta0 = fabs( Point[0]-m_PickedPosition[0] );
        double delta1 = fabs( Point[1]-m_PickedPosition[1] );
        double delta2 = fabs( Point[2]-m_PickedPosition[2] );

        if( delta0 < delta && delta1 < delta && delta2 < delta )
        {
            IdPicked.push_back( i );
        }
      }

      if( IdPicked.empty() == false )
      {
          double delta = 0.5;
          while( IdPicked.size() != 1 )
          {
              for( int i = 0 ; i < IdPicked.size() ; i++ )
              {
                  double Point[3];
                  m_Polydata -> GetPoints() -> GetPoint( IdPicked.at( i ) , Point );

                  double delta0 = fabs( Point[0]-m_PickedPosition[0] );
                  double delta1 = fabs( Point[1]-m_PickedPosition[1] );
                  double delta2 = fabs( Point[2]-m_PickedPosition[2] );

                  if( !( delta0 < delta && delta1 < delta && delta2 < delta ) )
                  {
                      IdPicked.erase( IdPicked.begin() + i );
                  }
              }
              delta = delta - 0.05;
          }
          m_IdPoint = IdPicked.at(0);
          //std::cout << "Point Id : " << m_IdPoint << std::endl;
      }
      /*else
      {
          std::cout << "Not a point" << std::endl;
      }*/

      m_Renderer->GetRenderWindow()->GetInteractor()->TerminateApp();
   }


   // forward events
   vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}
 

