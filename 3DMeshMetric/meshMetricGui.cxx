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

#include "meshMetricGui.h"

meshMetricGui::meshMetricGui( QWidget *Parent , Qt::WFlags f , QString path )
{
    setupUi(this);

    // initialisation of the attributs
    m_NumberOfMesh = 0;
    m_MeshSelected = -1;
    m_CameraX = 0 ;
    m_CameraY = 1;
    m_CameraZ = 0;
    m_NumberOfDisplay = 0;
    m_SelectedItemA = -1;
    m_SelectedItemB = -1;
    m_nbDecimate = 0.1;
    m_nbIteration = 200;

    m_IdealHeightWindow = 722;
    m_IdealWidthWindow = 1017;

    m_WidgetMesh = new QVTKWidget( this -> scrollAreaMesh );
    m_WidgetMesh->resize( scrollAreaMesh->size() );
    m_MyWindowMesh.setSizeH( m_WidgetMesh->height() );
    m_MyWindowMesh.setSizeW( m_WidgetMesh->width() );
    m_MyWindowMesh.setMeshWidget( m_WidgetMesh );

    widgetColor->setSize( widgetColor->rect().topLeft() , widgetColor->rect().bottomRight() );

    lineEditCenter -> setValidator( new QDoubleValidator( this ) );
    lineEditDelta -> setValidator( new QDoubleValidator( this ) );
    lineEditMin -> setValidator( new QDoubleValidator( this ) );
    lineEditMax -> setValidator( new QDoubleValidator( this ) );

    m_Path = path.toStdString();
    InitIcon();

    // shortcuts
    actionAddNewFile -> setShortcut( QKeySequence("Ctrl+A") );
    actionAddNewRepository -> setShortcut( QKeySequence("Ctrl+R") );
    actionSaveFile -> setShortcut( QKeySequence("Ctrl+S") );
    actionQuit -> setShortcut( QKeySequence("Ctrl+Q") );

    QStringList List;
    List << "2" << "0.5" << "0.2" << "0.1" << "0.05" << "0.02" ;
    comboBoxSamplingStep -> addItems( List );
    comboBoxSamplingStep -> setCurrentIndex( 1 );

    comboBoxMeshB -> addItem( m_NotOkIcon , QString(" no file selected ") );
    comboBoxMeshB -> setCurrentIndex( 0 );
    QStandardItemModel* Model = qobject_cast< QStandardItemModel* >( comboBoxMeshB -> model() );
    QStandardItem* Item = Model -> item( 0 );
    Item -> setSelectable( false );

    radioButtonAbsoluteDistance->setEnabled( false );
    radioButtonSignedDistance->setEnabled( false );
    radioButtonCorrespondantDistance->setEnabled( false );

    // connections
    QObject::connect( actionQuit , SIGNAL( triggered() ) , qApp , SLOT( quit() ) );
    QObject::connect( action3DMeshMetric , SIGNAL( triggered() ) , this , SLOT( MeshMetric() ) );
    QObject::connect( actionAbout , SIGNAL( triggered() ) , this , SLOT( About() ) );

    QObject::connect( actionAddNewFile , SIGNAL( triggered() ) , this , SLOT( OpenBrowseWindowFile() ) );
    QObject::connect( pushButtonAdd , SIGNAL( clicked() ) , this , SLOT( OpenBrowseWindowFile() ) );
    QObject::connect( actionAddNewRepository , SIGNAL( triggered() ) , this , SLOT( OpenBrowseWindowRepository() ) );
    QObject::connect( actionSaveFile , SIGNAL( triggered() ) , this , SLOT( SaveFile() ) );
    QObject::connect( pushButtonDeleteOne , SIGNAL( clicked() ) , this , SLOT( DeleteBoxOne() ) );
    QObject::connect( pushButtonDelete , SIGNAL( clicked() ) , this , SLOT( DeleteBoxAll() ) );

    QObject::connect( listWidgetLoadedMesh , SIGNAL( itemClicked( QListWidgetItem* ) ) , this , SLOT( UpdateDisplayedMesh( QListWidgetItem* ) ) );

    QObject::connect( horizontalSliderOpacity , SIGNAL( sliderReleased() ), this, SLOT( ChangeValueOpacity() ) );
    QObject::connect( pushButtonColor , SIGNAL( clicked() ) , this , SLOT( ChooseColor() ) );
    QObject::connect( radioButtonSurface , SIGNAL( toggled( bool ) ) , this , SLOT( ChangeTypeOfDisplay() ) );
    QObject::connect( radioButtonPoints , SIGNAL( toggled( bool ) ) , this , SLOT( ChangeTypeOfDisplay() ) );
    QObject::connect( radioButtonWireframe , SIGNAL( toggled( bool ) ) , this , SLOT( ChangeTypeOfDisplay() ) );
    QObject::connect( pushButtonResetAll , SIGNAL( clicked() ) , this , SLOT( ReloadOne() ) );
    QObject::connect( pushButtonDisplayAll , SIGNAL( clicked() ) , this , SLOT( DisplayAll() ) );
    QObject::connect( pushButtonHideAll , SIGNAL( clicked() ) , this , SLOT( HideAll() ) );
    QObject::connect( actionBackgroundColor , SIGNAL( triggered() ) , this , SLOT( ChooseColorBackground() ) );

    QObject::connect( pushButtonFront , SIGNAL( clicked() ) , this , SLOT( buttonAntClicked() ) );
    QObject::connect( pushButtonBack , SIGNAL( clicked() ) , this , SLOT( buttonPostClicked() ) );
    QObject::connect( pushButtonRight , SIGNAL( clicked() ) , this , SLOT( buttonRightClicked() ) );
    QObject::connect( pushButtonLeft , SIGNAL( clicked() ) , this , SLOT( buttonLeftClicked() ) );
    QObject::connect( pushButtonUp , SIGNAL( clicked() ) , this , SLOT( buttonSupClicked() ) );
    QObject::connect( pushButtonDown , SIGNAL( clicked() ) , this , SLOT( buttonInfClicked() ) );

    QObject::connect( spinBoxIteration , SIGNAL( valueChanged( int ) ), this, SLOT( ChangeNumberOfIteration() ) );
    QObject::connect( pushButtonRunSmoothing , SIGNAL( clicked() ) , this , SLOT( ApplySmoothing() ) );

    QObject::connect( spinBoxDecimate , SIGNAL( valueChanged( int ) ), this, SLOT( ChangeDecimate() ) );
    QObject::connect( pushButtonRunDecimate , SIGNAL( clicked() ) , this , SLOT( ApplyDecimate() ) );

    QObject::connect( comboBoxMeshB , SIGNAL( activated( int ) ) , this , SLOT( SelectMeshB() ) );
    QObject::connect( spinBoxMinSampFreq , SIGNAL( valueChanged( int ) ), this, SLOT( ChangeMinSampleFrequency() ) );
    QObject::connect( comboBoxSamplingStep , SIGNAL( activated( int ) ), this, SLOT( ChangeSamplingStep() ) );
    QObject::connect( radioButtonSignedDistance , SIGNAL( toggled( bool ) ), this, SLOT( ChangeSignedDistance() ) );
    QObject::connect( radioButtonAbsoluteDistance , SIGNAL( toggled( bool ) ), this, SLOT( ChangeSignedDistance() ) );
    QObject::connect( radioButtonCorrespondantDistance , SIGNAL( toggled( bool ) ), this, SLOT( ChangeSignedDistance() ) );
    QObject::connect( pushButtonApply , SIGNAL( clicked() ) , this , SLOT( ApplyDistance() ) );
    QObject::connect( checkBoxError , SIGNAL( toggled( bool ) ) , this , SLOT( ChangeDisplayError() ) );
    QObject::connect( checkBoxColorBar , SIGNAL( toggled( bool ) ) , this , SLOT( HideColorBar() ) );

    QObject::connect( lineEditMin , SIGNAL( returnPressed() ) , this , SLOT( ChangeValueMin() ) );
    QObject::connect( lineEditMax , SIGNAL( returnPressed() ) , this , SLOT( ChangeValueMax() ) );
    QObject::connect( lineEditDelta , SIGNAL( returnPressed() ) , this , SLOT( ChangeValueDelta() ) );
    QObject::connect( lineEditCenter , SIGNAL( returnPressed() ) , this , SLOT( ChangeValueCenter() ) );
    QObject::connect( pushButtonRefresh , SIGNAL( clicked() ) , this , SLOT( RefreshColorBar() ) );

    QObject::connect( &m_MyWindowMesh , SIGNAL( positionPicked(double,double,double) ) , this , SLOT( GetValueByClicking(double,double,double) ) );

    QObject::connect( horizontalSliderCloudRadius , SIGNAL( sliderReleased() ) , this , SLOT( ChangeValueRadius() ) );
    QObject::connect( pushButtonColorCloud , SIGNAL( clicked() ) , this , SLOT( ChangeColorCloud() ) );
}


