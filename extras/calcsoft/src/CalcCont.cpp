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
* Description:  Source file of "CCalcContainer",  CCalcContainer class 
*                which derived from CCoeControl class. Role of this class 
*                is to update the calculator data and display on user's input.
*
*/


// INCLUDE FILES
#include    <aknkeys.h>
#include    <AknUtils.h>   // AknLayoutUtils::LayoutControl 
#include    <Calcsoft.rsg>

#include    <layoutmetadata.cdl.h>

// For skin support. 
#include    <AknsDrawUtils.h>
#include    <AknsConstants.h>
#include    <AknsUtils.h>
#include    <AknsBasicBackgroundControlContext.h>
#include    <applayout.cdl.h>
#include    <aknlayoutscalable_apps.cdl.h>
#include    <csxhelp/calc.hlp.hrh> // for help context of Calculator
#include    "CalcApp.h" 

#include    "CalcAppUi.h"
#include    "CalcCont.h"
#include    "CalcDoc.h"
#include    "CalcEdit.h"
#include    "CalcFunc.h"
#include    "CalcOutSheet.h"
#include    "calc.hrh"
#include    "CalcView.h"
#include    "CalcEnv.h"
#include    "CalcHistory.h"
#include    "CalcDrawingConst.laf" // for layout information


#include <AknDef.h>
#include <eiksbfrm.h> 
#include <eikscrlb.h>


#define KEY_CODE_VAL 57 //for all number inputs
#define ASCII_ZERO 48

//  LOCAL CONSTANTS AND MACROS  
const TInt KCallBackDelay(1000000); // In microseconds
const TInt KCallBackInterval(1000000); // In microseconds
const TInt KCallBackPriority(CActive::EPriorityUserInput);

// Count of controls in Calculator.
// Controls are editor pane, output sheet, and function map.
// Therefore, this count is 3.  
const TInt KCalcCountOfControls(3);

// Define index of control
enum TCalcControlIndex
    {
    ECalcControlFunctionMap,
    ECalcControlOutputSheet,
    ECalcControlEditorPane
    };

// If const TChar is used, complie error occurs in THUMB build.
// To avoid this, #define is used.
#define KCalcAsteriskBtn '*'
//define a Comma  as decimal separator
#define KCommaSeparator ',' 
//define a dot as decimal separator 
#define KDotSeparator '.'
//define a '#' as input decimal separator by conventional 12 phone keypad.
#define KHashSeparator '#'
// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CCalcContainer* CCalcContainer::NewL(
                    CCalcView* aView)
    {
    CCalcContainer* self = new (ELeave) CCalcContainer();
    CleanupStack::PushL(self);
    self->ConstructL(aView);
    CleanupStack::Pop(self);
    return self;
    }


// Destructor
CCalcContainer::~CCalcContainer()
    {
    delete iEditorPane;
    delete iFuncmapPane;
    delete iSheetPane;

    delete iTimeout;

    if ( iTimeoutChr )
        {
        delete iTimeoutChr;
        iTimeoutChr = NULL;
        }
    if ( iTimeoutShift )
        {
        delete iTimeoutShift;
        iTimeoutShift = NULL;
        }

    delete iSkinContext;
    }

// ---------------------------------------------------------
// CCalcContainer::ProcessPreinputL
// This function is called when decimal point or a digit is inputted.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::ProcessPreinputL()
    {
    TInt currentState = iView->State();
    CCalcView::TKindOfInput latestInput(iView->LatestInput());

    if (currentState == CCalcView::ESelectResult)  
                              // State 5 : User has selected result
        {
        iEditorPane->ResetL();  // Set editline "0"
        iView->UpdateState(CCalcView::EAllClear);
        iCalcDocument->AddEmptyLine();
        iSheetPane->ScrollToBottomL();
        ScrollArrowUpdate();
        }
    else if (currentState == CCalcView::EOperandAndOperator ||  
                                           // State 3 : operand and operator
             latestInput == CCalcView::EMemoryRecall  ||
             latestInput == CCalcView::EMemorySave)
        {   
        iEditorPane->ClearOperand();
        }
    }

