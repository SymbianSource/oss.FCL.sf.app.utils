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
* Description:  The Function-Map subpane class, CCalcFuncmapSubPane
*                Derived from CCoeControl.
*                CCalcFuncmapSubPane is container class of CCalcCommandButton.
*
*/



// INCLUDE FILES

#include <AknUtils.h>
#include <aknViewAppUi.h>

#include <AknsSkinInstance.h> 
#include <aknpopuplayout.h> 
#include <Calcsoft.rsg>
#include <layoutmetadata.cdl.h>
#include "CalcFunc.h"
//#include "CalcCmdBtn.h"
#include "CalcEnv.h"
#include "CalcCont.h"
#include "CalcView.h"
#include "CalcOutSheet.h"
#include "calc.hrh"
#include "CalcDrawingConst.laf"
#include "CalcView.h"
#include <applayout.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AvkonInternalCRKeys.h>
#include <PtiDefs.h>
#include <PtiEngine.h>
#include <PtiKeyMappings.h>
#include <AknFepInternalCRKeys.h>
#include <aknbutton.h>
#include <AknsDrawUtils.h>

#include <featmgr.h>      // Feature Manager

//enum to hold the values of button states
enum
{
    EnNormal = 0,
    EnPressed,
    EnDimmed
};
//  LOCAL CONSTANTS AND MACROS  
const TInt KButtonsInColumn(2);      // The number of column of a button
const TInt KButtonsInRow(KCountOfButtons / KButtonsInColumn);
const TInt KButtonsInColumnTouch(4);      // The number of column of a button in Touch layout
const TInt KButtonsInRowTouch(KCountOfButtonsTouch / KButtonsInColumnTouch);
                                     // The number of row of a button
const TInt KButtonsIdTblTouch[] =         // The command ID of a button in Touch Layout
               { 
               ECalcCmdCancel,          // C
               ECalcCmdSeven,          // 7
               ECalcCmdFour,          // 4
               ECalcCmdOne,          // 1
			   ECalcCmdZero,          // 0
			   ECalcCmdSqrt, 		 // Sqrt
               ECalcCmdEight,          // 8
               ECalcCmdFive,          // 5
               ECalcCmdTwo,          // 2
			   ECalcCmdSeparator,          // .
			   ECalcCmdPercent,		 // %	
			   ECalcCmdNine,          // 9
               ECalcCmdSix,          // 6
               ECalcCmdThree,          // 3
               ECalcCmdChangeSign,   // +/- 
               ECalcCmdDivide,       // /
               ECalcCmdMultiply,     // *
               ECalcCmdSubtract,     // -
               ECalcCmdAdd,          // +
               ECalcCmdResult       // =
				};
				
const TInt KButtonsIdTbl[] =         // The command ID of a button in non Touch Layout
               { 
               ECalcCmdAdd,          // +
               ECalcCmdSubtract,     // -
               ECalcCmdMultiply,     // *
               ECalcCmdDivide,       // /
               ECalcCmdChangeSign,   // +/- 
               ECalcCmdResult,       // =
			   ECalcCmdSqrt, 		 // Sqrt
			   ECalcCmdPercent,		 // %
               ECalcCmdScrollUp,     // Scroll-up
               ECalcCmdScrollDown    // Scroll-down
				};


// If const TChar is used, complie error occurs in THUMB build.
// To avoid this, #define is used.
#define KCalcAsteriskBtn '*'


// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CCalcFuncmapSubPane* CCalcFuncmapSubPane::NewL(
                     CCalcContainer* aContainer)
    {
    CCalcFuncmapSubPane* self = new (ELeave) CCalcFuncmapSubPane();
    CleanupStack::PushL(self);
    self->ConstructL(aContainer);
    CleanupStack::Pop(self); 
    return self;
    }


