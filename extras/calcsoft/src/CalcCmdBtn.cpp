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
* Description:The Function-Map's button class, CCalcCommandButton
*                Derived from CEikCommandButtonBase
*                The CCalcCommandButton is flat button.
*                In making disable, it calls SetDimmed() function.
*
*/


// INCLUDE FILES
#include    <gulicon.h>
#include    <calcsoft.rsg>
#include    <aknutils.h>

#include    "CalcFunc.h"
#include    "CalcCmdBtn.h"
#include    "CalcDrawingConst.laf"
#include <AppLayout.cdl.h>
#include <AknIconUtils.h>
#include "fbs.h"

// skin support
#include    <AknsDrawUtils.h>
#include  <AknLayoutScalable_Avkon.cdl.h>  
#include <aknlayoutscalable_avkon.cdl.common.h>
#include <AknLayout2ScalableDef.h>

#include <aknsconstants.h>
#include <AknLayoutScalable_Apps.cdl.h>

// skin support
#include    <AknsDrawUtils.h>


//  LOCAL CONSTANTS AND MACROS  

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CCalcCommandButton* CCalcCommandButton::NewL(
                    CCalcFuncmapSubPane* aFuncmapSubPane,
                    const CGulIcon* aUnpressedBmp,
                    const CGulIcon* aPressedBmp,
                    const CGulIcon* aInactiveBmp) 
    {
    CCalcCommandButton* self = new (ELeave) CCalcCommandButton();
    CleanupStack::PushL(self);
    self->ConstructL(aFuncmapSubPane, aUnpressedBmp,
                     aPressedBmp, aInactiveBmp);
    CleanupStack::Pop(self); 
    return self;
    }

// Destructor
CCalcCommandButton::~CCalcCommandButton()
    {
    }



// ---------------------------------------------------------
// CCalcCommandButton::SetStateAndRedraw
// Button state is set and re-draw.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcCommandButton::SetStateAndRedraw
      (const TState& aState)      
    {
    iState = aState;
    DrawNow();
    }


// ---------------------------------------------------------
// CCalcCommandButton::SetIsSelectedAndRedraw
// Set flag which button is selected or not, and redraw.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcCommandButton::SetIsSelectedAndRedraw(TBool aIsSelected)
    {
    iIsSelected = aIsSelected;
    DrawNow();    
    }


// ---------------------------------------------------------
// CCalcCommandButton::State
// Button state is returned.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCalcCommandButton::TState CCalcCommandButton::State() const
    {
    return iState;
    }



// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcCommandButton::CCalcCommandButton()
        : iState(ENormal),
          iIsSelected(EFalse)
    {
    }

// Second-phase constructor
void CCalcCommandButton::ConstructL
            (CCalcFuncmapSubPane* aFuncmapSubPane,
             const CGulIcon* aUnpressedButtonIcon,
             const CGulIcon* aPressedButtonIcon,
             const CGulIcon* aInactiveButtonIcon)  
    {
    iUnpressedButton = aUnpressedButtonIcon;
    iPressedButton = aPressedButtonIcon;
    iInactiveButton = aInactiveButtonIcon;
    SetContainerWindowL(*aFuncmapSubPane);
    }


// ---------------------------------------------------------
// CCalcCommandButton::SizeChanged
// Data of control is set.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcCommandButton::SizeChanged()
    {
    TRect buttonRect(Rect());
    TRect aParentRect;
    TAknWindowLineLayout area;
    TAknWindowLineLayout cursor_color;
    
    if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
		{
	
	area = AknLayoutScalable_Apps::cell_calc_pane_g1().LayoutLine();
	
	// For drawing cursor color.     
    cursor_color = AknLayoutScalable_Apps::grid_highlight_pane_cp02().LayoutLine();
    
    	
		}
	
	else
	{
			
    area = AppLayout::Cell_pane_elements__calc__Line_1();
// For drawing cursor color.     
    cursor_color = AppLayout::Cell_pane_highlight_elements__various__Line_1(aParentRect);
    
	}
	
	area.iC = cursor_color.iC;
    iLayoutRect.LayoutRect(buttonRect,area); 
              
    }

// ---------------------------------------------------------
// CCalcCommandButton::Draw
// Draw a button
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcCommandButton::Draw
      (const TRect& /* aRect */) const  
    {
    const CGulIcon* drawButton = iUnpressedButton;
    
    switch (iState)
        {
        case EPressed:
            {
            drawButton = iPressedButton;
            break;
            }
        case EDimmed:
            {
            drawButton = iInactiveButton;
            break;
            }
        default:
            {
            break;
            }
        }
    CWindowGc& gc = SystemGc();

    // Clear with skin background
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsDrawUtils::Background( skin, AknsDrawUtils::ControlContext(this), this, gc, Rect());

	if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
		{
		TAknWindowLineLayout layout = AknLayoutScalable_Apps::grid_highlight_pane_cp02().LayoutLine();
		TAknWindowLineLayout area = AknLayoutScalable_Apps::cell_calc_pane_g1().LayoutLine();

		TAknLayoutRect topLeft;
		topLeft.LayoutRect(Rect(), area);

		TAknLayoutRect bottomRight;
		bottomRight.LayoutRect(Rect(),area);

		TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
		TRect innerRect = outerRect;
		innerRect.Shrink(TSize(5,5));
		gc.SetPenStyle(CGraphicsContext::ENullPen);
		 if (iIsSelected)
			TBool highlightDrawn = AknsDrawUtils::DrawFrame(skin, gc, outerRect, innerRect,
			KAknsIIDQsnFrGrid, KAknsIIDNone); 
		}
	else
		{
		if (iIsSelected)
			{
			iLayoutRect.DrawOutLineRect(gc);
			}

		}



	//Set the size of the bitmap. SVG-T upgrade.
	TRect rect(Rect()); 
	 AknIconUtils::SetSize(drawButton->Bitmap(),rect.Size()); 
	 AknIconUtils::SetSize(drawButton->Mask(), rect.Size());
    iLayoutRect.DrawImage(gc, drawButton->Bitmap(), drawButton->Mask());

    }
 
// ---------------------------------------------------------
// CCalcCommandButton::SetBitmapIcons
// Sets the button bitmap Icons when the skin changes.
// ---------------------------------------------------------
//
void CCalcCommandButton::SetBitmapIcons(CGulIcon* aUnpressedBmp,
                    CGulIcon* aPressedBmp,
                    CGulIcon* aInactiveBmp)
{
	iUnpressedButton = aUnpressedBmp;
    iPressedButton = aPressedBmp;
    iInactiveButton = aInactiveBmp;
}

// ---------------------------------------------------------
// CCalcCommandButton::ButtonLayout
// Gets the button layout. Used for pen input.
// ---------------------------------------------------------
//
TRect CCalcCommandButton::ButtonLayout()
	{
	TRect buttonRect(Rect());
	return buttonRect;
	}
//  End of File  
