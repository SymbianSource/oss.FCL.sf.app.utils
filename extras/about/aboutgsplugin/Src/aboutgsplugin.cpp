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
* Description:  Source file for CAboutGsPlugin class
*
*/


#include <aknViewAppUi.h>
#include <ConeResLoader.h>
#include <barsread.h>       // For TResourceReader
#include <StringLoader.h>
#include <apgcli.h>
#include <AknLaunchAppService.h>
#include <bautils.h>

// Includes from GS framework:
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>


// Plugin includes:
#include <aboutgspluginrsc.rsg>
#include <aboutgsplugin.mbg>


// Includes
#include "aboutgsplugin.h"

#define ABOUTAPPUID  0x10005a22

const TUid KAboutGsPluginUid = { 0x102833A0 };

// svg icons
_LIT( KGSAboutGsPluginIconFileName, "\\resource\\apps\\aboutgsplugin.mif");

// ---------------------------------------------------------------------------
// CAboutGsPlugin::CAboutGsPlugin()
//
// ---------------------------------------------------------------------------
//
CAboutGsPlugin::CAboutGsPlugin()
    : iPlgResourceLoader( *iCoeEnv ), iNullService( NULL )
    {
    }

// ---------------------------------------------------------------------------
// CAboutGsPlugin::~CAboutGsPlugin()
//
// ---------------------------------------------------------------------------
//
CAboutGsPlugin::~CAboutGsPlugin()
    {
    iPlgResourceLoader.Close();
    if ( iNullService )
    	{
    	delete iNullService;
    	iNullService = NULL;
    	}
    }

// ---------------------------------------------------------------------------
// CAboutGsPlugin::ConstructL()
//
// ---------------------------------------------------------------------------
//
void CAboutGsPlugin::ConstructL()
    {
    OpenLocalizedResourceFileL();
    }


void CAboutGsPlugin::OpenLocalizedResourceFileL()
    {
    TFileName resourceFileName;
    TPtrC driveLetter = TParsePtrC( RProcess().FileName() ).Drive();
    _LIT( KResoureFileName, "aboutgspluginrsc.rsc" );
   
    resourceFileName.Copy( driveLetter );  
    resourceFileName.Append( KDC_ECOM_RESOURCE_DIR );
    resourceFileName.Append( KResoureFileName );

    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( resourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    iPlgResourceLoader.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close ( );
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::NewL()
//
// ---------------------------------------------------------------------------
//
CAboutGsPlugin* CAboutGsPlugin::NewL( TAny* /*aInitParams*/ )
  {
  	CAboutGsPlugin* self = new( ELeave ) CAboutGsPlugin();
  	CleanupStack::PushL( self );
  	self->ConstructL();
  	CleanupStack::Pop( self );
  	return self;
  }

// ---------------------------------------------------------------------------
// CGSParentPlugin::Id()
//
// ---------------------------------------------------------------------------
//
TUid CAboutGsPlugin::Id() const
    {
    return KAboutGsPluginUid;
    }

// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
// ---------------------------------------------------------------------------
//
void CAboutGsPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    }

// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
// ---------------------------------------------------------------------------
//
void CAboutGsPlugin::DoDeactivate()
    {
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::GetCaptionL()
//
// ---------------------------------------------------------------------------
//
void CAboutGsPlugin::GetCaptionL( TDes& aCaption ) const
	{
	HBufC* result = StringLoader::LoadL( R_ABOUT_GS_VIEW_CAPTION );
	aCaption.Copy( *result );
	delete result;
	result = NULL;
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::PluginProviderCategory()
//
// ---------------------------------------------------------------------------
//
TInt CAboutGsPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::CreateIconL
// Return the icon, if has one.
// ---------------------------------------------------------------------------
//
CGulIcon* CAboutGsPlugin::CreateIconL( const TUid aIconType )
    {
    CGulIcon* icon;

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpDevAbout,
        KGSAboutGsPluginIconFileName,
        EMbmAboutgspluginQgn_prop_cp_dev_about,
        EMbmAboutgspluginQgn_prop_cp_dev_about_mask );
        }
     else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    return icon;
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::ItemType
// ---------------------------------------------------------------------------
//
TGSListboxItemTypes CAboutGsPlugin::ItemType()
    {
    // See TGSListboxItemTypes for details.
    return EGSItemTypeSettingDialog;
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::HandleSelection
// ---------------------------------------------------------------------------
//
void CAboutGsPlugin::HandleSelection( 
        const TGSSelectionTypes /*aSelectionType*/ )
    {
    // This used as a call-back from paren plugin to handle selection. Because
    // this plugin is of type EGSItemTypeSettingDialog, DoActivateL is never 
    // called on selection but only HandleSelection. See TGSListboxItemTypes
    // for details.
    TRAP_IGNORE( LaunchAboutAppL() );
    }


// ---------------------------------------------------------------------------
// CAboutGsPlugin::LaunchAboutAppL
// ---------------------------------------------------------------------------
//
void CAboutGsPlugin::LaunchAboutAppL()
    {
    TUid  aboutAppUid;
    aboutAppUid.iUid = ABOUTAPPUID;
    iNullService = CAknNullService::NewL( aboutAppUid, NULL );
    }


//End of File
