/*
* Copyright (c) 2000 Nokia Corporation and/or its subsidiary(-ies).
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
*     Implemented all inline functions.
*
*/


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCalcAppEnv::AppUi
// Return pointer of CCalcAppUi class
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CCalcAppUi* CCalcAppEnv::AppUi() const
    { 
    return iAppUi; 
    }

// ---------------------------------------------------------
// CCalcAppEnv::EditorOperator
// Return charactor for editor operator
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const TPtrC CCalcAppEnv::EditorOperator(
            const TCalcEditLine::TCalcOperatorType aType ) const
    {
    if ( ( aType == TCalcEditLine::ECalcMultiply ||
        aType == TCalcEditLine::ECalcDivide ) &&
        AknLayoutUtils::PenEnabled() )
        {
        return ( *iEditorOperArray )[( TInt ) aType + 6];
        }
    return ( *iEditorOperArray )[(TInt) aType];
    }

// ---------------------------------------------------------
// CCalcAppEnv::OutSheetOperator
// Return charactor for OutputSheet operator
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const TPtrC CCalcAppEnv::OutSheetOperator(
            const TCalcEditLine::TCalcOperatorType aType ) const
    {
    if ( ( aType == TCalcEditLine::ECalcMultiply ||
        aType == TCalcEditLine::ECalcDivide ) &&
        AknLayoutUtils::PenEnabled() )
        {
        return (*iOutSheetOperArray)[( TInt ) aType + 6];
        }
    return (*iOutSheetOperArray)[( TInt ) aType];
    }

// ---------------------------------------------------------
// CCalcAppEnv::DecimalSeparator
// Return decimal separator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const TChar CCalcAppEnv::DecimalSeparator() const
    {
    return iDecimalSeparator;
    }

// ---------------------------------------------------------
// CCalcAppEnv::SetDecimalSeparator
// Set new decimal separator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline void CCalcAppEnv::SetDecimalSeparator(TChar aChar)
    {
    iDecimalSeparator = aChar;    
    }

// ---------------------------------------------------------
// CCalcAppEnv::MinusIndicator
// Return minus indicator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const TChar CCalcAppEnv::MinusIndicator() const
    {
    return iMinusIndicator;
    }

// ---------------------------------------------------------
// CCalcAppEnv::MemoryIndicator
// Return memory indicator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline TPtrC CCalcAppEnv::MemoryIndicator() const
    {
    return iMemoryIndicator;
    }
        
// ---------------------------------------------------------
// CCalcAppEnv::MemoryIndicatorShadow
// Return shadow of memory indicator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline TPtrC CCalcAppEnv::MemoryIndicatorShadow() const
    {
    return iMemoryIndicatorShadow;
    }

// ---------------------------------------------------------
// CCalcAppEnv::NumberShadowFont
// Return font for number shadow in editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CFont* CCalcAppEnv::NumberShadowFont() const
    {
    return iNumberShadowFont;
    }

// ---------------------------------------------------------
// CCalcAppEnv::NumberShadowFont
// Return font for number in editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CFont* CCalcAppEnv::NumberFont() const
    {
    return iNumberFont;
    }

// ---------------------------------------------------------
// CCalcAppEnv::OperatorShadowFont
// Return font for operator shadow in editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CFont* CCalcAppEnv::OperatorShadowFont() const
    {
    return iOperatorShadowFont;
    }

// ---------------------------------------------------------
// CCalcAppEnv::OperatorFont
// Return font for operator in editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CFont* CCalcAppEnv::OperatorFont() const
    {
    return iOperatorFont;
    }

// ---------------------------------------------------------
// CCalcAppEnv::OutSheetOperatorFont
// Return font for operator in OutputSheet.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CFont* CCalcAppEnv::OutSheetOperatorFont() const
    {
    return iOutSheetOperatorFont; 
    }


// ---------------------------------------------------------
// CCalcAppEnv::DisplaySideLBitmap
// Return 'qsn_fr_calc_display_side_l' Bitmap pointer.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CGulIcon* CCalcAppEnv::DisplaySideLBitmap() const 
    { 
    return iDisplaySideLBitmap;
    }


// ---------------------------------------------------------
// CCalcAppEnv::DisplayCenterBitmap
// Return 'qsn_fr_calc_display_center' Bitmap pointer.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CGulIcon* CCalcAppEnv::DisplayCenterBitmap() const 
    { 
    return iDisplayCenterBitmap;
    }



// ---------------------------------------------------------
// CCalcAppEnv::DisplaySideRBitmap
// Return 'qsn_fr_calc_display_side_r' Bitmap pointer.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CGulIcon* CCalcAppEnv::DisplaySideRBitmap() const 
    { 
    return iDisplaySideRBitmap;
    }


// ---------------------------------------------------------
// CCalcAppEnv::PaperBitmap
// Return 'qgn_indi_calc_paper' Bitmap pointer.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CGulIcon* CCalcAppEnv::PaperBitmap() const 
    { 
    return iPaperBitmap;
    }

// ---------------------------------------------------------
// CCalcAppEnv::PaperBitmap
// Return 'qsn_fr_calc_paper_center' Bitmap pointers.
// (other items were commented in a header).
// ---------------------------------------------------------
//
 inline  void CCalcAppEnv::ScalablePaperBitmaps(const CGulIcon* aPaperBitmap[]) const
    { 
	
	for(TInt Cnt=0;Cnt < 9; Cnt++)
		{
		aPaperBitmap[Cnt]=iScalablePaperBitmap[Cnt];
		}
    }

// ---------------------------------------------------------
// CCalcAppEnv::ResultsLine
// Return 'qgn_graf_calc_resultsLine' Bitmap pointer.
// This returns a valid icon only when __SCALABLE_ICONS flag 
// is enabled. When there is no SVG icon support the result 
// line is drawn using DrawRect.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline const CGulIcon* CCalcAppEnv::ResultsLineBitmap() const 
    { 
    return iResultsLineBitmap;
    }

// ---------------------------------------------------------
// CCalcAppEnv::UnpressedButtonBmp
// Return unpressed bitmap of correspondent index
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CGulIcon* CCalcAppEnv::UnpressedButtonBmp(
             const TInt aIndex) const
    {
    return iUnpressedButtonBmp[aIndex];    
    }

// ---------------------------------------------------------
// CCalcAppEnv::PressedButtonBmp
// Return pressed bitmap of correspondent index
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CGulIcon* CCalcAppEnv::PressedButtonBmp(
             const TInt aIndex) const
    {
    return iPressedButtonBmp[aIndex];
    }

// ---------------------------------------------------------
// CCalcAppEnv::InactiveButtonBmp
// Return inactive bitmap of correspondent index
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CGulIcon* CCalcAppEnv::InactiveButtonBmp(
             const TInt aIndex) const
    {
    return iDimmedButtonBmp[aIndex];
    }

// End of File
