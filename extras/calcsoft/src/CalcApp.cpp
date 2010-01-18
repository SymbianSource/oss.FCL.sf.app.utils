/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  An application class, CCalcApplication, 
*                derived from CEikApplication
*
*/



// INCLUDE FILES 

#include    "CalcDoc.h"
#include    "CalcApp.h"

// EXTERNAL FUNCTION PROTOTYPES  

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CCalcApplication::CreateDocumentL
// This function is called by framework when this calculator is opened. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CApaDocument* CCalcApplication::CreateDocumentL()
    {
    return CCalcDocument::NewL(*this);
    }


// ---------------------------------------------------------
// CCalcApplication::AppDllUid
// This function is called by framework
// when this calculator is opened or closed. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid  CCalcApplication::AppDllUid() const
    {
    return KUidCalc;
    }


// ================= OTHER EXPORTED FUNCTIONS ==============

#include <eikstart.h>

LOCAL_C CApaApplication* NewApplication()
    {
    return new CCalcApplication;
    }

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }


