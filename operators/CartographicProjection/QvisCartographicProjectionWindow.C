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

#include "QvisCartographicProjectionWindow.h"

#include <CartographicProjectionAttributes.h>
#include <ViewerProxy.h>

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QvisColorTableButton.h>
#include <QvisOpacitySlider.h>
#include <QvisColorButton.h>
#include <QvisLineStyleWidget.h>
#include <QvisLineWidthWidget.h>
#include <QvisVariableButton.h>

#include <stdio.h>
#include <string>

using std::string;

// ****************************************************************************
// Method: QvisCartographicProjectionWindow::QvisCartographicProjectionWindow
//
// Purpose: 
//   Constructor
//
// Note:       Autogenerated by xml2window.
//
// Programmer: Jean Favre
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

QvisCartographicProjectionWindow::QvisCartographicProjectionWindow(const int type,
                         CartographicProjectionAttributes *subj,
                         const QString &caption,
                         const QString &shortName,
                         QvisNotepadArea *notepad)
    : QvisOperatorWindow(type,subj, caption, shortName, notepad)
{
    atts = subj;
}


// ****************************************************************************
// Method: QvisCartographicProjectionWindow::~QvisCartographicProjectionWindow
//
// Purpose: 
//   Destructor
//
// Note:       Autogenerated by xml2window.
//
// Programmer: Jean Favre
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

QvisCartographicProjectionWindow::~QvisCartographicProjectionWindow()
{
}


// ****************************************************************************
// Method: QvisCartographicProjectionWindow::CreateWindowContents
//
// Purpose: 
//   Creates the widgets for the window.
//
// Note:       Autogenerated by xml2window.
//
// Programmer: Jean Favre
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
QvisCartographicProjectionWindow::CreateWindowContents()
{
    QGridLayout *mainLayout = new QGridLayout(0);
    topLayout->addLayout(mainLayout);

    projectionIDLabel = new QLabel(tr("Projection type"), central);
    mainLayout->addWidget(projectionIDLabel,0,0);
    int sp = 30+(int)mainLayout->verticalSpacing();
    mainLayout->setVerticalSpacing(sp);

    projectionIDCombo = new QComboBox(central);
    projectionIDCombo->addItem(tr("Aitoff"));
    projectionIDCombo->addItem(tr("Eckert IV"));
    projectionIDCombo->addItem(tr("Equidistant Conic"));
    projectionIDCombo->addItem(tr("Hammer"));
    projectionIDCombo->addItem(tr("Lambert Azimuthal Equal Area"));
    projectionIDCombo->addItem(tr("Lambert Conformal Conic"));
    projectionIDCombo->addItem(tr("Mercator"));
    projectionIDCombo->addItem(tr("Miller Cylindrical"));
    projectionIDCombo->addItem(tr("Mollweide"));
    projectionIDCombo->addItem(tr("Orthographic"));
    projectionIDCombo->addItem(tr("Winkel II"));

    connect(projectionIDCombo, SIGNAL(activated(int)),
            this, SLOT(projectionIDChanged(int)));
    mainLayout->addWidget(projectionIDCombo, 0,1);
    projectionIDCombo->setCurrentIndex(atts->GetProjectionID());

    centralMeridianLabel = new QLabel(tr("Central Meridian"), central);
    mainLayout->addWidget(centralMeridianLabel,1,0);
    centralMeridian = new QLineEdit(central);
    connect(centralMeridian, SIGNAL(returnPressed()),
            this, SLOT(centralMeridianProcessText()));
    mainLayout->addWidget(centralMeridian, 1,1);
}


// ****************************************************************************
// Method: QvisCartographicProjectionWindow::UpdateWindow
//
// Purpose: 
//   Updates the widgets in the window when the subject changes.
//
// Note:       Autogenerated by xml2window.
//
// Programmer: Jean Favre
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
QvisCartographicProjectionWindow::UpdateWindow(bool doAll)
{

    for(int i = 0; i < atts->NumAttributes(); ++i)
    {
        if(!doAll)
        {
            if(!atts->IsSelected(i))
            {
                continue;
            }
        }

        switch(i)
        {
          case CartographicProjectionAttributes::ID_projectionID:
            projectionIDCombo->blockSignals(true);
            projectionIDCombo->setCurrentIndex(atts->GetProjectionID());
            projectionIDCombo->blockSignals(false);
            break;
          case CartographicProjectionAttributes::ID_centralMeridian:
            centralMeridian->setText(DoubleToQString(atts->GetCentralMeridian()));
            break;
        }
    }
}


// ****************************************************************************
// Method: QvisCartographicProjectionWindow::GetCurrentValues
//
// Purpose: 
//   Gets values from certain widgets and stores them in the subject.
//
// Note:       Autogenerated by xml2window.
//
// Programmer: Jean Favre
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
QvisCartographicProjectionWindow::GetCurrentValues(int which_widget)
{
    bool doAll = (which_widget == -1);

    // Do centralMeridian
    if(which_widget == CartographicProjectionAttributes::ID_centralMeridian || doAll)
    {
        double val;
        if(LineEditGetDouble(centralMeridian, val))
            atts->SetCentralMeridian(val);
        else
        {
            ResettingError(tr("Central Meridian"),
                DoubleToQString(atts->GetCentralMeridian()));
            atts->SetCentralMeridian(atts->GetCentralMeridian());
        }
    }

}


//
// Qt Slot functions
//


void
QvisCartographicProjectionWindow::projectionIDChanged(int val)
{
    if(val != atts->GetProjectionID())
    {
        atts->SetProjectionID(CartographicProjectionAttributes::ProjectionID(val));
        SetUpdate(false);
        Apply();
    }
}

void
QvisCartographicProjectionWindow::centralMeridianProcessText()
{
    GetCurrentValues(CartographicProjectionAttributes::ID_centralMeridian);
    Apply();
}
