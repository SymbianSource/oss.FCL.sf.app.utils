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
* Description:  Document class, CCalcDocument
*                Derived from CEikDocument
*                The history of inputted data is held.
*                The memory of four-arithmetical-operations operation 
*                or a calculation result is held.
*                The last result and memory are saved at a file and 
*                it restores.
*
*/


#ifndef     CALCDOC_H
#define     CALCDOC_H


//  INCLUDES
#include <eikdoc.h>

#include "CalcEditline.h"

#include "CalcView.h"
//  CONSTANTS
const TInt KCalcErrEditorSpaceFull(-10000); 

//  FORWARD DECLARATIONS

class  CEikAppUi;
class  CCalcHistory;
class  CCalcAppEnv;

//  CLASS DEFINITIONS 

/**
CCalcDocument : 'Document' class
*/
class   CCalcDocument 
        :public CEikDocument
    {
    public:  // Constructors and destructor  
        /**
        * Two-phased constructor.
        * @param aApp : Reference of CEikApplication class 
        */
        static CCalcDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CCalcDocument();

        
    public:  // New functions
		/**
        * Set the pointer to CalcAppEnv. 
        */
		void SetAppEnv(CCalcAppEnv* aCalcAppEnv);

        /**
        * Set the pointer to CalcAppEnv. 
        */		
        void SetCalcView( CCalcView* aCCalcView );
        
		/**
        * Make calculation, and add result to calcluation history.
        * Leave may occur, causes KErrOverflow or KErrDivideByZero or KErrNotSupported (negative value with square root).
        * @param aOperand : operand.
        * @param aLine : inputted edit line
        * @return TReal64: Provisional result.
        */
        TReal64 CalculateAndAddHistoryL(
                TReal64 aOperand,
               const TCalcEditLine& aLine);  
		
		/**
        * Make calculation, nothing is added to the calculation history.
        * Leave may occur, causes KErrOverflow or KErrDivideByZero or KErrNotSupported (negative value with square root).
        * @param aOperand : operand.
        * @param aLine : inputted edit line
        * @return TReal64: Provisional result.
        */
		TReal64 CalculateAndNoHistoryL(
				TReal64 aOperand,
               const TCalcEditLine& aLine);  

		/**
        * Make calculation, and modify the operator
		* before adding anything to the calculation history.
        * Leave may occur, causes KErrOverflow or KErrDivideByZero or KErrNotSupported (negative value with square root).
        * @param aOperand : operand.
        * @param aLine : inputted edit line
        * @return TReal64: Provisional result.
        */
		TReal64 CalculateAndModifyHistoryL(
				TReal64 aOperand,
               TCalcEditLine& aLine,
			   TCalcEditLine::TCalcOperatorType aOperator); 

        /**
        * Add a line which is "= (provisional result)" to calculation history.
        * And Last Result is updated to provisional result
        */
        void AddEqualLineAndUpdateLastResultL();

        /**
        * Empty line is added to calculation history.
        */
        void AddEmptyLine();
        
        /**
        * Memory is saved.
        * @param aNewMemory : New memory, current operand.
        */
        void MemorySaveL(TReal64 aNewMemory); 

        /**
        * Memory is saved.
        * @param aLine : Current value of editor
        */
        TBool HasMemory() const;

        /**
        * Return current memory.
        * @return TReal64 : current memory.
        */
        TReal64 Memory() const; 

        /**
        * Clear memory.
        */
        void MemoryClearL();
        
        /**
        * Return last result
        * @param TReal64 : Last result.
        */
        TReal64 LastResult() const;  

        /**
        * Return provisional result
        * @param aLine : For getting provisional result.
        */
        TReal64 ProvisionalResult() const;

        /**
        * Return pointer of CCalcHistory class. 
        * @return Pointer of CCalcHistory class
        */
        CCalcHistory* History() const;
        
        /**
        * Restore memory and last result
        */
        void LoadStateL();
        
        /**
        * Store memory and last result
        */
		void SaveStateL();
        
    private:   //  New functions
        /**
        * By default constructor is private.
        * @param aApp: Reference of CEikApplication class
        */
        CCalcDocument(CEikApplication& aApp); 

        /**
        * Second-phase constructor
        */
        void ConstructL();

        /**
        * Make calculation by using argument aLine 
        * Leave may occur, causes KErrOverflow or KErrDivideByZero.
        * @param aOperand : operand
        * @param aOperator : Operator type enum.
        * @return Result of calculation
        */
        TReal64 CalculateL(TReal64 aOperand,
            TCalcEditLine::TCalcOperatorType aOperator);
        
        /**
        * Check overflow and underflow 
        * Leave may occur, causes KErrOverflow
        * @param aResult : result of calculation
        */
        void CheckResultL(TReal64* aResult);  

        /**
        * Write last result and memory to document file.
        * @param aStream : Interface of write steam
        */
        void ExternalizeL(RWriteStream& aStream) const;
        
        /**
        * Read last result and memory from document file.
        * @param aStream : Interface of read steam
        */
        void InternalizeL(RReadStream& aStream);

        
    private:  // Functions from base classes  
        /**
        * From CEikDocument : Create CCalcAppUi object
        */
        CEikAppUi* CreateAppUiL();  

    private:    // Data
	    CCalcAppEnv* iCalcAppEnv;  // Not own
        TReal64 iProvisionalResult; // Provisional result
        TReal64 iMemory;        // Memory
        TReal64 iLastResult;    // Last result
        CCalcHistory* iHistory; // History
        CCalcView* iCCalcView;  // Not own
    };

#endif      //  CALCDOC_H
            
// End of File

