#include "MeshValmet.h"

meshValmet::meshValmet()
{
    m_Pargs.sampling_step = 0.5/100.0;
    m_Pargs.min_sample_freq = 2;
    m_Pargs.signeddist = false;

    m_Pargs.verb_analysis = false; // ???
    m_Pargs.quiet = 0; // ???

    //m_ModelError1
    //m_ModelError2
    //m_Stats;
    //m_StatsRev;
    //m_AbsSamplingStep;
    //m_AbsSamplingDens;

    m_DiceCoefficient[0] = 0.0; // ????
    m_IntUnionRatio[0] = 0.0; // ????

    m_Downsampling = 1; // ????
    m_Delta = 0.5;
    m_Center = 0;

    for (float i=0;i<256;i++)
    {
        m_Lookuptable[(int)i].R = 255*((-(i*i)/(float)4096)+(i/32));

        if (m_Lookuptable[(int)i].R<0)
        {
            m_Lookuptable[(int)i].R =0;
        }

        m_Lookuptable[(int)i].G = 255*((-((i-64)*(i-64))/(float)4096)+((i-64)/(float)32));

        if ( m_Lookuptable[(int)i].G<0 )
        {
            m_Lookuptable[(int)i].G =0;
        }

        m_Lookuptable[(int)i].B = 255*((-((i-128)*(i-128))/(float)4096)+((i-128)/(float)32));

        if ( m_Lookuptable[(int)i].B<0 )
        {
            m_Lookuptable[(int)i].B =0;
        }
    }
}

//***************************** SET ALL THE PARAMETERS WE WILL NEED LATER ***********************************
void meshValmet::SetData1( vtkSmartPointer <vtkPolyData> Data1 )
{
    m_Pargs.Data1 = Data1;
}

void meshValmet::SetData2(vtkSmartPointer<vtkPolyData> Data2 )
{
    m_Pargs.Data2 = Data2;
}

void meshValmet::SetSamplingStep( double SamplingStep )
{
    m_Pargs.sampling_step = SamplingStep/100;
}

void meshValmet::SetMinSampleFrequency( int MinSampleFrequency )
{
    m_Pargs.min_sample_freq = MinSampleFrequency;
}

void meshValmet::SetSignedDistance( bool SignedDistance )
{
    m_Pargs.signeddist = SignedDistance;
}

void meshValmet::SetMin( double Min )
{
    m_Dmin = Min;
}

void meshValmet::SetMax( double Max )
{
    m_Dmax = Max;
}

void meshValmet::SetDelta( double Delta )
{
    m_Delta = Delta;
}

void meshValmet::SetCenter( double Center )
{
    m_Center = Center;
}

//***************************** ACCESSOR TO THE FINAL POLYDATA THAT WE WILL DISPLAY *************************
vtkSmartPointer <vtkPolyData> meshValmet::GetFinalData()
{
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New();
    Cleaner -> SetInputData( m_FinalData );
    Cleaner -> Update();
    return Cleaner -> GetOutput();
}

vtkSmartPointer <vtkColorTransferFunction> meshValmet::GetLut()
{
    return m_Lut;
}

double meshValmet::GetMin()
{
    return m_Dmin;
}

double meshValmet::GetMax()
{
    return m_Dmax;
}

double meshValmet::GetDelta()
{
    return m_Delta;
}