//*************************************************************************************************
void meshMetricGui::dragEnterEvent(QDragEnterEvent *Qevent)
{
    Qevent->accept();
}


//*************************************************************************************************
void meshMetricGui::dropEvent(QDropEvent* Qevent)
{
    const QMimeData* mimeData = Qevent->mimeData();
    if( mimeData->hasUrls() )
    {
        QList <QUrl> urlList = mimeData->urls();
        for( int i = 0 ; i < urlList.size() ; i++ )
        {
            QString filePath = urlList.at(i).toLocalFile();

            if( filePath.endsWith( ".vtk" ) && QFileInfo( filePath ).exists() )
            {
                m_DataList.push_back( filePath.toStdString() );
                m_DataList[ m_NumberOfMesh ].initialization();

                QFileInfo File = QFileInfo(filePath);
                listWidgetLoadedMesh -> addItem( File.fileName().toStdString().c_str() );

                QListWidgetItem* currentIndex = listWidgetLoadedMesh -> item( m_NumberOfMesh );
                currentIndex -> setFlags( listWidgetLoadedMesh -> item( m_NumberOfMesh ) -> flags() | Qt::ItemIsUserCheckable );
                currentIndex -> setCheckState( Qt::Checked );

                comboBoxMeshB -> addItem( m_NotOkIcon , File.fileName().toStdString().c_str() );

                m_MyWindowMesh.addData( m_DataList[ m_NumberOfMesh ].getActor() );

                m_ErrorComputed.push_back( false );
                m_Visibility.push_back( true );

                m_MeshSelected = m_NumberOfMesh;
                m_NumberOfMesh++;
                listWidgetLoadedMesh -> setCurrentRow( m_MeshSelected );

                PreviousError();
                DisplayInit();
                ChangeMeshSelected();
            }
        }
    }
}


//*************************************************************************************************
void meshMetricGui::InitIcon()
{
    std::string visible = m_Path;
    visible += "/icons/visible.png";

    std::string unvisible = m_Path;
    unvisible += "/icons/unvisible.png";

    std::string ok = m_Path;
    ok += "/icons/ok.png";

    std::string notok = m_Path;
    notok += "/icons/NotOk.png";

    std::string plus = m_Path;
    plus += "/icons/plus.png";

    std::string minus = m_Path;
    minus += "/icons/minus.png";

    std::string deleteAll = m_Path;
    deleteAll += "/icons/deleteAll.png";

    std::string display = m_Path;
    display += "/icons/display.png";

    std::string reset = m_Path;
    reset += "/icons/reset.png";

    std::string minBubble = m_Path;
    minBubble += "/icons/miniBubble.png";

    std::string maxBubble = m_Path;
    maxBubble += "/icons/maxiBubble.png";

    m_Visible = QString::fromStdString( visible );
    m_Unvisible = QString::fromStdString( unvisible );
    m_Ok = QString::fromStdString( ok );
    m_NotOk = QString::fromStdString( notok );
    m_Plus = QString::fromStdString( plus );
    m_Minus = QString::fromStdString( minus );
    m_Delete = QString::fromStdString( deleteAll );
    m_Display = QString::fromStdString( display );
    m_Reset = QString::fromStdString( reset );
    m_MiniBubble = QString::fromStdString( minBubble );
    m_MaxiBubble = QString::fromStdString( maxBubble );

    m_VisibleIcon = QIcon( m_Visible );
    m_UnvisibleIcon = QIcon( m_Unvisible );
    m_OkIcon = QIcon( m_Ok );
    m_NotOkIcon = QIcon( m_NotOk );
    m_PlusIcon = QIcon( m_Plus );
    m_MinusIcon = QIcon( m_Minus );
    m_DeleteIcon = QIcon( m_Delete );
    m_DisplayIcon = QIcon( m_Display );
    m_ResetIcon = QIcon( m_Reset );

    pushButtonAdd -> setIcon( m_PlusIcon );
    pushButtonDeleteOne -> setIcon( m_MinusIcon );
    pushButtonDelete -> setIcon( m_DeleteIcon );
    pushButtonDisplayAll -> setIcon( m_VisibleIcon );
    pushButtonHideAll -> setIcon( m_UnvisibleIcon );
    pushButtonResetAll -> setIcon( m_ResetIcon );

    // style sheet eyes icon
    std::string styleSheetLoadedMesh = "QListWidget::indicator:checked{ image: url(";
    styleSheetLoadedMesh += m_Path;
    styleSheetLoadedMesh += "/icons/visible.png);} QListWidget::indicator:unchecked{ image: url(";
    styleSheetLoadedMesh += m_Path;
    styleSheetLoadedMesh += "/icons/unvisible.png);}";
    listWidgetLoadedMesh -> setStyleSheet( QString::fromStdString( styleSheetLoadedMesh ) );

    labelMiniBubble->setPixmap( m_MiniBubble );
    labelMaxiBubble->setPixmap( m_MaxiBubble );
}


//*************************************************************************************************
void meshMetricGui::DisplayInit()
{
    if( m_NumberOfDisplay == 0 )
    {
       scrollAreaMesh->setEnabled( true );
    }

    m_MyWindowMesh.initWindow();
    m_MyWindowMesh.updateWindow();
    m_NumberOfDisplay++;

    groupBoxCamera -> setEnabled( true );
    listWidgetLoadedMesh -> setEnabled( true );
    pushButtonDelete -> setEnabled( true );
    pushButtonDisplayAll -> setEnabled( true );
    pushButtonHideAll -> setEnabled( true );
    actionBackgroundColor -> setEnabled( true );
}


//*************************************************************************************************
void meshMetricGui::DeleteOneFile()
{
    if( !m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_DataList.erase( m_DataList.begin() + m_MeshSelected );
        m_ErrorComputed.erase( m_ErrorComputed.begin() + m_MeshSelected );
        m_Visibility.erase( m_Visibility.begin() + m_MeshSelected );

        delete listWidgetLoadedMesh->item( m_MeshSelected );
        comboBoxMeshB -> removeItem( m_MeshSelected + 1 );

        m_MyWindowMesh.deleteData( m_MeshSelected );

        m_NumberOfMesh--;
        m_MeshSelected = 0;

        horizontalSliderOpacity -> setValue( m_DataList[ m_MeshSelected ].getOpacity()/100.0 );
        lcdNumberOpacity -> display( m_DataList[ m_MeshSelected ].getOpacity() );
        spinBoxIteration -> setValue( 200 );
        spinBoxDecimate -> setValue( 10 );

        if( m_DataList[ m_MeshSelected ].getType() == 1 )
        {
            radioButtonSurface -> setChecked( true );
        }
        else if( m_DataList[ m_MeshSelected ].getType() == 2 )
        {
            radioButtonPoints -> setChecked( true );
        }
        else if( m_DataList[ m_MeshSelected ].getType() == 3 )
        {
            radioButtonWireframe -> setChecked( true );
        }

        checkBoxError -> setChecked( m_ErrorComputed[ m_MeshSelected ] );

        if( checkBoxError->isChecked() == true )
        {
            checkBoxColorBar->setChecked( true );
            checkBoxColorBar->setEnabled( true );
            m_DataList[ m_MeshSelected ].setColorBar( true );
            m_DataList[ m_MeshSelected ].setDisplayError( true );
        }
        else
        {
            checkBoxColorBar->setChecked( false );
            checkBoxColorBar->setEnabled( false );
            m_DataList[ m_MeshSelected ].setColorBar( false );
            m_DataList[ m_MeshSelected ].setDisplayError( false );
        }

        lineEditMeshA -> clear();
        lineEditNameMesh -> clear();
        lineEditClickValue -> clear();

        tabWidgetVisualization -> setEnabled( false );
        tabWidgetError -> setEnabled( false );
        pushButtonDeleteOne -> setEnabled( false );
        actionSaveFile -> setEnabled( false );

        m_MyWindowMesh.EraseCloud();

    }

    if( m_DataList.empty() )
    {
        groupBoxCamera -> setEnabled( false );
        tabWidgetVisualization -> setEnabled( false );
        tabWidgetError -> setEnabled( false );
        tabWidgetError->setCurrentIndex(0);
        pushButtonDeleteOne -> setEnabled( false );
        pushButtonDelete -> setEnabled( false );
        pushButtonDisplayAll -> setEnabled( false );
        pushButtonHideAll -> setEnabled( false );
        listWidgetLoadedMesh -> setEnabled( false );
        actionSaveFile -> setEnabled( false );
        checkBoxError -> setChecked( false );
        checkBoxColorBar->setChecked( false );
        pushButtonAdd -> setEnabled( true );
        actionAddNewFile -> setEnabled( true );
        actionAddNewRepository -> setEnabled( true );

        m_MeshSelected = -1;
        m_MyWindowMesh.updateLut( 0 );

     }
}


