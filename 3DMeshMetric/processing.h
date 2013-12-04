#ifndef PROCESSING_H
#define PROCESSING_H

// QT libraries
#include <QMessageBox>

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkColorTransferFunction.h>
#include <vtkPolyDataWriter.h>
#include <vtkDataArray.h>
#include <vtkContourFilter.h>

// My libraries
#include "dataM.h"
#include "MeshValmet.h"

// OTHER libraries
#include <string>
#include <iostream>
#include <vector>

class processing
{
    public:
        processing();

        /* computing the smoothing
         */
        vtkSmartPointer <vtkPolyData> processSmoothing(vtkSmartPointer<vtkPolyData> polyData , int nbIteration );

        /* computing the down sampling
         */
        vtkSmartPointer <vtkPolyData> processDownSampling( vtkSmartPointer <vtkPolyData> polyData , double nbDecimate );

        /* computing the distance
         */
        int processError(dataM &Data1 , dataM &Data2 );

        /* search if there is an error in the file
         */
        int CheckPreviousError( dataM &Data1 );

        /* save the file
         */
        void SaveFile( std::string Name , dataM &Data1 );

        /* update the color map
         */
        void updateColor(double Min , double Max, double Center, double Delta , dataM &Data1);

        /* function for testing equality between two datas
         */
        int testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData );



    private:
        meshValmet m_MyMeshValmet;
};

#endif
