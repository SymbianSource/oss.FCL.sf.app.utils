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
*      This is the implementation of the class defined in CCnvDocument.h
*
*/



#include "CCnvAppUi.h"
#include "CCnvDocument.h"

CCnvDocument::CCnvDocument( CAknApplication& aApp )
: CAknDocument( aApp )
	{
	}

CCnvDocument* CCnvDocument::NewL( CAknApplication& aApp )
	{
	return new( ELeave ) CCnvDocument( aApp );
	}

CCnvDocument::~CCnvDocument()
	{
	}

CEikAppUi* CCnvDocument::CreateAppUiL()
	{
	return new( ELeave ) CCnvAppUi;
	}
