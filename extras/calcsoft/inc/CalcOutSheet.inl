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
* Description:  Implemented all inline functions for CCalcOutputSheet.  
*
*/


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCalcEditorSubPane::RefreshPaperBitmap
// Updates iCalcPaper
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline void CCalcOutputSheet::RefreshPaperBitmap()
{
	iCalcPaper = iCalcAppEnv->PaperBitmap();
}
// ---------------------------------------------------------
// CCalcEditorSubPane::RefreshScalablePaperBitmap
// Updates iCalcScalablePaper
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline void CCalcOutputSheet::RefreshScalablePaperBitmap()
{
 
iCalcAppEnv->ScalablePaperBitmaps(iCalcScalablePaper);
}

// ---------------------------------------------------------
// CCalcEditorSubPane::RefreshResultsLineBitmap
// Updates iCalcResultsLine
// This is used only when __SCALABLE_ICONS flag 
// is enabled. When there is no SVG icon support the result 
// line is drawn using DrawRect.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline void CCalcOutputSheet::RefreshResultsLineBitmap()
{
	iCalcResultsLine = iCalcAppEnv->ResultsLineBitmap();
}
	
//  End of File  
