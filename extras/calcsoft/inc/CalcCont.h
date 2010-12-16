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
* Description:  Header file of "CCalcContainer",  CCalcContainer class 
*                which derived from CCoeControl class. Role of this class 
*                is to update the calculator data and display on user's input.
*
*/


#ifndef     CALCCONT_H
#define     CALCCONT_H


//  INCLUDES
#include <coecntrl.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include "CalcEditline.h"
#include "CalcView.h"


//  FORWARD DECLARATIONS
class   CCalcEditorSubPane;
class   CCalcFuncmapSubPane;
class   CCalcOutputSheet; 
class   CCalcView;
class   CCalcDocument;
class   CPeriodic;
class   CAknsBasicBackgroundControlContext;
//Enum declarations for touch values
enum enlayout_values
{
    enTouch_disabled = 0,
    enTouch_enabled,
    enTouch_with_prt,
    enTouch_with_lsc
};
//  CLASS DEFINITIONS 

/**
CCalcContainer : 'Main-Pane' class
*/
class   CCalcContainer
        :public CCoeControl
    {
    public:  // Constructors and destructor   
        /**
        * Two-phased constructor.
        */
        static CCalcContainer* NewL(CCalcView* aView); 
    
        /**
        * Destructor.
        */
        virtual ~CCalcContainer();  


    public:  // New functions
        /**
        * Return pointer of FunctionMap subpane class
        * @return Pointer of FunctionMap subpane class 
        */
        inline CCalcFuncmapSubPane* FuncmapSubPane() const; 

        /**
        * Return pointer of editor subpane.
        * @return Pointer of Editor subpane class 
        */
        inline CCalcEditorSubPane* EditorPane() const;
        
        /**
        * Return pointer of OutputSheet.
        * @return Pointer of OutputSheet class 
        */
        inline CCalcOutputSheet* OutputSheet() const;
        
        /**
        * Return pointer of CCalcView.
        * @return Pointer of CCalcView class 
        */
        inline CCalcView* View() const;

        /**
        * The pretreatment of a numerical input is performed.
        */
        void ProcessPreinputL();
        
        /**
        * Clear editor,
        * and if need, empty line is added to calculation history. 
        */
        void InputClearL();
        
        /**
        * Redraw scroll-up and down button on scrolling enable.
        */
        void ScrollArrowUpdate();

        /**
        * Redraw ChangeSign button on ChangeSign enable.
        */
        void SetChangeSignEnableL();
        
        /**
        * Redraw ChangeSign button on ChangeSign disable.
        */
        void SetChangeSignDisable();
  
        /**
        * Redraw Clear button on ClearKey enable.
        */
        void SetClearKeyEnable();

        /**
        * Show square root button only if no 0 in editor.
        */
        void SetSqrtEnableL();

        /**
        * Show percent button only if no 0 in editor.
        */
        void SetPercentEnableL();

        /**
        * Redraw square root button.
        */
        void ShowSqrtButton(TBool aEnable);

        /**
        * Redraw percent button.
        */
        void ShowPercentButton(TBool aEnable);

        /**
        * Returns the state of the calculator.
        */
        CCalcView::TStateNo GetState();

        /**
        * Time out CallBack function for *-key press
        * @param aObject this class.
        * @return always 0
        */
        static TInt TimeoutCallbackL(TAny* aObject);
     
        /**
        * Time out CallBack function for * /key press
        * @param aObject this class.
        * @return always 0
        */
        static TInt TimeoutCallbackChrL( TAny* aObject );
        /**
        * Time out CallBack function for +#key press
        * @param aObject this class.
        * @return always 0
        */
        static TInt TimeoutCallbackShiftL( TAny* aObject );
        
        /**
        * Notify changing decimal separator.
        * @param aOld : Old decimal separator
        * @param aNew : New decimal separator  
        */
        void NotifyChangeDecimal(TChar aOld, TChar aNew);
        
        /**
        * Called  when an error is displayed.
        * @param aErrorCode : an error code  from  CCalcContainer
        */
        void SetErrorCode(TInt aErrorCode);
        
    public: // Functions from base classes
        /**
        * From CCoeControl : Get help context
        * @param aContext : help context. 
        */
        void GetHelpContext(TCoeHelpContext& aContext) const;

        /**
        *Returns functionMap subpane
        */
        inline CCalcFuncmapSubPane* FunctionSubPane() const; 
        
        /**
        * HandleMiddleSoftKey
        * Handled when MSK is selected 
        */    
        void HandleMiddleSoftKey();

        /**
        * SetOperatorFromTouch
        * To handle the addition of Touch Input values for digits
        */    
        void SetOperatorFromTouchL(TInt akey);
        
        /**
        * ClearInputKey
        * To handle the clear input key through the touch UI 
        */    
        void ClearInputKeyL(TInt aRepeat = 0);
        
        /**
        * ClearInputKey
        * To handle the '.' key press
        */    
        
        void SetSeparatorFromTouchL();

        // From CCoeControl        
        void ActivateL();
    private:  // New functions 
        /**
        * C++ default constructor.
        * @param aView : Pointer of CCalcView
        */
        CCalcContainer();  

        /**
        * Second-phase constructor.
        */
        void ConstructL(CCalcView* aView);   

        /**
        * If no key is pressed until timeout of *-key,
        * this function is called.
        */
        void DoTimeoutL();

        /**
        * If no key is pressed until timeout of * /key,
        * this function is called.
        */
        void DoTimeoutChrL();
        /**
        * If no key is pressed until timeout of +#key,
        * this function is called.
        */
        void DoTimeoutShiftL();        

        /**
        * Called when any key is pressed.
        * If timeout notifier for *-key is active, make calculation or
        * stop notifier according to user's input.
        * @param aKeyEvent : The key event
        * @param aType     : The type of the event
        * @return : If ETrue, the key event is used only for *-key timeout.
        *           If EFalse, the key event is used not only for *-key 
        *           timeout but also for other effect of each key. 
        */
        TBool HandleAsterKeyTimeoutForKeyPressL(
             const TKeyEvent& aKeyEvent, TEventCode aType);
        
        /**
        * Called when any key is pressed.
        * If timeout notifier for * /key is active, make calculation or
        * stop notifier according to user's input.
        * @param aKeyEvent : The key event
        * @param aType     : The type of the event
        * @return : If ETrue, the key event is used only for * /key timeout.
        *           If EFalse, the key event is used not only for *+key 
        *           timeout but also for other effect of each key. 
        */
        TBool HandleChrKeyTimeoutForKeyPressL(
             const TKeyEvent& aKeyEvent, TEventCode aType );
        /**
        * Called when any key is pressed.
        * If timeout notifier for +#key is active, make calculation or
        * stop notifier according to user's input.
        * @param aKeyEvent : The key event
        * @param aType     : The type of the event
        * @return : If ETrue, the key event is used only for +#key timeout.
        *           If EFalse, the key event is used not only for +#key 
        *           timeout but also for other effect of each key. 
        */
        TBool HandleShiftKeyTimeoutForKeyPressL(
             const TKeyEvent& aKeyEvent, TEventCode aType );    

    private:  // Functions from base classes 
        /**
        * From CCoeControl : Return count of controls contained in a compound control.
        * @return Count of controls contained in a compound control 
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl : Get the components of a compound control
        * @param aIndex : index of control
        * @return Control of argument aIndex  
        */
        CCoeControl* ComponentControl(TInt aIndex) const;
        
        /**
        * From CCoeControl : Handling key event
        * @param aKeyEvent : The key event
        * @param aType     : The type of the event
        * @return EKeyWasConsumed    : Key event is used. 
        *         EKeyWasNotConsumed : Key event is not used.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, 
                                    TEventCode aType);

        /**
        * From CCoeControl : Notifier for changing language
        * @param aType : Type of resource change
        */
        void HandleResourceChange(TInt aType);
        
        /**
        * @param aType : Type of resource change
        */
        void HandleResourceChangeCalSoftL(TInt aType);
 
        /**
        * From CCoeControl : Control size is set.
        */
        void SizeChanged();

        /**
        * From CCoeControl : Clear whole rectangle. 
        * @param aRect : rectangle of control
        */
        void Draw(const TRect& aRect) const;

        /**
        * From CCoeControl.
        * Pass skin information if need.
        * @param aId : Type of supplied object.
        */
        TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
         /**
        * From CCoeControl : Handling pointer event
        * @param aKeyEvent : The key event
        * 
        */
        void HandlePointerEventL
                ( const TPointerEvent& aKeyEvent );
  
        /**
        * Converter other language number to Latin number.
        * @param aKeyEvent: An keyevent from system.
        * @param aLanguage: Current input Language.
        */
        void CCalcContainer::RemapNumericKey( TKeyEvent& aKeyEvent, TInt aLanguage );
    private:    // Data
        CCalcEditorSubPane*  iEditorPane;  // Editor subpane 
        CCalcFuncmapSubPane* iFuncmapPane; // FunctionMap subpane
        CCalcOutputSheet*    iSheetPane;   // OutputSheet
        CCalcView*           iView;        // Calculator view 
        CCalcDocument*       iCalcDocument;
        CPeriodic*           iTimeout;     // For handling *-key
       
        CPeriodic*           iTimeoutChr;     // For handling */key
        CPeriodic*           iTimeoutShift;     // For handling +#key   

        TUint                iPrevInput;   // Key code of previous input
        CAknsBasicBackgroundControlContext* iSkinContext; // skin data
        TInt iValue ; 
        CRepository* iFepRepository; //Use to get current input language.
    };

#include "CalcCont.inl"

#endif      //  CALCCONT_H
            
// End of File