//************************************** COMPUTE THE ERROR ***************************************
void meshValmet::CalculateError()
{
    printf( "\t-Sampling Step: %f\n\t-Min Sampling Frequency: %d\n\t-Signed Distance: %d\n" , m_Pargs.sampling_step , m_Pargs.min_sample_freq , m_Pargs.signeddist );
    fflush( stdout );

    m_FinalData = vtkSmartPointer <vtkPolyData> ::New();

    m_Out = NULL;
    m_Out = outbuf_new( stdio_puts , stdout );

    mesh_run( &m_Pargs , &m_ModelError1 , &m_ModelError2 , m_Out , NULL , &m_Stats , &m_StatsRev , &m_AbsSamplingStep , &m_AbsSamplingDens );

    int num_vert1 = m_ModelError1.mesh->num_vert;
    int num_vert2 = m_ModelError2.mesh->num_vert;
    int num_faces1 = m_ModelError1.mesh->num_faces;
    int num_faces2 = m_ModelError2.mesh->num_faces;

    double * L1 = new double[3*num_vert1];
    double * L2 = new double[3*num_vert2];
    int * T1 = new int[3*num_faces1];
    int * T2 = new int[3*num_faces2];

    vertex_t * vert_list = m_ModelError1.mesh->vertices;

    for(int i=0; i<num_vert1; i++)
    {
      L1[3*i+0] = vert_list[i].x;
      L1[3*i+1] = vert_list[i].y;
      L1[3*i+2] = vert_list[i].z;
    }

    vert_list = m_ModelError2.mesh->vertices;

    for(int i=0; i<num_vert2; i++)
    {
      L2[3*i+0] = vert_list[i].x;
      L2[3*i+1] = vert_list[i].y;
      L2[3*i+2] = vert_list[i].z;
    }

    face_t * face_list = m_ModelError1.mesh->faces;

    for(int i=0; i<num_faces1; i++)
    {
      T1[3*i+0] = face_list[i].f0+1;
      T1[3*i+1] = face_list[i].f1+1;
      T1[3*i+2] = face_list[i].f2+1;
    }

    face_list = m_ModelError2.mesh->faces;

    for(int i=0; i<num_faces2; i++)
    {
      T2[3*i+0] = face_list[i].f0+1;
      T2[3*i+1] = face_list[i].f1+1;
      T2[3*i+2] = face_list[i].f2+1;
    }

    ComputeRobustVolumeOverlap( L1 , L2 , num_vert1 , num_vert2 , T1 , T2 , num_faces1 , num_faces2 , m_DiceCoefficient , m_IntUnionRatio );

    delete [] L1;
    delete [] L2;
    delete [] T1;
    delete [] T2;

    //int t = m_Pargs.do_symmetric;
    bool signedd = m_Pargs.signeddist;

    // dmin dmax are updated according to signedd
      if(signedd)
      {
        m_Dmax = m_ModelError1.max_error;
        m_Dmin = m_ModelError1.min_error;
      }
      else
      {
        m_Dmax = m_ModelError1.abs_max_error;
        m_Dmin = m_ModelError1.abs_min_error;
      }

    drawVertexErrorT();
    CreateLutError();
}

