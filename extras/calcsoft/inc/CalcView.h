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
* Description:  Header file of CCalcView class 
*                which derived from CAknView class. 
*
*/


#ifndef     CALCVIEW_H
#define     CALCVIEW_H

//  INCLUDES

#include <aknview.h>

#include "CalcEditline.h"


//  CONSTANTS  
const TInt KCalcViewId(1); // view ID of CCalcView

//  FORWARD DECLARATIONS
class CCalcContainer;
class CEikMenuPane;
class CCalcDocument;
class CCalcEditorSubPane;
class CCalcOutputSheet;
class CAiwServiceHandler;

//  CLASS DEFINITIONS 

/**
View class for Calculator application
*/
class   CCalcView
        : public CAknView
    {
    public:
        enum TStateNo       // State number
            {
            EInitialMode,  // State 1 : initial mode
            EOperandOnly,   // State 2 : operand only
            EOperandAndOperator,  // State 3 : operand and operator
            EOperandAndOperatorAndOperand,  // State 4 : operand,operator and operand
            ESelectResult,   // State 5 : User has selected result
			EOperatorOnlyResult // State 6 : sqrt and percent display result right away
            };

        enum TKindOfInput   // kind of operand or operator
            {
            EOperand,
            EOperator,
            EEqual,
            EMemorySave,
            EMemoryRecall, // Latest input is MR, LR, or +/- on State3.
            EAllClear,
			EOperatorResult // For sqrt and percent, because they displayes the result right away
            };


    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CCalcView* NewLC(); 
        
        /**
        * Destructor.
        */
        virtual ~CCalcView();  
        
    public:  // New functions
        /**
        * Return current state
        * @return Current state
        */
        TStateNo State() const;
        
        /**
        * Return type of latest input
        */
        TKindOfInput LatestInput() const;
        
        /**
        * Update state by using kind of input
        * @param aKind : Kind of input
        */
        void UpdateState(const TKindOfInput& aKind);
        
        /**
        * Display error message
        * @param aErrorCode : error code, e.g KErrOverflow.
        */
        void ErrorMsgL(TInt aErrorCode);
        
        /**
        * Delete meaningless items from main menu.
        * @param aMenuPane : Pointer of menu pane
        */
        void DynInitMainMenuPane(CEikMenuPane* aMenuPane);
        
        /**
        * Delete meaningless items from menu of memory
        * @param aMenuPane : Pointer of menu pane
        */
        void DynInitMemoryMenuPane(CEikMenuPane* aMenuPane);
        
        /**
        * Notify changing decimal separator.
        * @param aOld : Old decimal separator
        * @param aNew : New decimal separator  
        */
        void NotifyChangeDecimal(TChar aOld, TChar aNew);
        
        /**
        * Handle Memory-Recall command
        */
        void HandleMemoryRecallL();
          
    public: // Functions from base classes
        /**
        * From MEikMenuObserver : Handle a command
        * @param aCommand : command ID
        */
        void HandleCommandL(TInt aCommand);

    
    private:    // New functions
        /**
        * By default constructor is private.
        */
        CCalcView();
        
        /**
        * Second-phase constructor
        */
        void ConstructL();
        
        /**
        * Create CCalcContainer class
        */
        void CreateContainerL();
        
        /** 
        * Handle Result command
        */
        void HandleResultL();
        
        /**
        * Handle Change-Sign command
        */
        void HandleChangeSignL();
        
        /**
        * Handle Last-Result command
        */
        void HandleLastResultL();
        
        /**
        * Handle Memory-Save command
        */
        void HandleMemorySaveL();
   
        /**
        * Handle Memory-Clear command
        */
        void HandleMemoryClearL();
        
        /**
        * Handle Scroll-up command
        */
        void HandleScrollUp();
        
        /**
        * Handle Scroll-Down command
        */
        void HandleScrollDown();

        /**
        * Handle Clear OutputSheet command
        */
        void DoClearCalcAndHistoryL();
        
        /**
        * Handle Add, Subtract, Multiply, Divide command
        * @param TCalcEditLine::TCalcOperatorType : Type of operator 
        */
        void HandleOperatorL(TCalcEditLine::TCalcOperatorType aOperator);
        
        /**
        * Common routine for handling result and handling memory save
        * on State4 and two operands and operator.
        * Role of this function is calculating result.  
        * But State is not updated.
        * @return KErrNone : Calculation error does not occur
        *         KErrOverflow, KErrDivideByZero : Calculation error occurs
        */
        TInt HandleEqualSubRoutineL();

    private: // From CAknView
        /**
        * When CCalcView becomes active, this is called.
        * @param aPrevViewId : 
        * @param aCustomMessageId : 
        * @param aCustomMessage : 
        */
        void DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId,
                         const TDesC8& aCustomMessage);
        
        /**
        * When CCalcView becomes deactive, this is called.
        */
        void DoDeactivate();
        
        /**
        * Return UID of CCalcView
        * @return UID of CCalcView, KCalcViewId
        */
        TUid Id() const;

        /** 
        * From MEikMenuObserver, Delete meaningless menu item.  
        * @param aResourceId : resource ID of menu pane
        * @param aMenuPane : Pointer of menu pane object
        */
        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

		/**
		*To handle when this application comes to foreground.
		*/
		void HandleForegroundEventL(TBool aForeground);
            
    private:    // Data
        CCalcDocument*  iCalcDocument; // Not own  
        CCalcContainer* iContainer;    // own 
        CCalcEditorSubPane* iEditorSubPane; // Not own
        CCalcOutputSheet*   iOutputSheet; // Not own
        TStateNo        iState;        // Current State 
        TKindOfInput    iLatestInput;  // Kind of latest input
	public:
      	CAiwServiceHandler* iServiceHandler;
      	TBool           iLastResultSetected;
    };

#endif      //  CALCVIEW_H
            
// End of File