// ---------------------------------------------------------
// CCalcContainer::InputClearL
// This function should be called when editor is cleared.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::InputClearL()
    {
    iEditorPane->ResetL();
    iView->UpdateState(CCalcView::EAllClear);
    
    const TCalcEditLine lastLine((*(iCalcDocument->History()))[0]);  
                                              // Get latest historical line.
    const TPtrC number(lastLine.NumberString());
    
    //  If latest historical line is not empty,
    // empty line is added in the history and the history is shown.
    if ( number.Length() )
        {
        iCalcDocument->AddEmptyLine();
        iSheetPane->ScrollToBottomL();
        ScrollArrowUpdate();
        }
    }

// ---------------------------------------------------------
// CCalcContainer::ScrollArrowUpdate
// This function should be called when OutputSheet is scrolled
// or history is added.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::ScrollArrowUpdate()
    {
    iFuncmapPane->RedrawScrollButtons();
    }

// ---------------------------------------------------------
// CCalcContainer::SetChangeSignEnableL
// This function is called when number of editor is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::SetChangeSignEnableL()
    {
    TCalcEditLine editLine(iEditorPane->EditLine());
    TBool changeSignEnable(ETrue);
    
    if (editLine.NumberL() == 0.0)
        {
        changeSignEnable = EFalse;    
        }
    
    iFuncmapPane->SetChangeSignEnable(changeSignEnable);
    }

// ---------------------------------------------------------
// CCalcContainer::SetClearKeyEnable
// This function is called when number of editor is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::SetClearKeyEnable() 
    {
    TCalcEditLine editLine( iEditorPane->EditLine() );
    TBool clearKeyEnable( ETrue );
    
    // eitline's number is 0.0 and length is 1  and the no operator
    TRAP_IGNORE(
        {
        if ( editLine.CheckZeroL() && ( editLine.Operator() ==
            TCalcEditLine::ECalcOperatorNone ) )
            {
            clearKeyEnable = EFalse; 
            }
        }
    )
    iFuncmapPane->SetClearKeyEnable( clearKeyEnable );
    }

// ---------------------------------------------------------
// CCalcContainer::SetChangeSignDisable
// This function is called when Operator +, - , *, / key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::SetChangeSignDisable()
    {
    
    TBool changeSignEnable(EFalse);
    iFuncmapPane->SetChangeSignEnable(changeSignEnable);
    
    }

// ---------------------------------------------------------
// CCalcContainer::SetSqrtEnableL
// This function is called when number of editor is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::SetSqrtEnableL()
    {
    TCalcEditLine editLine(iEditorPane->EditLine());
    TBool sqrtEnable(ETrue);
    
    if ( editLine.NumberL() == 0.0 )
        {
        sqrtEnable = EFalse;    
        }
    iFuncmapPane->SetSqrtEnable(sqrtEnable);
    }

// ---------------------------------------------------------
// CCalcContainer::SetPercentEnableL
// This function is called when number of editor is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::SetPercentEnableL()
    {
    TCalcEditLine editLine(iEditorPane->EditLine());
    TBool percentEnable(ETrue);
    
    if (editLine.NumberL() == 0.0)
        {
        percentEnable = EFalse;    
        }
    iFuncmapPane->SetPercentEnable(percentEnable);
    }


// ---------------------------------------------------------
// CCalcContainer::TimeoutCallbackL
// If no key is pressed after pressing *-button and a few time
// passes, this function is called    
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcContainer::TimeoutCallbackL(
                         TAny* aObject)
    {
    STATIC_CAST(CCalcContainer*, aObject)->DoTimeoutL();
    return 0;
    }

// ---------------------------------------------------------
// CCalcContainer::TimeoutCallbackL
// If no key is pressed after pressing */button and a few time
// passes, this function is called    
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcContainer::TimeoutCallbackChrL(
                         TAny* aObject)
    {
    STATIC_CAST( CCalcContainer*, aObject )->DoTimeoutChrL();
    return 0;
    }
// ---------------------------------------------------------
// CCalcContainer::TimeoutCallbackL
// If no key is pressed after pressing +#button and a few time
// passes, this function is called    
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcContainer::TimeoutCallbackShiftL(
                         TAny* aObject)
    {
    STATIC_CAST(CCalcContainer*, aObject)->DoTimeoutShiftL();
    return 0;
    }

