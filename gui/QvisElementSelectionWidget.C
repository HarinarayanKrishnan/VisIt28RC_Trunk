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

#include <QvisElementSelectionWidget.h>
#include <QvisPeriodicTableWidget.h>
#include <QApplication>
#include <QLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>

// ****************************************************************************
// Method: QvisElementSelectionWidget::QvisElementSelectionWidget
//
// Purpose: 
//   This is the constructor for the QvisElementSelectionWidget class.
//
// Arguments:
//   parent : The widget's parent.
//   name   : The name of the instance.
//   f      : The window flags.
//
// Note:       This widget acts as a layout for its children since it 
//   positions the children manually and dispatches mouse events to them.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
//   Brad Whitlock, Tue Apr  8 09:27:26 PDT 2008
//   Support for internationalization.
//
//   Brad Whitlock, Tue Jun  3 14:44:38 PDT 2008
//   Qt 4.
//
// ****************************************************************************

QvisElementSelectionWidget::QvisElementSelectionWidget(QWidget *parent,
    Qt::WindowFlags f) : QWidget(parent, f)
{
    // Create the timer.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(hide()));

    // Create the periodic table widget.
    periodicTable = new QvisPeriodicTableWidget(this);
    periodicTable->setFrame(true);
    periodicTable->move(0, 0);
    periodicTable->resize(periodicTable->sizeHint());
    connect(periodicTable, SIGNAL(selectedElement(int)),
            this, SLOT(handleSelectedElement(int)));

    // Create the "Match any element" button
    matchAnyElementButton = new QPushButton(tr("Match any element"), this);
    matchAnyElementButton->move(0, periodicTable->sizeHint().height());
    matchAnyElementButton->resize(periodicTable->sizeHint().width(),
                                  matchAnyElementButton->sizeHint().height());
    connect(matchAnyElementButton, SIGNAL(clicked()),
            this, SLOT(matchAnyElementClicked()));

    // Turn on mouse tracking for this widget so we can dispatch mouse
    // events to child widgets.
    setMouseTracking(true);
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::~QvisElementSelectionWidget
//
// Purpose: 
//   This is the destructor for the QvisElementSelectionWidget class.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

QvisElementSelectionWidget::~QvisElementSelectionWidget()
{
    // nothing here
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::sizeHint
//
// Purpose: 
//   Calculates the widget's preferred size.
//
// Returns:    The widget's preferred size.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

QSize
QvisElementSelectionWidget::sizeHint() const
{
    QSize s1 = periodicTable->sizeHint();
    QSize s2 = matchAnyElementButton->sizeHint();

    // Find the largest width.
    int maxWidth = s1.width();
    if (s2.width() > maxWidth)
        maxWidth = s2.width();

    return QSize(maxWidth, s1.height() + s2.height());
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::setSelectedElement
//
// Purpose: 
//   Tries to select the specified element in the periodic table widget.
//
// Arguments:
//   element : The element to select.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

void
QvisElementSelectionWidget::setSelectedElement(int element)
{
    periodicTable->setSelectedElement(element);
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::mouseMoveEvent
//
// Purpose: 
//   This method is called in response to mouse movement in this widget. This
//   method's job is to dispatch the event to the proper child widget.
//
// Arguments:
//   e : The mouse event.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

void
QvisElementSelectionWidget::mouseMoveEvent(QMouseEvent *e)
{
    QRect standardRect(periodicTable->x(),
                       periodicTable->y(),
                       periodicTable->width(),
                       periodicTable->height());

    // If the mouse is in the periodic table gridd, forward the event
    if (standardRect.contains(QPoint(e->x(), e->y())))
    {
        QApplication::sendEvent(periodicTable, e);
    }
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::mouseReleaseEvent
//
// Purpose: 
//   This method is called in response to the mouse button being released in
//   this widget. This method's job is to dispatch the event to the proper
//   child widget.
//
// Arguments:
//   e : The mouse event.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

void
QvisElementSelectionWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QRect standardRect(periodicTable->x(),
                       periodicTable->y(),
                       periodicTable->width(),
                       periodicTable->height());
    QRect buttonRect(matchAnyElementButton->x(),
                     matchAnyElementButton->y(),
                     matchAnyElementButton->width(),
                     matchAnyElementButton->height());

    // If the mouse is in the grid, forward the event
    if (standardRect.contains(e->pos()))
    {
        QApplication::sendEvent(periodicTable, e);
    }
    else if (buttonRect.contains(e->pos()))
    {
        // Pretend that the button was clicked.
        matchAnyElementClicked();
    }
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::matchAnyElementClicked
//
// Purpose: 
//   This selects the special wildcard element (index == -1).
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

void
QvisElementSelectionWidget::matchAnyElementClicked()
{
    // Hide the widget
    hide();
 
    emit selectedElement(-1);
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::show
//
// Purpose: 
//   This is a Qt slot function that shows the widget.
//
// Note:       Starts a timer to hide the widget.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
//   Brad Whitlock, Tue Jun  3 14:46:54 PDT 2008
//   Qt 4.
//
// ****************************************************************************

void
QvisElementSelectionWidget::show()
{
    QWidget::show();
    timer->setSingleShot(true);
    timer->start(15000);
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::hide()
//
// Purpose: 
//   This is a Qt slot function that hides the widget.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

void
QvisElementSelectionWidget::hide()
{
    QWidget::hide();
    timer->stop();
}

// ****************************************************************************
// Method: QvisElementSelectionWidget::handleSelectedElement
//
// Purpose: 
//   This is a Qt slot function that is called when an element is selected
//   using the periodic table grid.
//
// Arguments:
//   element : The selected element.
//
// Programmer: Jeremy Meredith
// Creation:   February 11, 2008
//
// Modifications:
// ****************************************************************************

void
QvisElementSelectionWidget::handleSelectedElement(int element)
{
    // Make sure the widget hides.
    hide();

    // Emit the element changed signal.
    emit selectedElement(element);
}


// ****************************************************************************
//  Method:  QvisElementSelectionWidget::setHintedElements
//
//  Purpose:
//    Accept a list of atomic numbers that we will use for hinting.
//
//  Arguments:
//    elements   the list of atomic numbers
//
//  Programmer:  Jeremy Meredith
//  Creation:    February 12, 2008
//
// ****************************************************************************

void
QvisElementSelectionWidget::setHintedElements(const std::vector<int> &elements)
{
    periodicTable->setHintedElements(elements);
}