// Destructor
CCalcFuncmapSubPane::~CCalcFuncmapSubPane()
    {
        for (TInt count(0); count < KCountOfButtonsTouch; count++)
            {
                if(iButtons[count])
                {
                     delete iButtons[count];    
                }
                
            }
   
    // delete of button controls
       iQwertyModeStatusProperty.Close();
       delete iEngine;
       delete iCRKey;
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetScrollupEnable
// \ scroll up and down button on scrolling enable.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::RedrawScrollButtons()
    {
    CCalcOutputSheet* outputSheet = iContainer->OutputSheet();
    SetScrollupEnable(outputSheet->CanUp());
    SetScrolldownEnable(outputSheet->CanDown());
    }



// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetChangeSignEnable
// Redraw ChangeSign button on ChangeSign enable.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetChangeSignEnable
                            (TBool aEnable)
    {

				/*Get the selected pointer 
				button -> different layouts, different values*/
        CAknButton* button = NULL;
        if( AknLayoutUtils::PenEnabled() )
        {
            button = iButtons[ECmdTChangeSign];
        }
        else
        {
            button = iButtons[EChangeSignButton];
        }
        
        //Set to dimmed state
        button->SetDimmed( !aEnable );
        button->DrawNow();
        iIsChangeSignDimmed = aEnable;
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetClearKeyEnable
// Redraw ClearKey button on ClearKey enable.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetClearKeyEnable( TBool aEnable )
    {
    if ( AknLayoutUtils::PenEnabled() )
        {
        CAknButton* button = NULL;
        button = iButtons[ECmdTCancel];

        if ( button )
            {
            button->SetDimmed( !aEnable );   
            button->DrawNow();
            }      
        }
    iIsClearKeyEnabled = aEnable;
   }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::NotifyTimeoutL
// If no key is pressed until timeout of *-key,
// this function is called.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::NotifyTimeoutL()
    {

	  /*Get the selected pointer 
		button -> different layouts, different values*/
    CAknButton* button = iButtons[iSelected];    
    if( !AknLayoutUtils::PenEnabled() )
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT  
        if ( iKeyboardType == EPtiKeyboardHalfQwerty )
        	{
            if( ( iLastKeyCode == EStdKeyLeftShift && iDecimalPoint ) || ( iLastKeyCode == EStdKeyLeftFunc && iDecimalPoint ))
    	        {
                iContainer->SetSeparatorFromTouchL();
        	    }
            else
                {
                iContainer->View()->HandleCommandL( KButtonsIdTbl[iSelected] );
                }
        	}
        else
#endif
        	{
            iContainer->View()->HandleCommandL( KButtonsIdTbl[iSelected] );
        	}
        }
    else
        {
        iContainer->View()->HandleCommandL( KButtonsIdTblTouch[iSelected] );
        }   
   
#ifdef RD_INTELLIGENT_TEXT_INPUT  
    if ( iKeyboardType == EPtiKeyboardHalfQwerty )
    	{
        iLastKeyCode = 0;
    	}
#endif
    
    //Set to dimmed state
    button->SetDimmed( EFalse );
    DrawDeferred();    
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::NotifyReleaseKeyL
// Notify any key is released. If pressed button exists.
// command which is releated is executed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::NotifyReleaseKeyL()
    {
				/*Get the selected pointer 
				button -> different layouts, different values*/
        CAknButton* button = iButtons[iSelected];
        CAknButtonState* state = button->State(EnNormal);      	
        
        if(state->Flags() == EnPressed)
        {
          TKeyEvent aKeyEvent;
          aKeyEvent.iScanCode = EStdKeyEnter;
          TEventCode aType = EEventKeyUp;
          //trigger offerkeyeventl to this button
          button->OfferKeyEventL(aKeyEvent,aType);
	      state->SetFlags(EnNormal);    
	      button->DrawNow();
        }
      if( !AknLayoutUtils::PenEnabled() )
       	{
       	  if(iSelected == EScrollUp)
            {
                CCalcOutputSheet* outputSheet = iContainer->OutputSheet();
                if(!outputSheet->CanUp())
                {
                    RedrawHighlight(EScrollUp,EScrollDown);
                    iSelected = EScrollDown;
                }
            }
            
          if(iSelected == EScrollDown)
            {
                CCalcOutputSheet* outputSheet = iContainer->OutputSheet();
                if(!outputSheet->CanDown())
                {
                    RedrawHighlight(EScrollDown,EScrollUp);
                    iSelected = EScrollUp;
                }
            }
            
       	}

	   }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::NotifyOtherThanOkKeyPressed
// If key other than OK-key is pressed this function is called.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::NotifyOtherThanOkKeyPressed()
    {
    //for (TInt cnt(0); cnt < iCountOfButtons; cnt++)
        {
          //Get the pointer for the button
         CAknButton* button = iButtons[iSelected];
         CAknButtonState* state = button->State(EnNormal);
         if(state->Flags() == EnPressed)
            {
          state->SetFlags(EnNormal);
          button->DrawNow();
            }
        }

    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::NotifyLangChange
// Called if language is changed.
// Change layout according to the language.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::NotifyLangChange()
    {
    // Write code here.
    SetLayout();
    DrawNow();
    }


// ---------------------------------------------------------
// CCalcFuncmapSubPane::IsQwertyKeypadActive
// Called to check qwerty keypad is active or not
// (other items were commented in a header).
// ---------------------------------------------------------
//
 TInt CCalcFuncmapSubPane::IsQwertyKeypadActive()
  {
 	TInt value = 0;
    
    iQwertyModeStatusProperty.Get(value);
    
    return value;
 }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::HandleControlEventL
// Observer event from avkon button change
// (other items were commented in a header).
// ---------------------------------------------------------
void CCalcFuncmapSubPane::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
    {    
    iIsKeyLongPress = EFalse;   
    if ( aEventType != EEventStateChanged && aEventType != CAknButton::ELongPressEvent )
        {
        return;
        }
    else if ( aEventType == CAknButton::ELongPressEvent )
        {
        iIsKeyLongPress = ETrue;
        }
        
    //Get the selected button
    CAknButton* button = (CAknButton*)aControl;
    
    for ( TInt i = 0; i < KCountOfButtons; ++i )
        {
        if ( button == iButtons[i] )
            {
            if(iSelected != i)
            return;
            
            //break;
            }
        }    
    
    if ( !button->IsDimmed() )
        {
        		//handles all the commands for the buttons
            if( !AknLayoutUtils::PenEnabled() )
            {
            iContainer->View()->HandleCommandL(KButtonsIdTbl[iSelected]);
            if(iSelected == EScrollUp)
            {
                CCalcOutputSheet* outputSheet = iContainer->OutputSheet();
                if(!outputSheet->CanUp())
                {
                    RedrawHighlight(EScrollUp,EScrollDown);
                    iSelected = EScrollDown;
                }
            }
            else if(iSelected == EScrollDown)
            {
                CCalcOutputSheet* outputSheet = iContainer->OutputSheet();
                if(!outputSheet->CanDown())
                {
                    RedrawHighlight(EScrollDown,EScrollUp);
                    iSelected = EScrollUp;
                }
            }
            }
            else
            {
            iContainer->View()->HandleCommandL(KButtonsIdTblTouch[iSelected]);
            }   

        }    
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::OfferKeyEventL
// This function is called by CCalcContainer::OfferKeyEventL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CCalcFuncmapSubPane::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    TKeyResponse exitCode(EKeyWasNotConsumed);
       
    TPtiKey keyQwertyValue = (TPtiKey)aKeyEvent.iScanCode;
    
    if(IsQwertyKeypadActive())  
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT  
        if ( iKeyboardType == EPtiKeyboardHalfQwerty )
        	{
            TBuf<KMaxNumCharsPerCase> keys;
            TInt firstoperator(-1);
            TInt secondoperator(-1);
                   
            TInt oldSelect( iSelected );
            
            if ( ( aKeyEvent.iScanCode == EStdKeyLeftFunc && aType == EEventKeyDown ) || 
                 ( aKeyEvent.iScanCode == EStdKeyLeftShift && aType == EEventKeyDown ) )
                 {            
                 iEngine->MappingDataForKey( keyQwertyValue, keys, EPtiCaseFnLower  );
                 MapDataForChrShiftKey( keys, firstoperator, secondoperator);
                 if ( (secondoperator == -1) && (firstoperator != -1))
                	 {
                     if ( AknLayoutUtils::PenEnabled() )
                         {
                         iSelected = firstoperator;    
                         }
                     else
                         {
                         if ( firstoperator == ECmdTSeparator )
                        	 {
                        	 iDecimalPoint = ETrue;
                        	 }
                         else
                        	 {
                        	 iSelected = firstoperator;
                        	 iDecimalPoint = EFalse;
                        	 }
                         }                	 
                	 }
                 else if ( (firstoperator == -1) && (secondoperator != -1) )
                	 {
                     if ( AknLayoutUtils::PenEnabled() )
                         {
                         iSelected = secondoperator;    
                         }
                     else
                         {
                         if ( secondoperator == ECmdTSeparator )
                        	 {
                        	 iDecimalPoint = ETrue;
                        	 }
                         else
                        	 {
                        	 iSelected = secondoperator;
                        	 iDecimalPoint = EFalse;
                        	 }
                         }                	 
                	 }
                 else if ( (firstoperator != -1) && (secondoperator != -1) )
                	 {
                     if ( iLastKeyCode == aKeyEvent.iScanCode )
                         {
                         if ( AknLayoutUtils::PenEnabled() )
                             {
                             if ( iSelected == firstoperator )
                                 {
                                 iSelected = secondoperator;
                                 }
                             else
                                 {
                                 iSelected = firstoperator;
                                 }
                             }
                         else
                        	 {
                              if ( firstoperator == ECmdTSeparator || secondoperator == ECmdTSeparator )
                            	  {
                            	  if ( iDecimalPoint )
                            		  {
                            		  iDecimalPoint = EFalse;
                            		  if ( firstoperator == ECmdTSeparator )
                            			  {
                            			  iSelected = secondoperator;
                            			  }
                            		  else
                            			  {
                            			  iSelected = firstoperator;
                            			  }                 		  
                            		  }
                            	  else
                            		  {
                            		  iDecimalPoint = ETrue;
                            		  }
                            	  }
                              else
                            	  {
                                  if ( iSelected == firstoperator )
                                      {
                                      iSelected = secondoperator;
                                      }
                                  else
                                      {
                                      iSelected = firstoperator;
                                      }                	  
                            	  }
                        	 }
                         }
                     else
                         {
                         if ( AknLayoutUtils::PenEnabled() )
                             {
                             iSelected = firstoperator;    
                             }
                         else
                             {
                             if ( firstoperator == ECmdTSeparator )
                            	 {
                            	 iDecimalPoint = ETrue;
                            	 }
                             else
                            	 {
                            	 iSelected = firstoperator;
                            	 iDecimalPoint = EFalse;
                            	 }
                             }

                         }
                     }
                     RedrawHighlight( oldSelect, iSelected );
                     iLastKeyCode = aKeyEvent.iScanCode;
                     exitCode = EKeyWasConsumed;
                     
                     iIsShiftKeyPressed = ETrue;
                 }
            }
         else
#endif
        	{
        	//For Multiplication operator in Qwerty keypad
        	if(((aKeyEvent.iScanCode == EStdKeyLeftShift) || (aKeyEvent.iScanCode == EStdKeyRightShift)) && aType == EEventKeyDown)
        	    {
        	     if(iShiftKeyPressed == EPtiCaseLower )
        		     {
        		   	 iShiftKeyPressed = EPtiCaseUpper;
        		   	 iShiftKey = 0;
        		   	 iValue = ETrue;
        		     }
        	      
        	     else
        	     	{
        			iShiftKeyPressed = EPtiCaseLower;
        	     	}
        	    exitCode = EKeyWasConsumed; 	
        	    
        	    iIsShiftKeyPressed = ETrue;
        	    }
        	else if((aKeyEvent.iScanCode == EStdKeyLeftShift || aKeyEvent.iScanCode == EStdKeyRightShift)&& aType == EEventKeyUp)
        		{
                        /*if(((iLanguage == ELangFrench) || (iLanguage == ELangTaiwanChinese) || 
                            (iLanguage == ELangCzech) || (iLanguage == ELangSlovak))&& iShiftKey >= 1)*/
                                if( iShiftKey >= 1 )
        			{
        			iShiftKeyPressed = EPtiCaseLower;
        			}
        		iValue = EFalse;
        	    exitCode = EKeyWasConsumed; 
        	    
        	    iIsShiftKeyPressed = ETrue;
        	    }
        	}
	      //else
     if ( !iIsShiftKeyPressed )
		{
		if (aType != EEventKey)
        {
	        if (aType == EEventKeyDown)
			    {
			        iIsOfferKeyEvent = ETrue;        
			    }
			    else if (aType == EEventKeyUp)
			    {
			        iIsOfferKeyEvent = EFalse;        
			    }
            // when users press send key, EKeyWasNotConsumed is returned to system
            // to open dialer 
            if ( aKeyEvent.iScanCode == EStdKeyYes )
                {
                return EKeyWasNotConsumed;
                }
        return EKeyWasConsumed;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if( MapNumericCharL( aKeyEvent ) )
            {
            exitCode = EKeyWasConsumed;
            iShiftKeyPressed = EPtiCaseLower;
            }
#else
		if(MapNumericCharL(keyQwertyValue,iShiftKeyPressed))
			{
			exitCode = EKeyWasConsumed;
			iShiftKeyPressed = EPtiCaseLower;		
			}
#endif
		else 
			{
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if( MapNumerics( aKeyEvent ) || aKeyEvent.iCode == EKeyBackspace )
                {
                exitCode = EKeyWasNotConsumed;
                iShiftKeyPressed = EPtiCaseLower;
                
                                /*if((iLanguage == ELangFrench) || (iLanguage == ELangTaiwanChinese) || 
                                         (iLanguage == ELangCzech) || (iLanguage == ELangSlovak))*/
                    {
                    if( !iValue )
                    iShiftKeyPressed = EPtiCaseLower;

                    else
                    iShiftKeyPressed = EPtiCaseUpper;
                    iShiftKey++;
                    }
                }
#else
			if(MapNumerics(keyQwertyValue,iShiftKeyPressed) || aKeyEvent.iCode == EKeyBackspace)
				{
				exitCode = EKeyWasNotConsumed;
				iShiftKeyPressed = EPtiCaseLower;

                                /*if((iLanguage == ELangFrench) || (iLanguage == ELangTaiwanChinese) || 
                                         (iLanguage == ELangCzech) || (iLanguage == ELangSlovak))*/
					{
					if(!iValue)
					iShiftKeyPressed = EPtiCaseLower;
					
					else
					iShiftKeyPressed = EPtiCaseUpper;
					iShiftKey++;	
					}
				}
#endif
			else
				{
				exitCode = EKeyWasConsumed;	
				iShiftKeyPressed = EPtiCaseLower;
				}
			
			}

		}	    

    // handle Enter key event	
    if ( aKeyEvent.iCode == EKeyEnter )
        {
        exitCode = EKeyWasConsumed;
        HandleMiddleSoftKeyOREKeyOKL();
        iShiftKeyPressed = EPtiCaseLower;
        return exitCode;
        }
    
	iIsShiftKeyPressed = EFalse;
		
	}
 
/*	if(aKeyEvent.iCode == EKeyOK)
    {
       
        HandleMiddleSoftKeyOREKeyOKL();
        exitCode = EKeyWasConsumed;
    }   
    else if (aType == EEventKey)*/
    if (aType == EEventKey)
    {
        TInt oldSelect(iSelected);
        TInt row(iSelected % iButtonsInRow);
        TInt column(iSelected / iButtonsInRow);
        iIsOfferKeyEvent = EFalse; 
        switch(aKeyEvent.iCode)
        {
            case EKeyLeftArrow:
            {
                
                do
                {   
                   
                        column--;
                        
                        if(column < 0)
                        {
                            column = iButtonsInColumn -1;
                            row--;

                            if( row < 0 )
                            {
                                row = iButtonsInRow -1;
                            }
                        }
                    
                    iSelected = row + column * iButtonsInRow;

                }while( iButtons[iSelected]->IsDimmed() );
                
                RedrawHighlight(oldSelect, iSelected);

                exitCode = EKeyWasConsumed;
                break;
            }
            
            case EKeyRightArrow:
            {
                               
                do
                {   
                        column++;
                        
                        if(column == iButtonsInColumn)
                        {
                            column = 0;
                            row++;
                            
                            if( row >= iButtonsInRow )
                            {
                                row = 0;
                            }
                        }
                    
                    iSelected = row + column * iButtonsInRow;

                }while( iButtons[iSelected]->IsDimmed() );
                
                RedrawHighlight(oldSelect, iSelected);
                exitCode = EKeyWasConsumed;
                break;
            }
            
            case EKeyUpArrow:
            {
                
                SetNextCursorPosForUpArrow(oldSelect);

                RedrawHighlight(oldSelect, iSelected);
                exitCode = EKeyWasConsumed;
                break;
            }
            
            case EKeyDownArrow:
            {
                
                SetNextCursorPosForDownArrow(oldSelect);
 
                RedrawHighlight(oldSelect, iSelected);
                exitCode = EKeyWasConsumed;
                break;
            }

            case EKeyOK:
            {
            if ( !AknLayoutUtils::PenEnabled() )
                {
                // get and judge the state of button
                CAknButton* button = iButtons[iSelected];
                CAknButtonState* state = button->State( EnNormal ); 
                if ( ( iSelected == EScrollUp || iSelected == EScrollDown ) 
                    && ( state->Flags() == EnPressed ) )
                    {
                    iContainer->View()->HandleCommandL( KButtonsIdTbl[iSelected] );
                    // when the history arrives the top or bottom,the button should be set dimmed
                    CCalcOutputSheet* outputSheet = iContainer->OutputSheet();
                    if ( !( outputSheet->CanDown() ) && iSelected == EScrollDown )
                        {
                        button->SetDimmed( ETrue );
                        DrawDeferred();  
                        }
                    if ( !( outputSheet->CanUp() ) && iSelected == EScrollUp )
                        {
                        button->SetDimmed( ETrue );
                        DrawDeferred();  
                        }   
                    }
                }
                break;
            }
            default:
            {
               if(!IsQwertyKeypadActive())
             {
                if(aKeyEvent.iCode == KCalcAsteriskBtn)
                {
                    if(iLastKeyCode == aKeyEvent.iCode)
                    {
                        row++;
                        
                        if(row == iButtonsInRow - 1)  // Equal is not contained in a loop.
                        {
                            row = 0;
                        }
                        iSelected = row + column * iButtonsInRow;
                    }
                    else
                    {
                        if(AknLayoutUtils::PenEnabled())
                            {
                              iSelected = ECmdTAdd;    
                            }
                        else
                            {
                              iSelected = EPlusButton;    
                            }    
                        
                    }
                    RedrawHighlight(oldSelect, iSelected);
                    exitCode = EKeyWasConsumed;
                }
             }
            break;
            }
        }
        iLastKeyCode = aKeyEvent.iCode;
        
    }
    else if (aType == EEventKeyDown)
    {
        iIsOfferKeyEvent = ETrue;        
    }
    else if (aType == EEventKeyUp)
    {
        iIsOfferKeyEvent = EFalse;        
    }
    
   
    return exitCode;
}


// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcFuncmapSubPane::CCalcFuncmapSubPane()
    {
    }


// default constructor can leave.
void CCalcFuncmapSubPane::ConstructL
      (CCalcContainer* aContainer) 
    {
    iSelected = 0;    
    iContainer = aContainer;
    iIsOfferKeyEvent = EFalse;

    SetContainerWindowL(*iContainer);
    // Read character sequence tables, such as -, +.
    iCalcAppEnv = CCalcAppEnv::Static();
    iIsChangeSignDimmed = ETrue;
    
       if( AknLayoutUtils::PenEnabled() )
    {
        iCountOfButtons = KCountOfButtonsTouch;
        iButtonsInRow = KButtonsInRowTouch;
        iButtonsInColumn = KButtonsInColumnTouch;
      
    }
    else
    {
        iCountOfButtons = KCountOfButtons;
        iButtonsInRow = KButtonsInRow;
        iButtonsInColumn = KButtonsInColumn;  
        
    }
    

    for (TInt count = 0; count < iCountOfButtons; count++)
        {        
        //Create the AknButtons
        if(AknLayoutUtils::PenEnabled())
            {
                iButtons[count] = CAknButton::NewL(
            iCalcAppEnv->UnpressedButtonBmp(count),
            NULL,
            NULL,
            NULL,
            _L(""),
            _L(""),
            0,
            0
            );     
            
            }
        else
            {
            
                iButtons[count] = CAknButton::NewL(
            iCalcAppEnv->UnpressedButtonBmp(count),
            NULL,
            iCalcAppEnv->PressedButtonBmp(count),
            NULL,
            _L(""),
            _L(""),
            0,
            0
            );     
            /*if((count == ESqrtButton) && !iIsChangeSignDimmed)
            {
                iButtons[count]->SetDimmed(ETrue);
            }*/
            
            }
        
        iButtons[count]->SetContainerWindowL( *this );
        iButtons[count]->SetObserver( this );     
        }

    if ( AknLayoutUtils::PenEnabled() )
        {
        iButtons[ECmdTCancel]->SetButtonFlags( KAknButtonReportOnLongPress );
        }
    iSelected = 0;
    RedrawHighlight( iSelected, iSelected );

    User::LeaveIfError(iQwertyModeStatusProperty.Attach(KCRUidAvkon, KAknQwertyInputModeActive));
   
    iShiftKeyPressed = EPtiCaseLower;
    iShiftKey		 = 0;

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

        // UI language is used to determine the used number format.
        iUiLanguage = User::Language();
  
        
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
    
    iIsShiftKeyPressed = EFalse;

    }

//	---------------------------------------------------------
// CCalcFuncmapSubPane::RefreshButtonIcons
// This function is called by CCalcContainer::OfferKeyEventL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::RefreshButtonIcons()
{
	/*for (TInt count(0); count < iCountOfButtons; count++)
        {
        iButtons[count]->SetBitmapIcons(iCalcAppEnv->UnpressedButtonBmp(count),
			iCalcAppEnv->PressedButtonBmp(count),
			iCalcAppEnv->InactiveButtonBmp(count));
        }*/
}
// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetScrollupEnable
// Redraw scroll-up button on scrolling enable.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetScrollupEnable
                     (TBool aEnable)  
    {
		//Enabling/Disabling of scroll up buttons only for Non-Touch   
    if(!AknLayoutUtils::PenEnabled())
    {
        CAknButton* scrollUp = iButtons[EScrollUp];
        scrollUp->SetDimmed( !aEnable );
        scrollUp->DrawNow();         
    }
    
    }


// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetScrolldownEnable
// Redraw scroll-down button on scrolling enable.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetScrolldownEnable
                     (TBool aEnable) 
    {
	//Enabling/Disabling of scroll down buttons only for Non-Touch
    if(!AknLayoutUtils::PenEnabled())
    {       
    CAknButton* scrollDown = iButtons[EScrollDown];
    scrollDown->SetDimmed( !aEnable );
    scrollDown->DrawNow();
    }
    }


// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetNextCursorPosForUpArrow
// Set next cursor position whwn pressing uparrow key.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetNextCursorPosForUpArrow(TInt aOldPos)
{
    
    TInt row(aOldPos % iButtonsInRow);
    TInt column(aOldPos / iButtonsInRow);

    do
    {   row--;

        if(row < 0)
        {
            column--;
        
            if(column < 0)
            {
                column = iButtonsInColumn -1;
            }
            row = iButtonsInRow - 1;
        }
        iSelected = row + column * iButtonsInRow;
    
    }while( iButtons[iSelected]->IsDimmed() );
}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetNextCursorPosForDownArrow
// Set next cursor position whwn pressing downarrow key.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetNextCursorPosForDownArrow(TInt aOldPos)
{
    TInt row(aOldPos % iButtonsInRow);
    TInt column(aOldPos / iButtonsInRow);

    do
    {   row++;

        if(row == iButtonsInRow)  // KButtonsInRow is equal to "number of rows in column"
        {
            row = 0;

            //move focus from last item to first item, column should be added one
            column++;     
        
            if( column == iButtonsInColumn )  
            {
                column = 0;
            }
        }
        iSelected = row + column * iButtonsInRow;
    
    }while( iButtons[iSelected]->IsDimmed() );
}


// ---------------------------------------------------------
// CCalcFuncmapSubPane::RedrawHighlight
// An old highlight rectangle is erased and 
// a new highlight rectangle is drawn.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::RedrawHighlight
      (TUint aOldId,            // The button chosen before
       TUint aNewId) const      // The button chosen newly
    {
    // The State of each button is set.
    CAknButton* button = iButtons[aOldId];
    CAknButtonState* state = button->State(EnNormal);      	
        
    if(state->Flags() == EnPressed)
        {
        TKeyEvent aKeyEvent;
        aKeyEvent.iScanCode = EStdKeyEnter;
        TEventCode aType = EEventKeyUp;
        //trigger offerkeyeventl to this button
        TRAP_IGNORE(button->OfferKeyEventL(aKeyEvent,aType));
	    state->SetFlags(EnNormal);
     	}
    button->SetFocus( EFalse, EDrawNow );      
    button = iButtons[aNewId];        
    button->SetFocus( ETrue, EDrawNow );
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetLayout
// Set functionmap button layout.
// (other items were commented in a header).
// ---------------------------------------------------------
//


void CCalcFuncmapSubPane::SetLayout()
    {
    TRect parentRect(Rect());    
    
	 TInt t = 0;
	 if( AknLayoutUtils::PenEnabled()  )
	    {
	        for (TInt count(0); count < KButtonsInColumnTouch; count++)
            {
            for (TInt row(0); row < 5; row++)   
      	        { 
      	        
      	         TAknWindowLineLayout area;
                 TAknWindowLineLayout area1;

                 TAknWindowLineLayout areaG2;
                 TAknWindowLineLayout areaGraphics;
     	        
     	            area= AknLayoutScalable_Apps::toolbar_button_pane_cp01(1).LayoutLine();
                   areaG2= AknLayoutScalable_Apps::cell_calc_pane_g2(1).LayoutLine();
    		         if (Layout_Meta_Data::IsLandscapeOrientation())
    		            {
    		                area1 = AknLayoutScalable_Apps::cell_calc_pane(count,row,enTouch_with_prt).LayoutLine();
    		            }
    		         else
    		            {
    		               area1 = AknLayoutScalable_Apps::cell_calc_pane(count,row,enTouch_enabled).LayoutLine();
    		            }   

                    areaGraphics = area1;
                    areaGraphics.iW = areaGraphics.iW - areaG2.il - areaG2.ir;
                    areaGraphics.iH = areaG2.iH;
    		    
    		        area1.il = area1.il + area.il;
    		        area1.ir = area1.ir - area.ir;
    		        area1.it = area1.it + area.it;
    		        area1.ib = area1.ib - area.ib;
    		        area1.iW = area1.iW - area.il - area.ir;
    		        area1.iH = area1.iH - area.it - area.ib;
    		    
                 
                //A change in values here
                if ( Layout_Meta_Data::IsLandscapeOrientation() )
                {
                AknLayoutUtils::LayoutControl(
                iButtons[t], parentRect,/*AknLayoutScalable_Apps::cell_calc_pane(count,Row,enTouch_with_prt).LayoutLine()*/area1 );
                }
                else
                {
                AknLayoutUtils::LayoutControl(
                iButtons[t], parentRect,/*AknLayoutScalable_Apps::cell_calc_pane(count,Row,enTouch_enabled).LayoutLine()*/area1 );
                }

                TSize iconSize( areaGraphics.iW, areaGraphics.iH );
                iButtons[t]->SetIconSize( iconSize );
                TRect rect = iButtons[t]->Rect();
                rect.iBr.iX += 5;
                rect.iBr.iY += 5; 
                rect.iTl.iX -= 5;
                rect.iTl.iY -= 5;

                if ( FeatureManager::FeatureSupported( KFeatureIdRockerKey ) )
                {
                iButtons[t]->SetHighlightRect( rect );
                }
                t++;
      	        }
            }
            
       }
            
	    
	    else
	        {
             for (TInt count(0); count < KButtonsInColumn; count++)
                {
                for (TInt row(0); row < 5; row++)   
           	        {   	        
               	        AknLayoutUtils::LayoutControl(
                        iButtons[t], parentRect, AknLayoutScalable_Apps::cell_calc_pane( count, row ).LayoutLine());
                        iButtons[t]->SetHighlightRect( iButtons[t]->Rect() );
                        t++;
           	        }
           	        
                }
	        }
                
        
    }




// ---------------------------------------------------------
// CCalcFuncmapSubPane::CountComponentControls
// The count of child control is returned.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcFuncmapSubPane::CountComponentControls() const
    {
    return iCountOfButtons;
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::ComponentControl
// Child control's pointer corresponding to the specified index is returned.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCoeControl* CCalcFuncmapSubPane::ComponentControl
              (TInt aIndex) const 
    {
    return iButtons[aIndex];
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetButtonBitmapControl
// This function is called when the size changes.
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*CCalcCommandButton*/CAknButton* CCalcFuncmapSubPane::GetButtonBitmapControl
                                    (TInt aIndex)
	{
		return iButtons[aIndex];
	}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::SizeChanged
// This function is called when the size changes.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SizeChanged()
    {
    SetLayout();
    }


// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetSqrtEnable
// Redraw square root button, either normal or dimmed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetSqrtEnable(TBool aEnable)
{
	//Get the sqrt button
	CAknButton* button = NULL;
	 if( !AknLayoutUtils::PenEnabled()  )
	    {
	     button = iButtons[ESqrtButton];     
	    }
	  else
	    {
	     button = iButtons[ECmdTSqrt];    
	    }  
	if( button )
	    {
         button->SetDimmed(!aEnable);   
         button->DrawNow();
        }
    iIsSqrtDimmed = aEnable;
	}


// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetPercentEnable
// Redraw percent button, either normal or dimmed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetPercentEnable(TBool aEnable)
{
	//Get the percent button
	CAknButton* button = NULL;
	 if( !AknLayoutUtils::PenEnabled()  )
	    {
	     button = iButtons[EPercentButton];     
	    }
	  else
	    {
	     button = iButtons[ECmdTPercent];    
	    }  
    if( button )
	    {
         button->SetDimmed(!aEnable);   
         button->DrawNow();
	   }
	iIsPercentDimmed = aEnable;
}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::MapNumericChar
// Redraw percent button, either normal or dimmed.
// (other items were commented in a header).
// ---------------------------------------------------------
//		
#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool CCalcFuncmapSubPane::MapNumericCharL( TKeyEvent aKeyEvent )
    {
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

        TInt loc1 = KAllowedOperatorPlus().Locate( firstChar );
        if ( loc1 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[0] );
            return ETrue;
            }
        TInt loc2 = KAllowedOperatorMinus().Locate( firstChar );
        if ( loc2 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[1] );
            return ETrue;
            }
        TInt loc3 = KAllowedOperatorStar().Locate( firstChar );
        if ( loc3 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[2] );
            return ETrue;
            }
        TInt loc4 = KAllowedOperatorDivide().Locate( firstChar );
        if ( loc4 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[3] );
            return ETrue;
            }
        TInt loc5 = KAllowedOperatorEquals().Locate( firstChar );
        if ( loc5 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[5] );
            return ETrue;
            }
        TInt loc6 = KAllowedOperatorPercent().Locate( firstChar );
        if ( loc6 != KErrNotFound &&  GetPercentButtonState() )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[7] );
            return ETrue;
            }

              }
        
    return EFalse;
    }

