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
* Description:  Application Document
*
*/


#ifndef ABOUTDOCUMENT_H
#define ABOUTDOCUMENT_H

// INCLUDES
#include <AknDoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CAboutDocument application class.
*/
class CAboutDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CAboutDocument* NewL( CEikApplication& aApp );

        /**
        * Destructor.
        */
        virtual ~CAboutDocument();

    private:

        /**
        * Constructor.
        */
        CAboutDocument( CEikApplication& aApp );

        /**
        * From CEikDocument, create CAboutAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