//*************************************************************************************************
void meshMetricGui::DeleteAllFiles()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 )
    {
        m_MyWindowMesh.deleteAll();

        m_NumberOfDisplay = 0;
        m_NumberOfMesh = 0;
        m_MeshSelected = -1;
        m_SelectedItemA = -1;
        m_SelectedItemB = -1;

        horizontalSliderOpacity -> setSliderPosition( horizontalSliderOpacity -> maximum() );
        lcdNumberOpacity -> display( horizontalSliderOpacity -> maximum()  );
        spinBoxIteration -> setValue( 200 );
        spinBoxDecimate -> setValue( 10 );
        radioButtonPoints -> setChecked( true );
        checkBoxError -> setChecked( false );
        checkBoxColorBar->setChecked( false );
        listWidgetLoadedMesh -> clear();
        comboBoxMeshB -> clear();
        comboBoxMeshB -> addItem( m_NotOkIcon , QString(" no file selected ") );
        comboBoxMeshB -> setCurrentIndex( 0 );
        lineEditMeshA -> clear();
        lineEditNameMesh -> clear();
        lineEditClickValue -> clear();
        m_DataList.clear();
        m_ErrorComputed.clear();
        m_Visibility.clear();

        radioButtonAbsoluteDistance->setEnabled( false );
        radioButtonSignedDistance->setEnabled( false );
        radioButtonCorrespondantDistance->setEnabled( false );

        m_MyWindowMesh.updateLut( 0 );
        groupBoxCamera -> setEnabled( false );
        tabWidgetVisualization -> setEnabled( false );
        tabWidgetError -> setEnabled( false );
        tabWidgetError->setCurrentIndex(0);
        pushButtonDeleteOne -> setEnabled( false );
        pushButtonDelete -> setEnabled( false );
        pushButtonDisplayAll -> setEnabled( false );
        pushButtonHideAll -> setEnabled( false );
        listWidgetLoadedMesh -> setEnabled( false );
        actionSaveFile -> setEnabled( false );

        pushButtonAdd -> setEnabled( true );
        actionAddNewFile -> setEnabled( true );
        actionAddNewRepository -> setEnabled( true );
    }
}

//*************************************************************************************************
void meshMetricGui::ResetSelectedFile()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_DataList[ m_MeshSelected ].setOpacity( 1.0 );
        m_DataList[ m_MeshSelected ].initialization();
        m_MyWindowMesh.updateWindow();
    }

    lcdNumberOpacity -> display( m_DataList[ m_MeshSelected ].getOpacity() );
    horizontalSliderOpacity -> setValue( 100 );
    m_Visibility[ m_MeshSelected ] = true;

    listWidgetLoadedMesh -> item( m_MeshSelected ) -> setCheckState( Qt::Checked );

    if( m_MyProcess.CheckPreviousError( m_DataList[ m_MeshSelected ] ) == 31 || m_MyProcess.CheckPreviousError( m_DataList[ m_MeshSelected ] ) == 32 || m_MyProcess.CheckPreviousError( m_DataList[ m_MeshSelected ] ) == 33)
    {
        m_ErrorComputed[ m_MeshSelected ] = true;
        tabWidgetError -> setCurrentWidget( tabResults );
    }
    else
    {
        m_ErrorComputed[ m_MeshSelected ] = false;
        tabWidgetError -> setCurrentWidget( tabDistance );
    }
    checkBoxError -> setEnabled( m_ErrorComputed[ m_MeshSelected ] );
    checkBoxError -> setChecked( false );

    ChangeMeshSelected();
}


//*************************************************************************************************
void meshMetricGui::resizeEvent( QResizeEvent *Qevent )
{
     scrollAreaMesh->resize( scrollAreaMesh->minimumWidth() + ( Qevent->size().width() - m_IdealWidthWindow ) ,  scrollAreaMesh->minimumHeight() + ( Qevent->size().height() - m_IdealHeightWindow ) );
     m_WidgetMesh->resize( scrollAreaMesh->size() );
     m_MyWindowMesh.setSizeH( m_WidgetMesh->height() );
     m_MyWindowMesh.setSizeW( m_WidgetMesh->width() );

     if( Qevent->size().width() < m_IdealWidthWindow || Qevent->size().height() < m_IdealHeightWindow )
     {
         //to do if I want a smaller size
     }
}


//*************************************************************************************************
void meshMetricGui::PreviousError()
{
    if( ! m_DataList.empty() && m_MeshSelected != -1 )
    {
        int out = m_MyProcess.CheckPreviousError( m_DataList[ m_MeshSelected ] );
        QMessageBox MsgBox;
        QFileInfo File;

        /*if( out == 1 )
        {
            MsgBox.setText( " the original scalar is missing ");
            MsgBox.exec();
        }
        else if( out == 2 )
        {
            MsgBox.setText( " the error scalar is missing ");
            MsgBox.exec();
        }
        else*/ if( out == 3 || out == 4 )
        {
            MsgBox.setText( " problem ");
            MsgBox.exec();
        }
        else if( out != -1 ) // diff from no previous error
        {
            if( out == 31 || out == 32 || out == 33 )
            {
                m_ErrorComputed[ m_MeshSelected ] = true;
                m_DataList[ m_MeshSelected ].setDisplayError( true );

                m_MyProcess.updateColor( m_DataList[ m_MeshSelected ] );

                File = QString::fromStdString( m_DataList[ m_MeshSelected ].getName() );
                lineEditA -> setText( File.fileName() );
                lineEditB -> setText( QString::fromStdString( " Unknown " ) );
                m_DataList[ m_MeshSelected ].setNameB( "Unknown" );
                lineEditNbPointsA->setText( "-" );
                lineEditNbPointsB->setText( "-" );

                lineEditMinR -> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
                lineEditMaxR -> setText( QString::number( m_DataList[ m_MeshSelected ].getMax() ) );

                lineEditMin -> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
                lineEditMax -> setText( QString::number( m_DataList[ m_MeshSelected ].getMax() ) );
                lineEditCenter -> setText( QString::number( m_DataList[ m_MeshSelected ].getCenter() ) );
                lineEditDelta -> setText( QString::number( m_DataList[ m_MeshSelected ].getDelta() ) );

                 m_MyWindowMesh.setLut( m_DataList[ m_MeshSelected ].getMapper()->GetLookupTable() , "File "+QString::number( m_MeshSelected + 1 ).toStdString() );
                 m_MyWindowMesh.updateLut( 1 );

                if( out == 31 )
                {
                    double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getCenter() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
                    double DeltaC = calculNewY( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
                    double Center = calculNewY( m_DataList[ m_MeshSelected ].getCenter() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );

                    widgetColor->updateGradientSigned( DeltaC , DeltaY , Center );
                    lineEditType -> setText( QString( "Signed distance" ) );
                    lineEditMin -> setReadOnly( false );
                    lineEditMin -> setPalette( lineEditMax->palette() );
                    lineEditCenter -> setReadOnly( false );
                    lineEditCenter -> setPalette( lineEditMax->palette() );
                }
                else if( out == 32 )
                {
                    double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );

                    widgetColor->updateGradientAbsolute( DeltaY );
                    lineEditType -> setText( QString( "Absolute distance" ) );
                    lineEditMin -> setReadOnly( true );
                    lineEditMin -> setPalette( lineEditA->palette() );
                    lineEditCenter -> setReadOnly( true );
                    lineEditCenter -> setPalette( lineEditA->palette() );
                }
                else if( out == 33 )
                {
                    double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );

                    widgetColor->updateGradientAbsolute( DeltaY );
                    lineEditType -> setText( QString( "Correspondant" ) );
                    lineEditMin -> setReadOnly( true );
                    lineEditMin -> setPalette( lineEditA->palette() );
                    lineEditCenter -> setReadOnly( true );
                    lineEditCenter -> setPalette( lineEditA->palette() );
                }
            }
            else
            {

            }
        }
    }
}


