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

#include "UseNetworkRPC.h"
#include <DebugStream.h>


// ****************************************************************************
//  Constructor: UseNetworkRPC::UseNetworkRPC
//
//  Purpose: 
//    This is the constructor.
//
//  Arguments:
//
//  Programmer: Jeremy Meredith
//  Creation:   November  7, 2001
//
// ****************************************************************************

UseNetworkRPC::UseNetworkRPC() : BlockingRPC("i")
{
}

// ****************************************************************************
//  Destructor: UseNetworkRPC::~UseNetworkRPC
//
//  Purpose: 
//    This is the destructor.
//
//  Arguments:
//
//  Programmer: Jeremy Meredith
//  Creation:   November  7, 2001
//
// ****************************************************************************

UseNetworkRPC::~UseNetworkRPC()
{
}

// ****************************************************************************
//  Method: UseNetworkRPC::operator()
//
//  Purpose: 
//    This is the RPC's invocation method.
//
//  Arguments:
//    id        the network id
//
//  Programmer: Jeremy Meredith
//  Creation:   November  7, 2001
//
//  Modifications:
// ****************************************************************************

void
UseNetworkRPC::operator()(int id_)
{
    SetID(id_);

    Execute();
}

// ****************************************************************************
//  Method: UseNetworkRPC::SelectAll
//
//  Purpose: 
//    This selects all variables for sending.
//
//  Arguments:
//
//  Programmer: Jeremy Meredith
//  Creation:   November  7, 2001
//
// ****************************************************************************

void
UseNetworkRPC::SelectAll()
{
    Select(0, (void*)&id);
}


// ****************************************************************************
//  Method: UseNetworkRPC::SetID
//
//  Purpose: 
//    This sets the id parameter.
//
//  Arguments:
//    id_         the id
//
//  Programmer: Jeremy Meredith
//  Creation:   November  7, 2001
//
// ****************************************************************************

void
UseNetworkRPC::SetID(int id_)
{
    id = id_;
    Select(0, (void*)&id);
}


// ****************************************************************************
//  Method: UseNetworkRPC::GetID
//
//  Purpose: 
//    This returns network id.
//
//  Arguments:
//
//  Programmer: Jeremy Meredith
//  Creation:   November  7, 2001
//
// ****************************************************************************

int
UseNetworkRPC::GetID() const
{
    return id;
}

