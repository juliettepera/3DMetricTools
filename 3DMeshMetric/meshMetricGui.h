#ifndef MESHMETRICGUI_H
#define MESHMETRICGUI_H

//Qt libraries
#include <QString>
#include <QIcon>
#include <QStringList>
#include <QLineEdit>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QFileDialog>
#include <QColorDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QCheckBox>
#include <QStyle>
#include <QPalette>
#include <QResizeEvent>
#include <QLinearGradient>

// VTK libraries
#include <QVTKWidget.h>

// My libraries
#include "dataM.h"
#include "processing.h"
#include "display.h"
#include "colorBar.h"
#include "ui_meshMetricGui.h"

// Other Libraries
#include <string>
#include <iostream>
#include <math.h>

#ifndef MESHMETRIC3D_VERSION
#define MESHMETRIC3D_VERSION "unknown"
#endif

#ifndef MESHMETRIC3D_CONTRIBUTOR
#define MESHMETRIC3D_CONTRIBUTOR "unknown"
#endif

#ifndef MESHMETRIC3D_DOCUMENTATION
#define MESHMETRIC3D_DOCUMENTATION "unknown"
#endif
class meshMetricGui : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT

    public:
            meshMetricGui(QWidget * Parent = 0 , Qt::WFlags f = 0 , QString path = "" );

            void InitIcon(); // init all the icons
            void DisplayInit(); // display the files when they are loaded
            void DeleteOneFile();
            void DeleteAllFiles();
            void ResetSelectedFile();
            void resizeEvent( QResizeEvent *Qevent ); // resize the window
            void PreviousError(); // check if there is a previous error loaded
            double calculNewY( double X , double Min , double Max ); // calcul the new position when delta change

    public slots:

            void MeshMetric();

            /* Load a file or repository
             * Save a file
             * Delete one or all the files
             */
            void OpenBrowseWindowFile();
            void OpenBrowseWindowRepository();
            QString SaveFile();
            void DeleteBoxOne();
            void DeleteBoxAll();

            /* display or not the file links to the check box eye
             * change the selected mesh
             */
            void UpdateDisplayedMesh( QListWidgetItem* itemClicked );
            void ChangeMeshSelected();

            /* Change the parameters of a file
             * Reload to the original a file
             * Hide all files or display all files
             * Change the background color
             */
            void ChangeValueOpacity();
            void ChooseColor();
            void ChangeTypeOfDisplay();
            void ReloadOne();
            void DisplayAll();
            void HideAll();
            void ChooseColorBackground();

            /* Change the position of the camera
             */
            void buttonAntClicked();
            void buttonPostClicked();
            void buttonRightClicked();
            void buttonLeftClicked();
            void buttonSupClicked();
            void buttonInfClicked();

            /* Change the parameter for the smoothing
             * Compute the smoothing
             */
            void ChangeNumberOfIteration();
            void ApplySmoothing();

            /* Change the parameter for the down sampling
             * Compute the down-sampling
             */
            void ChangeDecimate();
            void ApplyDecimate();

            /* List the available file for the distance computation
             * file selected for the computation
             * Change the parameters for the distance computation
             * Compute the distance
             * Display or not the error
             */
            void AvailableMesh();
            void SelectMeshB();
            void ChangeMinSampleFrequency();
            void ChangeSamplingStep();
            void ChangeSignedDistance();
            void ApplyDistance();
            void ChangeDisplayError();

            /* Change the parameters for the color map
             * Update the color map
             * Display or not the color map
             */
            void ChangeValueMin();
            void ChangeValueMax();
            void ChangeValueDelta();
            void ChangeValueCenter();
            void UpdateColor();
            void ChangeDisplayColorBar();

    private:
            // attibuts for the icons
            std::string m_Path;
            QIcon m_VisibleIcon;
            QIcon m_UnvisibleIcon;
            QIcon m_OkIcon;
            QIcon m_NotOkIcon;
            QIcon m_PlusIcon;
            QIcon m_MinusIcon;
            QIcon m_DeleteIcon;
            QIcon m_DisplayIcon;
            QIcon m_ResetIcon;
            QString m_Visible;
            QString m_Unvisible;
            QString m_Ok;
            QString m_NotOk;
            QString m_Plus;
            QString m_Minus;
            QString m_Delete;
            QString m_Display;
            QString m_Reset;

            // attibuts for the display
            int m_NumberOfDisplay;
            QVTKWidget *m_WidgetMesh;
            display m_MyWindowMesh;
            QColor m_ColorBg;

            // attributs for the lists of file
            int m_NumberOfMesh;
            int m_MeshSelected;
            std::vector <dataM::dataM> m_DataList;
            QColor m_Color;

            // attributs for the camera
            int m_CameraX;
            int m_CameraY;
            int m_CameraZ;

            //attributs for the filters
            processing m_MyProcess;
            int m_nbIteration;
            double m_nbDecimate;

            // attibuts for the distance computation
            int m_SelectedItemA;
            int m_SelectedItemB;
            std::vector <bool> m_ErrorComputed;
            std::vector <bool> m_Visibility;
};

#endif
