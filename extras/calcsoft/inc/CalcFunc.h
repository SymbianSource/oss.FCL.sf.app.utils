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


#ifndef     CALCFUNC_H
#define     CALCFUNC_H


//  INCLUDES
#include <coecntrl.h>
#include <e32property.h> 
#include <PtiDefs.h> 

#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
//  CONSTANTS  
const   TInt   KCountOfButtons(10);     // Number of buttons
const   TInt   KCountOfButtonsTouch(20);     // Number of buttons for touch UI layout
_LIT(KAllowedNumericChars, "0123456789.,");
_LIT(KAllowedOperatorPlus, "+");
_LIT(KAllowedOperatorMinus, "-");
_LIT(KAllowedOperatorStar, "*");
_LIT(KAllowedOperatorDivide, "/");
_LIT(KAllowedOperatorEquals, "=");
_LIT(KAllowedOperatorPercent, "%");
_LIT(KAllowedOperatorDecimal, ".");
const TInt KMaxNumCharsPerCase = 50;

//  FORWARD DECLARATIONS
//class   CCalcCommandButton;
//Use of new AknButtons
class 	CAknButton;
class   CCalcContainer;
class   CCalcAppEnv;
class   CPtiEngine;
class   CPtiQwertyKeyMappings;
class 	CPtiCoreLanguage;
#ifdef RD_INTELLIGENT_TEXT_INPUT 
class   CPtiHalfQwertyKeyMappings;
#endif

//  CLASS DEFINITIONS 

