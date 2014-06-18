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

#ifndef DATAM_H
#define DATAM_H

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkOBJReader.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkDecimatePro.h>
#include <vtkColorTransferFunction.h>
#include <vtkProperty.h>
#include <vtkTriangleFilter.h>
#include <vtkAbstractArray.h>
#include <vtkCleanPolyData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

// QT libraries
#include <QMessageBox>

// OTHER libraries
#include <string>
#include <iostream>
#include <vector>

class dataM
{
    public:
        dataM( std::string Name = "" );

        /* set and get the name of the file
         */
        void setName( std::string Name );
        std::string getName();

        /* set and get the type of the file
         */
        void setTypeFile( int type );
        int getTypeFile();

        /* set and get the polydata file
         */
        void setPolyData(vtkSmartPointer<vtkPolyData> OldData );
        vtkSmartPointer<vtkPolyData> getPolyData();

        /* get the mapper and actor used to display the polydata
         */
        vtkSmartPointer <vtkPolyDataMapper> getMapper();
        vtkSmartPointer <vtkActor> getActor();

        /* Set and get the different properties of the data
         */
        void setOpacity( double Opacity );
        void setColor( double Red , double Green , double Blue );
        void setType( int Type );

        double getOpacity();
        double getRed();
        double getBlue();
        double getGreen();
        int getType();

        /* set and get the parameters for the distance computation
         */
        void setNameB( std::string NameB );
        void setMinSamplingFrequency(int MinSamplingFrequency );
        void setSamplingStep( double SamplingStep );
        void setTypeDistance(int TypeDistance );
        void setDisplayError( bool DisplayError );
        void setColorBar( bool ColorBar );

        std::string getNameB();
        int getMinSamplingFrequency();
        double getSamplingStep();
        int getTypeDistance();
        bool getDisplayError();
        bool getColorBar();

        /* set and get the parameters used to compute the color bar
         */
        void setLut( vtkSmartPointer <vtkColorTransferFunction> Lut );
        void setMin( double Dmin );
        void setMax( double Dmax );
        void setCenter( double Center );
        void setDelta( double Delta );

        vtkSmartPointer <vtkColorTransferFunction> getLut();
        double getMin();
        double getMax();
        double getCenter();
        double getDelta();

        /* Read the file
         * Pass by a cleaner and triangler filter
         * Then connect to the mapper and actor
         */
        void initialization();

        /* Update the actor properties
         * the opacity and color
         */
        void updateActorProperties();

        /* update the type of display
         * surface, triangle and points
         */
        int updateTypeOfDisplay();

        /* Change the polydata input of the mapper
         */
        int changeMapperInput();

        /* change the scalar use to display the color map
         */
        void changeActivScalar();

        /* Get the Id of a point by knowing is coordinates
         */
        vtkIdType getIdPointClicked( double ClickedPosition[3] );

        /* Get the Scalar value of a point by knowing is Id
         */
        QString getScalarValue( vtkIdType MyId );

    private:
        std::string m_Name;
        int m_TypeFile; //1=vtk 2=obj 3=stl
        vtkSmartPointer <vtkPolyData> m_PolyData;
        vtkSmartPointer <vtkPolyDataMapper> m_Mapper;
        vtkSmartPointer <vtkActor> m_Actor;

        double m_Opacity;
        double m_Red;
        double m_Blue;
        double m_Green;
        int m_Type;

        std::string m_NameB;
        int m_MinSamplingFrequency;
        double m_SamplingStep;
        int m_TypeDistance; // 0=absolute 1=signed 2=correspondant
        bool m_DisplayError;
        bool m_ColorBar;

        vtkSmartPointer <vtkColorTransferFunction> m_Lut;
        double m_Dmin;
        double m_Dmax;
        double m_Center;
        double m_Delta;
};

#endif
