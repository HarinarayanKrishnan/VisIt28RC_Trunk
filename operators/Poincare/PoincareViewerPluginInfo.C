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
//  File: PoincareViewerPluginInfo.C
// ************************************************************************* //

#include <PoincarePluginInfo.h>
#include <QApplication>
#include <PoincareAttributes.h>

// ****************************************************************************
//  Function:  GetViewerInfo
//
//  Purpose:
//    Return a new ViewerPluginInfo for the Poincare operator.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
extern "C" OP_EXPORT ViewerOperatorPluginInfo* Poincare_GetViewerInfo()
{
    PoincareViewerPluginInfo::InitializeGlobalObjects();
    return new PoincareViewerPluginInfo;
}

//
// Storage for static data elements.
//
PoincareAttributes *PoincareViewerPluginInfo::clientAtts = NULL;
PoincareAttributes *PoincareViewerPluginInfo::defaultAtts = NULL;

// ****************************************************************************
//  Method:  PoincareViewerPluginInfo::InitializeGlobalObjects
//
//  Purpose:
//    Initialize the operator atts.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
void
PoincareViewerPluginInfo::InitializeGlobalObjects()
{
    PoincareViewerPluginInfo::clientAtts  = new PoincareAttributes;
    PoincareViewerPluginInfo::defaultAtts = new PoincareAttributes;
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::GetClientAtts
//
//  Purpose:
//    Return a pointer to the viewer client attributes.
//
//  Returns:    A pointer to the viewer client attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

AttributeSubject *
PoincareViewerPluginInfo::GetClientAtts()
{
    return clientAtts;
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::GetDefaultAtts
//
//  Purpose:
//    Return a pointer to the viewer default attributes.
//
//  Returns:    A pointer to the viewer default attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

AttributeSubject *
PoincareViewerPluginInfo::GetDefaultAtts()
{
    return defaultAtts;
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::SetClientAtts
//
//  Purpose:
//    Set the viewer client attributes.
//
//  Arguments:
//    atts      A pointer to the new client attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
PoincareViewerPluginInfo::SetClientAtts(AttributeSubject *atts)
{
    *clientAtts = *(PoincareAttributes *)atts;
    clientAtts->Notify();
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::GetClientAtts
//
//  Purpose:
//    Get the viewer client attributes.
//
//  Arguments:
//    atts      A pointer to return the client default attributes in.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
PoincareViewerPluginInfo::GetClientAtts(AttributeSubject *atts)
{
    *(PoincareAttributes *)atts = *clientAtts;
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::InitializeOperatorAtts
//
//  Purpose:
//    Initialize the operator attributes to the default attributes.
//
//  Arguments:
//    atts        The attribute subject to initialize.
//    plot        The viewer plot that owns the operator.
//    fromDefault True to initialize the attributes from the defaults. False
//                to initialize from the current attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
PoincareViewerPluginInfo::InitializeOperatorAtts(AttributeSubject *atts,
                                              const ViewerPlot *plot,
                                              const bool fromDefault)
{
    if (fromDefault)
        *(PoincareAttributes*)atts = *defaultAtts;
    else
        *(PoincareAttributes*)atts = *clientAtts;

    UpdateOperatorAtts(atts, plot);
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::UpdateOperatorAtts
//
//  Purpose:
//    Update the operator attributes when using operator expressions.
//
//  Arguments:
//    atts        The attribute subject to update.
//    plot        The viewer plot that owns the operator.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
PoincareViewerPluginInfo::UpdateOperatorAtts(AttributeSubject *atts,
                                              const ViewerPlot *plot)
{
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::GetOperatorVarDescription
//
//  Purpose:
//    Return the operator variable description.
//
//  Arguments:
//    atts        The attribute subject to initialize.
//    plot        The viewer plot that owns the operator.
//
//  Programmer: Allen Sanderson
//  Creation:   19 August 2013
//
// ****************************************************************************

#include <ViewerPlot.h>

std::string
PoincareViewerPluginInfo::GetOperatorVarDescription(AttributeSubject *atts,
                                                    const ViewerPlot *plot)
{
    PoincareAttributes *atts_in = (PoincareAttributes *)atts;

    const char *typeString[14] =
      { "None", "Safety Factor Q", "Safety Factor P",
        "Safety Factor Q != P", "Safety Factor P != Q",
        "Toroidal Windings", "Poloidal Windings Q", "Poloidal Windings P",
        "Fieldline Order", "Point Order","Plane Order",
        "Winding Group Order", "Winding Point Order",
        "Winding Point Order Modulo" };

    std::string var = plot->GetVariableName();

    if( strncmp(var.c_str(), "operators/Poincare/",
                strlen("operators/Poincare/")) == 0)
    {
      std::string justTheVar = var.substr(strlen("operators/Poincare/"));

      var = justTheVar;
    }
    
    var += std::string(" - ") +
        std::string(typeString[atts_in->GetDataValue()]);

    return var;
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::GetMenuName
//
//  Purpose:
//    Return a pointer to the name to use in the viewer menus.
//
//  Returns:    A pointer to the name to use in the viewer menus.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

QString *
PoincareViewerPluginInfo::GetMenuName() const
{
    return new QString(qApp->translate("OperatorNames", "Poincare"));
}

// ****************************************************************************
//  Method: PoincareViewerPluginInfo::XPMIconData
//
//  Purpose:
//    Return a pointer to the icon data.
//
//  Returns:    A pointer to the icon data.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

#include <Poincare.xpm>
const char **
PoincareViewerPluginInfo::XPMIconData() const
{
    return Poincare_xpm;
}