//********************************* CREATE THE FINAL POLYDATA ***********************************
void meshValmet::drawVertexErrorT()
{
  vtkSmartPointer <vtkPoints> Points = vtkSmartPointer <vtkPoints>::New();
  vtkSmartPointer <vtkCellArray> Polys = vtkSmartPointer <vtkCellArray>::New();
  vtkSmartPointer <vtkDoubleArray> ScalarsError = vtkSmartPointer <vtkDoubleArray>::New();
  vtkSmartPointer <vtkDoubleArray> ScalarsConst = vtkSmartPointer <vtkDoubleArray>::New();
  int k,i,j,jmax,n;
  vertex_t u,v;
  vertex_t a,b,c;
  face_t *cur_face;
  int i0,i1,i2,i3;
  int j0,j1,j2,j3;
  int l0,l1,l2,l3;
  vertex_t v0,v1,v2,v3;

  vtkIdType index = 0;
  double vertex[3];
  vtkIdType f[3];

  for ( k=0 ; k < m_ModelError1.mesh->num_faces ; k++)
  {
    n = m_ModelError1.fe[k].sample_freq;

    cur_face = &(m_ModelError1.mesh->faces[k]);

    if (n == 1 && m_Downsampling == 1)
    {
      // displaying only at triangle vertices + center
      a =m_ModelError1.mesh->vertices[cur_face->f0];
      b =m_ModelError1.mesh->vertices[cur_face->f1];
      c =m_ModelError1.mesh->vertices[cur_face->f2];

      v3.x = 1/3.0*(a.x+b.x+c.x);
      v3.y = 1/3.0*(a.y+b.y+c.y);
      v3.z = 1/3.0*(a.z+b.z+c.z);

      vertex[0] = a.x;
      vertex[1] = a.y;
      vertex[2] = a.z;

      Points->InsertPoint( index , vertex );
      ScalarsError->InsertTuple1( index++ , m_ModelError1.verror[cur_face->f0] );
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = b.x;
      vertex[1] = b.y;
      vertex[2] = b.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f1]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = v3.x;
      vertex[1] = v3.y;
      vertex[2] = v3.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++,m_ModelError1.fe[k].serror[0]);//?
      ScalarsConst->InsertTuple1( index , 1.0 );

      f[0] = index - 3;
      f[1] = index - 2;
      f[2] = index - 1;

      Polys->InsertNextCell(3,f);

      vertex[0] = a.x;
      vertex[1] = a.y;
      vertex[2] = a.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f0]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = v3.x;
      vertex[1] = v3.y;
      vertex[2] = v3.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[0]);//?
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = c.x;
      vertex[1] = c.y;
      vertex[2] = c.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f2]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      f[0] = index - 3;
      f[1] = index - 2;
      f[2] = index - 1;

      Polys->InsertNextCell(3,f);

      vertex[0] = b.x;
      vertex[1] = b.y;
      vertex[2] = b.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f1]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = c.x;
      vertex[1] = c.y;
      vertex[2] = c.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f2]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = v3.x;
      vertex[1] = v3.y;
      vertex[2] = v3.z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[0]);//?
      ScalarsConst->InsertTuple1( index , 1.0 );

      f[0] = index - 3;
      f[1] = index - 2;
      f[2] = index - 1;

      Polys->InsertNextCell(3,f);

    }
    else if ( m_Downsampling >= n)
    {
      //displaying only at triangle vertices
      vertex[0] = m_ModelError1.mesh->vertices[cur_face->f0].x;
      vertex[1] = m_ModelError1.mesh->vertices[cur_face->f0].y;
      vertex[2] = m_ModelError1.mesh->vertices[cur_face->f0].z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f0]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = m_ModelError1.mesh->vertices[cur_face->f1].x;
      vertex[1] = m_ModelError1.mesh->vertices[cur_face->f1].y;
      vertex[2] = m_ModelError1.mesh->vertices[cur_face->f1].z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f1]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      vertex[0] = m_ModelError1.mesh->vertices[cur_face->f2].x;
      vertex[1] = m_ModelError1.mesh->vertices[cur_face->f2].y;
      vertex[2] = m_ModelError1.mesh->vertices[cur_face->f2].z;

      Points->InsertPoint(index,vertex);
      ScalarsError->InsertTuple1(index++, m_ModelError1.verror[cur_face->f2]);
      ScalarsConst->InsertTuple1( index , 1.0 );

      f[0] = index - 3;
      f[1] = index - 2;
      f[2] = index - 1;

      Polys->InsertNextCell(3,f);
    }
    else
    {
      /* displaying at error samples and triangle vertices */
      //assert(n > 1);
      a = m_ModelError1.mesh->vertices[cur_face->f0];
      b = m_ModelError1.mesh->vertices[cur_face->f1];
      c = m_ModelError1.mesh->vertices[cur_face->f2];

      substract_v(&b,&a,&u);
      substract_v(&c,&a,&v);

      prod_v(1/(float)(n-1),&u,&u);
      prod_v(1/(float)(n-1),&v,&v);

      for (i=0; i<n-1; i+=m_Downsampling)
      {
        i2 = (i+m_Downsampling < n) ? i+m_Downsampling : n-1;

        for (j=0, jmax=n-i-1; j<jmax; j+=m_Downsampling)
        {
          if (i+j+m_Downsampling < n)
          {
            i0 = i;
            j0 = j;
            i1 = i+m_Downsampling;
            j1 = j;
            i2 = i;
            j2 = j+m_Downsampling;
            i3 = i1;
            j3 = j2;
          }
          else
          {
            i2 = i;
            j2 = j;
            i0 = (i+m_Downsampling < n) ? i+m_Downsampling : n-1;
            j0 = (j>0) ? j-m_Downsampling : j;
            //assert(j0 >= 0);
            i1 = i0;
            j1 = n-1-i1;
            i3 = i;
            j3 = n-1-i3;
            //assert(j3 >= 0);
          }

          l0 = j0+i0*(2*n-i0+1)/2;
          l1 = j1+i1*(2*n-i1+1)/2;
          l2 = j2+i2*(2*n-i2+1)/2;
          v0.x = a.x+i0*u.x+j0*v.x;
          v0.y = a.y+i0*u.y+j0*v.y;
          v0.z = a.z+i0*u.z+j0*v.z;
          v1.x = a.x+i1*u.x+j1*v.x;
          v1.y = a.y+i1*u.y+j1*v.y;
          v1.z = a.z+i1*u.z+j1*v.z;
          v2.x = a.x+i2*u.x+j2*v.x;
          v2.y = a.y+i2*u.y+j2*v.y;
          v2.z = a.z+i2*u.z+j2*v.z;

          if (i0 != i1 || j0 != j1) /* avoid possible degenerate */
          {
            vertex[0] = v0.x;
            vertex[1] = v0.y;
            vertex[2] = v0.z;

            Points->InsertPoint(index,vertex);
            ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[l0]);//?
            ScalarsConst->InsertTuple1( index , 1.0 );

            vertex[0] = v1.x;
            vertex[1] = v1.y;
            vertex[2] = v1.z;

            Points->InsertPoint(index,vertex);
            ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[l1]);//?
            ScalarsConst->InsertTuple1( index , 1.0 );

            vertex[0] = v2.x;
            vertex[1] = v2.y;
            vertex[2] = v2.z;

            Points->InsertPoint(index,vertex);
            ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[l2]);//?
            ScalarsConst->InsertTuple1( index , 1.0 );

            f[0] = index - 3;
            f[1] = index - 2;
            f[2] = index - 1;

            Polys->InsertNextCell(3,f);
          }

          if (i3+j3 < n)
          {
            l3 = j3+i3*(2*n-i3+1)/2;
            v3.x = a.x+i3*u.x+j3*v.x;
            v3.y = a.y+i3*u.y+j3*v.y;
            v3.z = a.z+i3*u.z+j3*v.z;

            vertex[0] = v3.x;
            vertex[1] = v3.y;
            vertex[2] = v3.z;

            Points->InsertPoint(index,vertex);
            ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[l3]);//?
            ScalarsConst->InsertTuple1( index , 1.0 );

            vertex[0] = v2.x;
            vertex[1] = v2.y;
            vertex[2] = v2.z;

            Points->InsertPoint(index,vertex);
            ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[l2]);
            ScalarsConst->InsertTuple1( index , 1.0 );

            vertex[0] = v1.x;
            vertex[1] = v1.y;
            vertex[2] = v1.z;

            Points->InsertPoint(index,vertex);
            ScalarsError->InsertTuple1(index++, m_ModelError1.fe[k].serror[l1]);
            ScalarsConst->InsertTuple1( index , 1.0 );

            f[0] = index - 3;
            f[1] = index - 2;
            f[2] = index - 1;

            Polys->InsertNextCell(3,f);
          }
        }
      }
    }
  }

  m_FinalData -> SetPoints( Points );
  m_FinalData -> SetPolys( Polys );

  ScalarsError -> SetName( "Error" );
  ScalarsConst -> SetName( "Original" );

  m_FinalData -> GetPointData() -> AddArray( ScalarsError );
  m_FinalData -> GetPointData() -> AddArray( ScalarsConst );
}

