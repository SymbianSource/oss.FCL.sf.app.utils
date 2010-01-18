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
* Description:  Helper class used by CalcDocument,CCalcContainer etc., 
*                TCalcEditLine.
*                The numerical value and the character sequence of the real 
*                number are held. Addition of a numerical character sequence 
*                and which clear operation are performed.
*
*/


#ifndef     CALCEDITLINE_H
#define     CALCEDITLINE_H


//  INCLUDES
#include <e32std.h>


//  Define maximum charactor number per line
const TInt KCalcMaxDigits(8);
const TInt KCalcMaxNumberWidth(11);
const TInt KCalcMaxEditNumberWidth( 10 );

//  CLASS DEFINITIONS 

/**
TCalcEditLine is Calculation data class.
A line data for 'Editor Sub-Pane'
*/
class   TCalcEditLine
    {
    public:
        enum TCalcOperatorType
            {
            ECalcAdd,
            ECalcSubtract,
            ECalcMultiply,
            ECalcDivide,
            ECalcEqual,
			ECalcSqrt,
			ECalcPercent,
            ECalcOperatorNone
            };    

    public:  // Constructors and destructor
        /**
        * C++ default constructor.
        */
        TCalcEditLine(); 
        
        /**
        * Destructor.
        */
        virtual ~TCalcEditLine(); 
        
        /**
        * Copy constructor.
        */
        TCalcEditLine(const TCalcEditLine& aEditLine); 
        
        /**
        * Assignment operator
        */
        const TCalcEditLine&  operator=(const TCalcEditLine& aEditLine);  

    public:     // New functions
        /**
        * Return string of number 
        * @return String of number.
        */
        inline const TPtrC NumberString() const;
        
        /**
        * Clear a line
        */
        void AllClear();
        
        /**
        * Delete last 1 digit from a line
        */
        void ClearL();
        
        /**
        * Clear number from a line
        */
        void ClearOperand();
        
        /**
        * Return number of a line  
        * @return TReal64 : Operand of a line
        */
        TReal64 NumberL() const;
        
        /**
        * Parameter aNumber is set to a line 
        * @param aNumber 
        */
        void SetNumber(TReal64 aNumber);
        
        /**
        * Append a charactor to number.
        * Leave may occur, KCalcErrEditorSpaceFull.
        * @param aInput : A input which is Appended 
        */
        void AppendL(TChar aInput);
        
		/**
		* Append any character to number before it is 
		* displayed inthe output sheet.
		* Leave may occur, KCalcErrEditorSpaceFull.
        * @param aInput : A char which is Appended 
        */
	    void AppendNumberStringL(TChar aChar);

        /**
        * Change sign of number
        */
        void ChangeSignL();
        
        /**
        * If number of editor is "iii.ddd000...", set number "iii.ddd".
        * Example: 
        *  Number of editor is "12",  number editor is set "12".
        *  Number of editor is "12." or "12.000...", number editor is set "12".
        *  Number of editor is "12.27", number editor is set "12.27".
        *  Number of editor is "12.27000...", number editor is set "12.27".
        * @param aLine : current editor
        */
        void TrimZeroL();  
        
        /**
        * Return operator type of a line.
        * @return TCalcOperatorType : ECalcAdd || ECalcSubtract || 
                                      ECalcMultiply || ECalcDivide ||
                                      ECalcEqual || ECalcOperatorNone
        */
        inline TCalcOperatorType Operator() const;
        
        
        /**
        * Set operator to a line
        * @param aType : operator enum
        */
        inline void SetOperator(const TCalcOperatorType& aType);

        /**
        * Check whether buffer of editline is "0" or not. 
        * @return ETrue :buffer of editline is "0".
        *         EFalse:buffer of editline is not "0".
        *         (Ex) If buffer is "0", return ETrue.
        *              If buffer is "0.0" or "2", return EFalse. 
        */
        TBool CheckZeroL() const;

        /**
        * Changing decimal separator.
        * @param aOld : Old decimal separator
        * @param aNew : New decimal separator  
        */
        void ChangeDecimal(TChar aOld, TChar aNew);
         

    private:    // New functions
        /**
        * Return count of digits.
        * @param aNumber : String of number
        * @return Count of digits. 
        *         E.g argument aNumber is "-226.89", return value is 5.
        */
        TInt NumberDigits() const;

    private:    // Data
        TBuf<KCalcMaxNumberWidth> iNumber; // Operand of a line
        TCalcOperatorType   iOperator; // Operator of a line 
    };

#include "CalcEditLine.inl"

#endif  // __CALCEDITLINE_H__

//  End of File
