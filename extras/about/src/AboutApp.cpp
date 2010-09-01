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
* Description:  About Application
*
*/


// INCLUDE FILES
#include "AboutApp.h"
#include "AboutDocument.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CAboutApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------
//
TUid CAboutApp::AppDllUid() const
    {
    return KUidabout;
    }

   
// ---------------------------------------------------------
// CAboutApp::CreateDocumentL()
// Creates CAboutDocument object
// ---------------------------------------------------------
//
CApaDocument* CAboutApp::CreateDocumentL()
    {
    return CAboutDocument::NewL( *this );
    }

#include <eikstart.h>

LOCAL_C CApaApplication* NewApplication()
    {
    return new CAboutApp;
    }

// ---------------------------------------------------------
// E32Main()
//  Main startup entry point
// Returns: KErrNone
// ---------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

// End of File  

