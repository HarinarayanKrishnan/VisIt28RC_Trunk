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

#include <avtLabelsMapper.h>
#include <avtLabelRenderer.h>

#include <vtkObjectFactory.h>

// ****************************************************************************
// Class: vtkUserDefinedMapperBridge2
//
// Purpose: 
//   Derives from vtkUserDefinedMapperBridge. Its job is to set the global
//   label for its renderer to the appropriate label associated with the
//   VTK dataset.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 3 17:59:56 PST 2005
//
// Modifications:
//   
//   Hank Childs, Tue Sep 26 14:08:00 PDT 2006
//   Remove class qualification for constructor, which xlC disliked.
//
// ****************************************************************************

class vtkUserDefinedMapperBridge2 : public vtkUserDefinedMapperBridge
{
public:
    vtkUserDefinedMapperBridge2() :
        vtkUserDefinedMapperBridge(), label()
    {
        rendererAction = RENDERER_ACTION_NOTHING;
    }

    virtual ~vtkUserDefinedMapperBridge2()
    {
    }

    static vtkUserDefinedMapperBridge2 *New(void)
    {
        //
        // First try to create the object from the vtkObjectFactory
        //
        vtkObject *ret = NULL;
        ret = vtkObjectFactory::CreateInstance("vtkUserDefinedMapperBridge2");
        if (ret == NULL)
        {
            ret = new vtkUserDefinedMapperBridge2;
        }

        return (vtkUserDefinedMapperBridge2 *) ret;
    }

    void SetLabel(const std::string &L)
    {
        label = L;
    }

    void SetRendererAction(int a)
    {
        rendererAction = a;
    }

    virtual void Render(vtkRenderer *r, vtkActor *a)
    {
        // Set the index of the dataset that we're rendering.
        avtLabelRenderer *LR = (avtLabelRenderer *)*ren;
        LR->SetGlobalLabel(label);
        LR->SetRendererAction(rendererAction);

        vtkUserDefinedMapperBridge::Render(r, a);
    }

private:
    std::string label;
    int         rendererAction;
};

// ****************************************************************************
// Method: avtLabelsMapper::avtLabelsMapper
//
// Purpose: 
//   Constructor for the avtLabelsMapper class.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 3 18:01:04 PST 2005
//
// Modifications:
//   
// ****************************************************************************

avtLabelsMapper::avtLabelsMapper(avtCustomRenderer_p r) :
    avtUserDefinedMapper(r)
{
}

// ****************************************************************************
// Method: avtLabelsMapper::~avtLabelsMapper
//
// Purpose: 
//   Destructor for the avtLabelsMapper class.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 3 18:01:21 PST 2005
//
// Modifications:
//   
// ****************************************************************************

avtLabelsMapper::~avtLabelsMapper()
{
}

// ****************************************************************************
// Method: avtLabelsMapper::CreateMapper
//
// Purpose: 
//   Creates a vtkUserDefinedMapperBridge2 mapper.
//
// Returns:    A pointer to the new mapper.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 3 18:01:40 PST 2005
//
// Modifications:
//   
// ****************************************************************************

vtkDataSetMapper *
avtLabelsMapper::CreateMapper(void)
{
    vtkUserDefinedMapperBridge2 *rv = vtkUserDefinedMapperBridge2::New();
    rv->SetRenderer(renderer);

    return rv;
}

// ****************************************************************************
// Method: avtLabelsMapper::CustomizeMappers
//
// Purpose: 
//   Sets the label to be used for labelling a particular dataset in the
//   mapper, which will then set the label into the renderer.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 3 18:02:05 PST 2005
//
// Modifications:
//   
// ****************************************************************************

void
avtLabelsMapper::CustomizeMappers(void)
{
    avtUserDefinedMapper::CustomizeMappers();

    vtkDataSetMapper *firstMapper = NULL;
    vtkDataSetMapper *lastMapper = NULL;

    for (int i = 0 ; i < nMappers ; i++)
    {
        if (mappers[i] != NULL)
        {
            if(firstMapper == NULL)
                firstMapper = mappers[i];
            lastMapper = mappers[i];

            vtkUserDefinedMapperBridge2 *M =
               (vtkUserDefinedMapperBridge2 *)mappers[i];
            if((size_t)i < labelNames.size())
                M->SetLabel(labelNames[i]);
            M->SetRendererAction(RENDERER_ACTION_NOTHING);
        }
    }

    // Set some "renderer action" flags
    if(firstMapper != NULL)
    {
        vtkUserDefinedMapperBridge2 *fM =
            (vtkUserDefinedMapperBridge2 *)firstMapper;

        if(firstMapper == lastMapper)
        { 
            fM->SetRendererAction(RENDERER_ACTION_INIT_ZBUFFER |
                                  RENDERER_ACTION_FREE_ZBUFFER);
        }
        else
        {
            vtkUserDefinedMapperBridge2 *lM =
                (vtkUserDefinedMapperBridge2 *)lastMapper;
            fM->SetRendererAction(RENDERER_ACTION_INIT_ZBUFFER);
            lM->SetRendererAction(RENDERER_ACTION_FREE_ZBUFFER);
        }
    }
}

// ****************************************************************************
// Method: avtLabelsMapper::SetLabels
//
// Purpose: 
//   Sets the label names to use when drawing labels.
//
// Arguments:
//   L : The labels to use.
//
// Programmer: Brad Whitlock
// Creation:   Wed Aug 3 18:03:19 PST 2005
//
// Modifications:
//   
// ****************************************************************************

void
avtLabelsMapper::SetLabels(std::vector<std::string> &L, bool fromTree)
{
    if(fromTree)
    {
        // These are the actual labels present in the plot.
        labelNames = L;
    }
    // else all of the possible levels.
}