// ---------------------------------------------------------
// CCalcContainer::NotifyChangeDecimal
// Call when decimal separator is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::NotifyChangeDecimal(TChar aOld, TChar aNew)
    {
    iEditorPane->NotifyChangeDecimal(aOld, aNew);
    iSheetPane->DrawNow();
    }
    
// ---------------------------------------------------------
// CCalcContainer::SetErrorCode
// Call from view  when an error code is display.
// ---------------------------------------------------------
//        
void  CCalcContainer::SetErrorCode(TInt aError)
    {
    iFuncmapPane->SetErrorCode( aError );
    }

// ---------------------------------------------------------
// CCalcContainer::GetHelpContext
// This function is called when Help application is launched.  
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::GetHelpContext(
                TCoeHelpContext& aContext) const
    {
    aContext.iMajor = KUidCalc;
    aContext.iContext = KCALC_HLP_MAIN;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcContainer::CCalcContainer():
    iPrevInput(EKeyNull) 
    {
    }

// Second-phase constructor
void CCalcContainer::ConstructL(
                     CCalcView* aView)
    {
    iView = aView;
    CCalcAppUi* appui = CCalcAppEnv::Static()->AppUi();
    iCalcDocument = STATIC_CAST(CCalcDocument*, appui->Document());
    
    // Make a window-owning control.
    CreateWindowL();
    
    
    
    iFuncmapPane = CCalcFuncmapSubPane::NewL(this);
    iSheetPane = CCalcOutputSheet::NewL(this);
    iEditorPane = CCalcEditorSubPane::NewL(this);

    iTimeout = CPeriodic::NewL(KCallBackPriority);
 
    iTimeoutChr = CPeriodic::NewL( KCallBackPriority );
    iTimeoutShift = CPeriodic::NewL( KCallBackPriority );

    TRect rect(0, 0, 0, 0); 
    // Temporary rect is passed. Correct rect is set in SizeChanged.
    iSkinContext = CAknsBasicBackgroundControlContext::NewL(
        KAknsIIDQsnBgAreaMainCalc, rect, EFalse);
        iValue = 0;

    // Set status pane layout usual.
    CEikonEnv::Static()->AppUiFactory()->StatusPane()->SwitchLayoutL( R_AVKON_STATUS_PANE_LAYOUT_USUAL ); 
    }
    
void CCalcContainer::ActivateL()
    {
    
    CCoeControl::ActivateL();   
    SetChangeSignEnableL();
    SetSqrtEnableL();
    SetPercentEnableL();
    ScrollArrowUpdate();
    SetClearKeyEnable(); 
    }
    

// ---------------------------------------------------------
// CCalcContainer::DoTimeoutL
// If no key is pressed until timeout of *-key,
// this function is called.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::DoTimeoutL()
    {
    iTimeout->Cancel();
    iFuncmapPane->NotifyTimeoutL(); 
    }

// ---------------------------------------------------------
// CCalcContainer::DoTimeoutL
// If no key is pressed until timeout of */key,
// this function is called.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::DoTimeoutChrL()
    {
    iTimeoutChr->Cancel();
    iFuncmapPane->NotifyTimeoutL(); 
    }
// ---------------------------------------------------------
// CCalcContainer::DoTimeoutL
// If no key is pressed until timeout of +#key,
// this function is called.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::DoTimeoutShiftL()
    {
    iTimeoutShift->Cancel();
    iFuncmapPane->NotifyTimeoutL(); 
    }

// ---------------------------------------------------------
// CCalcContainer::HandleAsterKeyTimeoutForKeyPressL
// Called when any key is pressed.
// If timeout notifier for *-key is active, make calculation or
// stop notifier according to user's input.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCalcContainer::HandleAsterKeyTimeoutForKeyPressL(
     const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    if (!(iTimeout->IsActive()))
        {
        return EFalse;
        }

    TBool ret(EFalse);
    
    if (aKeyEvent.iModifiers & EModifierShift)
        {
        // Stop timer for *-key and make calculaiton.
        DoTimeoutL();
        }

    if (aType == EEventKey)
        {
        switch (aKeyEvent.iCode)
            {
            // Stop timer and no calculation is made 
            // if pressed arrow or OK key.
            // This depends on spec of Editing.
            case EKeyLeftArrow:
            case EKeyRightArrow:
            case EKeyUpArrow:
            case EKeyDownArrow:
            case EKeyOK:
                {
                DoTimeoutL();
                ret = ETrue;
                break;
                }
            // Stop timer if *-key is pressed. 
            // If release *-key, reset timer for the key.       
            case KCalcAsteriskBtn:
                {
                iTimeout->Cancel();
                break;
                }
            default:
                {
                // Stop timer for *-key and make calculaiton.
                DoTimeoutL();
                break;
                }
            }
        }
    return ret;
    }

