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
* Description:  Declares UI class for application.
*
*/


#ifndef ABOUTAPPUI_H
#define ABOUTAPPUI_H

// INCLUDES
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>
#include <aknnavide.h>                 

// FORWARD DECLARATIONS
class CAboutContainer;

// CONSTANTS

// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
* 
*/
class CAboutAppUi : public CAknAppUi
    {
    public: // // Constructors and destructor

        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CAboutAppUi();
        
    public: // New functions

    public: // Functions from base classes

    private:
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL( TInt aCommand );
        
        /**
        * Set the name of the product to navi pane.
        */        
        void SetNaviPaneLabelL();

    private: // Data
        CAboutContainer* iAppContainer; 
        CAknNavigationDecorator* iNaviDecorator; 
        
        // record whether server is required or not.
        TBool iSettingType;
    };

#endif

// End of File
