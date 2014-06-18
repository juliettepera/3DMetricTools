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

#ifndef PROCESSING_H
#define PROCESSING_H

// QT libraries
#include <QMessageBox>

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkColorTransferFunction.h>
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSTLWriter.h>
#include <vtkDataArray.h>
#include <vtkContourFilter.h>

// My libraries
#include "dataM.h"
#include "MeshValmet.h"

// OTHER libraries
#include <string>
#include <iostream>
#include <vector>
#include "vtkOBJWriter.h"

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
        double CheckForDelta( double Rmax , double Rmin );

        /* save the file
         */
        int SaveFile(std::string Name , dataM &Data1 , int format);

        /* update the color map
         */
        void updateColor(dataM &Data1);

        /* function for testing equality between two datas
         */
        int testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData );



    private:
        meshValmet m_MyMeshValmet;
};

#endif