//*************************************************************************************************
double meshMetricGui::calculNewY( double X , double Min , double Max )
{
    double Y = 0;
    Y = ( X - Min )/( Max - Min );
    return Y;
}


//*************************************************************************************************
void meshMetricGui::MeshMetric()
{
    QMessageBox MsgBox;
    QString text( "\t3DMeshMetric\n\nVersion: " );
    text += MESHMETRIC3D_VERSION;
    text += "\n\nSoftware Developers:\n";
    text += MESHMETRIC3D_CONTRIBUTOR;
    text += "\n\nDocumentation:\n";
    text += MESHMETRIC3D_DOCUMENTATION;
    MsgBox.setText( text );
    MsgBox.exec();
}


//*************************************************************************************************
void meshMetricGui::About()
{
    QMessageBox MsgBox;
    QString text( "GNU General Public License" );
    text += "\nThis program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by";
    text += "\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.";
    text += "\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of";
    text += "\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.";
    text += "\n\nYou should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.";
    text += "\n\nReference\nMeshValmet, Validation Metric for Meshes\nhttp://www.nitrc.org/projects/meshvalmet/";
    text += "\n\nThis program also uses Qt libraries\nhttp://www.qt-project.org/";

    MsgBox.setText( text );
    MsgBox.exec();
}


//*************************************************************************************************
void meshMetricGui::OpenBrowseWindowFile()
{
    QStringList browseMesh = QFileDialog::getOpenFileNames( this , "Open a VTK file" , QString() , "vtk mesh (*.vtk)" );
    QLineEdit *lineEditLoad = new QLineEdit;

    if( ! browseMesh.isEmpty() )
    {
      for( int i = 0 ; i < browseMesh.size() ; i++ )
      {
          lineEditLoad -> setText( browseMesh[ i ] );

        if( !lineEditLoad->text().isEmpty() )
        {
            m_DataList.push_back( ( lineEditLoad -> text() ).toStdString() );
            m_DataList[ m_NumberOfMesh ].initialization();

            QFileInfo File = ( lineEditLoad -> text() );

            listWidgetLoadedMesh -> addItem( File.fileName().toStdString().c_str() );

            QListWidgetItem* currentIndex = listWidgetLoadedMesh -> item( m_NumberOfMesh );
            currentIndex -> setFlags( listWidgetLoadedMesh -> item( m_NumberOfMesh ) -> flags() | Qt::ItemIsUserCheckable );
            currentIndex -> setCheckState( Qt::Checked );

            comboBoxMeshB -> addItem( m_NotOkIcon , File.fileName().toStdString().c_str() );

            m_MyWindowMesh.addData( m_DataList[ m_NumberOfMesh ].getActor() );

            m_ErrorComputed.push_back( false );
            m_Visibility.push_back( true );

            m_MeshSelected = m_NumberOfMesh;
            m_NumberOfMesh++;
            listWidgetLoadedMesh -> setCurrentRow( m_MeshSelected );

            PreviousError();

        }
      }

      browseMesh.clear();
      delete lineEditLoad;
      DisplayInit();
      ChangeMeshSelected();
    }
}


//*************************************************************************************************
void meshMetricGui::OpenBrowseWindowRepository()
{
    QString path;
    QString dir = QFileDialog::getExistingDirectory( this , tr("Open .vtk Directory") , path , QFileDialog::ShowDirsOnly );

    if( !dir.isEmpty() )
    {
        QDir vtkDir( dir );
        vtkDir.setFilter( QDir::NoDotAndDotDot | QDir::Files );
        vtkDir.setNameFilters( QStringList() << "*.vtk" );

        QList <QFileInfo > FileList;
        FileList.append( vtkDir.entryInfoList() );

        for ( int i = 0 ; i < FileList.size() ; i++ )
        {
            QString FileName = FileList.at(i).canonicalFilePath();

            if ( !FileName.endsWith(".vtk") )
            {
                FileList.removeAt(i);
                i--;
            }
        }

        for( int i = 0 ; i < FileList.size() ; i++ )
        {
            m_DataList.push_back( FileList.at(i).canonicalFilePath().toStdString() );
            m_DataList[ m_NumberOfMesh ].initialization();

            listWidgetLoadedMesh -> addItem( FileList.at(i).fileName().toStdString().c_str() );
            QListWidgetItem* currentIndex = listWidgetLoadedMesh -> item( m_NumberOfMesh );
            currentIndex -> setFlags( listWidgetLoadedMesh -> item( m_NumberOfMesh ) -> flags() | Qt::ItemIsUserCheckable );
            currentIndex -> setCheckState( Qt::Checked );

            comboBoxMeshB -> addItem( m_NotOkIcon , FileList.at(i).fileName().toStdString().c_str() );

            m_MyWindowMesh.addData( m_DataList[ m_NumberOfMesh ].getActor() );

            m_ErrorComputed.push_back( false );
            m_Visibility.push_back( true );

            m_MeshSelected = m_NumberOfMesh;
            m_NumberOfMesh++;
            listWidgetLoadedMesh -> setCurrentRow( m_MeshSelected );

            PreviousError();
        }

        FileList.clear();
        DisplayInit();
        ChangeMeshSelected();
    }
}


//*************************************************************************************************
QString meshMetricGui::SaveFile()
{
    if( !m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        QFileInfo CurrentMesh = QString::fromStdString( m_DataList[ m_MeshSelected ].getName() );
        QString fileName = QFileDialog::getSaveFileName( this , " Create a new vtk file or open an existing one" , CurrentMesh.absolutePath() , "vtk file (*.vtk)" );
        if( ! fileName.isEmpty() )
        {
            std::cout << fileName.toStdString() << std::endl;
            int out = m_MyProcess.SaveFile( fileName.toStdString() , m_DataList[ m_MeshSelected ] );
            if( out != 0 )
            {
                SaveFile();
            }
        }
        return fileName;
    }
    return "";
}


//*************************************************************************************************
void meshMetricGui::DeleteBoxOne()
{
    QMessageBox::StandardButton MsgBox = QMessageBox::question( this, "Close file" , "Do you really want to close without saving?" , QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Yes );

    if( MsgBox == QMessageBox::Yes )
    {
        DeleteOneFile();
    }
    else if( MsgBox == QMessageBox::Save )
    {
        QString out = SaveFile();
        if( !out.isEmpty() )
        {
            DeleteOneFile();
        }
    }
}


//*************************************************************************************************
void meshMetricGui::DeleteBoxAll()
{
    QMessageBox::StandardButton MsgBox = QMessageBox::question( this, "Close files" , "Do you really want to close without saving?" , QMessageBox::Cancel | QMessageBox::Yes );
    if( MsgBox == QMessageBox::Yes )
    {
        DeleteAllFiles();
    }
}



//*************************************************************************************************
void meshMetricGui::UpdateDisplayedMesh( QListWidgetItem *itemClicked )
{
    QListWidgetItem* currentItem = itemClicked;
    int ItemRow = 0;


    if( listWidgetLoadedMesh -> currentItem() == currentItem )
    {
        ChangeMeshSelected();
        ItemRow = m_MeshSelected;
    }
    else
    {
        ItemRow = listWidgetLoadedMesh -> row( currentItem );
    }

    if( currentItem -> checkState() == Qt::Unchecked && m_Visibility[ ItemRow ] == true )
    {
        m_DataList[ ItemRow  ].setOpacity( 0.0 );
        m_Visibility[ ItemRow ] = false;

        m_DataList[ ItemRow ].updateActorProperties();
        m_MyWindowMesh.updateWindow();
    }

    if( currentItem -> checkState() == Qt::Checked && m_Visibility[ ItemRow ] == false )
    {
        m_DataList[ ItemRow  ].setOpacity( 1.0 );
        m_Visibility[ ItemRow ] = true;

        m_DataList[ ItemRow ].updateActorProperties();
        m_MyWindowMesh.updateWindow();
    }

    horizontalSliderOpacity -> setValue( m_DataList[ ItemRow ].getOpacity()*100 );
}


