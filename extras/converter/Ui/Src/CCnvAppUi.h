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
*     This class is a part of the standard application framework.
*     It Owns the application main form.
*     It Acts as a default command handler for the application forms.
*
*/



#ifndef __CCNVAPPUI_H__
#define __CCNVAPPUI_H__

// INCLUDES
#include <aknappui.h>

// FORWARD DECLARATIONS
class CCnvMainForm;

// CLASS DEFINITION
/**
* This class is a part of the standard application framework.
* It Owns the application main form.
* It Acts as a default command handler for the application forms.
*/
class CCnvAppUi : public CAknAppUi
	{

	public:	// Constructors and destructor

		/**
        * Default constructor.
        */
		CCnvAppUi();

		/**
        * Destructor.
        */
		~CCnvAppUi();

		/**
        * 2nd phase constructor.
        */
		void ConstructL();

	public:	// from CEikAppUi

		/**
        * Handles menu commands.
 		* @param aCommand Command identifier.
        */
		void HandleCommandL( TInt aCommand );

	private: // Data

		/**
		* Pointer to the main form. Owned.
		*/
		CCnvMainForm* iMainForm;

		/**
		* Set to ETrue by CMainForm at destruction.
		* Used to make sure that we don't double delete
		* the form after an automatic kill by the framework (appshutter).
		*/
		TBool iFormDeleted;

	};

#endif	// __CCNVAPPUI_H__