#else
TBool CCalcFuncmapSubPane::MapNumericCharL( TPtiKey aKey, TPtiTextCase aCase )	
    {
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

        TInt loc1 = KAllowedOperatorPlus().Locate( firstChar );
        if ( loc1 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[0] );
            return ETrue;
            }
        TInt loc2 = KAllowedOperatorMinus().Locate( firstChar );
        if ( loc2 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[1] );
            return ETrue;
            }
        TInt loc3 = KAllowedOperatorStar().Locate( firstChar );
        if ( loc3 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[2] );
            return ETrue;
            }
        TInt loc4 = KAllowedOperatorDivide().Locate( firstChar );
        if ( loc4 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[3] );
            return ETrue;
            }
        TInt loc5 = KAllowedOperatorEquals().Locate( firstChar );
        if ( loc5 != KErrNotFound )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[5] );
            return ETrue;
            }
        TInt loc6 = KAllowedOperatorPercent().Locate( firstChar );
        if ( loc6 != KErrNotFound &&  GetPercentButtonState() )
            {
            iContainer->View()->HandleCommandL( KButtonsIdTbl[7] );
            return ETrue;
            }

              }
        
    return EFalse;
    }
#endif       
	
// ---------------------------------------------------------
// CCalcFuncmapSubPane::MapNumericChar
// Redraw percent button, either normal or dimmed.
// (other items were commented in a header).
// ---------------------------------------------------------
//		
#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool CCalcFuncmapSubPane::MapNumerics( TKeyEvent aKeyEvent )
    {
    
    TBuf<KMaxNumCharsPerCase> keys;

    TPtiKey aKey = ( TPtiKey )aKeyEvent.iScanCode;

    if ( !keys.Length() )
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
        TInt loc = KAllowedNumericChars().Locate( firstChar );
        if ( loc != KErrNotFound )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

#else
TBool CCalcFuncmapSubPane::MapNumerics( TPtiKey aKey, TPtiTextCase aCase )	
    {

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
        TInt loc = KAllowedNumericChars().Locate( firstChar );
        if ( loc != KErrNotFound )
            {
            return ETrue;
            }
        }
    return EFalse;
    }
