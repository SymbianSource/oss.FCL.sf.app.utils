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
*                The numerical value and the character sequence of 
*                the real number are held. Addition of a numerical character 
*                sequence and which clear operation are performed.
*
*/



// INCLUDE FILES  
#include    "CalcDoc.h"
#include    "CalcEditline.h"
#include	"CalcEnv.h"


//  LOCAL CONSTANTS AND MACROS  


// ================= MEMBER FUNCTIONS =======================


// C++ default constructor can NOT contain any code, that
// might leave.
//
TCalcEditLine::TCalcEditLine():
                     iNumber(0), 
                     iOperator(ECalcOperatorNone)
    {
    }

TCalcEditLine::~TCalcEditLine()
    {
    }

// Copy constructor
TCalcEditLine::TCalcEditLine
           (const TCalcEditLine& aEditLine) 
    {
    *this = aEditLine;
    }

// Assignment operator
const TCalcEditLine& TCalcEditLine::operator=
                       (const TCalcEditLine& aEditLine) 
    {
    iNumber = aEditLine.iNumber;
    iOperator = aEditLine.iOperator;
    return  *this;
    }


// ---------------------------------------------------------
// TCalcEditLine::AllClear
// This function exists because long-press clear-key event is executed. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::AllClear()
    {
    iNumber.Zero();
    iOperator = ECalcOperatorNone;
    }

// ---------------------------------------------------------
// TCalcEditLine::ClearL
// This function exists because short-press clear-key event is executed. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::ClearL()
    {
    TInt length(iNumber.Length());
    if (length > 1)
        {
        CCalcAppEnv* env = CCalcAppEnv::Static();
        TChar minusIndicator(env->MinusIndicator());
        if (iNumber[0] == (TUint) minusIndicator && // Casted for THUMB build
            length == 2)
            {
            SetNumber(0);
            }
        else
            {
            iNumber.Delete(length - 1, 1);
            length--;
            if (length == 2 && iNumber[0] == (TUint) minusIndicator &&
                                             // Casted for THUMB build
                NumberL() == 0.0)
                {
                SetNumber(0);
                }
            }
        }
    else
        {
        SetNumber(0);
        }
    }


// ---------------------------------------------------------
// TCalcEditLine::ClearOperand
// Set number of editor "0".  
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::ClearOperand()
    {
    SetNumber(0);
    }

// ---------------------------------------------------------
// TCalcEditLine::NumberL
// Convert from TBuf<> to number.    
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 TCalcEditLine::NumberL() const
    {
    TLex  lex(iNumber);
    TReal64  data;
    TInt errorCode(lex.Val(data));
    // Converting is failed. 
    if (errorCode)
        {
        User::Leave(errorCode);
        }
    return  data;
    }

