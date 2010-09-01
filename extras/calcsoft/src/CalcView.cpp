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
* Description:  CCalcView is a view class of Calculator application.
*                Base class of CCalcView is CAknView. 
*
*/



// INCLUDE FILES
#include <avkon.hrh>
#include <eikon.hrh>
#include <eikmenup.h>
#include <aknnotewrappers.h> 
#include <AiwServiceHandler.h>

#include <aknbutton.h>

#include <hlplch.h>   // For HlpLauncher 
#include <featmgr.h>      // Feature Manager

#include <Calcsoft.rsg>

#include "CalcView.h"
#include "CalcCont.h" 
#include "CalcEdit.h"
#include "CalcAppUi.h"
#include "CalcDoc.h"
#include "CalcHistory.h"
#include "CalcOutSheet.h"
#include "CalcFunc.h"
#include "CalcEnv.h"
#include "calc.hrh" 
#include "CalcEditline.h"

//#include    "CalcCmdBtn.h"

#include <layoutmetadata.cdl.h>
#include <AiwMenu.h>
#include <AiwCommon.h>


//  LOCAL CONSTANTS AND MACROS  


// ================= MEMBER FUNCTIONS =======================


// Two-phased constructor.
CCalcView* CCalcView::NewLC()
    {
    CCalcView* self = new (ELeave) CCalcView();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
  
  // default constructor can leave.
void CCalcView::ConstructL()
    {
    
    BaseConstructL(R_CALC_VIEW_INFO);
    
    iCalcDocument = STATIC_CAST(CCalcDocument*, AppUi()->Document());

    iCalcDocument->SetCalcView( this );

    iCalcDocument->LoadStateL();
   
    /******** Initialize the ServiceHandler in the ConstructL *****/
    
    iServiceHandler = CAiwServiceHandler::NewL();
    
    /***** Attach the Menu Items of the Calculator and also the Interest *****/
    
    iServiceHandler->AttachMenuL(R_CALC_POPUPMENU, R_AIWHELPAPP_INTEREST);
    
    this->iLastResultSetected = EFalse;
    }

// Destructor
CCalcView::~CCalcView()
    {
    if ( iContainer )
        {
        delete iContainer;
        iContainer = NULL;
        }

    if( iServiceHandler )
        {
        delete iServiceHandler;
        iServiceHandler = NULL;
        }
     
     }

// ---------------------------------------------------------
// CCalcView::State
// Return current state
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCalcView::TStateNo CCalcView::State() const
    {
    return iState;
    }

// ----------------------------------------------------
// CCalcView::LatestInput
// Return type of latest input
// (other items were commented in a header).
// ----------------------------------------------------
//
CCalcView::TKindOfInput CCalcView::LatestInput() const
    {
    return iLatestInput;
    }

// ----------------------------------------------------
// CCalcView::UpdateState
// Update state by using kind of input
// (other items were commented in a header).
// ----------------------------------------------------
//
void CCalcView::UpdateState
        (const TKindOfInput& aKind)   
    {
    iLatestInput = aKind;
    
    switch (aKind)
       {
        case EOperand:
        case EMemoryRecall:
            {
            switch (iState)
                {
                case EInitialMode: 
                case ESelectResult:
                    iState = EOperandOnly;
                    break;
                case EOperandAndOperator:
                    iState = EOperandAndOperatorAndOperand;
                    break;
                default:
                    break;
                }
            break;
            }
        case EOperatorResult:
            {
            iState = EOperatorOnlyResult;
            break;
            }
        case EOperator:
            {
            iState = EOperandAndOperator;
            break;
            }
        case EEqual:
            {
            iState = ESelectResult;
            break;
            }
        case EMemorySave:
            {
            switch (iState)
                {
                case EOperandAndOperatorAndOperand:
                    iState = ESelectResult;
                    break;
                case ESelectResult:
                    iState = EOperandOnly;
                    break;
                default:
                    break;
                }
            break;
            }
        case EAllClear:
            {
            iState = EOperandOnly;
            break;
            }
        default:
            {
            break;
            }
        }
    }


// ----------------------------------------------------
// CCalcView::ErrorMsgL
// Display error message
// (other items were commented in a header).
// ----------------------------------------------------
//
void CCalcView::ErrorMsgL
        (TInt aErrorCode) 
    {
    TInt resourceId(0);

    switch (aErrorCode)
        {
        case KErrOverflow:
            {
            resourceId = R_CALC_ERROR_OUTOFRANGE;
            break;
            }
        case KErrDivideByZero:
            {
            resourceId = R_CALC_ERROR_DIVISIONBYZERO;

             // UpdateState( EOperator );
            break;
            }
        case KErrNotSupported:
            {
            resourceId = R_CALC_ERROR_NEGATIVEVALUESQRT;
            break;
            }
        default:
            {
            return;
            }
        }
    
    //Localisation of scalable UI.
    HBufC* message = iCoeEnv->AllocReadResourceLC(resourceId) ;
    CAknErrorNote* note = new (ELeave) CAknErrorNote();
    note->ExecuteLD(*message);
    CleanupStack::PopAndDestroy(message);
    
    }


// ----------------------------------------------------
// CCalcView::DynInitMainMenuPane
// Delete meaningless items from main menu.
// (other items were commented in a header).
// ----------------------------------------------------
//
void CCalcView::DynInitMainMenuPane    
              (CEikMenuPane* aMenuPane) 
    {
    
            

    switch (iState)
        {
        case ESelectResult:
            {
            aMenuPane->DeleteMenuItem(ECalcCmdLastResult);
            break;
            }
        case EInitialMode:
            {
            aMenuPane->DeleteMenuItem(ECalcCmdClearCalcAndHistory);
            break;
            }
        case EOperandOnly:
            {
            if (iCalcDocument->History()->Count() == 0)
                {
                aMenuPane->DeleteMenuItem(ECalcCmdClearCalcAndHistory);
                }
             if(iLastResultSetected)
                 {
                aMenuPane->DeleteMenuItem(ECalcCmdLastResult);     
                 }
            break;
            }
        case EOperandAndOperator:
        case EOperandAndOperatorAndOperand:
             if(iLastResultSetected)
                 {
                aMenuPane->DeleteMenuItem(ECalcCmdLastResult);     
                 }
            break;      
        default:
            {
            break;
            }
        }
    }                        


// ----------------------------------------------------
// CCalcView::DynInitMemoryMenuPane
// Delete meaningless items from menu of memory 
// (other items were commented in a header).
// ----------------------------------------------------
//
void CCalcView::DynInitMemoryMenuPane
        (CEikMenuPane* aMenuPane)
    {
    if (!iCalcDocument->HasMemory())
        {
        aMenuPane->DeleteMenuItem(ECalcCmdMemoryClear);
        }    
    }
    
// ---------------------------------------------------------
// CCalcView::NotifyChangeDecimal
// Call when decimal separator is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::NotifyChangeDecimal(TChar aOld, TChar aNew)
    {
    iContainer->NotifyChangeDecimal(aOld, aNew);
    }
    


// ---------------------------------------------------------
// CCalcView::HandleCommandL
// Handling command.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleCommandL
        ( TInt aCommand ) 
    {
    // Handle the command generated by:
    //   1. menu item selection
    //   2. short-cut key press
   
    switch ( aCommand )
        {
        case ECalcCmdResult:
            {
            HandleResultL();
            break;
            }
        case ECalcCmdAdd:
            {
            HandleOperatorL(TCalcEditLine::ECalcAdd);
            break;
            }
        case ECalcCmdSubtract:
            {
            HandleOperatorL(TCalcEditLine::ECalcSubtract);
            break;
            }
        case ECalcCmdMultiply:
            {
            HandleOperatorL(TCalcEditLine::ECalcMultiply);
            break;
            }
        case ECalcCmdDivide:
            {
            HandleOperatorL(TCalcEditLine::ECalcDivide);
            break;
            }
        case ECalcCmdChangeSign:
            {
            this->iLastResultSetected = EFalse;
            HandleChangeSignL();
            break;
            }
        case ECalcCmdLastResult:
            {
            HandleLastResultL();
            break;
            }
        case ECalcCmdPercent:
            {
            HandleOperatorL(TCalcEditLine::ECalcPercent);
            // The result is always is displayed
            // after the percent button is pressed.
            // But if there was a error in the calculation
            // this if will enable us to not call the
            // HandleResultL and thus making sure that
            // the state of the buttons is identical to the
            // situation before the calculation.
            if (iState == EOperatorOnlyResult)
                HandleResultL();
            break;
            }
        case ECalcCmdSqrt:
            {
//            TRequestStatus trs;
//            RTimer timer;
//            timer.CreateLocal();
//            timer.After(trs, 1000*2);
//            while ( trs == KRequestPending) {
//                  User::WaitForAnyRequest();
//            }
//            timer.Cancel();
//            timer.Close();
            HandleOperatorL(TCalcEditLine::ECalcSqrt);
            // HandleResultL only if sqrt was pressed after single operand.
            if (iState == EOperatorOnlyResult)
                HandleResultL();

            break;
            }
        case ECalcCmdMemorySave:
            {
            HandleMemorySaveL();
            break;
            }
        case ECalcCmdMemoryRecall:
            {
            this->iLastResultSetected = EFalse;
            HandleMemoryRecallL();
            break;
            }
        case ECalcCmdMemoryClear:
            {
            HandleMemoryClearL();
            break;
            }
        case ECalcCmdScrollUp:
            {
            HandleScrollUp();
            break;
            }
        case ECalcCmdScrollDown:
            {
            HandleScrollDown();
            break;
            }
        case ECalcCmdClearCalcAndHistory:
            {
            DoClearCalcAndHistoryL();
            break;
            }
        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            STATIC_CAST(CCalcAppUi*, AppUi())->ExitCalculator();
            break;
            }
        case ECalcCmdHelp: 
            {
            if (FeatureManager::FeatureSupported( KFeatureIdHelp ))
                {
                HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), AppUi()->AppHelpContextL());
                }
            break;
            }
        case EAknSoftkeySelect:
            {
            iContainer->HandleMiddleSoftKey();
            break;
            }
            
        //Below cases are possible only for Touch UI Layouts
        case ECalcCmdOne:
            {
            iContainer->SetOperatorFromTouchL(1);
            }
            break;
        case ECalcCmdTwo:
            {
            iContainer->SetOperatorFromTouchL(2);
            }
            break;
        case ECalcCmdThree:
            {
            iContainer->SetOperatorFromTouchL(3);
            }
            break;
        case ECalcCmdFour:
            {
            iContainer->SetOperatorFromTouchL(4);
            }
            break;
        case ECalcCmdFive:
            {
            iContainer->SetOperatorFromTouchL(5);
            }
            break;
        case ECalcCmdSix:
            {
            iContainer->SetOperatorFromTouchL(6);
            }
            break;
        case ECalcCmdSeven:
            {
            iContainer->SetOperatorFromTouchL(7);
            }
            break;
        
        case ECalcCmdEight:
            {
            iContainer->SetOperatorFromTouchL(8);
            }
            break;
        
        case ECalcCmdNine:
            {
            iContainer->SetOperatorFromTouchL(9);
            }
            break;
        
        case ECalcCmdZero:
            {
            iContainer->SetOperatorFromTouchL(0);
            }
            break;
            
        case ECalcCmdSeparator:
            {
            iContainer->SetSeparatorFromTouchL();
            }
            break;
        case ECalcCmdCancel:
            {
            TInt repeat( iContainer->FuncmapSubPane()->IsKeyLongPressed() );           
            iContainer->ClearInputKeyL( repeat );
            }
            break; 
            
    /****** If none of the cases handle the Aiw Help then it will come to the default case ****/
        default:
            {
                 CArrayFix<TCoeHelpContext>* buf = AppUi()->AppHelpContextL();
                 TCoeHelpContext& helpContext = buf->At(0);         
                   TAiwVariant uidVariant;
                 TAiwVariant contextVariant;         
                 uidVariant.Set(helpContext.iMajor);         
                 TAiwGenericParam uidParam(EGenericParamHelpItem, uidVariant);                          contextVariant.Set(helpContext.iContext);         
                 contextVariant.Set(helpContext.iContext);         
                 TAiwGenericParam contextParam(EGenericParamHelpItem, contextVariant);     
                 CAiwGenericParamList* list = CAiwGenericParamList::NewLC();         
                 list->AppendL(uidParam);
                 list->AppendL(contextParam); 
                 iServiceHandler->ExecuteMenuCmdL(aCommand,*list,iServiceHandler->OutParamListL());     
                 CleanupStack::PopAndDestroy(list);
                 delete buf;
                 buf= NULL;
            break;
            }
        }
    }


// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcView::CCalcView():
           iState(EInitialMode),
           iLatestInput(EOperand)
    {
    }


// ---------------------------------------------------------
// CCalcView::CreateContainerL
// Create CCalcContainer class
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::CreateContainerL()
    {
    iContainer = CCalcContainer::NewL(this);
    
    iContainer->SetMopParent(this);
        
    TRect mainPaneRect ;
    TRect statusPaneRect;
    
    if (Layout_Meta_Data::IsLandscapeOrientation())
      
      {
      
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,mainPaneRect);
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EStatusPane,statusPaneRect);
        mainPaneRect.iTl= statusPaneRect.iTl;
          
      }
      
      else
      
      {
      
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,mainPaneRect);
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EStatusPane,statusPaneRect);
       
       mainPaneRect.iTl= statusPaneRect.iTl;
                                                
      } 
    
    
    iContainer->SetRect(mainPaneRect);    
    iEditorSubPane = iContainer->EditorPane();
    iOutputSheet = iContainer->OutputSheet();
    }


// ---------------------------------------------------------
// CCalcView::HandleResultL
// Handle result command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleResultL()
    {
    if (!HandleEqualSubRoutineL())
        {
        iEditorSubPane->DrawNow();
        UpdateState(EEqual);

        iContainer->SetSqrtEnableL();
        iContainer->ShowPercentButton(EFalse);
        iContainer->SetClearKeyEnable();             
        }
    }