// ---------------------------------------------------------
// CCalcContainer::HandleAsterKeyTimeoutForKeyPressL
// Called when any key is pressed.
// If timeout notifier for */key is active, make calculation or
// stop notifier according to user's input.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCalcContainer::HandleChrKeyTimeoutForKeyPressL(
     const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if ( !( iTimeoutChr->IsActive() ) )
        {
        return EFalse;
        }

    TBool ret( EFalse );
    
    if ( aKeyEvent.iModifiers & EModifierShift )
        {
        DoTimeoutChrL();
        }
    if ( aKeyEvent.iModifiers & EModifierFunc )
        {
        // Stop timer for */key and make calculaiton.
        iTimeoutChr->Cancel();
        } 

    if ( aType == EEventKey )
        {
        switch ( aKeyEvent.iCode )
            {
            // Stop timer and no calculation is made 
            // if pressed arrow or OK key.
            // This depends on spec of Editing.
            case EKeyLeftArrow:
            case EKeyRightArrow:
            case EKeyUpArrow:
            case EKeyDownArrow:
            case EKeyOK:
                {
                DoTimeoutChrL();
                ret = ETrue;
                break;
                }
            default:
                {
                // Stop timer for */key and make calculaiton.
                DoTimeoutChrL();
                break;
                }
            }
        }
    return ret;
    }
// ---------------------------------------------------------
// CCalcContainer::HandleAsterKeyTimeoutForKeyPressL
// Called when any key is pressed.
// If timeout notifier for +#key is active, make calculation or
// stop notifier according to user's input.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCalcContainer::HandleShiftKeyTimeoutForKeyPressL(
     const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if ( !( iTimeoutShift->IsActive() ) )
        {
        return EFalse;
        }

    TBool ret( EFalse );
    
    if ( aKeyEvent.iModifiers & EModifierShift )
        {
        // Stop timer for +#key and make calculaiton.
        iTimeoutShift->Cancel();
        }
    if ( aKeyEvent.iModifiers & EModifierFunc )
        {
        DoTimeoutShiftL();
        }    

    if ( aType == EEventKey )
        {
        switch ( aKeyEvent.iCode )
            {
            // Stop timer and no calculation is made 
            // if pressed arrow or OK key.
            // This depends on spec of Editing.
            case EKeyLeftArrow:
            case EKeyRightArrow:
            case EKeyUpArrow:
            case EKeyDownArrow:
            case EKeyOK:
                {
                DoTimeoutShiftL();
                ret = ETrue;
                break;
                }
            default:
                {
                // Stop timer for +#key and make calculaiton.
                DoTimeoutShiftL();
                break;
                }
            }
        }
    return ret;
    }

// ---------------------------------------------------------
// CCalcContainer::CountComponentControls
// Return count of control components.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcContainer::CountComponentControls() const
    {
    return KCalcCountOfControls ;
    }

// ---------------------------------------------------------
// CCalcContainer::ComponentControl
// Return control pointer which correspond to argument aIndex 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCoeControl* CCalcContainer::ComponentControl
                ( TInt aIndex ) const   
    {
    CCoeControl* control = NULL; 
        
    switch (aIndex)
        {
        case ECalcControlEditorPane:
            {
            control = iEditorPane;
            break;
            }
        case ECalcControlFunctionMap:
            {
            control = iFuncmapPane;
            break;
            }
        case ECalcControlOutputSheet:
            {
            control = iSheetPane;
            break;
            }
        default:
            {         
            break;
            }
        }

    return control;
    }

