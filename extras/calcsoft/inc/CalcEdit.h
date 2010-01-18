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
* Description:  Header file of "editor sub-pane", CCalcEditorSubPane class 
*                which derived from CCoeControl class. Role of this class 
*                is to draw editor.
*
*/


#ifndef     CALCEDIT_H
#define     CALCEDIT_H


//  INCLUDES
#include <coecntrl.h>
#include <AknUtils.h>  // for TAknLayoutRect

#include "CalcView.h"
#include "CalcEnv.h"
#include <PtiDefs.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <e32property.h> 
#endif

//  CONSTANTS
const TInt KCalcCountOfEditorComponent(6);
_LIT(KAllowedDecimal, ".,");

//  FORWARD DECLARATIONS
class CCalcContainer;
class   CPtiEngine;
class   CPtiQwertyKeyMappings;
class 	CPtiCoreLanguage;
//class CCalcAppEnv;
#ifdef RD_INTELLIGENT_TEXT_INPUT 
class   CPtiHalfQwertyKeyMappings;
#endif

//  CLASS DEFINITIONS 

/**
CCalcEditorSubPane : 'Editor Sub-Pane' class
*/

class   CCalcEditorSubPane
        :public CCoeControl
    {
    // Enlarge size for rect.
    public:
         enum  RectEnlargeSizeID
            {
               EEnlargeFive = 5,   
               EEnlargeTwenty = 20
            };
            
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aContainer : Pointer of CCalcContainer class 
        */
        static CCalcEditorSubPane* NewL(CCalcContainer* aContainer); 
       
        /**
        * Destructor.
        */
        virtual ~CCalcEditorSubPane();
        
        
    public:     // New functions
        /**
        * Return curent value of editor
        * @return Reference of TCalcEditLine : curent value of editor
        */
        const TCalcEditLine& EditLine() const; 

        /**
        * Set operator to editor.
        * @param aType : new operator type.  
        */
        void SetOperator(TCalcEditLine::TCalcOperatorType aType);

        /**
        * Set TReal64 type editor number.
        * @param aNewNumber : new editor number.  
        */
        void SetEditorNumber(TReal64 aNewNumber);
        
        /**
        * Return curent editor number.
        * @return TReal64 type editor number.
        */
        TReal64 EditorNumber() const;
        
        /**
        * Number of editor is set "0" and operator is set none.  
        */
        void ResetL();
        
        /**
        * Operand of editor is erased.
        */
        void ClearOperand();
        
        /**
        * Clear latest input from editor. 
        */
        void ClearLastInputL();
        
        /**
        * Handle short-press-clear-key and CBA-clear-key event.
        */
        void OfferShortPressClearKeyEventL();

        /**
        * Changing sign.
        * @param aState : Current state of Calculator
        */
        void DoChangeSignL(CCalcView::TStateNo aState);

        /**
        * Update editor number to memory or last result.
        * @param aMemory : memory or last result
        * @param aState : Current state of Calculator
        */
        void DoMemoryRecallOrLastResult(
             TReal64 aMemoryOrLastResult, 
             CCalcView::TStateNo aState);

        /**
        * Notify changing decimal separator.
        * @param aOld : Old decimal separator
        * @param aNew : New decimal separator  
        */
        void NotifyChangeDecimal(TChar aOld, TChar aNew);
        
        /**
        * Gets the layout of the EditPane.
        * Used for touch support.
        */
        TRect EditPaneRect();
        
        /**
        * HandlePointerEventL
        * Handles pen inputs of Editor pane.
        */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent );
        
    public: // Functions from base classes
        /**
        
        /**
        * Called to find key pad is Active.
        * 
        */
        TInt IsQwertyKeypadActive();
        
        /* From CCoeControl : Handling key event
        * @param aKeyEvent : The key event
        * @param aType : The type of the event
        * @return EKeyWasConsumed : Key event is used.
        *         EKeyWasNotConsumed : Key event is not used.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);       

		/**
		 * Refresh the display bitmap when the skin change event has occured.
		 */
				inline void RefreshDisplaySideLBitmap();
     			inline void RefreshDisplayCenterBitmap();
     			inline void RefreshDisplaySideRBitmap();
          
    private:    // NewFunctions
        /**
        * C++ default constructor.
        */
        CCalcEditorSubPane();
       
        /**
        * Second-phase constructor.
        * @param aContainer : Pointer of CCalcContainer class 
        */
        void ConstructL(CCalcContainer* aContainer);

        /**
        * Delete the latest input
        */
        void ClearL();    // Delete the latest input

    private: // Functions from base classes
        /**
        * From CCoeControl : Size of control is set. 
        */
        void SizeChanged();

        /**
        * From CCoeControl : Draw editor
        * @param aRect : rectangle of editor space.
        */
        void Draw(const TRect& aRect) const;  
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TBool MapDecimal( TKeyEvent aKeyEvent ); 
#else
        TBool MapDecimal(TPtiKey aKey, TPtiTextCase aCase); 
#endif

    private:    // Data
       
      	const CGulIcon* iCalcSideLDisplay;
      	const CGulIcon* iCalcCenterDisplay;
      	const CGulIcon* iCalcSideRDisplay;
		TAknLayoutRect iSideLDisplayLayout;
	    TAknLayoutRect iCenterDisplayLayout;
	    TAknLayoutRect iSideRDisplayLayout;
      	  
        CCalcContainer* iCalcContainer;  // Not own
        CCalcAppEnv*    iCalcAppEnv;     // Not own 
        TCalcEditLine   iEditLine;  // editor data
        TReal64         iEditorNumber;
        TInt 			iIsQwertyActive;
        CPtiEngine*     iEngine;
        CRepository* 	iCRKey;
        CPtiCoreLanguage* iCoreLanguage;
        CPtiQwertyKeyMappings* iQwertyKeyMappings;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        CPtiHalfQwertyKeyMappings* iHalfQwertyKeyMappings;
#endif
        TInt 			iLanguage;

        TInt            iUiLanguage;
        TBuf<256> 		iAllowedDecimal;
        RProperty       iQwertyModeStatusProperty; // To check the Qwerty keypad active 
        TInt			iDecimalCounter;
        
        TInt            iKeyboardType;
     public:
     	void IsQwertyActive();
     	void IsQwertyNotActive(); 
     private:   
        enum TCalcEditorComponentIndex
            {
            ECalcEditorOperandShadow,
            ECalcEditorOperand,
            ECalcEditorMemoryShadow,
            ECalcEditorMemory,
            ECalcEditorOperatorShadow,
            ECalcEditorOperator
            };
        TAknLayoutText iLayoutText[KCalcCountOfEditorComponent]; 
                     // Layout of each drawing component.
    };

#include "CalcEdit.inl"
            
// End of File

