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
* Description:  Implemented all inline functions for CCalcAppUi.  
*
*/


// ================= MEMBER FUNCTIONS =======================

//----------------------------------------------------------
//CCalcAppUi::IsSkinChanged
// Get the boolean value of iIsSkinChanged
//----------------------------------------------------------
inline TBool CCalcAppUi::IsSkinChanged()const
{
	return iIsSkinChanged;
}

//----------------------------------------------------------
//CCalcAppUi::SetSkinChanged
// Set the boolean value to iIsSkinChanged
//----------------------------------------------------------
inline void CCalcAppUi::SetSkinChanged(TBool aETrue)
{
	iIsSkinChanged = aETrue;
}


//  End of File  