//*************************************************************************************************
void meshMetricGui::ChangeMeshSelected()
{
   actionSaveFile -> setEnabled( true );
   tabWidgetVisualization -> setEnabled( true );
   pushButtonDeleteOne -> setEnabled( true );
   tabWidgetError -> setCurrentWidget( tabDistance );

   comboBoxMeshB->setCurrentIndex(0);

   if( m_NumberOfMesh >= 1 )
   {
       tabWidgetError -> setEnabled( true );
       distanceWidget -> setEnabled( false ) ;
   }

   if( m_NumberOfMesh >= 2 )
   {
       distanceWidget -> setEnabled( true ) ;

       pushButtonApply -> setEnabled( false );
       radioButtonAbsoluteDistance->setEnabled( false );
       radioButtonCorrespondantDistance->setEnabled( false );
       radioButtonSignedDistance->setEnabled( false );
   }
   m_nbIteration = 200;
   m_nbDecimate = 0.1;
   m_MeshSelected = listWidgetLoadedMesh -> currentRow();

   horizontalSliderOpacity -> setValue( m_DataList[ m_MeshSelected ].getOpacity()*100 );
   lcdNumberOpacity -> display( m_DataList[ m_MeshSelected ].getOpacity() );
   spinBoxIteration -> setValue( 200 );
   spinBoxDecimate -> setValue( 10 );

   lineEditNbPointsA -> setText( QString::number( m_DataList[ m_MeshSelected ].getPolyData()->GetNumberOfPoints() ) );
   if( comboBoxMeshB->currentIndex() != 0 )
   {
       lineEditNbPointsB -> setText( QString::number( m_DataList[ m_SelectedItemB ].getPolyData()->GetNumberOfPoints() ) );
   }
   else
   {
       lineEditNbPointsB -> setText( "-" );
   }

   if( m_DataList[ m_MeshSelected ].getType() == 1 )
   {
       radioButtonSurface -> setChecked( true );
   }
   else if( m_DataList[ m_MeshSelected ].getType() == 2 )
   {
       radioButtonPoints -> setChecked( true );
   }
   else if( m_DataList[ m_MeshSelected ].getType() == 3 )
   {
       radioButtonWireframe -> setChecked( true );
   }

   spinBoxMinSampFreq -> setValue( m_DataList[ m_MeshSelected ].getMinSamplingFrequency() );

   if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
   {
       radioButtonSignedDistance -> setChecked( true );
   }
   else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 )
   {
       radioButtonAbsoluteDistance -> setChecked( true );
   }
   else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
   {
       radioButtonCorrespondantDistance -> setChecked( true );
   }

   QFileInfo File = QString::fromStdString( m_DataList[ m_MeshSelected ].getName() );
   lineEditMeshA -> setText( File.fileName() );
   lineEditNameMesh -> setText( File.fileName() );
   m_MyWindowMesh.EraseCloud();
   lineEditClickValue->clear();
   AvailableMesh();

   if( m_ErrorComputed[ m_MeshSelected ] == true )
   {
       checkBoxError -> setEnabled( true );

       tabWidgetError -> setCurrentWidget( tabResults );
       tabResults -> setEnabled( true );

       if( m_DataList[ m_MeshSelected ].getDisplayError() == true )
       {
           checkBoxError -> setChecked( true );
           checkBoxColorBar -> setEnabled( true );
           m_DataList[ m_MeshSelected ].setColorBar( true );
           checkBoxColorBar -> setChecked( true );
       }
       else if( m_DataList[ m_MeshSelected ].getDisplayError() == false )
       {
           checkBoxError -> setChecked( false );
           checkBoxColorBar -> setEnabled( false );
           m_DataList[ m_MeshSelected ].setColorBar( false );
           checkBoxColorBar -> setChecked( false );
       }

       m_MyWindowMesh.setLut( m_DataList[ m_MeshSelected ].getMapper()->GetLookupTable() , "File "+QString::number( m_MeshSelected + 1 ).toStdString() );
       m_MyWindowMesh.updateLut( m_DataList[m_MeshSelected].getColorBar() );

       lineEditA -> setText( File.fileName() );
       lineEditB -> setText( QString::fromStdString( m_DataList[ m_MeshSelected ].getNameB() ) );

       lineEditMinR -> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
       lineEditMaxR -> setText( QString::number( m_DataList[ m_MeshSelected ].getMax() ) );

       lineEditMin -> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
       lineEditMax -> setText( QString::number( m_DataList[ m_MeshSelected ].getMax() ) );
       lineEditCenter -> setText( QString::number( m_DataList[ m_MeshSelected ].getCenter() ) );
       lineEditDelta -> setText( QString::number( m_DataList[ m_MeshSelected ].getDelta() ) );

       if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
       {
           double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getCenter() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           double DeltaC = calculNewY( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           double Center = calculNewY( m_DataList[ m_MeshSelected ].getCenter() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           widgetColor->updateGradientSigned( DeltaC , DeltaY , Center );
           lineEditType -> setText( QString( "Signed distance" ) );
           lineEditMin -> setReadOnly( false );
           lineEditMin -> setPalette( lineEditMax->palette() );
           lineEditCenter -> setReadOnly( false );
           lineEditCenter -> setPalette( lineEditMax->palette() );
       }
       else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 )
       {
           double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           widgetColor->updateGradientAbsolute( DeltaY );
           lineEditType -> setText( QString( "Absolute distance" ) );
           lineEditMin -> setReadOnly( true );
           lineEditMin -> setPalette( lineEditA->palette() );
           lineEditCenter -> setReadOnly( true );
           lineEditCenter -> setPalette( lineEditA->palette() );
       }
       else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
       {
           double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           widgetColor->updateGradientAbsolute( DeltaY );
           lineEditType -> setText( QString( "Correspondant" ) );
           lineEditMin -> setReadOnly( true );
           lineEditMin -> setPalette( lineEditA->palette() );
           lineEditCenter -> setReadOnly( true );
           lineEditCenter -> setPalette( lineEditA->palette() );
       }

   }
   else
   {
       checkBoxError -> setEnabled( false );
       checkBoxError -> setChecked( false );
       tabWidgetError -> setCurrentWidget( tabDistance );
       tabResults -> setEnabled( false );
       lineEditMaxR -> clear();
       lineEditMinR -> clear();
       lineEditA -> clear();
       lineEditB -> clear();
       lineEditType -> clear();
       lineEditMin -> setText( "-1.0" );
       lineEditMax -> setText( "1.0" );
       lineEditDelta -> setText( "0.5" );
       lineEditCenter -> setText( "0.0" );
    }

   m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::ChangeValueOpacity()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_DataList[ m_MeshSelected ].setOpacity( horizontalSliderOpacity -> value()/100. );
        lcdNumberOpacity -> display( m_DataList[ m_MeshSelected ].getOpacity() );

        m_DataList[ m_MeshSelected ].updateActorProperties();

        if( m_DataList[ m_MeshSelected ].getOpacity() == 0.0 )
        {
            listWidgetLoadedMesh -> item( m_MeshSelected ) -> setCheckState( Qt::Unchecked );
            m_Visibility[ m_MeshSelected ] = false;
        }
        else
        {
            listWidgetLoadedMesh -> item( m_MeshSelected ) -> setCheckState( Qt::Checked );
            m_Visibility[ m_MeshSelected ] = true;
        }

        m_MyWindowMesh.updateWindow();
    }
}


//*************************************************************************************************
void meshMetricGui::ChooseColor()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_Color = QColorDialog::getColor( Qt::white , this );

        if( m_Color.isValid() )
        {
            m_DataList[ m_MeshSelected ].setColor( m_Color.redF(), m_Color.greenF() , m_Color.blueF() );
            m_DataList[ m_MeshSelected ].updateActorProperties();
            m_MyWindowMesh.updateWindow();
        }
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeTypeOfDisplay()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        if( radioButtonSurface -> isChecked() )
        {
            m_DataList[ m_MeshSelected ].setType( 1 );
        }
        else if( radioButtonPoints -> isChecked() )
        {
             m_DataList[ m_MeshSelected ].setType( 2 );
        }
        else if( radioButtonWireframe -> isChecked() )
        {
             m_DataList[ m_MeshSelected ].setType( 3 );
        }

        m_DataList[ m_MeshSelected ].updateTypeOfDisplay();
        m_MyWindowMesh.updateWindow();
    }
}

