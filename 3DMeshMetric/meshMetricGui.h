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

// VTK libraries
#include <QVTKWidget.h>

// My libraries
#include "dataM.h"
#include "processing.h"
#include "display.h"
#include "ui_meshMetricGui.h"

// Other Libraries
#include <string>
#include <iostream>


class meshMetricGui : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT

    public:
    meshMetricGui(QWidget * parent = 0 , Qt::WFlags f = 0 , QString path = "" );

        // functions for the icons
            void InitIcon();

            void DisplayInit();

            //void resizeEvent( QResizeEvent *Qevent );
            //void resizeWidgetInArea();

    public slots:

        // functions for loadind/saving files
            void OpenBrowseWindowFile();
            void OpenBrowseWindowRepository();
            void SaveFile();

        // functions for delete files
            void DeleteOneFile();
            void DeleteAllFiles();

        // functions for display files
            void DisplayAll();
            void HideAll();

            void ChooseColorBackground();

        // functions to change the camera
            void buttonFrontClicked();
            void buttonBackClicked();
            void buttonRightClicked();
            void buttonLeftClicked();
            void buttonUpClicked();
            void buttonDownClicked();

        // functions to change files properties
            void ChangeMeshSelected();
            void UpdateDisplayedMesh( QListWidgetItem* itemClicked );
            void ResetSelectedFile();
            void ChangeValueOpacity();
            void ChooseColor();
            void ChangeTypeOfDisplay();

        // functions for the smoothing
            void ChangeNumberOfIteration();
            void ApplySmoothing();

        // functions for the down sampling
            void ChangeDecimate();
            void ApplyDecimate();

        // function for the distance
            void AvailableMesh();
            void SelectMeshB();

            void ChangeMinSampleFrequency();
            void ChangeSamplingStep();
            void ChangeSignedDistance();
            void ChangeValueMin();
            void ChangeValueMax();
            void ChangeValueDelta();

            void ChangeDisplayError();
            void ChangeDisplayColorBar();

            void ApplyDistance();
            void UpdateColor();

            void PreviousError();

    private:
        // attributs for the files
            int m_NumberOfMesh;
            int m_MeshSelected;
            std::vector <dataM::dataM> m_DataList;
            QColor m_Color;
            QColor m_ColorBg;

        // attributs for the display
            int m_CameraX;
            int m_CameraY;
            int m_CameraZ;
            int m_NumberOfDisplay;
            QVTKWidget *m_WidgetMesh;
            display m_MyWindowMesh;

        // attributs for the icons
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

        // attributs for the smoothing, down sampling , error
            int m_SelectedItemA;
            int m_SelectedItemB;
            processing m_MyProcess;
            int m_nbIteration;
            double m_nbDecimate;
            std::vector <bool> m_ErrorComputed;
            std::vector <bool> m_Visibility;

            double m_Min;
            double m_Max;
            double m_Delta;
};

#endif