// ---------------------------------------------------------
// CCalcView::HandleChangeSignL
// Handle ChangeSign command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleChangeSignL()
    {
    iEditorSubPane->DoChangeSignL(iState);        
    
    if (iState == CCalcView::EOperandAndOperator)
        {
        UpdateState(EMemoryRecall);
        }
    }

// ---------------------------------------------------------
// CCalcView::HandleLastResultL
// Handle LastResult command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleLastResultL()
    {
    iLastResultSetected = ETrue;
    //  Recall "LastResult", which is the answer based on latest "=" input.
    iEditorSubPane->DoMemoryRecallOrLastResult(
        iCalcDocument->LastResult(), iState);

    UpdateState(EMemoryRecall);
    if ( iState == EOperandOnly && 
         iEditorSubPane->EditLine().CheckZeroL() )
        {
        UpdateState(EAllClear);
        }
    iContainer->SetChangeSignEnableL();
    iContainer->SetSqrtEnableL();
    iContainer->SetClearKeyEnable(); 

    //highlighting the sign "%".
    if ( iState == EOperandAndOperatorAndOperand )
        {
        iContainer->SetPercentEnableL();
        }        
    }

// ---------------------------------------------------------
// CCalcView::HandleMemorySaveL
// Handle MemorySave command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleMemorySaveL()
    {
    CCalcView::TKindOfInput kindOfInput(CCalcView::EMemorySave);
    
    if (iState == EOperandAndOperatorAndOperand)
        {
        if (HandleEqualSubRoutineL()) // Calculate result
            {
            return;
            }
        }
    else
        {
        if (iState == ESelectResult)
            {
            iCalcDocument->AddEmptyLine();
            iOutputSheet->ScrollToBottomL();
            iContainer->ScrollArrowUpdate();
            iEditorSubPane->SetOperator(TCalcEditLine::ECalcOperatorNone);
            
            if (iEditorSubPane->EditLine().CheckZeroL())
                {
                kindOfInput = CCalcView::EAllClear;
                }
            }
        }
    iCalcDocument->MemorySaveL(iEditorSubPane->EditorNumber());
    iEditorSubPane->DrawNow();
    UpdateState(kindOfInput);
    iContainer->SetChangeSignEnableL();
    iContainer->SetSqrtEnableL();
    }