#endif
		
// ---------------------------------------------------------
// CCalcFuncmapSubPane::HandlePointerEventL
// Handles apen input in Function Pane. .
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::HandlePointerEventL
                ( const TPointerEvent& aPointerEvent )           
    {
    if ( !AknLayoutUtils::PenEnabled() )
        {
        return;
        }
    TInt oldSelect( iSelected );
    TInt buttonValue = CalculatingCorrectButton( aPointerEvent );
    if ( !iButtons[buttonValue]->IsDimmed() )
        {
        iSelected = buttonValue;
        if ( iButtons[buttonValue]->Rect().Contains( aPointerEvent.iPosition ) )
            {
            RedrawHighlight( oldSelect, iSelected );
            }
        }
    CCoeControl::HandlePointerEventL( aPointerEvent );
    }
    
// ---------------------------------------------------------
// CCalcFuncmapSubPane::FunctionPaneRect
// This function gives the layout of Function Pnae.
// (other items were commented in a header).
// ---------------------------------------------------------
//

TRect CCalcFuncmapSubPane::FunctionPaneRect()
   {
   TRect parentRect(Rect()); 
   return 	parentRect;
   }
   
// ---------------------------------------------------------
// CCalcFuncmapSubPane::CalculatingCorrectButton
// Handled when there is Pen Input.
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CCalcFuncmapSubPane::CalculatingCorrectButton
					(const TPointerEvent& aPointerEvent)
   {
    TInt selected = iSelected;
    
    //Function that calculates the correct button where the pointer event had occurred
    for(TInt i = 0; i<iCountOfButtons; i++)
    {
   		if(iButtons[i]->Rect().Contains(aPointerEvent.iPosition))
   		{
   			selected = i;
   		}
    }
    return selected;
   }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::HandleMiddleSoftKeyOREKeyOK
