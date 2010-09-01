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
* Description:  Calculator environment class, CCalcAppEnv
*                Derived from CBase
*                This class maintain global data. e.g font, bitmap.
*
*/


#ifndef     CALCENV_H
#define     CALCENV_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <gulicon.h>  
#include <eikenv.h>

//Extended Icon Skin Implementation
#include <AknsConstants.h>
#include "CalcFunc.h"
#include "CalcEditline.h"
#include <AknUtils.h>

// CONSTANTS
const  TInt  KMaxMemoryIndicatorLength(4);
const  TInt  KCalcCountOfOperatorType( 10 ); // +,-,*,/,=, sqrt, % , none ,* and /. 
//Extended Icon Skin implementation.
const  TInt KCalcAknsConstCount(26);
const  TInt  KCalcCountOfButtonPressed(10);
const  TInt  KCalcCountOfButtonInactive(16);
const  TInt  KCountOfBitmaps(9);


//  FORWARD DECLARATIONS
class CCalcAppUi;
class CFont;

//  CLASS DEFINITIONS 

/**
CCalcAppEnv : Maintains global data.
*/
class   CCalcAppEnv 
        : public CBase
    {
    public:  // Constructors and destructor
		//Button Type
		//Extended Icon Skin Implementation
		enum ButtonType {EPRESSED, ENORMAL, EACTIVE_INACTIVE};


        /**
        * Two-phased constructor.
        * @param aApp : Pointer of CCalcAppUi class 
        */
        static CCalcAppEnv* NewL(CCalcAppUi* aAppUi);
        
        /**
        * Destructor.
        */
        virtual ~CCalcAppEnv();
        
    public:     // New functions
        /**
        * Return pointer of CCalcAppEnv class  
        * @return Pointer of CCalcAppEnv class
        */
        static CCalcAppEnv* Static();

        
        /**
        * Return pointer of CCalcAppUi class
        * @return Pointer of CCalcAppUi class
        */
        inline CCalcAppUi* AppUi() const;
        
        /**
        * Return charactor for editor operator  
        * @param Operator type enum
        */
        inline const TPtrC EditorOperator(
            const TCalcEditLine::TCalcOperatorType aType) const;

        /**
        * Return charactor for OutputSheet operator  
        * @param Operator type enum
        */
        inline const TPtrC OutSheetOperator(
            const TCalcEditLine::TCalcOperatorType aType) const;
        
        /**
        * Return decimal separator. 
        * @return Decimal separator, e.g "."
        */
        inline const TChar DecimalSeparator() const;
        
        /**
        * Set new decimal separator. 
        * @param aChar: New decimal separator, e.g "."
        */
        inline void SetDecimalSeparator(TChar aChar);
        
        /**
        * Return minus indicator. 
        * @return minus indicator, e.g "-"
        */
        inline const TChar MinusIndicator() const;

        /**
        * Return memory indicator. 
        * @return memory indicator, e.g "M".
        */
        inline TPtrC MemoryIndicator() const;
        
        /**
        * Return shadow of memory indicator. 
        * @return memory indicator, e.g "M".
        */
        inline TPtrC MemoryIndicatorShadow() const;
        
        /**
        * Return font for number shadow in editor.
        * @return Font for number shadow in editor
        */
        inline const CFont* NumberShadowFont() const;

        /**
        * Return font for number in editor.
        * @return Font for number in editor
        */
        inline const CFont* NumberFont() const;

        /**
        * Return font for operator shadow in editor.
        * @return Font for operator shadow in editor
        */
        inline const CFont* OperatorShadowFont() const;

        /**
        * Return font for operator in editor.
        * @return Font for operator in editor
        */
        inline const CFont* OperatorFont() const;

        /**
        * Return font for operator in OutputSheet.
        * @return Font for operator in OutputSheet
        */
        inline const CFont* OutSheetOperatorFont() const;

        /**
        * Return 'qsn_fr_calc_display_side_l' 'qsn_fr_calc_display_center.svg' 
		  and 'qsn_fr_calc_display_side_r 'Bitmap pointers.
        */
		    inline const CGulIcon* DisplaySideLBitmap() const;
    	    inline const CGulIcon* DisplayCenterBitmap() const;
    	    inline const CGulIcon* DisplaySideRBitmap() const;		
    
        
        /**
        * Return 'qgn_indi_calc_paper' Bitmap pointer.
        * @return 'qgn_indi_calc_paper' Bitmap pointer.
        */
        inline const CGulIcon* PaperBitmap() const;

		/**
        * Return 'qgn_fr_calc_paper' Bitmap pointers.
        * @return 'qgn_fr_calc_paper' Bitmap pointers.
        */
		inline void ScalablePaperBitmaps(const CGulIcon* [])const;

        /**
        * Return 'qgn_graf_calc_results_line' Bitmap pointer.
        * @return 'qgn_graf_calc_results_line' Bitmap pointer.
        */
		inline const CGulIcon* ResultsLineBitmap()const;

        /**
        * Return unpressed bitmap of correspondent index
        * @param aIndex : index of bitmap array
        * @return : Bitmap pointer.
        */
        inline CGulIcon* UnpressedButtonBmp(
            const TInt aIndex) const;

        /**
        * Return pressed bitmap of correspondent index
        * @param aIndex : index of bitmap array
        * @return : Bitmap pointer.
        */
        inline CGulIcon* PressedButtonBmp(
            const TInt aIndex) const;

        /**
        * Return inactive bitmap of correspondent index
        * @param aIndex : index of bitmap array
        * @return : Bitmap pointer. If return value is NULL,
        *          no bitmap exists on correspondent index.
        */
        inline CGulIcon* InactiveButtonBmp(
            const TInt aIndex) const;

		/**
		* To load the functionmap bitmaps when the skin change event  has occured.
		*/
		void LoadFuncMapBitmapSkinChangeL();
		
		        /**
        * Load bitmaps for FunctionMap SubPane.
        */
        void LoadFuncMapBitmapL();

        /*update the array when function map changed*/
        void UpdateAknConstArrayForFuncMap();
        
        //Function sets value as to whether skin is changed or not		
        void SetSkinChangedValue(TBool aValue);
		
    private:    // New functions
        /**
        * By default constructor is private.
        * @param aApp : CCalcAppUi class pointer 
        */
		
		CCalcAppEnv();

		
        CCalcAppEnv(CCalcAppUi* aAppUi);
    	
		
        /**
        * Second-phase constructor
        */
        void ConstructAppEnvL();


		/**
        * Load bitmaps for Paper Pane.
        */
		void LoadPaperBitmapsL();

        void InitializeAppUI(CCalcAppUi* aAppUi);

        /**
        * Load bitmaps for function map button.
        * @param aStartBitmapId : Bitmap ID of loading start  
        * @param aStartMaskId : Mask ID of loading start  
        * @param aStartIndex : Start index of array which is set to 
        *                      bitmap.
        * @param aCount : Count of loading bitmaps
        * @param aBitmapArray : Load bitmap to this array. 
        */
             
        void LoadBitmapForFuncMapL(TInt aStartBitmapId, TInt aStartMaskId, TInt aStartIndex,
	    TInt aCount, CGulIcon* aBitmapArray[], ButtonType aType);
	

		//Extended Icon Skin Implementation
		void UpdateAknConstArray();
		
		//Extended Icon Skin Implementation for Paper
		void UpdateAknConstArrayforPaper();
        
        
    private:    // Data 
        CCalcAppUi*     iAppUi;  // Pointer of CCalcAppUi class  
		
	     static CCalcAppEnv iself;
	     CEikonEnv* iEnv;
			
        // Text
        CDesCArrayFlat* iEditorOperArray;  // Operator for editor
        CDesCArrayFlat* iOutSheetOperArray;// Operator for OutputSheet
        TChar iDecimalSeparator;  // Decimal separator
        TChar iMinusIndicator;    // Minus indicator
        TBuf<KMaxMemoryIndicatorLength> iMemoryIndicator;  
        TBuf<KMaxMemoryIndicatorLength> iMemoryIndicatorShadow;  
                                                           
        // Font
        CFont* iNumberShadowFont;
        CFont* iNumberFont;
        CFont* iOperatorShadowFont;
        CFont* iOperatorFont;
        CFont* iOutSheetOperatorFont;
        // Bitmap
         // Bitmap for background for editor
        CGulIcon* iDisplaySideLBitmap;
        CGulIcon* iDisplayCenterBitmap;
        CGulIcon* iDisplaySideRBitmap;
        CGulIcon* iPaperBitmap;     // Bitmap for background for OutputSheet
		CGulIcon* iScalablePaperBitmap[KCountOfBitmaps];//Bitmpaps for background for outputSheet
		CGulIcon* iResultsLineBitmap;    //  Bitmap for drawing line in OutputSheet
        CGulIcon* iUnpressedButtonBmp[KCountOfButtonsTouch];  // Bitmap for button of FunctionMap
        CGulIcon* iPressedButtonBmp[KCountOfButtons];
        CGulIcon* iDimmedButtonBmp[KCountOfButtons];

		//Extended Icon Skin Implementation
		RArray<TAknsItemID> iAknsConstArray;
		RArray<TAknsItemID> iAknsConstArrayForPaper;
		TBool	iIsSkinChanged;

    };

#include    "CalcEnv.inl"

#endif      //  CALCENV_H
            
// End of File


