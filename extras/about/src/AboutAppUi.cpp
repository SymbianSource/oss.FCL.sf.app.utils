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
* Description: Application UI. 
*
*/



// INCLUDE FILES
#include "AboutAppUi.h"
#include "AboutContainer.h" 
#include <avkon.hrh>
#include <sysversioninfo.h>                       

// local constant
const TUint32 KSettingAppID = 0x100058EC;

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CAboutAppUi::ConstructL()
// Default constructor
// ----------------------------------------------------------
//
void CAboutAppUi::ConstructL()
    {
    BaseConstructL();
    
    // Gets whether server is required or not.
    iSettingType = iEikonEnv->StartedAsServerApp();
    
    iAppContainer = new( ELeave ) CAboutContainer;
    iAppContainer->SetMopParent( this );
    iAppContainer->ConstructL( ClientRect() );
    AddToStackL( iAppContainer );
    SetNaviPaneLabelL();                                     
    }

// ----------------------------------------------------
// CAboutAppUi::~CAboutAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CAboutAppUi::~CAboutAppUi()
    {
    if ( iAppContainer )
        {
        RemoveFromStack( iAppContainer );
        delete iAppContainer;
        }
    if ( iNaviDecorator )
        {
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        }
    }

// ----------------------------------------------------
// CAboutAppUi::HandleCommandL( TInt aCommand )
// Takes care of command handling
// ----------------------------------------------------
//
void CAboutAppUi::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyExit:
        case EAknSoftkeyBack:
        	{
            Exit();
            break;        	
        	}
        case EEikCmdExit:
            {
            // Close Ctrl.panel after pressing call terminal key.
            if ( iSettingType )
                {
                RWsSession& ws = iCoeEnv->WsSession();
                TApaTaskList taskList( ws );
                TUid appUid = { KSettingAppID };
                TApaTask task = taskList.FindApp( appUid );
                if ( task.Exists() )
                    {
                    task.SendToBackground();
                    task.EndTask();
                    }
                }
            Exit();
            break;
            }

        default:
            break;
        }
    }

// ----------------------------------------------------
// CAboutAppUi::SetNaviPaneLabel()
// Set the name of the product to navi pane.
// ----------------------------------------------------
//    
void CAboutAppUi::SetNaviPaneLabelL()
    {
    TUid naviPaneUid;
    naviPaneUid.iUid = EEikStatusPaneUidNavi;
    CEikStatusPane* statusPane = StatusPane();
    CEikStatusPaneBase::TPaneCapabilities subPane = statusPane->PaneCapabilities( naviPaneUid );
  
    if ( subPane.IsPresent() && subPane.IsAppOwned() )
        {
        CAknNavigationControlContainer* naviPane = dynamic_cast<CAknNavigationControlContainer*>
                                ( statusPane->ControlL( naviPaneUid ) );
        if ( naviPane )
            {    
            TBuf<KSysVersionInfoTextLength> name;
            SysVersionInfo::TProductVersion info;
            SysVersionInfo::GetVersionInfo( info );
            name.Append( info.iModel );
            
            if ( iNaviDecorator )
                {
                delete iNaviDecorator;
                iNaviDecorator = NULL;
                }
           
            //set label for the pane
            iNaviDecorator = naviPane->CreateNavigationLabelL( name );      
            naviPane->PushL( *iNaviDecorator );      
            }
        }
    }
// End of File 