// Handled when MSK or EKeyOk is selected.
// ---------------------------------------------------------
//

void CCalcFuncmapSubPane::HandleMiddleSoftKeyOREKeyOKL()
   {
     	//get the selected button id
	    CAknButton* button = iButtons[iSelected];
	    //put the button in the pressed state
        CAknButtonState* state = button->State(EnNormal);
    	//Put the button in pressed state
        
        TKeyEvent aKeyEvent;
        aKeyEvent.iScanCode = EStdKeyEnter;
        TEventCode aType = EEventKeyDown;
        button->OfferKeyEventL(aKeyEvent,aType);
	    //put to pressed state
	    state->SetFlags(EnPressed);
        button->DrawNow();
        
	    if(!iIsOfferKeyEvent)
        {
            
            NotifyReleaseKeyL();
        }
   }
   
// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetChangeSignButtonState
// Returns the state of change sign button
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::GetChangeSignButtonState()
{
    return(iIsChangeSignDimmed);
}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetClearButtonState
// Returns the state of Clear button
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::GetClearButtonState()
    {
    return( iIsClearKeyEnabled );
    } 

// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetSqrtButtonState
// Returns the state of square root button
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::GetSqrtButtonState()
{
    return(iIsSqrtDimmed);
}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetPercentButtonState
// returns the state of percent button
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::GetPercentButtonState()
{
    return(iIsPercentDimmed);
}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetSelectedButtonId
// returns the id of current button
// ---------------------------------------------------------
//
TInt CCalcFuncmapSubPane::GetSelectedButtonId()
    {
    return iSelected;
    }
   
