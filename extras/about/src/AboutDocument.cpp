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
* Description:  Application Document.
*
*/


// INCLUDE FILES
#include "AboutDocument.h"
#include "AboutAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// constructor
CAboutDocument::CAboutDocument( CEikApplication& aApp ) : CAknDocument( aApp )
    {
    }

// destructor
CAboutDocument::~CAboutDocument()
    {
    }

// Two-phased constructor.
CAboutDocument* CAboutDocument::NewL( CEikApplication& aApp )
    {
    return new( ELeave ) CAboutDocument( aApp );
    }
    
// -----------------------------------------------------------------------------
// CAboutDocument::CreateAppUiL()
// constructs CAboutAppUi
// -----------------------------------------------------------------------------
//
CEikAppUi* CAboutDocument::CreateAppUiL()
    {
    return new( ELeave ) CAboutAppUi;
    }

// End of File  
