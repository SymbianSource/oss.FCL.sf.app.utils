/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for the About sub-folder
*
*/


#ifndef ABOUTGSPLUGIN_H
#define ABOUTGSPLUGIN_H

// Includes
#include <aknview.h>
#include <eikclb.h>
#include <gsplugininterface.h>
#include <gsfwviewuids.h>
#include <ConeResLoader.h>
#include <AknNullService.h>

// Classes referenced
class CAknViewAppUi;
class RConeResourceLoader;


// CLASS DECLARATION

/**
* CAboutGsPlugin view class.
* @since S60_5.0
*/
class CAboutGsPlugin : public CGSPluginInterface
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CAboutGsPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CAboutGsPlugin();

    public: // From CAknView

        /**
        * See base class.
        */
        TUid Id() const;

        /**
        * See base class.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        /**
        * See base class.
        */
        void DoDeactivate();

    public: // From CGSPluginInterface

        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;

        /**
        * @see CGSPluginInterface header file.
        */
        CGulIcon* CreateIconL( const TUid aIconType );

        /**
        * See base class.
        */
        TGSListboxItemTypes ItemType();

        /**
        * See base class.
        */
        void HandleSelection(const TGSSelectionTypes aSelectionType );
        
    protected: // New
        /**
        * C++ default constructor.
        */
        CAboutGsPlugin();
    
     private:
        /**
        * Symbian OS default constructor.
        */
        void ConstructL();
        
        void LaunchAboutAppL();
        void OpenLocalizedResourceFileL();
         
    private: // Data

        // Resource loader.
        RConeResourceLoader iPlgResourceLoader;
        CAknNullService *iNullService;
    };

#endif //ABOUTGSPLUGIN_H

// End of File