//*************************************************************************************************
void meshMetricGui::ReloadOne()
{
    QMessageBox::StandardButton MsgBox = QMessageBox::question( this, "Reload file" , "Do you really want to reload without saving?" , QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Yes );

    if( MsgBox == QMessageBox::Yes )
    {
        ResetSelectedFile();
    }
    else if( MsgBox == QMessageBox::Save )
    {
        QString out = SaveFile();
        if( !out.isEmpty() )
        {
            ResetSelectedFile();
        }
    }
}

//*************************************************************************************************
void meshMetricGui::DisplayAll()
{
    int IndiceOfMesh;
    for( IndiceOfMesh = 0 ; IndiceOfMesh < m_NumberOfMesh ; IndiceOfMesh++ )
    {
       m_DataList[ IndiceOfMesh ].setOpacity( 1.0 );
       horizontalSliderOpacity -> setValue( m_DataList[ IndiceOfMesh ].getOpacity()*100 );
       lcdNumberOpacity -> display( m_DataList[ IndiceOfMesh ].getOpacity() );
       listWidgetLoadedMesh -> item( IndiceOfMesh ) -> setCheckState( Qt::Checked );
       m_DataList[ IndiceOfMesh ].updateActorProperties();
       m_Visibility[ IndiceOfMesh ] = true;
    }
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::HideAll()
{
    int IndiceOfMesh;
    for( IndiceOfMesh = 0 ; IndiceOfMesh < m_NumberOfMesh ; IndiceOfMesh++ )
    {
        m_DataList[ IndiceOfMesh ].setOpacity( 0.0 );
        horizontalSliderOpacity -> setValue( 0.0 );
        lcdNumberOpacity -> display( 0.0 );
        listWidgetLoadedMesh -> item( IndiceOfMesh ) -> setCheckState( Qt::Unchecked );
        m_DataList[ IndiceOfMesh ].updateActorProperties();
        m_Visibility[ IndiceOfMesh ] = false;
    }
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::ChooseColorBackground()
{
    if( m_NumberOfDisplay != 0 )
    {
        m_ColorBg = QColorDialog::getColor( Qt::white , this );

        if( m_ColorBg.isValid() )
        {
            m_MyWindowMesh.changeBackgroundColor( m_ColorBg.redF() , m_ColorBg.greenF() , m_ColorBg.blueF() );
            m_MyWindowMesh.updateWindow();
        }
    }
}


//*************************************************************************************************
void meshMetricGui::buttonAntClicked()
{
    m_CameraX = 0 ; m_CameraY = 1 ; m_CameraZ = 0;

    m_MyWindowMesh.setCameraX( m_CameraX );
    m_MyWindowMesh.setCameraY( m_CameraY );
    m_MyWindowMesh.setCameraZ( m_CameraZ );
    m_MyWindowMesh.updatePositionCamera();
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::buttonPostClicked()
{
    m_CameraX = 0 ; m_CameraY = -1 ; m_CameraZ = 0;

    m_MyWindowMesh.setCameraX( m_CameraX );
    m_MyWindowMesh.setCameraY( m_CameraY );
    m_MyWindowMesh.setCameraZ( m_CameraZ );
    m_MyWindowMesh.updatePositionCamera();
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::buttonLeftClicked()
{
    m_CameraX = 1 ; m_CameraY = 0 ; m_CameraZ = 0;

    m_MyWindowMesh.setCameraX( m_CameraX );
    m_MyWindowMesh.setCameraY( m_CameraY );
    m_MyWindowMesh.setCameraZ( m_CameraZ );
    m_MyWindowMesh.updatePositionCamera();
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::buttonRightClicked()
{
    m_CameraX = -1 ; m_CameraY = 0 ; m_CameraZ = 0;

    m_MyWindowMesh.setCameraX( m_CameraX );
    m_MyWindowMesh.setCameraY( m_CameraY );
    m_MyWindowMesh.setCameraZ( m_CameraZ );
    m_MyWindowMesh.updatePositionCamera();
    m_MyWindowMesh.updateWindow();
}
//*************************************************************************************************
void meshMetricGui::buttonSupClicked()
{
    m_CameraX = 0 ; m_CameraY = 0 ; m_CameraZ = 1;

    m_MyWindowMesh.setCameraX( m_CameraX );
    m_MyWindowMesh.setCameraY( m_CameraY );
    m_MyWindowMesh.setCameraZ( m_CameraZ );
    m_MyWindowMesh.updatePositionCamera();
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::buttonInfClicked()
{
    m_CameraX = 0 ; m_CameraY = 0 ; m_CameraZ = -1;

    m_MyWindowMesh.setCameraX( m_CameraX );
    m_MyWindowMesh.setCameraY( m_CameraY );
    m_MyWindowMesh.setCameraZ( m_CameraZ );
    m_MyWindowMesh.updatePositionCamera();
    m_MyWindowMesh.updateWindow();
}


//*************************************************************************************************
void meshMetricGui::ChangeNumberOfIteration()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_nbIteration = spinBoxIteration -> value();
        pushButtonRunSmoothing -> setEnabled( true );
    }
}


//*************************************************************************************************
void meshMetricGui::ApplySmoothing()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        vtkSmartPointer <vtkPolyData> smoothedData = vtkSmartPointer <vtkPolyData>::New();
        vtkSmartPointer <vtkPolyData> originalData = vtkSmartPointer <vtkPolyData>::New();

        originalData = m_DataList[ m_MeshSelected ].getPolyData();
        smoothedData = m_MyProcess.processSmoothing( originalData , m_nbIteration );

        m_DataList[ m_MeshSelected ].setPolyData( smoothedData );
        m_DataList[ m_MeshSelected ].changeMapperInput();

        m_MyWindowMesh.updateWindow();

        spinBoxIteration -> setValue( 200 );
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeDecimate()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_nbDecimate = spinBoxDecimate -> value()/100.0;
        pushButtonRunDecimate -> setEnabled( true );
    }
}


//*************************************************************************************************
void meshMetricGui::ApplyDecimate()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        vtkSmartPointer <vtkPolyData> decimatedData = vtkSmartPointer <vtkPolyData>::New();
        vtkSmartPointer <vtkPolyData> originalData = vtkSmartPointer <vtkPolyData>::New();

        originalData = m_DataList[ m_MeshSelected ].getPolyData();
        decimatedData = m_MyProcess.processDownSampling( originalData , m_nbDecimate );

        m_DataList[ m_MeshSelected ].setPolyData( decimatedData );
        m_DataList[ m_MeshSelected ].changeMapperInput();

        m_MyWindowMesh.updateWindow();

        spinBoxDecimate -> setValue( 10 );
    }
}


//*************************************************************************************************
void meshMetricGui::AvailableMesh()
{
    m_SelectedItemA = m_MeshSelected;

    for( int IndiceOfMesh = 0 ; IndiceOfMesh < m_NumberOfMesh ; IndiceOfMesh++ )
    {
        if( IndiceOfMesh != m_SelectedItemA )
        {
           comboBoxMeshB -> setItemIcon( IndiceOfMesh + 1 , m_OkIcon );
           QStandardItemModel* Model = qobject_cast< QStandardItemModel* >( comboBoxMeshB -> model() );
           QStandardItem* Item = Model -> item( IndiceOfMesh + 1 );
           Item -> setSelectable( true );
        }
        else
        {
           comboBoxMeshB ->setItemIcon( IndiceOfMesh + 1 , m_NotOkIcon );
           QStandardItemModel* Model = qobject_cast< QStandardItemModel* >( comboBoxMeshB -> model() );
           QStandardItem* Item = Model -> item( IndiceOfMesh + 1 );
           Item -> setSelectable( false );
        }
    }
}