// ---------------------------------------------------------
// CCalcView::HandleMemoryRecallL
// Handle MemoryRecall command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleMemoryRecallL()
    {
    iEditorSubPane->DoMemoryRecallOrLastResult(
        iCalcDocument->Memory(), iState );

    if ( iState == ESelectResult )
        {
        iCalcDocument->AddEmptyLine();
        iOutputSheet->ScrollToBottomL();
        iContainer->ScrollArrowUpdate();
        }
        
    UpdateState( EMemoryRecall );
    if ( iState == EOperandOnly && 
         iEditorSubPane->EditLine().CheckZeroL() )
        {
        UpdateState( EAllClear );
        }

    iContainer->SetChangeSignEnableL();
    iContainer->SetSqrtEnableL();

    iContainer->SetClearKeyEnable();

    // highlighting the sign "%".
    if ( iState == EOperandAndOperatorAndOperand )
        {
        iContainer->SetPercentEnableL();
        }        
    }

// ---------------------------------------------------------
// CCalcView::HandleMemoryClear
// Handle MemoryClear command
// (other items were commented in a header).
// ---------------------------------------------------------
// 

void CCalcView::HandleMemoryClearL()
    {
    iCalcDocument->MemoryClearL();
    iEditorSubPane->DrawNow();
    }

