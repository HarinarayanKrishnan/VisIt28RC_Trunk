/*****************************************************************************
*
* Copyright (c) 2000 - 2014, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                         avtCondenseDatasetFilter.C                        //
// ************************************************************************* //

#include <avtCondenseDatasetFilter.h>

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataRelevantPointsFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridRelevantPointsFilter.h>

#include <DebugStream.h>


// ****************************************************************************
//  Method: avtCondenseDatasetFilter constructor
//
//  Programmer: Kathleen Bonnell 
//  Creation:   November 07, 2000
//
//  Modifications:
//    Kathleen Bonnell, Wed Nov 12 18:26:21 PST 2003
//    Initialize keepAVTandVTK.
//
//    Kathleen Bonnell, Wed Apr 14 17:51:36 PDT 2004 
//    Initialize bypassHeuristic.
//
//    David Camp, Thu May 23 12:52:53 PDT 2013
//    Removed the rpfPD and rpfUG variables for thread safety.
//
// ****************************************************************************

avtCondenseDatasetFilter::avtCondenseDatasetFilter()
{
    keepAVTandVTK = false;
    bypassHeuristic = false;
}


// ****************************************************************************
//  Method: avtCondenseDatasetFilter destructor
//
//  Programmer: Kathleen Bonnell
//  Creation:   November 07, 2000
//
//  Modifications:
//
//    David Camp, Thu May 23 12:52:53 PDT 2013
//    Removed the rpfPD and rpfUG variables for thread safety.
//
// ****************************************************************************

avtCondenseDatasetFilter::~avtCondenseDatasetFilter()
{
}


// ****************************************************************************
//  Method: avtCondenseDatasetFilter::ExecuteData
//
//  Purpose:
//      Sends the specified input and output through the CondenseDataset filter.
//
//  Arguments:
//      in_dr      The input data representation.
//
//  Returns:       The output data representation.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   November 07, 2000
//
//  Modifications:
//    Kathleen Bonnell, Fri Mar  2 17:31:01 PST 2001
//    Added check for input datasets with no cells
//
//    Kathleen Bonnell, Tue Apr 10 10:49:10 PDT 2001 
//    Renamed method from ExecuteDomain to ExecuteData. 
//
//    Hank Childs, Thu Sep 20 17:53:57 PDT 2001
//    Added heuristic to not apply the filter when it won't rule out very many
//    irrelevant points.
//
//    Hank Childs, Fri Jan 25 10:18:32 PST 2002
//    Fix memory leak.
//
//    Hank Childs, Fri Jul 25 21:25:31 PDT 2003
//    Removed unneeded variables.
//
//    Kathleen Bonnell, Wed Nov 12 18:26:21 PST 2003 
//    Allow AVT and VTK variables to be kept if requested. 
//
//    Kathleen Bonnell, Wed Apr 14 17:51:36 PDT 2004 
//    Allow execution of relevant points filter to be forced, bypassing
//    heurisitic. 
//
//    Kathleen Bonnell, Tue Oct 12 16:06:20 PDT 2004
//    Allow avtOriginalNodeNumbers and avtOriginalCellNumbers to be kept
//    around, independent of the value of keepAVTandVTK.  Needed by pick for
//    Vector Plots and Point Meshes.
//
//    Brad Whitlock, Wed Dec 22 11:20:42 PDT 2004
//    Added an heuristic to skip relevant points for line data that has 
//    many more points than cells. Also set the flag to skip relevant points
//    for point meshes so it agrees with the debug log message about skipping
//    relevant points.
//
//    Hank Childs, Wed Dec 27 10:27:48 PST 2006
//    Allow ghost data to pass through for structured grids.
//
//    David Camp, Thu May 23 12:52:53 PDT 2013
//    Changed function to be thread safe. I remove the class variable rpfPD
//    and rpfUG. Now I allocate the needed VTK object during execution.
//
//    Burlen Loring, Mon Apr 14 15:56:19 PDT 2014
//    Don't take two references to the output
//
//    Eric Brugger, Fri Jul 18 16:07:04 PDT 2014
//    Modified the class to work with avtDataRepresentation.
//
// ****************************************************************************

avtDataRepresentation *
avtCondenseDatasetFilter::ExecuteData(avtDataRepresentation *in_dr)
{
    int i;

    //
    // Get the VTK data set.
    //
    vtkDataSet *in_ds = in_dr->GetDataVTK();

    // Remove any variable that has "VTK" or "AVT" in its name.  Make a for
    // loop with two iterations.  Iteration #1 is to determine if we have
    // to strip out arrays.  Iteration #2 is to strip out the arrays.  The
    // purpose for putting this in a for loop is reduce duplicate code.
    // The reason not to just always strip the arrays is that it all of the
    // copies of the data can really slow things down.
    //
    vtkDataSet *no_vars = in_ds;
    bool needToDeleteNoVars = false;
    bool needToRemoveAVar = true;
    for (int j = 0 ; (j < 2) && needToRemoveAVar ; j++)
    {
        if (j == 0)
            needToRemoveAVar = false; // The purpose of this iteration
                                      // is to set this variable.
        else if (j == 1)
        {
            // We're still here, so that means we do need to strip out
            // the arrays.
            no_vars = (vtkDataSet *) in_ds->NewInstance();
            no_vars->ShallowCopy(in_ds);
            needToDeleteNoVars = true;
        }

        if (!keepAVTandVTK)
        {
            bool keepNodeZone = 
            GetInput()->GetInfo().GetAttributes().GetKeepNodeZoneArrays();
            for (i = no_vars->GetPointData()->GetNumberOfArrays()-1 ; i >= 0 ; i--)
            {
                vtkDataArray *arr = no_vars->GetPointData()->GetArray(i);
                const char *name = arr->GetName();
                if (name == NULL)
                    continue;
                if (strstr(name, "vtk") != NULL)
                    no_vars->GetPointData()->RemoveArray(name);
                else if (strstr(name, "avt") != NULL)
                {
                    if (keepNodeZone && 
                        ((strcmp(name, "avtOriginalNodeNumbers") == 0) ||
                         (strcmp(name, "avtOriginalCellNumbers") == 0)))
                        continue;
                    else if (strcmp(name, "avtGhostNodes") == 0 &&
                         (in_ds->GetDataObjectType() == VTK_RECTILINEAR_GRID ||
                          in_ds->GetDataObjectType() == VTK_STRUCTURED_GRID))
                        continue;
                    else
                    {
                        if (j == 0)
                            needToRemoveAVar = true;
                        else
                            no_vars->GetPointData()->RemoveArray(name);
                    }
                }
            }
            for (i = no_vars->GetCellData()->GetNumberOfArrays()-1 ; i >= 0 ; i--)
            {
                vtkDataArray *arr = no_vars->GetCellData()->GetArray(i);
                const char *name = arr->GetName();
                if (name == NULL)
                    continue;
                if (strstr(name, "vtk") != NULL)
                    no_vars->GetCellData()->RemoveArray(name);
                else if (strstr(name, "avt") != NULL)
                {
                    if (keepNodeZone && 
                        ((strcmp(name, "avtOriginalNodeNumbers") == 0) ||
                         (strcmp(name, "avtOriginalCellNumbers") == 0)))
                        continue;
                    else if (strcmp(name, "avtGhostZones") == 0 &&
                         (in_ds->GetDataObjectType() == VTK_RECTILINEAR_GRID ||
                          in_ds->GetDataObjectType() == VTK_STRUCTURED_GRID))
                        continue;
                    else
                    {
                        if (j == 0)
                            needToRemoveAVar = true;
                        else
                            no_vars->GetCellData()->RemoveArray(name);
                    }
                }
            }
        }
    }
 
    //
    // Decide whether or not we need to determine the relevant points.
    //
    int  nPoints = no_vars->GetNumberOfPoints();
    int  nCells  = no_vars->GetNumberOfCells();
    bool shouldTakeRelevantPoints = true;
    if (!bypassHeuristic && 2*nCells > nPoints)
    {
        debug5 << "Relevant points filter stopped by heuristic.  Points = "
               << nPoints << ", cells = " << nCells << endl;
        shouldTakeRelevantPoints = false;
    }
    else if (!bypassHeuristic &&
             GetInput()->GetInfo().GetAttributes().GetTopologicalDimension() 
             == 0)
    {
        debug5 << "Not taking relevant points because we have a point mesh."
               << endl;
        shouldTakeRelevantPoints = false;
    }
    else if (!bypassHeuristic &&
             GetInput()->GetInfo().GetAttributes().GetTopologicalDimension()==1&&
             nPoints > (nCells*5))
    {
        debug5 << "Not taking relevant points because we have a mesh "
                  "consisting of lines and the number of points is more "
                  "than 5x the number of cells. This is common with "
                  "streamlines and we need to skip because the relevant "
                  "points filter tends to die right now with line data of "
                  "this kind."
               << endl;
        shouldTakeRelevantPoints = false;
    }
    else
    {
        if (!bypassHeuristic)
        {
            debug5 << "Relevant points filter allowed to execute by heuristic.  " 
                   << "Points = " << nPoints << ", cells = " << nCells << endl;
        }
        else 
        {
            debug5 << "Relevant points filter forced to execute." << endl; 
        }
    }

    vtkDataSet *out_ds = NULL;  
    bool needToDeleteOutDs = false;
    if (shouldTakeRelevantPoints)
    {
        vtkPolyData *out_pd = NULL;
        vtkUnstructuredGrid *out_ug = NULL;

        if (no_vars->GetNumberOfCells() == 0)
        {
            out_ds = no_vars;
        }
        else
        {
            switch(no_vars->GetDataObjectType())
            {
                case VTK_POLY_DATA:
                {
                    vtkPolyDataRelevantPointsFilter *rpfPD = vtkPolyDataRelevantPointsFilter::New();
                    rpfPD->SetInputData((vtkPolyData*)no_vars);
                    out_pd = vtkPolyData::New();
                    rpfPD->SetOutput(out_pd);
                    rpfPD->Update();
                    rpfPD->Delete();
                    out_ds = (vtkDataSet*)out_pd;
                    needToDeleteOutDs = true;
                    break;
                }
    
                case VTK_UNSTRUCTURED_GRID:
                {
                    vtkUnstructuredGridRelevantPointsFilter *rpfUG = vtkUnstructuredGridRelevantPointsFilter::New();
                    rpfUG->SetInputData((vtkUnstructuredGrid*)no_vars);
                    out_ug = vtkUnstructuredGrid::New();
                    rpfUG->SetOutput(out_ug);
                    rpfUG->Update();
                    rpfUG->Delete();
                    out_ds = (vtkDataSet*)out_ug;
                    needToDeleteOutDs = true;
                    break;
                }
    
                default :
                // We don't know what type this is.  It is probably a mistake that
                // this was called, so minimize the damage by passing this through.
                    out_ds = no_vars;
                    break;
            }
        }
    }
    else
    {
        out_ds = no_vars;
    }

    avtDataRepresentation *out_dr = new avtDataRepresentation(out_ds,
        in_dr->GetDomain(), in_dr->GetLabel());

    if (needToDeleteOutDs)
        out_ds->Delete();

    if (needToDeleteNoVars)
        no_vars->Delete();

    return out_dr;
}


// ****************************************************************************
//  Method: avtCondenseDatasetFilter::ReleaseData
//
//  Purpose:
//      Releases all of the problem size data associated with this filter.
//
//  Programmer: Hank Childs
//  Creation:   September 10, 2002
//
//  Modifications:
//
//    Hank Childs, Fri Mar  4 08:12:25 PST 2005
//    Do not set outputs of filters to NULL, since this will prevent them
//    from re-executing correctly in DLB-mode.
//
//    Hank Childs, Fri Mar 11 07:37:05 PST 2005
//    Fix non-problem size leak introduced with last fix.
//
//    David Camp, Thu May 23 12:52:53 PDT 2013
//    Removed the rpfPD and rpfUG variables for thread safety.
//
// ****************************************************************************

void
avtCondenseDatasetFilter::ReleaseData(void)
{
    avtDataTreeIterator::ReleaseData();
}


// ****************************************************************************
//  Method:  avtCondenseDatasetFilter::FilterUnderstandsTransformedRectMesh
//
//  Purpose:
//    If this filter returns true, this means that it correctly deals
//    with rectilinear grids having an implied transform set in the
//    data attributes.  It can do this conditionally if desired.
//
//  Arguments:
//    none
//
//  Programmer:  Jeremy Meredith
//  Creation:    February 15, 2007
//
// ****************************************************************************
bool
avtCondenseDatasetFilter::FilterUnderstandsTransformedRectMesh()
{
    // the condense filter should need to do nothing special
    // to handle these
    return true;
}