// ---------------------------------------------------------
// CCalcContainer::OfferKeyEventL
// This function is called when a key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CCalcContainer::OfferKeyEventL
                ( const TKeyEvent& aKeyEvent,  
                  TEventCode aType )           
    {
    switch (aType)
        {
        case EEventKeyDown:
        case EEventKey:
            {
            if (aKeyEvent.iScanCode != EStdKeyLeftArrow &&
            aKeyEvent.iScanCode != EStdKeyRightArrow &&
            aKeyEvent.iScanCode != EStdKeyUpArrow &&
            aKeyEvent.iScanCode != EStdKeyDownArrow &&
            aKeyEvent.iCode != EKeyOK &&
            aKeyEvent.iCode != EKeyEnter &&
            aKeyEvent.iScanCode != EStdKeyEnter )
                {
                iFuncmapPane->NotifyOtherThanOkKeyPressed();
                }
            iPrevInput = aKeyEvent.iCode;

            if(!(iFuncmapPane->IsQwertyKeypadActive()))
             {
            if (HandleAsterKeyTimeoutForKeyPressL(aKeyEvent, aType))
                {
                return EKeyWasConsumed;
                }
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
            else
                {
                if ( iFuncmapPane->GetKeyboardType() == EPtiKeyboardHalfQwerty )
                    {
                    if ( HandleChrKeyTimeoutForKeyPressL( aKeyEvent, aType ) )
                        {
                        return EKeyWasConsumed;
                        }  
                    if ( HandleShiftKeyTimeoutForKeyPressL( aKeyEvent, aType ) )
                        {
                        return EKeyWasConsumed;
                        } 
                    }
                }
#endif
            iValue =1 ;
            break;
            }
        case EEventKeyUp:
            {
            if(!(iFuncmapPane->IsQwertyKeypadActive()))
             {
                
            if (iPrevInput == KCalcAsteriskBtn && !iTimeout->IsActive() && iValue ==1 )
                {
                TCallBack callback(TimeoutCallbackL, this);
                iTimeout->Start((TTimeIntervalMicroSeconds32) KCallBackDelay,
                                (TTimeIntervalMicroSeconds32) KCallBackInterval, 
                                callback);
                }
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT  
            else
                {
                if ( iFuncmapPane->GetKeyboardType() == EPtiKeyboardHalfQwerty )
                    {
                    if ( aKeyEvent.iScanCode == EStdKeyLeftFunc && !iTimeoutChr->IsActive() && iValue == 1 )
                        {
                        TCallBack callback( TimeoutCallbackChrL, this );
                        iTimeoutChr->Start( ( TTimeIntervalMicroSeconds32 ) KCallBackDelay,
                                     ( TTimeIntervalMicroSeconds32 ) KCallBackInterval, 
                                     callback );
                        }
                    if ( aKeyEvent.iScanCode == EStdKeyLeftShift && !iTimeoutShift->IsActive() && iValue == 1 )
                        {
                        TCallBack callback( TimeoutCallbackShiftL, this );
                        iTimeoutShift->Start( ( TTimeIntervalMicroSeconds32 ) KCallBackDelay,
                                     ( TTimeIntervalMicroSeconds32 ) KCallBackInterval, 
                                     callback );
                        }
                    }
                }
#endif

            iFuncmapPane->NotifyReleaseKeyL();
            iValue =0;
            
            DrawNow(); //redraw screen when a button up
            break;
            }
        default:
            {
            break;
            }
        }

    TKeyResponse keyResponse(iFuncmapPane->OfferKeyEventL(aKeyEvent, aType));
    if (keyResponse == EKeyWasNotConsumed)
        {
        if(iFuncmapPane->IsQwertyKeypadActive())
             {
             iEditorPane->IsQwertyActive();
             }
        else
            {
            iEditorPane->IsQwertyNotActive();    
            }
        // Edit buffer of line
        keyResponse = iEditorPane->OfferKeyEventL(aKeyEvent, aType);
        }
    return keyResponse;
    }

// ---------------------------------------------------------
// CCalcContainer::HandleResourceChange
// Notifier for changing language
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::HandleResourceChange(TInt aType)
    {
    TRAP_IGNORE( HandleResourceChangeCalSoftL( aType ) );
    }

// ---------------------------------------------------------
// CCalcContainer::HandleResourceChange
// Notifier for changing language
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::HandleResourceChangeCalSoftL(TInt aType)
    {
    if ((aType == KEikDynamicLayoutVariantSwitch) || (aType == KAknsMessageSkinChange) )
        {
                 
         TRect mainPaneRect ;
         TRect statusPaneRect;
         TBool signstate = EFalse;
         TBool sqrtstate = EFalse;
         TBool percentstate = EFalse;
        TBool clearstate = EFalse;
                 
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            // when calculator is in Landscape layout, the statuspane displays     
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EStatusPane, statusPaneRect );
            mainPaneRect.iTl = statusPaneRect.iTl;
            }
        else
            {
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EStatusPane, statusPaneRect );
            mainPaneRect.iTl = statusPaneRect.iTl;
            }
        
        
        //check if funcpane already exists

      TInt selected = 0;
        if(iFuncmapPane)
        {
            //store the  states of the buttons
            signstate = iFuncmapPane->GetChangeSignButtonState();
            sqrtstate = iFuncmapPane->GetSqrtButtonState();
            percentstate = iFuncmapPane->GetPercentButtonState();

            selected = iFuncmapPane->GetSelectedButtonId();
            
            clearstate = iFuncmapPane->GetClearButtonState(); 
             
             //delete the function pane
            delete(iFuncmapPane);
            iFuncmapPane =  NULL;
            
        }
        //Reload the bitmaps
        (CCalcAppEnv::Static())->SetSkinChangedValue(EFalse);
        (CCalcAppEnv::Static())->UpdateAknConstArrayForFuncMap();
        (CCalcAppEnv::Static())->LoadFuncMapBitmapL();
        
        //Create the new pane
        iFuncmapPane = CCalcFuncmapSubPane::NewL(this); 

        if ( AknLayoutUtils::PenEnabled() )
        {            
        iFuncmapPane->SetHighlightButton( 0, selected );   
        }
        
        iFuncmapPane->ActivateL();
        
        SetRect(mainPaneRect);
        iFuncmapPane->NotifyLangChange();
        DrawNow();

        //Restore the states of the buttons
        iFuncmapPane->SetChangeSignEnable(signstate);
        iFuncmapPane->SetSqrtEnable(sqrtstate);
        iFuncmapPane->SetPercentEnable(percentstate);
        iFuncmapPane->SetClearKeyEnable( clearstate );
        
        //Update scroll bar here
        ScrollArrowUpdate();
                iSheetPane->HandleResourceChange(aType);
        }
    else
        {
        
        CCoeControl::HandleResourceChange(aType);
        }
      
    }
 

