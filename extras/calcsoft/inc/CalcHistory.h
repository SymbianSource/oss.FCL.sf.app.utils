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
* Description:  Define CCalcHistory class. This class is handled historical 
*                line of Calculator. 
*
*/


#ifndef     CALCHISTORY_H
#define     CALCHISTORY_H

//  INCLUDES
#include   <e32base.h>

//  CONSTANTS  
const TInt KCalcMaxHistoryLines(100);

//  FORWARD DECLARATIONS
class TCalcEditLine;

//  CLASS DEFINITIONS 

/**
CCalcHistory handles historical line of the calculator. 
*/
class   CCalcHistory :
        public CBase
    {
    public:  // Constructors and destructor
        /**
        * C++ default constructor.
        */
        CCalcHistory();
        
        /**
        * Destructor.
        */
        virtual ~CCalcHistory();
        
        
    public:     // New functions
        /**
        * Return (aIndex + 1)th latest history line.
        * @param aIndex  
        * @return (aIndex + 1)th latest history line.
        */
        const TCalcEditLine& operator[](TInt aIndex);  
        
        /**
        * Return count of historical lines. 
        * @return Count of historical lines
        */
        TInt Count() const;
        
        /**
        * Add newest history line
        * @param aLine : added line  
        */
        void Add(const TCalcEditLine aLine); 

        /**
        * Clear calculation history.
        */
        void ClearHistory();

        /**
        * Notify changing decimal separator.
        * @param aOld : Old decimal separator
        * @param aNew : New decimal separator  
        */
        void NotifyChangeDecimal(TChar aOld, TChar aNew);
        
    private:    // New functions
            
    private:    // Data
        //
        //  historical data
        //  Newest data index is iNewestIndex.
        //  
        //  <Case that iCount is smaller than KCalcMaxHistoryLines>
        //    As data is older, index is smaller. The oldest dara index is zero.
        //
        //  <Case that iCount is KCalcMaxHistoryLines>
        //    As data is older, index is smaller. But previous data index
        //   to index zero is KCalcMaxHistoryLines-1. previous index to
        //   KCalcMaxHistoryLines-1 is KCalcMaxHistoryLines-2. ..... 
        //   Therefore the oldest data index is iNewestIndex+1.
        //
        TCalcEditLine iHistory[KCalcMaxHistoryLines]; // Calculation history
        TInt      iCount;  // Valid historical lines
        TInt      iNewestIndex;  // Index of newest history
    };

#endif      //  CALCHISTORY_H
            
// End of File
