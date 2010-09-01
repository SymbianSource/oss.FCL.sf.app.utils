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
* Description:  Source file of "editor sub-pane", CCalcEditorSubPane class 
*                which derived from CCoeControl class. 
*                Role of this class is to draw editor.
*
*/


// INCLUDE FILES
#include <AknUtils.h>
#include <Calcsoft.rsg>
#include <avkon.hrh>
#include <aknsoundsystem.h>
#include    <applayout.cdl.h>
#include <AknIconUtils.h>
#include    <AknsDrawUtils.h>
#include  <aknlayoutscalable_avkon.cdl.h>  

#include <AknLayout2ScalableDef.h>
#include <AvkonInternalCRKeys.h>
#include    <aknlayoutscalable_apps.cdl.h>

#include "CalcAppUi.h"
#include "CalcCont.h"
#include "CalcDoc.h"
#include "CalcEdit.h"
#include "CalcEnv.h"
#include "CalcDrawingConst.laf" // for drawing information.
#include "fbs.h"
#include <PtiKeyMappings.h>

#include <PtiDefs.h>
#include <PtiEngine.h>
#include <AknFepInternalCRKeys.h>
#include <layoutmetadata.cdl.h>

//  LOCAL CONSTANTS AND MACROS  

// If const TChar is used, complie error occurs in THUMB build.
// To avoid this, #define is used.
#define KCalcHashBtn '#' 
#define KZero 0.0 

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CCalcEditorSubPane* CCalcEditorSubPane::NewL(
                    CCalcContainer* aContainer)
    {
    CCalcEditorSubPane* self = new (ELeave) CCalcEditorSubPane();
    CleanupStack::PushL(self);
    self->ConstructL(aContainer);
    CleanupStack::Pop(self); 
    return self;
    }




// Destructor
CCalcEditorSubPane::~CCalcEditorSubPane()
    {
    iQwertyModeStatusProperty.Close();
    delete iEngine;
    delete iCRKey;
    }