//*************************************************************************************************
void meshMetricGui::SelectMeshB()
{
    if( m_SelectedItemA != -1 )
    {
        m_SelectedItemB = comboBoxMeshB -> currentIndex() - 1;

        lineEditNbPointsA -> setText( QString::number( m_DataList[ m_SelectedItemA ].getPolyData()->GetNumberOfPoints() ) );
        if( comboBoxMeshB->currentIndex() != 0 )
        {
            lineEditNbPointsB -> setText( QString::number( m_DataList[ m_SelectedItemB ].getPolyData()->GetNumberOfPoints() ) );
        }
        else
        {
            lineEditNbPointsB -> setText( "-" );
        }

        pushButtonApply -> setEnabled( true );
        radioButtonAbsoluteDistance->setEnabled( true );
        radioButtonSignedDistance->setEnabled( true );
        if( m_DataList[ m_SelectedItemA ].getPolyData()->GetNumberOfPoints() == m_DataList[ m_SelectedItemB ].getPolyData()->GetNumberOfPoints() )
        {
           radioButtonCorrespondantDistance -> setEnabled( true );
        }
        else
        {
           radioButtonCorrespondantDistance -> setEnabled( false );
        }
    }
    else
    {
        QMessageBox MsgBox;
        MsgBox.setText( " choose first the file you want to apply the distance error ");
        MsgBox.exec();
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeMinSampleFrequency()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_DataList[ m_MeshSelected ].setMinSamplingFrequency( spinBoxMinSampFreq -> value() );
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeSamplingStep()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        double value;
        value = ( comboBoxSamplingStep -> currentText() ).toDouble();
        m_DataList[ m_MeshSelected ].setSamplingStep( value );
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeSignedDistance()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        if( radioButtonSignedDistance -> isChecked() )
        {
            m_DataList[ m_MeshSelected ].setTypeDistance( 1 );
            comboBoxSamplingStep -> setEnabled( true );
            spinBoxMinSampFreq-> setEnabled( true );
        }
        else if( radioButtonAbsoluteDistance -> isChecked() )
        {
            m_DataList[ m_MeshSelected ].setTypeDistance( 0 );
            comboBoxSamplingStep -> setEnabled( true );
            spinBoxMinSampFreq -> setEnabled( true );
        }
        else if( radioButtonCorrespondantDistance -> isChecked() )
        {
            m_DataList[ m_MeshSelected ].setTypeDistance( 2 );
            comboBoxSamplingStep -> setEnabled( false );
            spinBoxMinSampFreq -> setEnabled( false );
        }
    }
}


//*************************************************************************************************
void meshMetricGui::ApplyDistance()
{
    if( m_SelectedItemA != -1 && m_SelectedItemB != -1 )
    {
        int result;

        result = m_MyProcess.processError( m_DataList[ m_SelectedItemA ] , m_DataList[ m_SelectedItemB ] );

        if( result == -1 )
        {
            QMessageBox MsgBox;
            QString text( " Houston, we have a problem " );
            MsgBox.setText( text );
            MsgBox.exec();
        }
        else
        {
            checkBoxError -> setChecked( true );
            checkBoxColorBar -> setEnabled( true );
            checkBoxColorBar -> setChecked( true );
            m_ErrorComputed[ m_MeshSelected ] = true;

            m_DataList[ m_SelectedItemB ].setOpacity( 0.0 );
            m_Visibility[ m_SelectedItemB ] = false;
            listWidgetLoadedMesh -> item( m_SelectedItemB ) -> setCheckState( Qt::Unchecked );
            m_DataList[ m_SelectedItemB ].updateActorProperties();
            QFileInfo File = QString::fromStdString( m_DataList[ m_SelectedItemB ].getName() );
            lineEditB -> setText( File.fileName() );
            lineEditNbPointsB -> setText( QString::number( m_DataList[ m_SelectedItemB ].getPolyData()->GetNumberOfPoints() ) );

            m_DataList[ m_SelectedItemA ].setColorBar( true );
            m_DataList[ m_SelectedItemA ].setDisplayError( true );
            m_DataList[ m_SelectedItemA ].setNameB( File.fileName().toStdString() );
            m_DataList[ m_SelectedItemA ].setOpacity( 1.0 );
            m_Visibility[ m_SelectedItemA ] = true;
            listWidgetLoadedMesh -> item( m_SelectedItemA ) -> setCheckState( Qt::Checked );
            m_DataList[ m_SelectedItemA ].updateActorProperties();
            File = QString::fromStdString( m_DataList[ m_SelectedItemA ].getName() );
            lineEditA -> setText( File.fileName() );

            lineEditMinR -> setText( QString::number( m_DataList[ m_SelectedItemA ].getMin() ) );
            lineEditMaxR -> setText( QString::number( m_DataList[ m_SelectedItemA ].getMax()) );

            lineEditMin -> setText( QString::number( m_DataList[ m_SelectedItemA ].getMin() ) );
            lineEditMax -> setText( QString::number( m_DataList[ m_SelectedItemA ].getMax() ) );
            lineEditDelta -> setText( QString::number( m_DataList[ m_MeshSelected ].getDelta() ) );
            lineEditCenter -> setText( QString::number( m_DataList[ m_SelectedItemA ].getCenter() ) );

            m_MyWindowMesh.setLut( m_DataList[ m_SelectedItemA ].getMapper()->GetLookupTable() , "File "+QString::number( m_SelectedItemA + 1 ).toStdString() );
            m_MyWindowMesh.updateLut( 1 );
            m_MyWindowMesh.updateWindow();

            if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
            {
                double DeltaY = calculNewY( m_DataList[ m_SelectedItemA ].getCenter() + m_DataList[ m_SelectedItemA ].getDelta() , m_DataList[ m_SelectedItemA ].getMin() , m_DataList[ m_SelectedItemA ].getMax() );
                double DeltaC = calculNewY( m_DataList[ m_SelectedItemA ].getCenter() - m_DataList[ m_SelectedItemA ].getDelta() , m_DataList[ m_SelectedItemA ].getMin() , m_DataList[ m_SelectedItemA ].getMax() );
                double Center = calculNewY( m_DataList[ m_SelectedItemA ].getCenter() , m_DataList[ m_SelectedItemA ].getMin() , m_DataList[ m_SelectedItemA ].getMax() );               
                widgetColor->updateGradientSigned( DeltaC , DeltaY , Center );
                lineEditType -> setText( QString( "Signed distance" ) );
                lineEditMin -> setReadOnly( false );
                lineEditMin -> setPalette( lineEditMax->palette() );
                lineEditCenter -> setReadOnly( false );
                lineEditCenter-> setPalette( lineEditMax->palette() );
            }
            else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 )
            {
                double DeltaY = calculNewY( m_DataList[ m_SelectedItemA ].getCenter() + m_DataList[ m_SelectedItemA ].getDelta() , m_DataList[ m_SelectedItemA ].getMin() , m_DataList[ m_SelectedItemA ].getMax() );
                widgetColor->updateGradientAbsolute( DeltaY );
                lineEditType -> setText( QString( "Absolute distance" ) );
                lineEditMin -> setReadOnly( true );
                lineEditMin -> setPalette( lineEditA->palette() );
                lineEditCenter -> setReadOnly( true );
                lineEditCenter -> setPalette( lineEditA->palette() );
            }
            else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
            {
                double DeltaY = calculNewY( m_DataList[ m_SelectedItemA ].getMin() + m_DataList[ m_SelectedItemA ].getDelta() , m_DataList[ m_SelectedItemA ].getMin() , m_DataList[ m_SelectedItemA ].getMax() );
                widgetColor->updateGradientAbsolute( DeltaY );
                lineEditType -> setText( QString( "Correspondant distance" ) );
                lineEditMin -> setReadOnly( true );
                lineEditMin -> setPalette( lineEditA->palette() );
                lineEditCenter -> setReadOnly( true );
                lineEditCenter -> setPalette( lineEditA->palette() );
            }

            comboBoxMeshB -> setCurrentIndex( 0 );
            ChangeMeshSelected();
        }
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeDisplayError()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        if( checkBoxError -> isChecked() )
        {
            checkBoxColorBar->setEnabled( true );
            checkBoxColorBar->setChecked( true );
            m_DataList[ m_MeshSelected ].setDisplayError( true );
            m_DataList[ m_MeshSelected ].setColorBar( true );
        }
        else if( ! checkBoxError -> isChecked() )
        {
            checkBoxColorBar->setEnabled( false );
            checkBoxColorBar->setChecked( false );
            m_DataList[ m_MeshSelected ].setDisplayError( false );
            m_DataList[ m_MeshSelected ].setColorBar( false );
        }

        m_DataList[ m_MeshSelected ].changeActivScalar();
        m_MyWindowMesh.updateLut( m_DataList[ m_MeshSelected ].getColorBar() );
        m_MyWindowMesh.updateWindow();
    }
}

