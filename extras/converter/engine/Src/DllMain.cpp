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
*	  The E32Dll() function is required by all DLLs; it is defined 
*     as the main DLL entry point which implies that it is exported, 
*     even though there is no explicit EXPORT_C in the C++ code. Its 
*     primary use is to allocate thread-local storage.
*
*/



#include <e32base.h>


// End of File