// ---------------------------------------------------------
// CCalcEditorSubPane::EditLine
// Return reference of editor line which has been edited by user.
// (other items were commented in a header).
// ---------------------------------------------------------
//
const TCalcEditLine& CCalcEditorSubPane::EditLine() const
    {
    return iEditLine;
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::SetEditorNumber
// Set TReal64 type editor number.
// (other items were commented in a header). 
// ---------------------------------------------------------
//
void CCalcEditorSubPane::SetOperator(
     TCalcEditLine::TCalcOperatorType aType)
    {
    iEditLine.SetOperator(aType);
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::SetEditorNumber
// Set TReal64 type editor number.
// (other items were commented in a header). 
// ---------------------------------------------------------
//
void CCalcEditorSubPane::SetEditorNumber(TReal64 aNewNumber)
    {
    iEditorNumber = aNewNumber;
    iEditLine.SetNumber(aNewNumber);
    }
        
// ---------------------------------------------------------
// CCalcEditorSubPane::EditorNumber
// Return TReal64 type editor number
// (other items were commented in a header). 
// ---------------------------------------------------------
//
TReal64 CCalcEditorSubPane::EditorNumber() const
    {
    return iEditorNumber;
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::ResetL
// Clear editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::ResetL()
    {
    iEditLine.AllClear();  
    iEditLine.ClearL();
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::ClearOperand
// Clear operand
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::ClearOperand()
    {
    iEditLine.ClearOperand();  
    }


// ---------------------------------------------------------
// CCalcEditorSubPane::ClearLastInput
// Clear last input.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::ClearLastInputL()
    {
    const TPtrC number(iEditLine.NumberString());
    
    if (number.Length() == 1 && 
        iCalcContainer->View()->State() == CCalcView::EOperandOnly)
        {
        iCalcContainer->InputClearL();
        }
    else
        {
        if (iEditLine.CheckZeroL())
            {
            iCalcContainer->InputClearL();
            }
        else
            {
            ClearL();
            if (iEditLine.CheckZeroL() && 
                iCalcContainer->View()->State() == CCalcView::EOperandOnly)
                {
                iCalcContainer->InputClearL();
                }
            }
        }
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::OfferShortPressClearKeyEventL
// Handle short-press-clear-key and CBA-clear-key event.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::OfferShortPressClearKeyEventL()
    {
    CCalcView* view = iCalcContainer->View();
    CCalcView::TStateNo state(view->State());
    CCalcView::TKindOfInput latestInput(view->LatestInput());

    switch (latestInput)
        {
        case (CCalcView::EMemoryRecall):
        case (CCalcView::EOperator):
            {
            if (state == CCalcView::EOperandOnly ||
                iEditLine.CheckZeroL())
                {
                iCalcContainer->InputClearL();
                }
            else 
                {
                ClearOperand();
                }
            break;
            }
        case (CCalcView::EMemorySave):
            {
            if (state != CCalcView::EOperandAndOperator ||
                iEditLine.CheckZeroL())
                {
                iCalcContainer->InputClearL();
                }
            else
                {
                ClearOperand();
                }
            break;
            }
        case (CCalcView::EEqual):
            {
            iCalcContainer->InputClearL();
            break;
            }
        default:
            {
            ClearLastInputL();
            break;
            }
        }
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::DoChangeSign
// Changing sign.
// This is not called when editor number is 0. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::DoChangeSignL(
     CCalcView::TStateNo aState)
    {
    // If current state is other than State1, "+/-" command is ignored.
    if (aState != CCalcView::EInitialMode)
        {
        if (aState == CCalcView::ESelectResult)
            {
            // "=" is disappeared from editor.
            iEditLine.SetOperator(TCalcEditLine::ECalcOperatorNone);  
            }
        
        iEditLine.ChangeSignL();
        iEditorNumber = iEditorNumber * (-1);
        DrawNow();
        }
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::DoMemoryRecallOrLastResult
// Update editor number to memory or last result.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::DoMemoryRecallOrLastResult(
                    TReal64 aMemoryOrLastResult, 
                    CCalcView::TStateNo aState)
    {
    if (aState == CCalcView::ESelectResult)
        {
        // "=" is disappeared from editor.
        iEditLine.SetOperator(TCalcEditLine::ECalcOperatorNone);
        }
    
    iEditLine.SetNumber(aMemoryOrLastResult);
    iEditorNumber = aMemoryOrLastResult;
    DrawNow();
    }


// ---------------------------------------------------------
// CCalcEditorSubPane::NotifyChangeDecimal
// Call when decimal separator is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::NotifyChangeDecimal(TChar aOld, TChar aNew)
    {
    iEditLine.ChangeDecimal(aOld, aNew);
    DrawNow();
    }
    
// ---------------------------------------------------------
// CCalcEditorSubPane::IsQwertyKeypadActive
// Called to check qwerty keypad is active or not
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CCalcEditorSubPane::IsQwertyKeypadActive()
	{
 	TInt value = 0;
    iQwertyModeStatusProperty.Get(value);
    return value;
	}


// ---------------------------------------------------------
// CCalcEditorSubPane::OfferKeyEventL
// This function is called by CCalcContainer::OfferKeyEventL()
// when inputted key is number or "#". 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CCalcEditorSubPane::OfferKeyEventL
                ( const TKeyEvent& aKeyEvent,  
                  TEventCode aType )           
    {
	
    if (aType != EEventKey)
        {
        return EKeyWasNotConsumed;
        }
	TPtiKey keyQwertyValue = (TPtiKey)aKeyEvent.iScanCode;
    TChar decimal(iCalcAppEnv->DecimalSeparator());
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TChar keyCode( aKeyEvent.iCode );
#else 
    TChar keyCode(aKeyEvent.iScanCode);
#endif
    TChar decimalkey(aKeyEvent.iCode);
    
    if( iDecimalCounter < KCalcMaxNumberWidth  )
    {
    	iAllowedDecimal.Append(decimal);
    	iDecimalCounter++;	
    }
    
		if(IsQwertyKeypadActive())
		{
 
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if( MapDecimal(aKeyEvent) || MapDecimal(aKeyEvent) ||aKeyEvent.iCode == KCalcHashBtn )
                {
                keyCode = decimal;
                }		    	
#else
		    if(MapDecimal(keyQwertyValue,EPtiCaseUpper) || MapDecimal(keyQwertyValue,EPtiCaseLower) ||aKeyEvent.iCode == KCalcHashBtn )
		      {
		      keyCode = decimal;
		      }
#endif
                  //}
		}
	else
		{
		if (aKeyEvent.iCode == KCalcHashBtn)
	        {
	        keyCode = decimal;
	        }	
		}
    
    TKeyResponse response(EKeyWasNotConsumed);
    CCalcView* view = iCalcContainer->View();
    
    if (keyCode.IsDigit() || keyCode == decimal)
        {
        DrawDeferred();
        iCalcContainer->ProcessPreinputL();
        view->iLastResultSetected = EFalse;
        // Append input to editor space if editor space is not full.
        TRAPD(errorCode, iEditLine.AppendL(keyCode));

        //  If editor space is full, error message is shown.
        if (errorCode)  
            {
            CAknKeySoundSystem* soundSystem = 
                iCalcAppEnv->AppUi()->KeySounds();
            if (soundSystem)
                {
                soundSystem->PlaySound(EAvkonSIDInformationTone);
                }
            }
        else
            {
            if (iEditLine.CheckZeroL() && 
                iEditLine.Operator() == TCalcEditLine::ECalcOperatorNone)
                {
                view->UpdateState(CCalcView::EAllClear);
                }
            else
                {
                view->UpdateState(CCalcView::EOperand);
                }
            iEditorNumber = iEditLine.NumberL();
            }
		
		// Enable ChangeSign and Square root buttons
		// if there is no 0 in the editor.
		iCalcContainer->SetChangeSignEnableL();
		iCalcContainer->SetSqrtEnableL();
        iCalcContainer->SetClearKeyEnable();
        
		// Enable the Percent button if the calculator
		// is in correct state and there is no 0 in the editor.
		if ( iCalcContainer->GetState() == CCalcView::EOperandAndOperator || 
			 iCalcContainer->GetState() == CCalcView::EOperandAndOperatorAndOperand )
			if (keyCode.IsDigit())
				iCalcContainer->SetPercentEnableL();
		
			response = EKeyWasConsumed;
        }
    else if (decimalkey == EKeyBackspace)   
        {view->iLastResultSetected = EFalse;
        DrawDeferred();
        //  If Clear-Key is pressed long, number editor and operand
        // is clear and the state is set initial one.
        if (aKeyEvent.iRepeats)  
            {
            iCalcContainer->InputClearL();
            }
        // If Clear-key is pressed short, the latest input is deleted.
        else   
            {
            OfferShortPressClearKeyEventL();
            }
        iEditorNumber = iEditLine.NumberL();
        iCalcContainer->SetChangeSignEnableL();
		iCalcContainer->SetSqrtEnableL();
        iCalcContainer->SetClearKeyEnable();
        		
        if ( iCalcContainer->GetState() == CCalcView::EOperandAndOperator || 
        	 iCalcContainer->GetState() == CCalcView::EOperandAndOperatorAndOperand ||
        	 iEditorNumber == KZero )
            iCalcContainer->SetPercentEnableL();
        response = EKeyWasConsumed;
        }
        	
    return response;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcEditorSubPane::CCalcEditorSubPane()
    {
  
    }

// Second-phase constructor
void CCalcEditorSubPane::ConstructL
        (CCalcContainer* aContainer) 
    {
    // The state is set initial one.
    // Number editor is set "0".
    iEditLine.ClearL();
    iEditorNumber = 0.0;

    SetContainerWindowL(*aContainer);
    
    iCalcContainer = aContainer;  
    // Load calc_display bitmap 
    iCalcAppEnv = CCalcAppEnv::Static();
    iCalcSideLDisplay = iCalcAppEnv->DisplaySideLBitmap();
    iCalcCenterDisplay = iCalcAppEnv->DisplayCenterBitmap();
    iCalcSideRDisplay = iCalcAppEnv->DisplaySideRBitmap();
    iIsQwertyActive = 0;
    User::LeaveIfError(iQwertyModeStatusProperty.Attach(KCRUidAvkon, KAknQwertyInputModeActive));
  
    // Try to get the English key mappings. If those are not available, use the 
    // mappings of the current input language. The order is this because all mappings
    // do not necessarily contain all characters neeeded by the Calculator.
    iLanguage = ELangEnglish;
    iEngine = CPtiEngine::NewL(ETrue);
    iCoreLanguage = static_cast<CPtiCoreLanguage*>(iEngine->GetLanguage( iLanguage ));

    if( !iCoreLanguage )
        {
        iCRKey = CRepository::NewL(KCRUidAknFep);
        TInt ret = iCRKey->Get(KAknFepInputTxtLang,iLanguage);
        if( ret!= KErrNone )
            {
            //Exit application if server is busy( Error Raised )
            iAvkonViewAppUi->Exit();
            }
            iCoreLanguage = static_cast<CPtiCoreLanguage*>(iEngine->GetLanguage(iLanguage));
        }
    
    RProperty::Get( KCRUidAvkon, KAknKeyBoardLayout, iKeyboardType );

    if( iCoreLanguage )
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if ( iKeyboardType == EPtiKeyboardHalfQwerty)
           {
           iHalfQwertyKeyMappings = static_cast<CPtiHalfQwertyKeyMappings*>(iCoreLanguage->GetHalfQwertyKeymappings());
           }
       else
#endif
          {
          iQwertyKeyMappings = static_cast<CPtiQwertyKeyMappings*>(iCoreLanguage->GetQwertyKeymappings());
          }
        }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
        
    if ( iKeyboardType == EPtiKeyboardHalfQwerty )
        { 
        iEngine->ActivateLanguageL( iLanguage, EPtiEngineHalfQwerty );
        }
    else if ( IsQwertyKeypadActive())
        { 
        iEngine->ActivateLanguageL( iLanguage, EPtiEngineQwerty );
        } 
       
  //      RProperty::Get( KCRUidAvkon, KAknKeyBoardLayout, iKeyboardType );
        iEngine->SetKeyboardType( TPtiKeyboardType (iKeyboardType ));
        
#endif
        
    // UI language is used to determine the used number format.
    iUiLanguage = User::Language();
    iDecimalCounter = 0;
    }
    
// ---------------------------------------------------------
// CCalcEditorSubPane::IsQwertyActive
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::IsQwertyActive()
    {
    iIsQwertyActive = 1;
    }
    
// ---------------------------------------------------------
// CCalcEditorSubPane::IsQwertyNotActive
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::IsQwertyNotActive()
    {
    iIsQwertyActive = 0;
    } 

// ---------------------------------------------------------
// CCalcEditorSubPane::ClearL
// Clear 1 charactor from editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::ClearL()
    {
    iEditLine.ClearL();
    }

// ---------------------------------------------------------
// CCalcEditorSubPane::SizeChanged
// Size of control is set. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::SizeChanged()
    {
    TRect parentRect(iCalcContainer->Rect());
    TRect EditorPaneRect(iCalcContainer->EditorPane()->Rect());
    

	if (!(AknLayoutUtils::ScalableLayoutInterfaceAvailable()))
		{
		iLayoutText[ECalcEditorOperandShadow].LayoutText(
			parentRect,AppLayout::Calculator_texts_Line_1(),iCalcAppEnv->NumberShadowFont());
		}


                    
     	if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
			{
                        if( iUiLanguage == ELangFarsi || iUiLanguage == ELangUrdu || iUiLanguage == ELangArabic
                            || iUiLanguage == ELangHindi )
				{
				TAknTextLineLayout layout1; 
			    if( AknLayoutUtils::PenEnabled() )
    	        {
    	            layout1	= AknLayoutScalable_Apps:: calc_display_pane_t1(enTouch_enabled).LayoutLine();
    	        }
    	        else
    	        {
    	            layout1	= AknLayoutScalable_Apps:: calc_display_pane_t1(enTouch_disabled).LayoutLine();    
    	        }     
				// Do this only in A&H etc. variants...
				layout1.iFont &= 0xFFFFFFF0; // clear first 4 bits (font category)
				layout1.iFont |= 0x00000001; // set first 4 bits to primary font category value (=1)					

                // When the pen enable and the current screen is transverse,
                // Enlarge the size of parent rect.
                if ( AknLayoutUtils::PenEnabled() && Layout_Meta_Data::IsLandscapeOrientation() )
                    {
                    TRect enlargeEditorPaneRect;
                    enlargeEditorPaneRect = EditorPaneRect;
                    // Enlarging the parent rect, so that the editor could display a long number absolutely.
                    enlargeEditorPaneRect.iBr.iX += EEnlargeFive;
                    enlargeEditorPaneRect.iTl.iX -= EEnlargeTwenty;
                    iLayoutText[ECalcEditorOperand].LayoutText( enlargeEditorPaneRect,
					                                 layout1, iCalcAppEnv->NumberFont() );
                    }
                else
                    {
				iLayoutText[ECalcEditorOperand].LayoutText(EditorPaneRect,
						layout1,iCalcAppEnv->NumberFont());
                    }
				}
			else
				{
				 if( AknLayoutUtils::PenEnabled() )
				 {
                    // If the current screen is transverse,enlarge the size of parent rect.
                    if ( Layout_Meta_Data::IsLandscapeOrientation() )
                        {
                        TRect enlargeEditorPaneRect;
                        enlargeEditorPaneRect = EditorPaneRect;
                        // Enlarging the parent rect, so that the editor could display a long number absolutely.
                        enlargeEditorPaneRect.iBr.iX += EEnlargeFive;
                        enlargeEditorPaneRect.iTl.iX -= EEnlargeTwenty;
                        iLayoutText[ECalcEditorOperand].LayoutText( enlargeEditorPaneRect,
                             AknLayoutScalable_Apps::calc_display_pane_t1( enTouch_enabled ).LayoutLine(), iCalcAppEnv->NumberFont() );
                        }
                    else
                        {
				    iLayoutText[ECalcEditorOperand].LayoutText(EditorPaneRect,
					AknLayoutScalable_Apps::calc_display_pane_t1(enTouch_enabled).LayoutLine(),iCalcAppEnv->NumberFont());    
                        }   
				 }
				 else
				 {
				    iLayoutText[ECalcEditorOperand].LayoutText(EditorPaneRect,
					AknLayoutScalable_Apps::calc_display_pane_t1(enTouch_disabled).LayoutLine(),iCalcAppEnv->NumberFont());    
				 }
				}
			}
		else
			{
       		// Set layout for number
    		iLayoutText[ECalcEditorOperand].LayoutText(
            	parentRect,AppLayout::Calculator_texts_Line_2(),iCalcAppEnv->NumberFont());  
       		}


	if (!(AknLayoutUtils::ScalableLayoutInterfaceAvailable()))
		{
		iLayoutText[ECalcEditorOperatorShadow].LayoutText(
			parentRect,AppLayout::Calculator_texts_Line_3(),iCalcAppEnv->OperatorShadowFont());   
		}


     if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
		{
    	    if( AknLayoutUtils::PenEnabled() )
    	    {
    	        iLayoutText[ECalcEditorOperator].LayoutText(EditorPaneRect,
    			AknLayoutScalable_Apps::calc_display_pane_t2(enTouch_enabled).LayoutLine(),iCalcAppEnv->OperatorFont());
    	    }
    	    else
    	    {
    	        iLayoutText[ECalcEditorOperator].LayoutText(EditorPaneRect,
    			AknLayoutScalable_Apps::calc_display_pane_t2(enTouch_disabled).LayoutLine(),iCalcAppEnv->OperatorFont());
    	    }
		}
	
	else
       	{
       		// Set layout for operator
    	iLayoutText[ECalcEditorOperator].LayoutText(
                parentRect,AppLayout::Calculator_texts_Line_4(),iCalcAppEnv->OperatorFont());   
       	} 
                        
    if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
		{
		    if( AknLayoutUtils::PenEnabled() )
		    {
		        iLayoutText[ECalcEditorMemory].LayoutText(EditorPaneRect,
				AknLayoutScalable_Apps::calc_display_pane_t3(enTouch_enabled).LayoutLine(),NULL);    
		    }
		    else
		    {
		        iLayoutText[ECalcEditorMemory].LayoutText(EditorPaneRect,
				AknLayoutScalable_Apps::calc_display_pane_t3(enTouch_disabled).LayoutLine(),NULL);
		    }
		}
	else
		{
		//	Set layout for memory indicator shadow
		iLayoutText[ECalcEditorMemoryShadow].LayoutText(
			parentRect,AppLayout::Calculator_texts_Line_5(),NULL);
		// Set layout for memory indicator
		iLayoutText[ECalcEditorMemory].LayoutText(
			parentRect,AppLayout::Calculator_texts_Line_6(),NULL); 
		}
	
	if( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
		{               
		TAknWindowLineLayout layoutOfDisplay;
		if( AknLayoutUtils::PenEnabled() )
		{
		    layoutOfDisplay =  AknLayoutScalable_Apps:: bg_calc_display_pane(enTouch_enabled).LayoutLine();    
		}
		else
		{
		    layoutOfDisplay =  AknLayoutScalable_Apps:: bg_calc_display_pane(enTouch_enabled).LayoutLine();    
		}
		
		TAknWindowLineLayout layoutOfSideLDisplay = AknLayoutScalable_Apps:: bg_calc_display_pane_g1().LayoutLine();              
		TAknWindowLineLayout layoutOfCenterDisplay = AknLayoutScalable_Apps:: bg_calc_display_pane_g2().LayoutLine();             
		TAknWindowLineLayout layoutOfSideRDisplay  = AknLayoutScalable_Apps:: bg_calc_display_pane_g3().LayoutLine();
		TAknLayoutRect rectParent;
		rectParent.LayoutRect(iCalcContainer->EditorPane()->Rect(),layoutOfDisplay) ;
		iSideLDisplayLayout.LayoutRect(rectParent.Rect(),layoutOfSideLDisplay);
		iCenterDisplayLayout.LayoutRect(rectParent.Rect(),layoutOfCenterDisplay);          
		iSideRDisplayLayout.LayoutRect(rectParent.Rect(),layoutOfSideRDisplay);  
		}
    }


// ---------------------------------------------------------
// CCalcEditorSubPane::Draw
// Drawing editor
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcEditorSubPane::Draw
              (const TRect& aRect) const   
    {
    // Get a graphics context
     CWindowGc& gc = SystemGc();
    gc.Clear(aRect);
    gc.SetClippingRect(aRect);
    // Draw calc_display outline
    TRect rect(Rect()); 
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	AknsDrawUtils::Background( skin, AknsDrawUtils::ControlContext(this), this, gc, rect );
                                   
    AknIconUtils::SetSize(iCalcSideLDisplay->Bitmap(),iSideLDisplayLayout.Rect().Size(),EAspectRatioNotPreserved);
	AknIconUtils::SetSize(iCalcSideLDisplay->Mask(), iSideLDisplayLayout.Rect().Size(),EAspectRatioNotPreserved);  
	
	AknIconUtils::SetSize(iCalcCenterDisplay->Bitmap(),iCenterDisplayLayout.Rect().Size(),EAspectRatioNotPreserved);
	AknIconUtils::SetSize(iCalcCenterDisplay->Mask(), iCenterDisplayLayout.Rect().Size(),EAspectRatioNotPreserved);        
                    
    AknIconUtils::SetSize(iCalcSideRDisplay->Bitmap(),iSideRDisplayLayout.Rect().Size(),EAspectRatioNotPreserved);
	AknIconUtils::SetSize(iCalcSideRDisplay->Mask(), iSideRDisplayLayout.Rect().Size(),EAspectRatioNotPreserved);     

	iSideLDisplayLayout.DrawImage(gc, iCalcSideLDisplay->Bitmap(),iCalcSideLDisplay->Mask());
	iCenterDisplayLayout.DrawImage(gc,iCalcCenterDisplay->Bitmap(),iCalcCenterDisplay->Mask());
	iSideRDisplayLayout.DrawImage(gc, iCalcSideRDisplay->Bitmap(),iCalcSideRDisplay->Mask());
	
	TRgb shadowcolor ;
	AknsUtils::GetCachedColor( skin,shadowcolor, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG31);
	
	TRgb color;
    AknsUtils::GetCachedColor( skin, color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG30);
    TBuf<256> keyvalue ;

    if (iUiLanguage == ELangFarsi || iUiLanguage == ELangArabic 
            || iUiLanguage == ELangHindi || iUiLanguage == ELangUrdu)
	    {
	    TPtrC number(iEditLine.NumberString());
	    keyvalue.Append(number);
	    AknTextUtils::LanguageSpecificNumberConversion  (keyvalue);
	    }

	if (!(AknLayoutUtils::ScalableLayoutInterfaceAvailable()))
		{
                    if ( iUiLanguage == ELangArabic || iUiLanguage == ELangUrdu )
		    {
		        TInt len = keyvalue.Length();
			    TInt pos = KErrNotFound;
			    TBuf<1> ch;
			    ch.Append(iCalcAppEnv->MinusIndicator());
			    if( (pos = keyvalue.Locate(iCalcAppEnv->MinusIndicator())) != KErrNotFound &&
			         pos == keyvalue.LocateReverse(iCalcAppEnv->MinusIndicator()))
			    {
			     keyvalue.Delete( pos, 1 );
			     keyvalue.Insert( 0, ch );	 
			    }
			iLayoutText[ECalcEditorOperandShadow].DrawText(gc, keyvalue,EFalse,shadowcolor);
			}
                    else if( iUiLanguage == ELangHindi || iUiLanguage == ELangFarsi )
			{
			 iLayoutText[ECalcEditorOperandShadow].DrawText(gc, keyvalue,EFalse,shadowcolor);	
			}
		else 
			{
			iLayoutText[ECalcEditorOperandShadow].DrawText(gc, iEditLine.NumberString(),EFalse,shadowcolor);	
			}
		}

    if ( iUiLanguage == ELangArabic || iUiLanguage == ELangUrdu )
	    {
	        TInt len = keyvalue.Length();
		    TInt pos = KErrNotFound;
		    TBuf<1> ch;
		    ch.Append(iCalcAppEnv->MinusIndicator());
		    if( (pos = keyvalue.Locate(iCalcAppEnv->MinusIndicator())) != KErrNotFound &&
		         pos == keyvalue.LocateReverse(iCalcAppEnv->MinusIndicator()))
		    {
		     keyvalue.Delete( pos, 1 );
		     keyvalue.Insert( 0, ch );	 
		    }
	    iLayoutText[ECalcEditorOperand].DrawText(gc, keyvalue,EFalse,color);
		}
    else if( iUiLanguage == ELangHindi || iUiLanguage == ELangFarsi )
		{
	    iLayoutText[ECalcEditorOperand].DrawText(gc, keyvalue,EFalse,color);
		}
    else
	    {
		iLayoutText[ECalcEditorOperand].DrawText(gc, iEditLine.NumberString(),EFalse,color);
		}

	TCalcEditLine::TCalcOperatorType operatorType(
		iEditLine.Operator());
	TPtrC operatorString(
		iCalcAppEnv->EditorOperator(operatorType));

	if (!(AknLayoutUtils::ScalableLayoutInterfaceAvailable()))
		{
		iLayoutText[ECalcEditorOperatorShadow].DrawText(gc,operatorString,EFalse,shadowcolor);
		}

   	iLayoutText[ECalcEditorOperator].DrawText(gc, operatorString,EFalse,color);
    CCalcDocument* calcDocument = 
        STATIC_CAST(CCalcDocument*, iCalcAppEnv->AppUi()->Document());
    if (calcDocument->HasMemory())
        {
        TPtrC mIndicatorShadow(
            iCalcAppEnv->MemoryIndicatorShadow());
        TPtrC mIndicator(iCalcAppEnv->MemoryIndicator());


	if (!(AknLayoutUtils::ScalableLayoutInterfaceAvailable()))
		{
		iLayoutText[ECalcEditorMemoryShadow].DrawText(gc, mIndicatorShadow);
		}


        iLayoutText[ECalcEditorMemory].DrawText(gc, mIndicator);
        }
    }
    
// ---------------------------------------------------------
// CCalcEditorSubPane::MapNumericChar
// Redraw percent button, either normal or dimmed.
// (other items were commented in a header).
// ---------------------------------------------------------
//		
#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool CCalcEditorSubPane::MapDecimal( TKeyEvent aKeyEvent )
    {
    const TInt KMaxNumCharsPerCase = 50;
    TBuf<KMaxNumCharsPerCase> keys;

    TPtiKey aKey = ( TPtiKey )aKeyEvent.iScanCode;
    if( !keys.Length() )
            {
            keys.Append( aKeyEvent.iCode );
            }

    if ( keys.Length() > 0 )
        {
        TUint16 firstChar = keys[0];
        switch ( firstChar )		
            {
            case KPtiPinyinMarker:
            case KPtiStrokeMarker:
            case KPtiZhuyinMarker:
            case KPtiCangjieMarker:
            case KPtiGetAllMarker:
            if ( keys.Length() > 1 )
                {
                firstChar = keys[1];
                }
            default:
                break;
            }
        TInt loc = iAllowedDecimal.Locate( firstChar );

                if ( ( loc != KErrNotFound ) )/*|| (HandleSpeciallyForChineseNubers(aKey, aCase)))*/
            {
            return ETrue;
            }
        }
    return EFalse;
    }

#else
TBool CCalcEditorSubPane::MapDecimal( TPtiKey aKey, TPtiTextCase aCase )	
    {
    const TInt KMaxNumCharsPerCase = 50;
    TBuf<KMaxNumCharsPerCase> keys;

    iQwertyKeyMappings->GetDataForKey( aKey, keys, aCase );

    if ( keys.Length() > 0 )
        {
        TUint16 firstChar = keys[0];
        switch ( firstChar )
            {
            case KPtiPinyinMarker:
            case KPtiStrokeMarker:
            case KPtiZhuyinMarker:
            case KPtiCangjieMarker:
            case KPtiGetAllMarker:
                if ( keys.Length() > 1 )
                    {
                    firstChar = keys[1];
                    }
            default:
                break;
            }
        TInt loc = iAllowedDecimal.Locate( firstChar );

                if ( ( loc != KErrNotFound ) )/*|| (HandleSpeciallyForChineseNubers(aKey, aCase)))*/
            {
            return ETrue;
            }
        }
    return EFalse;
    }
#endif
	
// ---------------------------------------------------------
// CCalcEditorSubPane::EditPaneRect
// This function gives the editor pane layout.
// (other items were commented in a header).
// ---------------------------------------------------------
//

TRect CCalcEditorSubPane::EditPaneRect()
   {
   TRect parentRect(Rect()); 
   return 	parentRect;
   }
   
// ---------------------------------------------------------
// CCalcEditorSubPane::HandlePointerEventL
// Handled when there is a pen input in EditorPane.
// (other items were commented in a header).
// ---------------------------------------------------------
//   
void CCalcEditorSubPane::HandlePointerEventL
            ( const TPointerEvent& aPointerEvent )           
    {
    TRect memoryLayoutRect = iLayoutText[ECalcEditorMemory].TextRect();
    CCalcDocument* calcDocument = 
        STATIC_CAST( CCalcDocument*, iCalcAppEnv->AppUi()->Document() );
    if( ( memoryLayoutRect.Contains( aPointerEvent.iPosition ) ) && ( calcDocument->HasMemory() ) )
        {
        // When we set the memory value to the editor, 
        // We should change the menu item "Lase Result"'s flag to "EFalse".
        iCalcContainer->View()->iLastResultSetected = EFalse;
        iCalcContainer->View()->HandleMemoryRecallL();
        }

    }

// End of file  CALCEDIT_CPP
