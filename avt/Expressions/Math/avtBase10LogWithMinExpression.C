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
//                         avtBase10LogWithMinExpression.C                          //
// ************************************************************************* //

#include <avtBase10LogWithMinExpression.h>

#include <vtkDataArray.h>
#include <vtkDataSet.h>

#include <avtExprNode.h>
#include <ExprToken.h>
#include <DebugStream.h>

#include <ExpressionException.h>


// ****************************************************************************
//  Method: avtBase10LogWithMinExpression constructor
//
//  Purpose:
//      Defines the constructor.  Note: this should not be inlined in the
//      header because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   February 5, 2004
//
// ****************************************************************************

avtBase10LogWithMinExpression::avtBase10LogWithMinExpression()
{
}


// ****************************************************************************
//  Method: avtBase10LogWithMinExpression destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   February 5, 2004
//
// ****************************************************************************

avtBase10LogWithMinExpression::~avtBase10LogWithMinExpression()
{
    ;
}


// ****************************************************************************
//  Method: avtBase10LogWithMinExpression::DoOperation
//
//  Purpose:
//      Performs the base-10 logarithm to each component,tuple of a data array.
//      Also checks against a minimum value.
//
//  Arguments:
//      in1           Input data array #1.
//      in2           Input data array #2.
//      out           The output data array.
//      ncomponents   The number of components ('1' for scalar, '2' or '3' for
//                    vectors, etc.)
//      ntuples       The number of tuples (ie 'npoints' or 'ncells')
//
//  Programmer: Hank Childs
//  Creation:   May 20, 2010
//
//  Modifications:
//
//    Hank Childs, Thu Sep 23 14:10:45 PDT 2010
//    Correct minimum test.
//
// ****************************************************************************

void
avtBase10LogWithMinExpression::DoOperation(vtkDataArray *in1,vtkDataArray *in2,
                                           vtkDataArray *out,
                                           int ncomponents, int ntuples)
{
    if (in2->GetNumberOfComponents() != 1)
    {
        EXCEPTION2(ExpressionException, outputVariableName,  
                   "The minimum value must be a scalar");
    }

    bool var1IsSingleton = (in1->GetNumberOfTuples() == 1);
    bool var2IsSingleton = (in2->GetNumberOfTuples() == 1);
    int in1ncomps = in1->GetNumberOfComponents();

    for (int i = 0 ; i < ntuples ; i++)
    {
        vtkIdType tup2 = (var2IsSingleton ? 0 : i);
        double f2 = in2->GetTuple1(tup2);
        for (int j = 0 ; j < in1ncomps ; j++)
        {
            vtkIdType tup1 = (var1IsSingleton ? 0 : i);
            double f1 = in1->GetComponent(tup1, j);
            double f = (f1 < f2 ? f2 : f1);
            out->SetComponent(i, j, log10(f));
        }
    }
}


