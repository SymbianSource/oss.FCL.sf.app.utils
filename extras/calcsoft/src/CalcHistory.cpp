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
*                line of the calculator. 
*
*/



// INCLUDE FILES
#include "CalcEditline.h" 
#include "CalcHistory.h" 

//  LOCAL CONSTANTS AND MACROS  


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcHistory::CCalcHistory():
              iNewestIndex(KCalcMaxHistoryLines - 1)
    {
    iHistory[iNewestIndex].AllClear();
    }

// Destructor
CCalcHistory::~CCalcHistory()
    {
    }



// Assignment operator
// This inplementation depends on how to have history data. 
// Refer to description of member data, CCalcHistory.h 
// 
const TCalcEditLine& CCalcHistory::operator[]
                       (TInt aIndex) // Get (aIndex + 1)th latest history line 
    {
    TInt lineIndex(iNewestIndex - aIndex);
    
    if ( lineIndex < 0 )
        {
        lineIndex += KCalcMaxHistoryLines;
        }
        
    return iHistory[lineIndex];      
    }

// ---------------------------------------------------------
// CCalcHistory::Count
// Return count of histoical lines.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCalcHistory::Count() const     
    {
    return iCount;
    }

// ---------------------------------------------------------
// CCalcHistory::Add
// This inplementation depends on how to have history data. 
// Refer to description of member data, CCalcHistory.h 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcHistory::Add
                (const TCalcEditLine aLine) 
    {
    if ( iNewestIndex == (KCalcMaxHistoryLines - 1) )
        {
        iNewestIndex = 0;
        }
    else
        {
        iNewestIndex++;
        }
    
    if ( iCount < KCalcMaxHistoryLines )
        {
        iCount++;
        }
    
    iHistory[iNewestIndex] = aLine;
    }

// ---------------------------------------------------------
// CCalcHistory::ClearHistory
// Clear calculation history.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcHistory::ClearHistory()
    {
    iCount = 0;
    iNewestIndex = KCalcMaxHistoryLines - 1;
    for (TInt cnt(0); cnt < KCalcMaxHistoryLines; cnt++)
        {
        iHistory[cnt].AllClear();
        }
    }


// ---------------------------------------------------------
// CCalcHistory::NotifyChangeDecimal
// Call when decimal separator is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcHistory::NotifyChangeDecimal(TChar aOld, TChar aNew)
    {
    for (TInt cnt(0); cnt < iCount; cnt++)
        {
        iHistory[cnt].ChangeDecimal(aOld, aNew);
        }
    }
        


// End of File