// ---------------------------------------------------------
// CCalcContainer::SizeChanged
// Control size is set.
// This function is called when the size changes.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::SizeChanged()
    {
    TRect parentRect(Rect());
    
    if (iSkinContext)
        {
        iSkinContext->SetRect(parentRect);
        }

    
    if( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
        {
             
            // Set layout of function map subpane, output sheet  and editor sub pane.        
            if( AknLayoutUtils::PenEnabled() )
            {
                if (Layout_Meta_Data::IsLandscapeOrientation())
                {
                
                    AknLayoutUtils::LayoutControl(
                    iFuncmapPane, parentRect, AknLayoutScalable_Apps::grid_calc_pane(enTouch_with_lsc).LayoutLine());
                    
                    AknLayoutUtils::LayoutControl(
                    iSheetPane, parentRect,AknLayoutScalable_Apps::calc_paper_pane(enTouch_with_lsc).LayoutLine());
                        
                    AknLayoutUtils::LayoutControl(
                    iEditorPane, parentRect,AknLayoutScalable_Apps::calc_display_pane(enTouch_with_lsc).LayoutLine());
                }
                else
                {
                    AknLayoutUtils::LayoutControl(
                    iFuncmapPane, parentRect, AknLayoutScalable_Apps::grid_calc_pane(enTouch_with_prt).LayoutLine());
                    
                    AknLayoutUtils::LayoutControl(
                    iSheetPane, parentRect,AknLayoutScalable_Apps::calc_paper_pane(enTouch_with_prt).LayoutLine());
                    
                    AknLayoutUtils::LayoutControl(
                    iEditorPane, parentRect,AknLayoutScalable_Apps::calc_display_pane(enTouch_with_prt).LayoutLine());
                }
            }
            else
            {
                AknLayoutUtils::LayoutControl(
                    iFuncmapPane, parentRect, AknLayoutScalable_Apps::grid_calc_pane(enTouch_disabled).LayoutLine());
                    
                AknLayoutUtils::LayoutControl(
                iSheetPane, parentRect,AknLayoutScalable_Apps::calc_paper_pane(enTouch_disabled).LayoutLine());
                
                AknLayoutUtils::LayoutControl(
                iEditorPane, parentRect,AknLayoutScalable_Apps::calc_display_pane(enTouch_disabled).LayoutLine());
                
            }    

        }
    else
        {
    // Set layout of function map subpane.
    AknLayoutUtils::LayoutControl(
    iFuncmapPane, parentRect, AppLayout::grid_calc_pane());    
    
    
    // Set layout of output sheet.
    AknLayoutUtils::LayoutControl(
         iSheetPane, parentRect,AppLayout::gqn_graf_calc_paper());
    
    // Set layout of editor subpane.
    AknLayoutUtils::LayoutControl(
        iEditorPane, parentRect,AppLayout::Calculator_elements_Line_1());
        }    
    }

// ---------------------------------------------------------
// CCalcContainer::Draw
// Clear whole screen. After this, draw editor, output sheet,
// and function map.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    gc.Clear(aRect);

    // Drawing skin
    if (iSkinContext)
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        
        AknsDrawUtils::Background(
            skin, iSkinContext, this, gc, aRect);
        }
     
    }

