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



// INCLUDE FILES  
#include    "CalcDoc.h"
#include    "CalcEditline.h"
#include    "CalcAppUi.h"
#include    "CalcApp.h"
#include	"CalcEnv.h"
#include    "CalcHistory.h"
#include	"e32math.h"
#include <e32cmn.h>
#include <s32file.h>
// CONSTANTS
const TReal64 KCalcDefaultMemory(0.0);
const TReal64 KCalcDefaultLastResult(0.0);
const TReal64 KCalcDefaultZero(1E-12);

_LIT( KCalculatorFilename,"Calcsoft.ini");
_LIT(KDelimiter, ":");

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CCalcDocument* CCalcDocument::NewL
                (CEikApplication& aApp) 
    {
    CCalcDocument* self = new (ELeave) CCalcDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CCalcDocument::~CCalcDocument()
    {
    delete iHistory;
    }

// ---------------------------------------------------------
// CCalcDocument::CalculateAndAddHistoryL
// This function is called when State is changed to State3 or State5. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CCalcDocument::CalculateAndAddHistoryL
                   (TReal64 aOperand,
                    const TCalcEditLine& aLine)
    {
	iProvisionalResult = CalculateL(aOperand, aLine.Operator()); 
	iHistory->Add(aLine);       // Add a line to history.	
	return iProvisionalResult;	
    }

// ---------------------------------------------------------
// CCalcDocument::CalculateAndNoHistoryL
// This function is called when the calculations is not
// written to the Outputsheet (sqrt and percent in some cases).
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CCalcDocument::CalculateAndNoHistoryL
                   (TReal64 aOperand,
                    const TCalcEditLine& aLine)
	{
	TReal64 result = CalculateL(aOperand, aLine.Operator()); 
	return result;
	}

// ---------------------------------------------------------
// CCalcDocument::CalculateAndModifyHistoryL
// This function is called when the output needs to be edited
// before it is displayed in the Outputsheet. This function 
// may call the CalculateL twice, because it may need to
// calculate the percent first.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CCalcDocument::CalculateAndModifyHistoryL
                   (TReal64 aOperand,
                    TCalcEditLine& aLine,
					TCalcEditLine::TCalcOperatorType aOperator)
	{
	if (aOperator == TCalcEditLine::ECalcMultiply)
		iProvisionalResult = CalculateL(aOperand, aLine.Operator()); 
	else
		{
		TReal64 iTempResult = CalculateL(aOperand, aLine.Operator()); 
		iProvisionalResult = CalculateL(iTempResult, aOperator);
		}

	aLine.SetOperator(aOperator);
	// Get the percent character from iCalcAppEnv (it uses the resource
	// file to get the character).
	TChar character((iCalcAppEnv->OutSheetOperator(TCalcEditLine::ECalcPercent))[0]);
	aLine.AppendNumberStringL(character); // Add percent to the line
	iHistory->Add(aLine);       // Add a line to history.	
	return iProvisionalResult;
	}

// ---------------------------------------------------------
// CCalcDocument::AddEqualLineAndUpdateLastResult
// This is called when "Equal" command is selected. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::AddEqualLineAndUpdateLastResultL()
    {
    TCalcEditLine line;
    line.SetOperator(TCalcEditLine::ECalcEqual);
    line.SetNumber(iProvisionalResult);
    iHistory->Add(line);         // Add a line to history.
    iLastResult = iProvisionalResult;
    SaveStateL();
    }

// ---------------------------------------------------------
// CCalcDocument::AddEmptyLine
// Add a empty line
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::AddEmptyLine()
    {
    TCalcEditLine line;
    iHistory->Add(line);       
    }

// ---------------------------------------------------------
// CCalcDocument::MemorySave
// Save a memory from value of current editor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::MemorySaveL
                   (TReal64 aNewMemory)
    {
    iMemory = aNewMemory;
    SaveStateL();
    }


// ---------------------------------------------------------
// CCalcDocument::HasMemory
// Check memory non-zero.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCalcDocument::HasMemory() const
    {
    return (iMemory != 0.0);
    }

// ---------------------------------------------------------
// CCalcDocument::Memory
// Return memory.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CCalcDocument::Memory() const
    {
    return (iMemory);
    }

// ---------------------------------------------------------
// CCalcDocument::MemoryClear
// Clear memory.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::MemoryClearL()
    {
    iMemory = 0.0;
    SaveStateL(); 	
    }

// ----------------------------------------------------
// CCalcDocument::LastResult
// Return Last result.
// (other items were commented in a header).
// ----------------------------------------------------
//
TReal64 CCalcDocument::LastResult() const
    {
    return (iLastResult);
    }

// ----------------------------------------------------
// CCalcDocument::ProvisionalResult
// Return provisional result
// (other items were commented in a header).
// ----------------------------------------------------
//
TReal64 CCalcDocument::ProvisionalResult() const
    {
    return iProvisionalResult;
    }


// ----------------------------------------------------
// CCalcDocument::History
// Return CCalcHistory class.
// (other items were commented in a header).
// ----------------------------------------------------
//
CCalcHistory* CCalcDocument::History() const
    {
    return iHistory;
    }


// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcDocument::CCalcDocument
               (CEikApplication& aApp) 
              : CEikDocument(aApp)
    {
    }

// Second phase constructor.
void CCalcDocument::ConstructL()
    {
    iHistory = new (ELeave) CCalcHistory();
    }

// ---------------------------------------------------------
// CCalcDocument::CalculateL
// Calculate result
// Leave may occur, causes KErrOverflow or KErrDivideByZero.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CCalcDocument::CalculateL
        (TReal64 aOperand,
         TCalcEditLine::TCalcOperatorType aOperator) 
    {
    TReal64  result(iProvisionalResult);

    switch (aOperator)
        {
        case  TCalcEditLine::ECalcAdd:
            {
            result += aOperand;
            if( Abs(result)<= KCalcDefaultZero )
            	{
            	result = 0;
            	}
            break;
            }
        case  TCalcEditLine::ECalcSubtract:
            {
            result -= aOperand;
            if( Abs(result)<= KCalcDefaultZero )
            	{
            	result = 0;
            	}
            break;
            }
        case  TCalcEditLine::ECalcMultiply:
            {
            result *= aOperand;
            break;
            }
        case  TCalcEditLine::ECalcDivide:
            {
            if ( aOperand == 0 )
                {
                iCCalcView->UpdateState( CCalcView::EOperator );
                User::Leave(KErrDivideByZero); //  Error causes
                }
            else
                {
                result /= aOperand;
                }
            break;
            }
		case TCalcEditLine::ECalcSqrt:
			{
			if ( aOperand < 0 )
				{
                User::Leave(KErrNotSupported); //  Error causes
                }
			else
				Math::Sqrt(result, aOperand);
			break;
			}
		case TCalcEditLine::ECalcPercent:
			{
			result = (result * aOperand) / 100;
			break;
			}
        case  TCalcEditLine::ECalcOperatorNone:
            {
            result = aOperand;
            break;
            }
        default:
            {
            break;
            }
        }
    //  Check Result. If overflow, Leave occurs.
    CheckResultL(&result);

    // Overflow do not occur
    return  result;
    }


// ---------------------------------------------------------
// CCalcDocument::CheckResultL
// Check overflow and underflow  
// Leave may occur, causes KErrOverflow.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::CheckResultL
                   (TReal64* aResult)    
    {
    // Set data format 
    TRealFormat realFormat(KCalcMaxNumberWidth, KCalcMaxDigits);  
    if ( *aResult >= 0 )
        {
        realFormat.iWidth--;
        }
    realFormat.iType = KRealFormatNoExponent;
    realFormat.iTriLen = 0;  //  Delimit character is nothing
    TLocale locale;
    TChar separator(locale.DecimalSeparator());
    
    realFormat.iPoint = separator;

    TBuf<KCalcMaxNumberWidth> buffer;  
    TInt code = buffer.Num(*aResult, realFormat);
    
    if(KErrOverflow == code || KErrUnderflow == code)
	    {
	    TRealFormat realFormat(KCalcMaxNumberWidth);  
	    if ( *aResult >= 0 )
	        {
	        realFormat.iWidth--;
	        }
	    realFormat.iType = KRealFormatExponent;
	    realFormat.iPlaces = 3;
	    realFormat.iTriLen = 0;  //  Delimit character is nothing
	    TLocale locale;
	    TChar separator(locale.DecimalSeparator());
	    
	    realFormat.iPoint = separator;

	    TBuf<KCalcMaxNumberWidth> buffer;  
	    code = buffer.Num(*aResult, realFormat);
	    }
	
	switch (code)
        {
        case KErrOverflow:
            {
            User::Leave(KErrOverflow);  
            break;
            }
        case KErrUnderflow:
            {
            *aResult = 0.0;
            break;
            }
        default:
            {
            break;
            }
        }
   
    }


// ---------------------------------------------------------
// CCalcDocument::ExternalizeL
// Externalising of the last result and memory.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::ExternalizeL
        (RWriteStream& aStream) const 
    {
    aStream.WriteReal64L(iLastResult);  // Write last result
    aStream.WriteReal64L(iMemory);      // Write memory
    }

// ---------------------------------------------------------
// CCalcDocument::InternalizeL
// Internalising of the last result and memory.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::InternalizeL
        (RReadStream& aStream) 
    {
    iLastResult = aStream.ReadReal64L();    // Read last result
    iMemory = aStream.ReadReal64L();        // Read memory

    // If read value is out of range, 
    // memory is replaced to default one.
    TRAPD(error, CheckResultL(&iMemory));
    if (error == KErrOverflow)
        {
        iMemory = KCalcDefaultMemory;
        }

    // If read value is out of range, 
    // last result is replaced to default one.
    TRAP(error, CheckResultL(&iLastResult));
    if (error == KErrOverflow)
        {
        iLastResult = KCalcDefaultLastResult;
        }

    }

// ---------------------------------------------------------
// CCalcDocument::CreateAppUiL
// This function is called when Calculator application is opened. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CEikAppUi* CCalcDocument::CreateAppUiL()
    {
    return new (ELeave) CCalcAppUi;
    }

// ---------------------------------------------------------
// CCalcDocument::SaveStateL
// Store memory and last result
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::SaveStateL()
{
		RFileWriteStream out;
		RFs fileSession;
			
		User::LeaveIfError(fileSession.Connect());
    	CleanupClosePushL(fileSession);
    
    	TFileName filePath;
		TBuf<1> tempDes;
		TChar driveChar;
	   	
    	User::LeaveIfError(fileSession.PrivatePath(filePath));
    	fileSession.DriveToChar(KDefaultDrive, driveChar);
    	tempDes.Append(driveChar);
    	filePath.Insert(0,KDelimiter);
    	filePath.Insert(0,tempDes);
    	filePath.Append(KCalculatorFilename);
    	
    	TInt err( out.Replace( fileSession, filePath,EFileWrite ) );
	
	if( !err )
		{
		TRAP( err, ExternalizeL( out ) );
	
		out.Close();
		}
		  
  	CleanupStack::PopAndDestroy();
  		
}

 // ---------------------------------------------------------
// CCalcDocument::LoadStateL
// Restore memory and last result
// This function is called when document file exists. 
// (other items were commented in a header).
// ---------------------------------------------------------
//   
    void CCalcDocument::LoadStateL()
    {
    	RFileReadStream in;
    	RFs fileSession;
    		
    	User::LeaveIfError(fileSession.Connect());
    	CleanupClosePushL(fileSession);

		TFileName filePath;
		TBuf<1> tempDes;
		TChar driveChar;
    	User::LeaveIfError(fileSession.PrivatePath(filePath));
    	fileSession.DriveToChar(KDefaultDrive, driveChar);
    	tempDes.Append(driveChar);
    	filePath.Insert(0,KDelimiter);
    	filePath.Insert(0,tempDes);
    	filePath.Append(KCalculatorFilename);
  		
		TInt err( in.Open( fileSession, filePath,
			EFileRead ) );

	if( !err )
		{
		TRAPD(readErr,InternalizeL( in ) );
		
		if ( readErr )
        {
        // Internalizing fails.
        iMemory = KCalcDefaultMemory;         // Reset memoey
        iLastResult = KCalcDefaultLastResult; // Reset last result
        }
        
		in.Close();
		}
			CleanupStack::PopAndDestroy();
		
        SetChanged(EFalse);
	
    }

// ---------------------------------------------------------
// CCalcDocument::SetAppEnv
// This function is used to receive a pointer to the
// CCalcAppEnv.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::SetAppEnv(CCalcAppEnv* aCalcAppEnv)
	{
		iCalcAppEnv = aCalcAppEnv;
	}

// ---------------------------------------------------------
// CCalcDocument::SetCalcView
// This function is used to receive a pointer to the
// SetCalcView.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcDocument::SetCalcView( CCalcView* aCCalcView )
    {
    iCCalcView = aCCalcView;
    }
//  End of File  