// ---------------------------------------------------------
// CCalcView::HandleScrollUp
// Handle Scroll up button command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleScrollUp()
    {
    iOutputSheet->ScrollUp();
    iContainer->ScrollArrowUpdate();
    }

// ---------------------------------------------------------
// CCalcView::HandleScrollDown
// Handle Scroll down button command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleScrollDown()
    {
    iOutputSheet->ScrollDown();
    iContainer->ScrollArrowUpdate();
    }

// ---------------------------------------------------------
// CCalcView::DoClearCalcAndHistoryL
// Clear calculation history and clear calculation.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::DoClearCalcAndHistoryL()
    {
    iCalcDocument->History()->ClearHistory();
    iOutputSheet->NotifyClearHistoryL();
    iContainer->ScrollArrowUpdate();
    // Clear calculation
    TKeyEvent keyEvent;
    keyEvent.iCode = EKeyBackspace;
    keyEvent.iRepeats = 1; 
    iEditorSubPane->OfferKeyEventL(keyEvent, EEventKey);
    iContainer->ShowPercentButton(EFalse);
    }
        

// ---------------------------------------------------------
// CCalcView::HandleOperatorL
// Operator Handler common routin.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::HandleOperatorL
        (TCalcEditLine::TCalcOperatorType aOperator) 
    {
    TCalcEditLine editLine(iEditorSubPane->EditLine());
    TReal64 editorNumber(iEditorSubPane->EditorNumber());
    TReal64 result(editorNumber);

    this->iLastResultSetected = EFalse;
    // User has pressed %
    if (aOperator == TCalcEditLine::ECalcPercent)
        {    
        if (iState == EOperandAndOperatorAndOperand)
            {
            editLine.TrimZeroL();
            // Save the previous operator
            TCalcEditLine::TCalcOperatorType previousOperator = editLine.Operator();
            // Set the operator to percent
            editLine.SetOperator(TCalcEditLine::ECalcPercent);
            
            
            // Calculate
            TRAPD(errorCode3, 
                result = iCalcDocument->CalculateAndModifyHistoryL(
                editorNumber, editLine, previousOperator));
            
            if (errorCode3)
                {
                iContainer->SetErrorCode(errorCode3);
                return;
                }

            iOutputSheet->ScrollToBottomL();
            // Show the correct operand in the output sheet
            aOperator = previousOperator;
            // Set the state to EOperatorResult so that the HandleResult knows what to do
            UpdateState(EOperatorResult);
            }    
        }

    // User has pressed sqrt
    else if (aOperator == TCalcEditLine::ECalcSqrt)
        {
        // We are in a middle of calculation chain
        if (iState == EOperandAndOperatorAndOperand)
            {
            editLine.TrimZeroL();
            // Save the previous operator
            TCalcEditLine::TCalcOperatorType previousOperator = editLine.Operator();
            // Set the operator to sqrt
            editLine.SetOperator(TCalcEditLine::ECalcSqrt);
            
            // First calculate the sqrt
            TRAPD(errorCode, 
                result = iCalcDocument->CalculateAndNoHistoryL(
                editorNumber, editLine));
                                                 
            if (errorCode)
                {
                iContainer->SetErrorCode(errorCode);
                return;
                }
            
            // Set the operator to previous one
            editLine.SetOperator(previousOperator);
            // Result goes to the outputsheet
            editLine.SetNumber(result);

            // Calculate again
            TRAPD(errorCode2, 
                result = iCalcDocument->CalculateAndAddHistoryL(
                result, editLine));
                                                 
            if (errorCode2)
                {
                iContainer->SetErrorCode(errorCode2);
                return;
                }
            // Set this so that we will display correct operator in the output sheet
            aOperator = previousOperator;
            iOutputSheet->ScrollToBottomL();
            UpdateState(EOperator);
            // Dimm the square root and percent
            iContainer->ShowSqrtButton(EFalse);
            iContainer->ShowPercentButton(EFalse);
            }
        // Only operand and sqrt
        else
            {
            editLine.SetOperator(aOperator);
                    
            // Trap harness, because the CalculateAndHistoryL leaves in case of negative operand
            TRAPD(errorCode,
                result = iCalcDocument->CalculateAndAddHistoryL(
                editorNumber, editLine));
            
            if (errorCode)
                {
                iContainer->SetErrorCode(errorCode);
                return;
                }

            iOutputSheet->ScrollToBottomL();
            //Set the state to EOperatorResult so that we know to call HandleResult later
            UpdateState(EOperatorResult);
            }
        }
    //  Current state is state4 and previous input is MS,
    // provisional result is get and show this the editor.
    // And a line is not added to OutputSheet because a line
    // is already added.
    
    else if (iState == ESelectResult)
        {
        iCalcDocument->AddEmptyLine();
        editLine.SetOperator(TCalcEditLine::ECalcOperatorNone);
        //  Because the state is "User has selected result",
        // this function does not occur leave.
        result = iCalcDocument->CalculateAndAddHistoryL(
                 editorNumber, editLine);
        iOutputSheet->ScrollToBottomL();
        UpdateState(EOperator);    
        iContainer->SetSqrtEnableL();
        }
        
    //  If a input is a operator and state is updated,
    // calculate and update history.
    else if (iState != EOperandAndOperator)  
        {
        editLine.TrimZeroL();

        // If the number in the EditLine is not a operand
        // we need to dim square root and percent buttons
        if (iState == EOperandAndOperatorAndOperand)
            {
            iContainer->ShowSqrtButton(EFalse);
            iContainer->ShowPercentButton(EFalse);
            }
        // Calculate by user's input
        TRAPD(errorCode, 
              result = iCalcDocument->CalculateAndAddHistoryL(
              editorNumber, editLine));
                                                 
        if (errorCode)
            {
            iContainer->SetErrorCode(errorCode);
            return;
            }
        iOutputSheet->ScrollToBottomL();
        UpdateState(EOperator);
        }
    
    iEditorSubPane->SetEditorNumber(result);
    iEditorSubPane->SetOperator(aOperator);
    
    if ( aOperator != TCalcEditLine::ECalcSqrt )
        {
        iEditorSubPane->DrawNow();  
        }
    iContainer->ScrollArrowUpdate();
 
    iContainer->SetClearKeyEnable(); 
    if (aOperator == TCalcEditLine::ECalcAdd ||aOperator == TCalcEditLine::ECalcSubtract ||
         aOperator == TCalcEditLine::ECalcMultiply ||aOperator == TCalcEditLine::ECalcDivide )
        {
        iContainer->SetChangeSignDisable();
        }  
    
    }


