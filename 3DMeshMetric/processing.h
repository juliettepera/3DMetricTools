#ifndef PROCESSING_H
#define PROCESSING_H

/* CLASS WHERE ALL THE PROCESS IS DONE THANK TO THE DATA STOCKED ON DATA
 *  -> DO THE SMOOTHING
 *  -> DO THE DOWNSAMPLING
 *  -> DO THE ERROR
 */

// My libraries
#include "dataM.h"
#include "meshValmet.h"

// QT libraries
#include <QMessageBox>

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkColorTransferFunction.h>
#include <vtkPolyDataWriter.h>
#include <vtkDataArray.h>

// OTHER libraries
#include <string>
#include <iostream>
#include <vector>

class processing
{
    public:
        processing();
        vtkSmartPointer <vtkPolyData> processSmoothing(vtkSmartPointer<vtkPolyData> polyData , int nbIteration );
        vtkSmartPointer <vtkPolyData> processDownSampling( vtkSmartPointer <vtkPolyData> polyData , double nbDecimate );
        int processError(dataM &Data1 , dataM &Data2 );
        void updateColor(double Min , double Max, double Delta , dataM &Data1);
        int testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData );
        void SaveFile( std::string Name , dataM &Data1 );
        int CheckPreviousError( dataM &Data1 );

    private:
        meshValmet m_MyMeshValmet;
};

#endif