void meshMetricGui::HideColorBar()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 && checkBoxError->isChecked() )
    {
        if( checkBoxColorBar -> isChecked() )
        {
            m_DataList[ m_MeshSelected ].setColorBar( true );
        }
        else if( ! checkBoxColorBar -> isChecked() )
        {
            m_DataList[ m_MeshSelected ].setColorBar( false );
        }
        m_MyWindowMesh.updateLut( m_DataList[ m_MeshSelected ].getColorBar() );
        m_MyWindowMesh.updateWindow();

    }
}


//*************************************************************************************************
void meshMetricGui::ChangeValueMin()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        double Min = lineEditMin->text().toDouble();

        if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
        {
            if( Min < ( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getDelta() ) )
            {
                m_DataList[ m_MeshSelected ].setMin( Min );
                UpdateColor();
            }
            else
            {
                QMessageBox MsgBox;
                QString text( " Change Min value such as :\n Min < " );
                text += QString::number( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getDelta() );
                MsgBox.setText( text );
                MsgBox.exec();

                lineEditMin-> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
            }
        }
        else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 || m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
        {
            lineEditMin-> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
        }
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeValueMax()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        double Max = lineEditMax->text().toDouble();

        if( Max > ( m_DataList[ m_MeshSelected ].getCenter() + m_DataList[ m_MeshSelected ].getDelta() ) )
        {
            m_DataList[ m_MeshSelected ].setMax( Max );
            UpdateColor();
        }
        else
        {
            QMessageBox MsgBox;
            QString text( " Change Max value such as :\n Max > " );
            text += QString::number( m_DataList[ m_MeshSelected ].getCenter() + m_DataList[ m_MeshSelected ].getDelta() );
            MsgBox.setText( text );
            MsgBox.exec();

            lineEditMax-> setText( QString::number( m_DataList[ m_MeshSelected ].getMax() ) );
        }
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeValueDelta()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
        {
            double Delta = lineEditDelta->text().toDouble();

            if( Delta < ( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getMin() ) &&
                    Delta <  ( m_DataList[ m_MeshSelected ].getMax() - m_DataList[ m_MeshSelected ].getCenter() )  &&
                Delta > 0.02 )
            {
                m_DataList[ m_MeshSelected ].setDelta( Delta );
                UpdateColor();
            }
            else
            {
                QMessageBox MsgBox;
                QString text( " Change Delta value such as :\n 0.02 < Delta < " );
                text += QString::number( min( ( m_DataList[ m_MeshSelected ].getMax() - m_DataList[ m_MeshSelected ].getCenter() ) , ( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getMin() ) ) );
                MsgBox.setText( text );
                MsgBox.exec();
                lineEditDelta->setText( QString::number( m_DataList[ m_MeshSelected ].getDelta() ) );
            }
        }
        else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 || m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
        {
            double Delta = lineEditDelta->text().toDouble();

            if( Delta < ( m_DataList[ m_MeshSelected ].getMax() - m_DataList[ m_MeshSelected ].getCenter() )  &&
                Delta > 0.02 )
            {
                m_DataList[ m_MeshSelected ].setDelta( Delta );
                UpdateColor();
            }
            else
            {
                QMessageBox MsgBox;
                QString text( " Change Delta value such as :\n " );
                text += QString::number( m_DataList[ m_MeshSelected ].getMin() );
                text += " < Delta < ";
                text += QString::number( m_DataList[ m_MeshSelected ].getMax() );
                MsgBox.setText( text );
                MsgBox.exec();
                lineEditDelta->setText( QString::number( m_DataList[ m_MeshSelected ].getDelta() ) );
            }
        }
    }
}

//*************************************************************************************************
void meshMetricGui::ChangeValueCenter()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
        {
            double Center = lineEditCenter->text().toDouble();

            if( Center < ( m_DataList[ m_MeshSelected ].getMax() - m_DataList[ m_MeshSelected ].getDelta() ) &&
                Center > ( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() ) )
            {
                m_DataList[ m_MeshSelected ].setCenter( Center );
                UpdateColor();
            }
            else
            {
                QMessageBox MsgBox;
                QString text( " Change Center value such as :\n " );
                text += QString::number( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() );
                text += " < Center < ";
                text += QString::number( m_DataList[ m_MeshSelected ].getMax() - m_DataList[ m_MeshSelected ].getDelta() );
                MsgBox.setText( text );
                MsgBox.exec();
                lineEditCenter -> setText( QString::number( m_DataList[ m_MeshSelected ].getCenter() ) );
            }
        }
        else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 || m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
        {
            lineEditCenter -> setText( QString::number( m_DataList[ m_MeshSelected ].getMin() ) );
        }
    }
}

//*************************************************************************************************
void meshMetricGui::UpdateColor()
{
    if( ! m_DataList.empty() && m_MeshSelected != -1  && m_ErrorComputed[ m_MeshSelected ] == true )
    {
        if( m_DataList[ m_MeshSelected ].getTypeDistance() == 1 )
        {
           m_MyProcess.updateColor( m_DataList[ m_MeshSelected ] );
           m_MyWindowMesh.setLut( m_DataList[ m_MeshSelected ].getMapper()->GetLookupTable() , "File "+QString::number( m_MeshSelected + 1 ).toStdString() );
           m_MyWindowMesh.updateWindow();

           double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getCenter() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           double DeltaC = calculNewY( m_DataList[ m_MeshSelected ].getCenter() - m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );
           double Center = calculNewY( m_DataList[ m_MeshSelected ].getCenter() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );

           widgetColor->updateGradientSigned( DeltaC , DeltaY , Center );

        }
        else if( m_DataList[ m_MeshSelected ].getTypeDistance() == 0 || m_DataList[ m_MeshSelected ].getTypeDistance() == 2 )
        {
               m_MyProcess.updateColor( m_DataList[ m_MeshSelected ] );
               m_MyWindowMesh.setLut( m_DataList[ m_MeshSelected ].getMapper()->GetLookupTable() , "File "+QString::number( m_MeshSelected + 1 ).toStdString() );
               m_MyWindowMesh.updateWindow();

               double DeltaY = calculNewY( m_DataList[ m_MeshSelected ].getMin() + m_DataList[ m_MeshSelected ].getDelta() , m_DataList[ m_MeshSelected ].getMin() , m_DataList[ m_MeshSelected ].getMax() );

               widgetColor->updateGradientAbsolute( DeltaY );
        }
    }
}

//*************************************************************************************************
void meshMetricGui::RefreshColorBar()
{
    ChangeValueMin();
    ChangeValueCenter();
    ChangeValueDelta();
    ChangeValueMax();
}

//*************************************************************************************************
void meshMetricGui::GetValueByClicking(double X, double Y, double Z)
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        double Picked[3];
        Picked[0] = X;
        Picked[1] = Y;
        Picked[2] = Z;

        vtkIdType MyId = m_DataList[ m_MeshSelected ].getIdPointClicked( Picked );
        QString value = m_DataList[ m_MeshSelected ].getScalarValue( MyId );

        if( value != "Not a Point" )
        {
           m_MyWindowMesh.PointCloud( X ,Y , Z );
        }
        else
        {
           m_MyWindowMesh.EraseCloud();
        }

        tabWidgetVisualization->setCurrentWidget( tabClickPoint );
        lineEditClickValue -> setText( value );
     }
}


//*************************************************************************************************
void meshMetricGui::ChangeValueRadius()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        double NewValue = horizontalSliderCloudRadius->value()*0.045;
        m_MyWindowMesh.ChangeCloudRadius( NewValue );
    }
}


//*************************************************************************************************
void meshMetricGui::ChangeColorCloud()
{
    if( ! m_DataList.empty() && m_NumberOfDisplay != 0 && m_MeshSelected != -1 )
    {
        m_Color = QColorDialog::getColor( Qt::white , this );

        if( m_Color.isValid() )
        {
            m_MyWindowMesh.ChangeCloudColor( m_Color.redF() , m_Color.greenF() , m_Color.blueF() );
        }
    }
}