// ---------------------------------------------------------
// CCalcView::HandleEqualSubRoutineL
// Common routine for select result and MemorySave on two operands
// and operator or State4.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcView::HandleEqualSubRoutineL()
    {
    TCalcEditLine editLine(iEditorSubPane->EditLine());
    TInt calculationError(KErrNone);
    TReal64 result(0);

    if (iState == EOperandAndOperator || iState == EOperatorOnlyResult)
        {
        // User has selected equal or operand and sqrt, or percent
        result = iCalcDocument->ProvisionalResult();
        }
    else 
        {
        editLine.TrimZeroL();
        // We are calculating the intermediate result. 
        // So we need to dim the sqrt and percent buttons.
        iContainer->ShowSqrtButton(EFalse); 
        iContainer->ShowPercentButton(EFalse);
        
        if (iState == ESelectResult)
            {
            iCalcDocument->AddEmptyLine();
            editLine.SetOperator(TCalcEditLine::ECalcOperatorNone);
            }
        TRAP(calculationError,
             result = iCalcDocument->CalculateAndAddHistoryL(
             iEditorSubPane->EditorNumber(), editLine));
        }
    
    //  If overflow or division by zero occurs,
    // error message is displayed.
    if (calculationError)
        {
        iContainer->SetErrorCode(calculationError);
        }
    else
        {
        iCalcDocument->AddEqualLineAndUpdateLastResultL();
        iEditorSubPane->SetEditorNumber(result);
        iEditorSubPane->SetOperator(TCalcEditLine::ECalcEqual);
        // Show result on the sheet
        CCalcOutputSheet* sheet = iContainer->OutputSheet();
        sheet->ScrollToBottomL();
        iContainer->ScrollArrowUpdate();
        }
    return calculationError;
    }

