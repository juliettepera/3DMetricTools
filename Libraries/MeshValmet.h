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

#ifndef meshValmet_H
#define meshValmet_H

// Vtk Libraries
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkIdTypeArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataNormals.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkDoubleArray.h>
#include <vtkColorTransferFunction.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <vtkCleanPolyData.h>

// Other Libraries
#include <iostream>
#include <string>

// MeshValmet Libraries
#include "MeshValmet/mesh_run.h"

class meshValmet
{

    public:
        meshValmet();

        void SetData1( vtkSmartPointer <vtkPolyData> Data1 );
        void SetData2( vtkSmartPointer <vtkPolyData> Data2 );
        void SetSamplingStep( double SamplingStep );
        void SetMinSampleFrequency( int MinSampleFrequency );
        void SetSignedDistance( bool SignedDistance );
        void SetMin( double Min );
        void SetMax( double Max );
        void SetDelta( double Delta );
        void SetCenter( double Center );

        vtkSmartPointer <vtkPolyData> GetFinalData();
        vtkSmartPointer <vtkColorTransferFunction> GetLut();
        double GetMin();
        double GetMax();
        double GetDelta();
        double GetCenter();

        void CalculateError();
        friend void mesh_run(const args *args,
                             model_error *model1, model_error *model2,
                             outbuf *out, prog_reporter *progress,
                             dist_surf_surf_stats *stats, dist_surf_surf_stats *stats_rev,
                             double *abs_sampling_step, double *abs_sampling_dens);
        void drawVertexErrorTSigned();
        void drawVertexErrorTAbsolute();
        void CreateLutError();
        int testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData );

    private:
        vtkSmartPointer <vtkPolyData> m_FinalData;
        vtkSmartPointer <vtkColorTransferFunction> m_Lut;

        struct args m_Pargs;

        struct model_error m_ModelError1;
        struct model_error m_ModelError2;

        struct outbuf *m_Out;

        struct dist_surf_surf_stats m_Stats;
        struct dist_surf_surf_stats m_StatsRev;

        double m_AbsSamplingStep;
        double m_AbsSamplingDens;

        double m_DiceCoefficient[1];
        double m_IntUnionRatio[1];

        double m_Dmax;
        double m_Dmin;
        double m_Center;
        double m_Delta;

        double m_Downsampling;


        struct look
        {
           float R;
           float G;
           float B;
        };

        look m_Lookuptable[256];

};

#endif

