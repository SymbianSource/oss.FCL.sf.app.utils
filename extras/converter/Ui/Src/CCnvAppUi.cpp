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
* Description:  
*      This is the implementation of the class defined in CCnvAppUi.h
*
*/


#include <avkon.hrh>
#include <hlplch.h>
#include <featmgr.h>

#include <Converter.rsg>

#include "CCnvApplication.h"
#include "CCnvAppUi.h"
#include "CCnvMainForm.h"

CCnvAppUi::CCnvAppUi()
	: iFormDeleted( EFalse )
    {
    }

CCnvAppUi::~CCnvAppUi()
    {
	// This can be done safely because the form never "self-destructs"
	// (exit is done via HandleCommandL( EEikCmdExit) below)
	if( !iFormDeleted )
		{
		delete iMainForm;
		}
    FeatureManager::UnInitializeLib();
    }

void CCnvAppUi::ConstructL()
	{
	// Construct softkeys and menubar using the base class
    BaseConstructL( CAknAppUi::EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible);

	// Construct and show our main form
	iMainForm = new( ELeave ) CCnvMainForm( iFormDeleted );
	iMainForm->ConstructL();
	iMainForm->ExecuteLD( R_CNV_MAINFORM );

	iMainForm->SetMopParent( this );

	CAknAppUi::SetKeyBlockMode(ENoKeyBlock);

    FeatureManager::InitializeLibL();
	}

void CCnvAppUi::HandleCommandL( TInt aCommand )
	{
	switch( aCommand )
		{
		case EAknCmdHelp:
			{
			HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(),
												 AppHelpContextL() );
			break;
			}
		case EEikCmdExit:
		case EAknCmdExit: 
			{
			Exit();
			break;
			}
		default:
			{
			break;
			}
		}
	}
