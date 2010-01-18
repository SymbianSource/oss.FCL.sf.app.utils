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
*     Declares the document class for this application.
*
*/



#ifndef __CCNVDOCUMENT_H__
#define __CCNVDOCUMENT_H__

// INCLUDES
//#include <eikdoc.h>
#include <AknDoc.h> 
#include <aknapp.h> 

// CLASS DEFINITION
/**
* This class is a part of the standard application framework.
* Declares the document class for this application.
*/
class CCnvDocument : public CAknDocument
	{

	public:	// Constructors and destructor.

		/**
        * Static two-phase constructor.
		* @param aApp Reference to the application object.
        */
		static CCnvDocument* NewL( CAknApplication& aApp );

		/**
        * Destructor.
        */
		~CCnvDocument();

	public: // from CEikDocument

		/**
        * Instantiates the application UI class.
		* @return CCnvAppUi object instance.
        */
		CEikAppUi* CreateAppUiL();

	private: // Private constructors

		/**
        * Constructor.
		* @param aApp Reference to the application object.
        */
		CCnvDocument( CAknApplication& aApp );

	};

#endif // __CCNVDOCUMENT_H__
