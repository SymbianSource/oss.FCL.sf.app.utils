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
* Description:  An application class, CCalcApplication, 
*                derived from CEikApplication
*
*/


#ifndef     CALCAPP_H
#define     CALCAPP_H


//  INCLUDES
#include <aknapp.h>

// UID of app
const   TUid    KUidCalc = { 0x10005902 };

//  FORWARD DECLARATIONS
class CApaDocument;


//  CLASS DEFINITIONS 

/**
CCalcApplication : Application class
*/

class   CCalcApplication
        :public CAknApplication
    {
    private: // Functions from base classes 
        /**
        * From CApaApplication, Create CApaDocument object   
        * @return Pointer of document class.
        */
        CApaDocument* CreateDocumentL(); 
        
        /**
        * From CApaApplication, return Uid of Calculator application 
        * @return Uid of Calculator application 
        */
        TUid AppDllUid() const; 
    };

#endif      //  CALCAPP_H   
            
// End of File