// ---------------------------------------------------------
// CCalcFuncmapSubPane::SetHighlightButton
// set highlight button
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::SetHighlightButton( TUint aOldId, TUint aNewId )
    {
    iSelected = aNewId;
    RedrawHighlight( aOldId, aNewId );
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::IsClearKeyLongPressed
// Find Clear is long Pressed.
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::IsKeyLongPressed()
    {
    return iIsKeyLongPress;
    }

// ---------------------------------------------------------
// CCalcFuncmapSubPane::MapDataForChrShiftKey
// Redraw percent button, either normal or dimmed.
// ---------------------------------------------------------
//
void CCalcFuncmapSubPane::MapDataForChrShiftKey( TDes& aOperators, TInt& aFirstOperator, TInt& aSecondOperator)
	{ 
	if( aOperators.Length() > 0 )
		{
		TUint16 firstChar = aOperators[0];
		TUint16 secondChar = firstChar;
		
		if ( aOperators.Length() > 1 )
			{
			secondChar = aOperators[1];
			}
		
        switch ( firstChar )
            {
            case KPtiPinyinMarker:
            case KPtiStrokeMarker:
            case KPtiZhuyinMarker:
            case KPtiCangjieMarker:
            case KPtiGetAllMarker:
                if ( aOperators.Length() > 2 )
                    {
                    firstChar = aOperators[1];
                    secondChar = aOperators[2];
                    }
                else
                	{
                	firstChar = aOperators[1];
                	secondChar = firstChar;
                	}
                break;
            default:
                break;
		
		}
        
       MapOperators( firstChar, aFirstOperator );

       MapOperators( secondChar, aSecondOperator );
       
       }
	}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::MapOperators
// Redraw percent button, either normal or dimmed.
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::MapOperators( TUint16 aOperator, TInt& aResult)
	{
    TInt loc1 = KAllowedOperatorPlus().Locate( aOperator );
    if ( loc1 != KErrNotFound )
        {
        if ( AknLayoutUtils::PenEnabled() )
        	{
            aResult = ECmdTAdd;
        	}
        else
        	{
        	aResult = EPlusButton;
        	}
        return ETrue;
        }
    TInt loc2 = KAllowedOperatorMinus().Locate( aOperator );
    if ( loc2 != KErrNotFound )
        {
        if ( AknLayoutUtils::PenEnabled() )
        	{
            aResult = ECmdTSubtract;
        	}
        else
        	{
        	aResult = EMinusButton;
        	}
        return ETrue;
        }
    TInt loc3 = KAllowedOperatorStar().Locate( aOperator );
    if ( loc3 != KErrNotFound )
        {
        if ( AknLayoutUtils::PenEnabled() )
        	{
            aResult = ECmdTMultiply;
        	}
        else
        	{
        	aResult = EMultiplyButton;
        	}
        return ETrue;
        }
    TInt loc4 = KAllowedOperatorDivide().Locate( aOperator );
    if ( loc4 != KErrNotFound )
        {
        if ( AknLayoutUtils::PenEnabled() )
        	{
            aResult = ECmdTDivide;
        	}
        else
        	{
        	aResult = EDivideButton;
        	}
        return ETrue;
        }
    TInt loc5 = KAllowedOperatorEquals().Locate( aOperator );
    if ( loc5 != KErrNotFound )
        {
        if ( AknLayoutUtils::PenEnabled() )
        	{
            aResult = ECmdTResult;
        	}
        else
        	{
        	aResult = EEqualButton;
        	}
        return ETrue;
        }
    TInt loc6 = KAllowedOperatorPercent().Locate( aOperator );
    if ( loc6 != KErrNotFound )
        {
        if ( AknLayoutUtils::PenEnabled() )
        	{
            aResult = ECmdTPercent;
        	}
        else
        	{ 
        	aResult = EPercentButton;
        	}
        return ETrue;
        }
    TInt loc7 = KAllowedOperatorDecimal().Locate( aOperator );
    if ( loc7 != KErrNotFound )
        {
        aResult = ECmdTSeparator;
        return ETrue;
        }
    return EFalse;
	}

// ---------------------------------------------------------
// CCalcFuncmapSubPane::GetKeyboardType
// Returns the type of keyboard
// ---------------------------------------------------------
//
TBool CCalcFuncmapSubPane::GetKeyboardType()
    {
    return iKeyboardType;
    }

// End of file
