/* $Id: mesh_run.h,v 1.2 2006/10/18 17:38:10 xushun Exp $ */


/*
 *
 *  Copyright (C) 2001-2004 EPFL (Swiss Federal Institute of Technology,
 *  Lausanne) This program is free software; you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 *  USA.
 *
 *  In addition, as a special exception, EPFL gives permission to link
 *  the code of this program with the Qt non-commercial edition library
 *  (or with modified versions of Qt non-commercial edition that use the
 *  same license as Qt non-commercial edition), and distribute linked
 *  combinations including the two.  You must obey the GNU General
 *  Public License in all respects for all of the code used other than
 *  Qt non-commercial edition.  If you modify this file, you may extend
 *  this exception to your version of the file, but you are not
 *  obligated to do so.  If you do not wish to do so, delete this
 *  exception statement from your version.
 *
 *  Authors : Nicolas Aspert, Diego Santa-Cruz and Davy Jacquet
 *
 *  Web site : http://mesh.epfl.ch
 *
 *  Reference :
 *   "MESH : Measuring Errors between Surfaces using the Hausdorff distance"
 *   in Proceedings of IEEE Intl. Conf. on Multimedia and Expo (ICME) 2002, 
 *   vol. I, pp. 705-708, available on http://mesh.epfl.ch
 *
 */

#ifndef _MESH_RUN_PROTO
#define _MESH_RUN_PROTO

// MeshValmet Libraries
#include <MeshValmet/compute_error.h>
#include <MeshValmet/geomutils.h>
#include <MeshValmet/compute_volume_overlap.h>

// vtk Libraries
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
#include  <vtkTriangleFilter.h>

// Other Libraries
#include <time.h>
#include <string.h>


#ifdef __cplusplus
#define BEGIN_DECL extern "C" {
#define END_DECL }
#else
#define BEGIN_DECL
#define END_DECL
#endif

BEGIN_DECL
#undef BEGIN_DECL

#ifndef min
#  define min(__X, __Y) ((__X)<(__Y)?(__X):(__Y))
#endif
#ifndef max
#  define max(__X, __Y) ((__X)>(__Y)?(__X):(__Y))
#endif

/* To store the parsed arguments */
struct args {

  //char *m1_fname; // filename of model 1
  //char *m2_fname; // filename of model 2
  double sampling_step; // The sampling step, as fraction of the bounding box diagonal of model 2.
  bool signeddist;
  int min_sample_freq;  // Minimum sampling frequency to enforce on each triangle
  int quiet;      // do not display extra info flag
  int verb_analysis; // verbose analysis
  vtkSmartPointer <vtkPolyData> Data1;
  vtkSmartPointer <vtkPolyData> Data2;
};


/* Runs the mesh program, given the parsed arguments in *args. The models and
 * their respective errors are returned in *model1 and *model2. If
 * args->no_gui is zero a QT window is opened to display the visual
 * results. All normal (non error) output is printed through the output buffer
 * out. If not NULL the progress object is used to report the progress. */
void mesh_run(const struct args *args, struct model_error *model1,
              struct model_error *model2, /*vtkSmartPointer <vtkPolyData> Data1, vtkSmartPointer <vtkPolyData> Data2,*/
              struct outbuf *out,
              struct prog_reporter *progress, struct dist_surf_surf_stats *stats,
              struct dist_surf_surf_stats *stats_rev, double *abs_sampling_step,
              double *abs_sampling_dens);

model* ConvertDataToModel(vtkSmartPointer <vtkPolyData> data );
vtkSmartPointer <vtkPolyData> ConvertFileToData( char* File );

int testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData );

END_DECL
#undef END_DECL

#endif /* _MESH_RUN_PROTO */