// ---------------------------------------------------------
// CCalcContainer::MopSupplyObject()
// Pass skin information if need.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TTypeUid::Ptr CCalcContainer::MopSupplyObject(TTypeUid aId)
    {
    if (aId.iUid == MAknsControlContext::ETypeId && iSkinContext)
        {
        return MAknsControlContext::SupplyMopObject(aId, iSkinContext);
        }

    return CCoeControl::MopSupplyObject(aId);
    }

// ---------------------------------------------------------
// CCalcContainer::ShowSqrtButton()
// Enable (or disable) the square root button.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::ShowSqrtButton(TBool aEnable)
    {
    TCalcEditLine editLine( iEditorPane->EditLine() );
    TBool sqrtEnable(ETrue);
    TRAP_IGNORE(
    if ( editLine.NumberL() == 0 )
        {
        sqrtEnable = EFalse;    
        }
    )

    iFuncmapPane->SetSqrtEnable( sqrtEnable );
    }    

// ---------------------------------------------------------
// CCalcContainer::ShowPercentButton()
// Enable (or disable) the percent button.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::ShowPercentButton(TBool aEnable)
    {
    iFuncmapPane->SetPercentEnable(aEnable);
    }

// ---------------------------------------------------------
// CCalcContainer::GetState()
// Returns the state of the calculator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCalcView::TStateNo CCalcContainer::GetState()
{
    return (iView->State());
}

// ---------------------------------------------------------
// CCalcContainer::HandlePointerEventL
// Handled when pen input occured.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcContainer::HandlePointerEventL
                ( const TPointerEvent& aPointerEvent )           
    {
    CCoeControl::HandlePointerEventL( aPointerEvent );
    }
    

