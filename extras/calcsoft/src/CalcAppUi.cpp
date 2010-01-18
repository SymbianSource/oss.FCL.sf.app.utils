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
* Description:  An App UI class, CCalcAppUi, derived from CAknAppUi
*                Mainly menu command is handled.
*
*/



// INCLUDE FILES  
#include    <avkon.hrh>
#include    <bacntf.h>   // For CEnvironmentChangeNotifier
#include    <Calcsoft.rsg>
#include    <featmgr.h>

#include    "CalcApp.h"
#include    "CalcAppUi.h"
#include    "CalcDoc.h"
#include    "CalcHistory.h"
#include	 "CalcEnv.h"
#include    "CalcView.h"
#include    "calc.hrh"

//  LOCAL CONSTANTS AND MACROS  


// ================= MEMBER FUNCTIONS =======================


// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcAppUi::CCalcAppUi()
    {
    }

// Destructor
CCalcAppUi::~CCalcAppUi()
    {
		delete iCalcAppEnv;
		iCalcAppEnv = NULL;
    
    if (iLocaleChangeNotifier)
        {
        iLocaleChangeNotifier->Cancel();
        }
    
    if	(iLocaleChangeNotifier)
    	{
   		delete iLocaleChangeNotifier;
    	iLocaleChangeNotifier = NULL;
    	}
    FeatureManager::UnInitializeLib();
    }


// ---------------------------------------------------------
// CCalcAppUi::ExitCalculator
// Exit Calculator application.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcAppUi::ExitCalculator()
    {
    TRAP_IGNORE(CEikAppUi::SaveL()); // Save memory and last result
    Exit();
    }

// ---------------------------------------------------------
// CCalcAppUi::LocaleCallback
// Called when locale is changed or application is launched.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcAppUi::LocaleCallback
                    (TAny* aThisPtr)
    {
    STATIC_CAST(CCalcAppUi*, aThisPtr)->HandleLocaleChangeEvent();
    return 0;
    }

// ---------------------------------------------------------
// CCalcAppUi::HandleLocaleChangeEvent
// Handle locale change event.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcAppUi::HandleLocaleChangeEvent()
    {
    TLocale locale;
    TChar newDecimal(locale.DecimalSeparator());
    TChar oldDecimal(iCalcAppEnv->DecimalSeparator());

    if (newDecimal != oldDecimal)
        {
        iCalcAppEnv->SetDecimalSeparator(newDecimal);
        CCalcView* view = STATIC_CAST(CCalcView*, iView);
        view->NotifyChangeDecimal(oldDecimal, newDecimal);
        CCalcDocument* doc = STATIC_CAST(CCalcDocument*, Document());
        doc->History()->NotifyChangeDecimal(oldDecimal, newDecimal);
        }
    }

// Second-phase constructor.
void CCalcAppUi::ConstructL()
    {
    // Allow base class (CEikAppUi) to perform necessary construction
    BaseConstructL(CAknAppUi::EAknEnableSkin | EAknEnableMSK);

    FeatureManager::InitializeLibL();
    
    iCalcAppEnv = CCalcAppEnv::NewL(this);
    
    CCalcView* calcView = CCalcView::NewLC();
    AddViewL(calcView);
    CleanupStack::Pop(calcView); 
    
	// Get pointer to CCalcDocument
	CCalcDocument* doc = STATIC_CAST(CCalcDocument*, Document());
	// CCalcDocument needs the pointer to the iCalcAppEnv
	doc->SetAppEnv(iCalcAppEnv);	

    // Set locale change notifier 
    TCallBack callBack(LocaleCallback, this);
    iLocaleChangeNotifier = CEnvironmentChangeNotifier::NewL
                            (EActivePriorityLogonA, callBack);
                            
     if(iLocaleChangeNotifier->IsActive())
     	iLocaleChangeNotifier->Cancel();
    iLocaleChangeNotifier->Start();

	//EISkin2.6
	//Set IsSkinchanged value to EFalse when the application launches.
	SetSkinChanged(EFalse);

	TFileName path;
    iEikonEnv->FsSession().PrivatePath(path);
    iEikonEnv->FsSession().MkDirAll(path);
    }

// ---------------------------------------------------------
// CCalcAppUi::HandleCommandL
// Handle Exit command. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcAppUi::HandleCommandL(TInt aCommand)
    {
    if (aCommand == EEikCmdExit)
        {
        ExitCalculator();
        }
    }
//EISkin2.6
//----------------------------------------------------------
//CCalcAppUi::HandleResourceChangeL
// To Handle Skin Change event
// Update Bitmaps
//----------------------------------------------------------
void CCalcAppUi::HandleResourceChangeL(TInt aType)
{
	if (aType == KAknsMessageSkinChange)
		{
			iCalcAppEnv->LoadFuncMapBitmapSkinChangeL();
			SetSkinChanged(ETrue);
		}
	else
	{
		CAknViewAppUi::HandleResourceChangeL(aType);
	}
}

//----------------------------------------------------------
//CCalcAppUi::HandleWsEventL
// Handles events sent to the application by the window server.
// When EKeyOK is long pressed, to avoid being filtered by the AppUi, 
// send this event directly to the container. 
// Thus the container can get the EKeyOK key event.
//----------------------------------------------------------
void CCalcAppUi::HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination )     
    {                                                                                     
    CAknViewAppUi::HandleWsEventL(aEvent, aDestination);   //pass the event to appui
    if ( aEvent.Type() == EEventKey)
        {
        //judge the key is EKeyOK or not and the key is long pressed or not
        TKeyEvent* key = aEvent.Key();
        if ( key->iCode == EKeyOK && key->iRepeats )       
            {
            //send EEventKey to the Container                                              
            TopFocusedControl()->OfferKeyEventL( *key, EEventKey ); 
            }                                                      
        }
    }
//  End of File  
