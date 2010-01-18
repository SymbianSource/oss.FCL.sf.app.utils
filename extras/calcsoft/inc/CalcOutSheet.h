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
* Description:  The OutputSheet sub-pane class, CCalcOutputSheet
*                Derived from CCoeControl
*                The CCalcOutputSheet displays histroy data.
*                History data is scroll up and down.
*
*/


#ifndef     CALCOUTSHEET_H
#define     CALCOUTSHEET_H


//  INCLUDES
#include <coecntrl.h>
#include <AknUtils.h>
#include <e32std.h>

#include <eikscrlb.h>
#include <eiksbobs.h>

#include <eiksbfrm.h>
 

// CONSTANTS
const TInt KCountOfPaperBitmaps(9);
//  FORWARD DECLARATIONS
class   CCalcHistory;
class   CCalcContainer;
class   CCalcAppEnv;

//  CLASS DEFINITIONS 

/**
CCalcOutputSheet : 'OutputSheet Sub-Pane' class
*/
//New scrollbar used for Touch UI layout
class CEikScrollBarFrame;
class CCalcOutputSheet
      : public CCoeControl , public MEikScrollBarObserver
    {/*Class also derived from scrollbarobserver to capture scrollbar events*/
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aContainer : Pointer of CCalcContainer class 
        */
        static CCalcOutputSheet* NewL(CCalcContainer* aContainer); 
       
        /**
        * Destructor.
        */
        virtual ~CCalcOutputSheet();

    
    public:     // New functions
        
        
        /**
        * Scroll to bottom line.
        */
        void ScrollToBottomL(); 

        /**
        * Called after clearing calculation history.
        */
        void NotifyClearHistoryL();
        
        /**
        * It returns whether it can scroll up or not.
        * @return ETrue : Scroll up can be done. 
        *         EFalse: Scroll up cannot be done.
        */
        TBool CanUp() const;  
        
        /**
        * It returns whether it can scroll down or not.
        * @return ETrue : Scroll up can be done. 
        *         EFalse: Scroll up cannot be done.
        */
        TBool CanDown() const; 
        
        /**
        * Output Sheet is scrolled up.
        */
        void ScrollUp();   
        
        /**
        * Output Sheet is scrolled down.
        */
        void ScrollDown();

		/**
		* Refresh PaperBitmap Icon when the skin change event has occured.
		*/
		inline void RefreshPaperBitmap() ;
		/**
		* Refresh ScalablePaperBitmap Icon when the skin change event has occured.
		*/
		inline void RefreshScalablePaperBitmap() ;


		inline void RefreshResultsLineBitmap() ;
		
		/**
		* From MEikScrollBarObserver: to Handle scrollbar events
		* @param CEikScrollBar* : Pointer to the scrollbar
		* @param TEikScrollEvent : Event on the Scrollbar
		*/
		void HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType);
		
		
        /**
        * From CCoeControl : Notifier for changing language
        * @param aType : Type of resource change
        */
        void HandleResourceChange(TInt aType);
        void HandleResourceChangeOutSheetL(TInt aType);
 
		
    private:    // New functions
        /**
        * C++ default constructor.
        */
        CCalcOutputSheet();

        /**
        * Second-phase constructor.
        */
        void ConstructL(CCalcContainer* aContainer);


    private:
    
            
        /**
        * From CCoeControl : Set control size 
        */
        void SizeChanged();
        
        void SizeChangedOutSheetL();

        /**
        * From CCoeControl : Draw Output Sheet
        * @param aRect : rectangle of Output Sheet
        */
        void Draw(const TRect& aRect) const;
        
  		/**
        * Function used to dynamically update scrollbar indicators.
        */
        void UpdateScrollIndicatorL();
        

    private:    // Data
        TInt iScrollOffset;     // Scroll position
        const CGulIcon* iCalcPaper; // Not own
		const CGulIcon* iCalcScalablePaper[KCountOfPaperBitmaps];
		const CGulIcon* iCalcResultsLine;
        CCalcHistory*   iCalcHistory;    // Not own
        CCalcContainer* iCalcContainer;  // Not own
        CCalcAppEnv*    iCalcAppEnv;     // Not own
		TAknLayoutRect iCalcPaperLayout[KCountOfPaperBitmaps];
		TInt iCalcOutputSheetLines;
		CAknsBasicBackgroundControlContext* iOpSheetContext;
		CArrayFixFlat<TAknLayoutText>* iOperatorLayout;
		CArrayFixFlat<TAknLayoutText>* iOperandLayout;
		CArrayFixFlat<TAknLayoutRect>* iResultsLineLayout;
		CArrayFixFlat<TAknLayoutRect>* iEqualLineLayout;
		CRepository* 	iCRKey;
		TInt 			iLanguage;
        TInt            iUiLanguage;
		CEikScrollBarFrame *iSBFrame;		//own -> pointer used for scrollbar frame
		TAknLayoutRect  iPaperPaneRect;
		TInt            iPrevThumbPosition;
		            
       
    };

#include "CalcOutSheet.inl"

#endif      //  CALCOUTSHEET_H

// End of File