// ---------------------------------------------------------
// ---------------------------------------------------------
// CCalcContainer::HandleMiddleSoftKey()
// Handled when MSK is selected.
// ---------------------------------------------------------
//
void CCalcContainer::HandleMiddleSoftKey()
                
    {
    TRAP_IGNORE( iFuncmapPane->HandleMiddleSoftKeyOREKeyOKL() );
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
// SetOperatorFromTouchL
// Handle digit inputs
// ---------------------------------------------------------
//
void CCalcContainer::SetOperatorFromTouchL(TInt akey )
{
      //Simulate events as events are occurring from the KB Events
      TKeyEvent eventkey;
      TEventCode keycode;
    keycode = EEventKeyDown;
    eventkey.iCode = 0;
    eventkey.iScanCode = ASCII_ZERO + akey;
    
    //First send Keydown event
       OfferKeyEventL(eventkey,keycode);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    eventkey.iCode = ASCII_ZERO + akey;
#else
    eventkey.iCode = KEY_CODE_VAL;
#endif
    keycode = EEventKey;
    
    //Next send EventKey
    OfferKeyEventL(eventkey,keycode);
    
    
    //Finally send Keyup
    eventkey.iScanCode = ASCII_ZERO + akey;
    keycode = EEventKeyUp;
    OfferKeyEventL(eventkey,keycode);
}

// ---------------------------------------------------------
// ---------------------------------------------------------
// ClearInputKeyL
// Handle 'c' input key from Touch
// ---------------------------------------------------------
//
void CCalcContainer::ClearInputKeyL(TInt aRepeat)
{
      //Simulate events as events are occurring from the KB Events
    TKeyEvent eventkey;
    TEventCode keycode;
    keycode = EEventKeyDown;
    eventkey.iCode = 0; 
    eventkey.iScanCode = 1; //for clear input key
    eventkey.iRepeats = aRepeat;
    
    //First send Keydown event
    OfferKeyEventL(eventkey,keycode);
    eventkey.iCode = 8;  //clear input key icode
    keycode = EEventKey;
    
    //Next send EventKey
    OfferKeyEventL(eventkey,keycode);
    keycode = EEventKeyUp;
    
     //Finally send Keyup
    OfferKeyEventL(eventkey,keycode);    
    
}


// ---------------------------------------------------------
// ---------------------------------------------------------
// SetSeparatorFromTouch
// Handle '.' input key from Touch
// ---------------------------------------------------------
//
void CCalcContainer::SetSeparatorFromTouchL()
{
    // set the right iCode and iScanCode for
    // decimal point
    if ( iFuncmapPane->IsQwertyKeypadActive() )
        {
        // Simulate events as events are occurring from the KB Events
        TKeyEvent eventkey;
        TEventCode keycode = EEventKeyDown;
        eventkey.iCode = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT  
        if ( iFuncmapPane->GetKeyboardType() == EPtiKeyboardHalfQwerty )
            {
            eventkey.iScanCode = EStdKeySingleQuote;  //scan code for separator
            eventkey.iCode = ( TUint )KDotSeparator; // icode for separator
            }
        else
#endif
            {
            TLocale locale;
            //Get a decimal separator from system for current phone language.
            TChar decimalSeparator( locale.DecimalSeparator() );
            
            if ( KCommaSeparator == decimalSeparator )
                {
                //separator is Comma
                eventkey.iScanCode = EStdKeyComma; //scan code for separator
                }
            else
                {
                //separator is dot
                eventkey.iScanCode = EStdKeyFullStop; //scan code for separator
                }
            eventkey.iCode = ( TUint )decimalSeparator;   // icode for separator
            }

        // First send Keydown event
        OfferKeyEventL( eventkey, keycode );
        
        keycode = EEventKey;

        // Next send EventKey
        OfferKeyEventL( eventkey, keycode );
        }
    else
        {
        // Simulate events as events are occurring from the KB Events    
        TKeyEvent eventkey;
        TEventCode keycode = EEventKeyDown;
        eventkey.iCode = 0;
        eventkey.iScanCode = EStdKeyHash;  // scan code for separator

        // First send Keydown event
        OfferKeyEventL( eventkey, keycode );
        eventkey.iCode = ( TUint )KHashSeparator;  // icode for separator
        keycode = EEventKey;

        // Next send EventKey
        OfferKeyEventL( eventkey, keycode );
        }     
}
   
//  End of File  CALCCONT_CPP