/**
CCalcFuncmapSubPane : 'FunctionMap Sub-Pane' class
										: Class also derived from MCoeControlObserver to get events
										  from Avkon buttons
*/
class   CCalcFuncmapSubPane
        :public CCoeControl , public MCoeControlObserver
    {
   public:
   
   enum  TButtonTID// ButtonIDs for Touch Layout
            {
               ECmdTCancel = 0,   
               ECmdTSeven,        
               ECmdTFour,         
               ECmdTOne,          
			   ECmdTZero,         
			   ECmdTSqrt, 		 
               ECmdTEight,        
               ECmdTFive,         
               ECmdTTwo,          
			   ECmdTSeparator,    
			   ECmdTPercent,		 
			   ECmdTNine,         
               ECmdTSix,          
               ECmdTThree,        
               ECmdTChangeSign,   
               ECmdTDivide,       
               ECmdTMultiply,     
               ECmdTSubtract,     
               ECmdTAdd,          
               ECmdTResult,       
               ECmdTScrollUp,
               ECmdTScrollDown
            };

  

    enum TButtonID // ButtonID for non touch Layout
            {
            EPlusButton,            // +
            EMinusButton,           // -
            EMultiplyButton,        // *
            EDivideButton,          // /
			EChangeSignButton,      // +/-
            EEqualButton,           // =
			ESqrtButton,			// sqrt
			EPercentButton,			// %
			EScrollUp,              // << 
            EScrollDown             // >> 
            };
   public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aContainer : Pointer of CCalcContainer class 
        */
        static CCalcFuncmapSubPane* NewL(CCalcContainer* aContainer); 
       
        /**
        * Destructor.
        */
        virtual ~CCalcFuncmapSubPane();
       

    public:  // New functions
        /**
        * Redraw scroll up and down button on scrolling enable.
        */
        void RedrawScrollButtons();

        /**
        * Redraw ChangeSign button on ChangeSign enable.
        * @param aEnable : Flag of ChangeSign enable
        */
        void SetChangeSignEnable(TBool aEnable);
		
		/**
        * Redraw Sqrt button on Sqrt enable.
        * @param aEnable : Flag of SqrtEnable enable
        */
		void SetSqrtEnable (TBool aEnable);

        /**
        * Redraw Clear button on Clear enable.
        * @param aEnable : Flag of ClearEnable enable
        */
        void SetClearKeyEnable( TBool aEnable );
		
		/**
        * Redraw Sqrt button on Percent enable.
        * @param aEnable : Flag of PercentEnable enable
        */
		void SetPercentEnable(TBool aEnable);

		/**
        * If no key is pressed until timeout of *-key,
        * this function is called.
        */
        void NotifyTimeoutL();

        /**
        * Notify any key is released. If pressed button exists.
        * command which is releated is executed.
        */
        void NotifyReleaseKeyL();

        /**
        * If key other than OK-key is pressed this function is called.
        */
        void NotifyOtherThanOkKeyPressed();

        /**
        * Called if language is changed.
        * Change layout according to the language.
        */
        void NotifyLangChange();
        
        
        /**
        * Called to find key pad is Active.
        * 
        */
        TInt IsQwertyKeypadActive();
        
        /**
        * Called to find Key is long Pressed.
        * 
        */
        TBool IsKeyLongPressed();

        
    public:   // Functions from base classes
    
        /**
        * From MCoeControlObserver.
        * Handles events from Avkon Buttons
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
        /**
        * From CCoeControl ?member_description.
        * @param aKeyEvent : Key event details
        * @param aType     : EEventKeyDown || EEventKey || EEventKeyUp
        * @return EKeyWasConsumed    : Key event was used
        *         EKeyWasNotConsumed : Key event was not used
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

		/**
		*Refresh with latest button Icons when the skin change event has occured.
	    */
		void RefreshButtonIcons();
		
		 /**
        * CCalcFuncmapSubPane::GetButtonBitmapControl
		* This function is called when the size changes.
        * 
        */
		    CAknButton* GetButtonBitmapControl(TInt aIndex); //Return iButtons
		
		 /**
        * From CCoeControl : Handling pointer event
        * Handles all pen input events on function pane.
        * 
        */
        void HandlePointerEventL
                ( const TPointerEvent& aKeyEvent );
        /**
        * FunctionPaneRect
        * Gets the layout of function pane.
        */
        TRect FunctionPaneRect();

        /**
        * HandleMiddleSoftKeyOREKeyOK
        * Called to handle MSK or EKeyOk
        */
        void HandleMiddleSoftKeyOREKeyOKL();             

				/**
        * GetChangeSignButtonState
        * Gets the state of the Sign Button
        */
        TBool GetChangeSignButtonState();
        
        
				/**
        * GetSqrtButtonState
        * Gets the state of the Squareroot Button
        */
        TBool GetSqrtButtonState();
        
        
        /**
        * GetSqrtButtonState
        * Gets the state of the Percent Button
        */
        TBool GetPercentButtonState();
        
        /**
        * GetClearButtonState
        * Gets the state of the Clear Button
        */        
        TBool GetClearButtonState();
        
        /**
        * GetSelectedButtonId
        * returns the id of current button
        */
        TInt GetSelectedButtonId();
 
        /**
        * SetHighlightButton
        * set highlight button
        */
        void SetHighlightButton( TUint aOldId, TUint aNewId );  
        
        /**
        * GetKeyboardType
        * returns the type of keyboard
        */
        TInt GetKeyboardType();

    private:    // New functions
        /**
        * C++ default constructor.
        */
        CCalcFuncmapSubPane();

        /**
        * Second-phase constructor
        * @param aContainer : Pointer of CCalcContainer class
        */
        void ConstructL(CCalcContainer* aContainer);

        /**
        * Redraw scroll-up button on scrolling enable.
        * @param aEnable : Flag of scroll-up  enable 
        */
        void SetScrollupEnable(TBool aEnable);
        
        /**
        * Redraw scroll-down button on scrolling enable.
        * @param aEnable : Flag of scroll-down  enable
        */
        void SetScrolldownEnable(TBool aEnable);

        /**
        * Set next cursor position whwn pressing uparrow key.
        */
        void SetNextCursorPosForUpArrow(TInt aOldPos);

        /**
        * Set next cursor position whwn pressing downarrow key.
        */
        void SetNextCursorPosForDownArrow(TInt aOldPos);
                
        /**
        * An old highlight rectangle and 
        * a new highlight rectangle are redrawn.
        * @param aOldId : The button chosen before
        * @param aNewId : The button chosen newly
        */
        void RedrawHighlight(TUint aOldId, TUint aNewId) const;
        
        /**
        * Set functionmap button layout. 
        */
        void SetLayout();
        /**
        * MapNumericChar. 
        */
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TBool MapNumericCharL( TKeyEvent aKeyEvent );
        TBool MapNumerics( TKeyEvent aKeyEvent );
#else 
        TBool MapNumericCharL(TPtiKey aKey, TPtiTextCase aCase);
        TBool MapNumerics(TPtiKey aKey, TPtiTextCase aCase);
#endif

        /**
        * MapDataForChrShiftKey. 
        */  
        void MapDataForChrShiftKey( TDes& aOperators, TInt& aFirstOperator, TInt& aSecondOperator);
        
        /**
        * MapOperators. 
        */         
        TBool MapOperators( TUint16 aOperator, TInt& aResult);

      
    private:    // Functions from base classes
        /**
        * From CCoeControl : Return count of controls contained 
        * in a compound control.
        * @return Count of buttons in FunctionMap Subpane 
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl : Get the components of a compound control
        * @param aIndex
        * @return Control of argument aIndex  
        */
        CCoeControl* ComponentControl(TInt aIndex) const;
        
        /**
        * From CCoeControl : This is called when control size is changed.
        */
        void SizeChanged();

        /**
        * From CCoeControl : This is called when control size is changed.
        */
        TInt CalculatingCorrectButton(const TPointerEvent& aPointerEvent);
        
    private:     // Data
        CAknButton* iButtons[KCountOfButtonsTouch];  // Array of buttons
        TInt iSelected;                 // Index of the selected button

        TBool    iDecimalPoint;

        TUint iLastKeyCode;             // The key code inputted at the last.
        CCalcContainer* iContainer;    // Pointer of CCalcContainer class 
        CCalcAppEnv*    iCalcAppEnv;   // Not own
        RProperty       iQwertyModeStatusProperty; // To check the Qwerty keypad active 
        
        TPtiTextCase	  iShiftKeyPressed;
        CPtiEngine*     iEngine;
        CRepository* 	  iCRKey;
        CPtiCoreLanguage* iCoreLanguage;
        CPtiQwertyKeyMappings* iQwertyKeyMappings;
#ifdef RD_INTELLIGENT_TEXT_INPUT       
        CPtiHalfQwertyKeyMappings* iHalfQwertyKeyMappings;
#endif

        TInt 			      iLanguage;
        TInt                  iUiLanguage;
        TInt 			      iShiftKey;
        TBool 			    iValue;
        TInt            iCountOfButtons;	//keeps track of the count of buttons
        TInt            iButtonsInRow;   //Number of buttons in the row
        TInt            iButtonsInColumn; //Number of buttons in the Column
        TBool           iIsSqrtDimmed;  //Holds the state of the squareroot button
        TBool           iIsPercentDimmed;  //Holds the state of the percent button 
        TBool           iIsChangeSignDimmed; //Checks if the change sign button is dimmed or not
        TBool           iIsOfferKeyEvent; //checks if the event on select is from Offer key or pointer
        TBool           iIsClearKeyEnabled;  //Checks if the Clear button is Enabled or not      
        TBool           iIsKeyLongPress;     //Checks if the button is Long Pressed or not
        TInt            iKeyboardType;
        TBool           iIsShiftKeyPressed;
    };

#endif      //  CALCFUNC_H
            
// End of File