void meshValmet::CreateLutError()
{
    m_Lut = vtkSmartPointer <vtkColorTransferFunction>::New();

    m_Lut -> SetColorSpaceToRGB();

    if( m_Pargs.signeddist == true )
    {
        if( m_Dmin < - m_Delta && -m_Delta < 0 && 0 < m_Delta && m_Delta < m_Dmax )
        {
           m_Lut -> AddRGBSegment( m_Dmin , 0 , 0 , 1 , m_Center-m_Delta , 0 , 1 , 1 );
           m_Lut -> AddRGBSegment( m_Center-m_Delta , 0 , 1 , 1 , m_Center  , 0 , 1 , 0 );
           m_Lut -> AddRGBSegment( m_Center , 0 , 1 , 0 , m_Center+m_Delta , 1 , 1 , 0 );
           m_Lut -> AddRGBSegment( m_Center+m_Delta , 1 , 1 , 0 , m_Dmax , 1 , 0 , 0 );
        }
    }
    else
    {
        if( m_Dmin < m_Delta && m_Delta < m_Dmax )
        {
           m_Lut -> AddRGBSegment( m_Dmin , 0 , 1 , 0 , m_Dmin+m_Delta , 1 , 1 , 0 );
           m_Lut -> AddRGBSegment( m_Dmin+m_Delta , 1 , 1 , 0 , m_Dmax , 1 , 0 , 0 );
        }
    }
}

