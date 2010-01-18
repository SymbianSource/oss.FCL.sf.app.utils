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
* Description:  Implemented all inline functions for CCalcEditorSubPane.  
*
*/


// ================= MEMBER FUNCTIONS =======================

inline void CCalcEditorSubPane::RefreshDisplaySideLBitmap()
{
	 iCalcSideLDisplay = iCalcAppEnv->DisplaySideLBitmap();
}


inline void CCalcEditorSubPane::RefreshDisplayCenterBitmap()
{
	 iCalcCenterDisplay = iCalcAppEnv->DisplayCenterBitmap();
}


inline void CCalcEditorSubPane::RefreshDisplaySideRBitmap()
{
	 iCalcSideRDisplay = iCalcAppEnv->DisplaySideRBitmap();
}


//  End of File  