// ----------------------------------------------------
// CCalcView::DoActivateL
// This is called when CCalcView becomes active.
// (other items were commented in a header).
// ----------------------------------------------------
//
void CCalcView::DoActivateL
                (const TVwsViewId& /* aPrevViewId */,
                 TUid /* aCustomMessageId */,
                 const TDesC8& /* aCustomMessage */)
    {
    if ( iContainer == NULL )
        {
        CreateContainerL();
        AppUi()->AddToStackL( *this, iContainer );
        iContainer->ActivateL();
        } 
    
    }

// ----------------------------------------------------
// CCalcView::DoDeactivate
// This is called when CCalcView becomes deactive.
// (other items were commented in a header).
// ----------------------------------------------------
//
void CCalcView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------
// CCalcView::Id
// Return UID of CCalcView
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid CCalcView::Id() const
    {
    return TUid::Uid(KCalcViewId);
    }

// ---------------------------------------------------------
// CCalcView::DynInitMenuPaneL
// This app can have only one menu.
// But menu of Calculator application is changed on State, memory etc.
// Therefore to delete meaningless items from menu, this function is defined. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcView::DynInitMenuPaneL
        (TInt aResourceId,        
         CEikMenuPane* aMenuPane) 
    {
    if(iServiceHandler->HandleSubmenuL(*aMenuPane))
        {
            return;
        }
    switch (aResourceId)
        {
        case (R_CALC_POPUPMENU):
            {
                
                       DynInitMainMenuPane(aMenuPane);
                    
                /***** Get the context of the Help topic from here *****/
                  CArrayFix<TCoeHelpContext>* buf= AppUi()->AppHelpContextL();
                   TInt i= buf->Count();
                   if(i!=0)
                       {
                       /***** Check whether our flag is defined or not *****/
                       #ifdef RD_HELP_AIW_INTEGRATION
                       
                       /***** Call the AiwProvider's AddItemsL  to attach the mnu items********/
                           
                            iServiceHandler->InitializeMenuPaneL(*aMenuPane,aResourceId,
                                        ECmdLast,iServiceHandler->InParamListL()); 
                    /**** Set the old embedded help as dimmed one ****/ 
                        aMenuPane->SetItemDimmed(ECalcCmdHelp, ETrue);                     
                    #else
                    /**** Set the Aiw Help as dimmed one as our feature flag is not defined ****/ 
                        aMenuPane->SetItemDimmed(ECmdAiwPlaceHolder,ETrue);    
                    #endif
                       }
                else 
                    {
                 /**** If the Feature flag is not defined dont show the Aiw Help ****/
                    aMenuPane->SetItemDimmed(ECmdAiwPlaceHolder,ETrue);
                    }
                    
                /******* Delete the buffer and make it to NULL *****/
                   delete buf;
                buf = NULL;   
                if (!FeatureManager::FeatureSupported( KFeatureIdHelp ))
                    {
                    aMenuPane->DeleteMenuItem(ECalcCmdHelp);
                    aMenuPane->DeleteMenuItem(ECmdAiwPlaceHolder);
                    }  
            break;
            }

        case (R_CALC_MEMORY_MENU):
            {
            DynInitMemoryMenuPane(aMenuPane);
            break;
            }
        default:
            {
            break;
            }
        }
    }   
// ---------------------------------------------------------
// CCalcView::HandleForegroundEventL
//Refresh and draw display ,paper and function map Icons.
// ---------------------------------------------------------
//
void CCalcView::HandleForegroundEventL(TBool aForeground)
    {
    if( (aForeground) && (STATIC_CAST(CCalcAppUi*, AppUi())->IsSkinChanged()) )
        {
        iEditorSubPane->RefreshDisplaySideLBitmap();
        iEditorSubPane->RefreshDisplayCenterBitmap();
        iEditorSubPane->RefreshDisplaySideRBitmap();    
        
        iEditorSubPane->DrawNow();

        iOutputSheet->RefreshPaperBitmap();
        iOutputSheet->RefreshScalablePaperBitmap();
#ifdef __SCALABLE_ICONS
        iOutputSheet->RefreshResultsLineBitmap();
#endif
        iOutputSheet->DrawNow();

        iContainer->FunctionSubPane()->RefreshButtonIcons();        
        for(TInt i =0; i< 10; i++)
            {
            iContainer->FunctionSubPane()->GetButtonBitmapControl(i)->DrawNow();
            }
        }
    else
        {
        CAknView::HandleForegroundEventL(aForeground);
        }
    }


//  End of File  
