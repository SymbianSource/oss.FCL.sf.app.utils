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
*     The application gets instantiated starting from this class.
*     Provides a factory method for instantiating the document object.
*
*/



#ifndef __CCNVAPPLICATION_H__
#define __CCNVAPPLICATION_H__

// INCLUDES
//#include <eikapp.h>
#include <aknapp.h> 

// CONSTANTS
const TUid KUidCnv = { 0x101f4668 };	// UID of the application

// CLASS DEFINITION
/**
* This class is a part of the standard application framework.
* The application gets instantiated starting from this class.
* Provides a factory method for instantiating the document object.
*/
class CCnvApplication : public CAknApplication 
	{

	private: // from CApaApplication

		/**
		* Creates a CCnvDocument instance.
		* @return A pointer to the created document object.
		*/
		CApaDocument* CreateDocumentL();

		/**
		* Returns application's UID value.
		* @return KUidCnv.
		*/
		TUid AppDllUid() const;

	};

#endif // __CCNVAPPLICATION_H__
