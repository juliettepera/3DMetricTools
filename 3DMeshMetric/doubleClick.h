#ifndef DOUBLECLICK_H
#define DOUBLECLICK_H

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

class MouseInteractorStyleDoubleClick : public vtkInteractorStyleTrackballCamera
{
   public:
        static MouseInteractorStyleDoubleClick* New();
        vtkTypeMacro( MouseInteractorStyleDoubleClick , vtkInteractorStyleTrackballCamera );

        MouseInteractorStyleDoubleClick();

        void SetRenderer( vtkSmartPointer <vtkRenderer> Renderer );
        void SetPolydata( vtkSmartPointer <vtkPolyData> Polydata );
        vtkIdType GetIdMatch();
        void OnLeftButtonDown();

    private:
        int m_NumberOfClicks;
        int m_ClickedPosition[2];
        double m_PickedPosition[3];
        vtkIdType m_IdPoint;
        vtkSmartPointer <vtkRenderer> m_Renderer;
        vtkSmartPointer<vtkPolyData> m_Polydata;
};



#endif