int meshValmet::testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData )
{
    // comparer pointeurs
    std::cout << "          -number of points  " << std::endl;
    if( outData -> GetNumberOfPoints() != inData -> GetNumberOfPoints() )
    {
        std::cout << " ERROR : number of points " << std::endl;
        return 1;
    }

    std::cout << "          -number of cells  " << std::endl;
    if( outData -> GetNumberOfCells() != inData -> GetNumberOfCells() )
    {
        std::cout << " ERROR : number of cells " << std::endl;
        return 1;
    }
    vtkIdType b = outData -> GetNumberOfCells();


    std::cout << "          -get points  " << std::endl;
    vtkIdType Id = 0;
    double inV[3];
    double outV[3];

    for( Id = 0 ; Id < inData -> GetNumberOfPoints() ; Id++ )
    {
        inData -> GetPoints() -> GetPoint( Id , inV );
        outData -> GetPoints() -> GetPoint( Id , outV );
        if( inV[0] != outV[0] || inV[1] != outV[1] || inV[2] != outV[2] )
        {
            std::cout << " ERROR :  points " << std::endl;
            return 1;
        }

    }

    std::cout << "          -get cells  " << std::endl;
    vtkSmartPointer <vtkIdList> inF = vtkSmartPointer <vtkIdList>::New();
    vtkSmartPointer <vtkIdList> outF = vtkSmartPointer <vtkIdList>::New();
    vtkSmartPointer <vtkCellArray> inPolys = vtkSmartPointer <vtkCellArray> ::New();
    vtkSmartPointer <vtkCellArray> outPolys = vtkSmartPointer <vtkCellArray> ::New();
    inPolys = inData -> GetPolys();
    outPolys = outData -> GetPolys();

    for( Id = 0 ; Id < b ; Id++ )
    {
        inPolys -> GetCell( Id , inF );
        outPolys -> GetCell( Id , outF );

        vtkIdType c = inF->GetNumberOfIds();
        vtkIdType d = outF->GetNumberOfIds();
        if( c!= d && c != 3 )
        {
            return 1 ;
        }
        for( vtkIdType i = 0 ; i < c ; i++ )
        {
            if( inF->GetId(i) != outF->GetId(i) )
            {
                std::cout << " ERROR :  cells " << std::endl;
                return 1;
            }
        }
    }
    return 0;
}