// ---------------------------------------------------------
// TCalcEditLine::SetNumber
// Convert from number to TBuf<>.    
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::SetNumber
                (TReal64 aNumber)
    {
    // Set data format. 
    TRealFormat realFormat( KCalcMaxEditNumberWidth, KCalcMaxDigits );
    if ( aNumber >= 0 )
        {
        realFormat.iWidth--;
        }
    realFormat.iType = KRealFormatNoExponent;
    realFormat.iTriLen = 0;  //  Delimit character is nothing
    CCalcAppEnv* env = CCalcAppEnv::Static();
    TChar separator(env->DecimalSeparator());
    realFormat.iPoint = separator;

    TInt code = iNumber.Num(aNumber, realFormat);
    
    if(KErrOverflow == code || KErrUnderflow == code)
	    {
	    // Set data format 
	    TRealFormat realFormat(KCalcMaxNumberWidth);  
	    if ( aNumber >= 0 )
	        {
	        realFormat.iWidth--;
	        }
	    realFormat.iType = KRealFormatExponent;
	    realFormat.iPlaces = 3;
	    realFormat.iTriLen = 0;  //  Delimit character is nothing
	    CCalcAppEnv* env = CCalcAppEnv::Static();
	    TChar separator(env->DecimalSeparator());
	    realFormat.iPoint = separator;
        code = iNumber.Num(aNumber, realFormat);
	    }
	   
	  switch (code)
        {
        case KErrOverflow:
            {
            TRAP_IGNORE( User::Leave( KErrOverflow ) );  
            break;
            }
        case KErrUnderflow:
            {
            iNumber.Num(0,0);
            break;
            }
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------
// TCalcEditLine::AppendL
// This function exists to handle numeric or "#" key event.    
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::AppendL
               (TChar aInput) // Input type is digit or decimal point only  
    {
    //  Check whether editline is not full
    if ( iNumber.Length() < KCalcMaxNumberWidth )
        {
        if ( aInput.IsDigit() )
            {
            if ( NumberDigits() < KCalcMaxDigits )
                {
                if (CheckZeroL())  
                    {
                    iNumber.Zero();
                    }
                iNumber.Append(aInput);
                }
            else
                {
                // If editor space is full, error code returned.
                User::Leave(KCalcErrEditorSpaceFull); 
                }
            }
        // If decimal point do not exist, decimal point is added 
        // But editor has 8 dights, decimal point is not added.
        else 
            {
            CCalcAppEnv* env = CCalcAppEnv::Static();
            TChar separator(env->DecimalSeparator());
            if ( iNumber.Length() )
                {
                if ( iNumber.Locate(separator) == KErrNotFound )
                    {
                    if (NumberDigits() < KCalcMaxDigits)
                        {
                        iNumber.Append(separator);
                        }
                    }
                }
            else
                {
                SetNumber(0);  
                iNumber.Append(separator);
                }
            }
        }
    }

// ---------------------------------------------------------
// TCalcEditLine::AppendNumberString
// This function appends a character after the number.
// Used for example in percent calculations.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::AppendNumberStringL
				(TChar aChar)
	{
	//  This kind of a paranoid check as when this function
	//  is called the amount of digits can never be more
	//  than 8. Because the iNumber is going
	//  to the output sheet we can display 10 characters
	//  8 digits + operator + aChar
    if ( NumberDigits() < KCalcMaxDigits + 1 )
        { 
			iNumber.Append(aChar);
        }
    else
        {
			// If editor space is full, error code returned.
            User::Leave(KCalcErrEditorSpaceFull); 
         }
	}

// ---------------------------------------------------------
// TCalcEditLine::ChangeSignL
// This function exists to handle "Change Sign" command
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::ChangeSignL()
    {
    TInt length(iNumber.Length()); 

    // If the number is zero or empty, this command is ignored
    if ((length > 0) && (NumberL() != 0.0))
        {
        CCalcAppEnv* env = CCalcAppEnv::Static();
        TChar minusIndicator(env->MinusIndicator());
        // If the number is less than zero, '-' is deleted.
        if ( iNumber[0] == (TUint) minusIndicator ) // Casted for THUMB build
            {
            iNumber.Delete(0, 1);
            }
        // Case number is more than zero.
        else
            {
            TBuf<1> buf;
            buf.Append(minusIndicator);
            iNumber.Insert(0, buf);
            }
        }
    }

// ---------------------------------------------------------
// TCalcEditLine::TrimZeroL
// If number of editor is "iii.ddd000...", set number "iii.ddd".
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::TrimZeroL() 
    {
    TReal64 realValue(NumberL());
    SetNumber(realValue);
    }


// ---------------------------------------------------------
// TCalcEditLine::CheckZero
// For checking that editline is cleared. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool TCalcEditLine::CheckZeroL() const
    {
    return ((iNumber.Length() == 1) && (NumberL() == 0.0));
    }

// ---------------------------------------------------------
// TCalcEditLine::ChangeDecimal
// Changing decimal separator.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TCalcEditLine::ChangeDecimal(TChar aOld, TChar aNew)
    {
    TInt pos(iNumber.Locate(aOld));

    if (pos != KErrNotFound)
        {
        iNumber[pos] = (TUint16) aNew;
        }
    }
        

// ---------------------------------------------------------
// TCalcEditLine::NumberDigits
// To count of digits in a line
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt TCalcEditLine::NumberDigits() const
    {
    TInt len(iNumber.Length());
    TInt num(0);
    for (TInt i(0); i < len; ++i)
        {
        TChar onechar(iNumber[i]);
        if (onechar.IsDigit())
            {
            ++num;
            }
        }
    return num;
    }


//  End of File
