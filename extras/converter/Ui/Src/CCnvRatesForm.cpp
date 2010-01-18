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
* Description:  
*      This is the implementation of the class defined in CCnvRatesForm.h
*
*/



#include <eikcapc.h>
#include <eikfpne.h>
#include <AknQueryDialog.h>
#include <aknnotewrappers.h>
#include <akntitle.h>
#include <StringLoader.h>
#include <featmgr.h>
#include <eikmenup.h>

#include <TCnvCategory.h>
#include "CCnvRatesForm.h"
#include "CCnvApplication.h"

#include <csxhelp/cnv.hlp.hrh>

#include <Converter.rsg>
#include "Converter.hrh"

#include <e32math.h> //Header to be included for getting CEikEdwin Text to a number

//CONSTANTS for Max Number of Added Currency
const TInt KMaxNumberAddCurrency = 40;

CCnvRatesForm::CCnvRatesForm( TBool* aWantsRelaunch, TBool* aShouldExit,
							 TBool* aModelChanged, TCnvCategory& aCategory ) 
							 : iWantsRelaunch( aWantsRelaunch ), iShouldExit( aShouldExit ),
							 iModelChanged( aModelChanged ), iCategory( aCategory ),
							 iLastUsedEditorId(-1)
	{
	}

CCnvRatesForm::CCnvRatesForm()
	{

	//Member variable set to False in the Constructor
	iEKeyOk = EFalse;
	}

CCnvRatesForm::~CCnvRatesForm()
	{
	iFormIdMapArray.Close();
	iFormIdEdwinMapArray.Close();
	SetDefaultTitle();
	}

void CCnvRatesForm::ConstructL()
	{
	// Use base class to construct menubar
	CAknDialog::ConstructL( R_CNV_RATESFORM_MENUBAR );
    iCategory.CreateTempUnitArrayForRestore();
    iCategory.GetDefaultUnits( iSrcUnitIndexForRestore , iDesUnitIndexForRestore );	
     iHistoryBaseUnit = iCategory.BaseUnit();
	}

void CCnvRatesForm::GetHelpContext( TCoeHelpContext& aContext ) const
	{
	aContext.iMajor = KUidCnv;
	aContext.iContext = KCNV_HLP_RATES;
	}

void CCnvRatesForm::ProcessCommandL( TInt aCommandId )
	{
	// Base class handles EAknFormCmdSave among the other things
	CAknForm::ProcessCommandL( aCommandId );

	switch ( aCommandId )
		{
		case ECmdAddCurrency:
			{			
			AddNewCurrencyL();
			break;
			} 
		case ECmdDeleteCurrency:
			{
			DeleteCurrencyL();
			break;
			}
		case ECmdChangeCurrencyName:
			{
			CmdChangeCurrencyNameL();
			break;
			}
		case ECmdSetAsBaseCurrency:
			{
			CmdSetAsBaseCurrencyL();
			break;
			}
		case EAknCmdHelp:
			{
			iEikonEnv->EikAppUi()->HandleCommandL( EAknCmdHelp );
			break;
			}
		case EEikCmdExit:
			{
			*iShouldExit = ETrue;
			TryExitL( EAknSoftkeyBack );
			break;
			}
		default:
			{
			break;
			}
		}
	}

// Takes any action required when the current line is changed to aControlId.
void CCnvRatesForm::LineChangedL( TInt aControlId )
	{

	CEikFloatingPointEditor* editor =
		static_cast< CEikFloatingPointEditor* >( Control( aControlId ) );

	//editor->ClearSelectionL();
    editor->SetCursorPosL( editor->Text()->DocumentLength(), ETrue );

    if ( iLastUsedEditorId > 0 )
        {
        editor =static_cast<CEikFloatingPointEditor*> ( Control( iLastUsedEditorId ) );  
        TBuf<KEditorTextMaxLength> editorText;
        editor->GetText( editorText );
        
        // Use local value for different area
        TBuf<1> localeZero;
        localeZero.AppendNum( 0 );
        AknTextUtils::LanguageSpecificNumberConversion( localeZero ); 
        if ( editorText.Length() > 1 && editorText[0] == localeZero[0] )
            {
            // remove all leading zeroes
            while ( editorText.Length() > 0 && editorText[0] == localeZero[0] )
                {
                editorText.Delete( 0, 1 );
                }
            // if too many zeroes removed, insert one.
            TLocale locale;
            if ( editorText.Length() == 0 ||
                TChar( editorText[0] ) == locale.DecimalSeparator() )
                {
                editorText.Insert( 0, localeZero );
                }
            editor->SetTextL( &editorText );
            editor->DrawDeferred();
            }
        }
    iLastUsedEditorId = aControlId;
	}

void CCnvRatesForm::DoNotSaveFormDataL()
	{
	iCategory.RestoreUnitArray();
	iCategory.SetDefaultUnits( iSrcUnitIndexForRestore , iDesUnitIndexForRestore );
	iCategory.SetBaseUnit( iHistoryBaseUnit );
	}

// Saves values from form fields back to the model
TBool CCnvRatesForm::SaveFormDataL()
	{
	SetEditableL( ETrue );

	const MDesCArray* names = iCategory.MdcArray();
	TInt len( names->MdcaCount() );
	TInt formIdIndex = 0;
	for( TInt i = 0; i < len; i ++ )
		{

		if( i != iCategory.BaseUnit() )
			{
			TInt formId = iFormIdMapArray[formIdIndex];
			//A seperate formIdIndex is maintained for the form as iFormIdMapArray
			//does not have an index for Base Unit
			formIdIndex++;

			TReal value = 0; //Contains the value of the editor

			if(KErrNotFound != iFormIdEdwinMapArray.Find(formId))
				{
				TBuf<32> edText;
				GetEdwinText( edText, formId );				
				TInt64 power = 1;
				TBool fraction = EFalse;
				TChar ch;

				for(TInt i = 0; i < edText.Length(); i++)
					{
					ch = edText[i];

					if(ch == '+')
						{
						if( i != 0 )
							{
							value = 0;
							break;
							}
						}

					else if(ch == '.')
						{
						fraction = ETrue;
						}
					else if(IsDigit(ch))
						{
						value = value * 10 + ch.GetNumericValue();
						if(fraction)
							{
							power = power * 10;
							}
						}
					}
				value = value / power;				

				}
			else
				{
				value = FloatEditorValue( formId  );
				}
			iCategory.SetUnitFactor( i, /*FloatEditorValue( formId  )*/value );
			}		
		}

	*iModelChanged = ETrue;
	return ETrue;
	}

// Sets the focus to the topmost line of the form when the dialog is activated
void CCnvRatesForm::SetInitialCurrentLine()
	{
	CEikDialog::SetInitialCurrentLine();
	TRAP_IGNORE(TryChangeFocusToL( IdOfFocusControl() ) );	
	}

void CCnvRatesForm::PreLayoutDynInitL()
	{
	SetTitleL( ETrue );

	const MDesCArray* currencies = iCategory.MdcArray();
	TInt len( currencies->MdcaCount() );

	for( TInt i = 0; i < len; i++ )
		{
		if( i != iCategory.BaseUnit() )
			{
			// form controls don't like id = 0, so we need to do id += 1..
			CCoeControl* control =
				CreateLineByTypeL( currencies->MdcaPoint( i ),
				ActivePageId(),
				i + 1,
				EEikCtFlPtEd,
				NULL );
			//Keep the id's of the form in an array so that deletion based on form ids is not a problem.
			iFormIdMapArray.Append(i+1);
			CEikFloatingPointEditor* editor =
				static_cast< CEikFloatingPointEditor* >( control );
			editor->ConstructL( 0, 9e99, 12 );	// 9e99 = max TReal
			TReal value( iCategory.UnitFactor( i ) );
			editor->SetValueL( &value );
			}
		}
	}

void CCnvRatesForm::CmdChangeCurrencyNameL()
	{
	// 16 is long enough, current edit window max length is 13
	// (defined in R_CNV_UNITNAME_QUERY )
	TBuf< 16 > newName;
	TBool nameOk( EFalse );

	TInt controlId = IdOfFocusControl();	
	TInt index =  controlId - 1;

	index = iFormIdMapArray.Find( controlId ); //get the index where the id i

	TUint uintindex = index ;//To remove warnings

	if( uintindex  >= iCategory.BaseUnit())
		{
		index++;
		}
	const MDesCArray* names = iCategory.MdcArray();
	newName.Copy( names->MdcaPoint( index  ).Left( MAXCURRENCYNAMELENGTH ) );


	TCurrencyType type = NewCurrencyNameDialogL( newName, nameOk );

	if(nameOk)
		{
		iCategory.SetUnitName( index , newName , type);
		Line( controlId )->SetCaptionL( newName );
		Line( controlId )->DrawDeferred();
		nameOk = ETrue;
		*iModelChanged = ETrue;
		}	
	}


void CCnvRatesForm::CmdSetAsBaseCurrencyL()
	{
	CAknQueryDialog* dlg = CAknQueryDialog::NewL();

	if( dlg->ExecuteLD( R_CNV_CONFIRMATION_QUERY ) )
		{
		// form controls don't like ID = 0, so IDs are unit id + 1;
		TInt baseUnit;
		TUint baseUnitIndex; //To remove warnings
		//Get the current id of the form.
		TInt currentFocusIndex = IdOfFocusControl();
		//Find the id in the map array
		baseUnit = iFormIdMapArray.Find(currentFocusIndex);
		baseUnitIndex = baseUnit; //To remove warnings
		//adjust the baseUnitIndxex to the index in the Currency Category array..
		if( baseUnitIndex >= iCategory.BaseUnit())
			{
			baseUnit++;
			}
		iHistoryBaseUnit = baseUnit;
		iCategory.SetBaseUnit( baseUnit );

		// Reset all currency rates to 1
		TInt len( iCategory.MdcArray()->MdcaCount() );
		for( TInt i = 0; i < len; i++ )
			{
			iCategory.SetUnitFactor( i, 1.0 );
			}

		// Except the base unit factor is set to 1.0
		iCategory.SetUnitFactor( baseUnit, 1.0 );

		// We have to destroy and relaunch the form
		*iWantsRelaunch = ETrue;
		delete this;
		}
	}


// Reset the title to default. Don't want a leave.
// If it fails, it fails. For use in the destructor.
void CCnvRatesForm::SetDefaultTitle()
	{
	 TRAP_IGNORE(SetTitleL( EFalse ) );
	}

void CCnvRatesForm::SetTitleL( TBool aNonDefault )
	{
	CAknTitlePane* title = static_cast< CAknTitlePane* >(
		iEikonEnv->AppUiFactory()->StatusPane()->
		ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

	if( aNonDefault )
		{
		TInt baseUnit = iCategory.BaseUnit();
		const MDesCArray* currencies = iCategory.MdcArray();
		TPtrC unitName = currencies->MdcaPoint( baseUnit );
		// stuff it into the title text's placeholder
		HBufC* titleText = StringLoader::LoadL( R_CNV_RATESFORM_TITLE,
			unitName, iEikonEnv );
		TPtr titleTextPtr = titleText->Des();
		CleanupStack::PushL( titleText );
		AknTextUtils::DisplayTextLanguageSpecificNumberConversion(titleTextPtr);
		title->SetTextL( *titleText );
		CleanupStack::PopAndDestroy( titleText ); // titleText
		}
	else
		{
		title->SetTextToDefaultL();
		}
	}	

void CCnvRatesForm::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
	{
	//No delete option  for more than two currencies
	const MDesCArray* names = iCategory.MdcArray();
	TInt count = names->MdcaCount();

	if (aResourceId == R_CNV_RATESFORM_MENUPANE)
		{

		if(count <= 2)
			{
			aMenuPane->SetItemDimmed(ECmdDeleteCurrency, ETrue);

			}
		else
			{
			aMenuPane->SetItemDimmed(ECmdDeleteCurrency, EFalse);
			}
		if(count >= KMaxNumberAddCurrency )
			{
			aMenuPane->SetItemDimmed(ECmdAddCurrency, ETrue);
			}
		else
			{
			aMenuPane->SetItemDimmed(ECmdAddCurrency, EFalse);
			}
		if (!FeatureManager::FeatureSupported(KFeatureIdHelp))
			{
			aMenuPane->SetItemDimmed(EAknCmdHelp, ETrue);
			}
		}
	}
//New methods which will imlement add and delete currency implementation
void CCnvRatesForm::AddNewCurrencyL()
	{	
	//_LIT(KDefaultText,"Foreign");
	//TBuf<16> newFieldPtr;
	//newFieldPtr.Format(KDefaultText);
	HBufC* foreignText = StringLoader::LoadLC( R_QTN_CNV_FOREIGN,iEikonEnv );
	TBuf<16> newFieldPtr;
	
	newFieldPtr.Copy(foreignText->Left(MAXCURRENCYNAMELENGTH));
	
	TBool nameEntered(EFalse);
	TCurrencyType type = 	NewCurrencyNameDialogL( newFieldPtr, nameEntered, ETrue );

	TInt id = IdOfFocusControl(); //Get the focus controls id
	TInt index = iFormIdMapArray.Find(id); //get the index where the id i
	TInt indexOfCurrency = index;
	TUint uintindex = indexOfCurrency;//To remove warnings

	if( uintindex  >= iCategory.BaseUnit())
		{
		indexOfCurrency++;
		}

	if(nameEntered)//Dont add any field if the user cancels the dialog
		{
		TReal real = 1.0;

		iCategory.AddNewCurrencyL( indexOfCurrency + 1, newFieldPtr, real, type);		

		//TInt newItemId = 0;
		TInt newItemId = iFormIdMapArray[0];

		//Get the largest formId present now in the form...
		for(TInt i = 0; i < iFormIdMapArray.Count() ; i++ )
			{		
			if( newItemId  < iFormIdMapArray[i])
				newItemId = iFormIdMapArray[i];
			}

		newItemId++;

		//Update the new FormId into the form id map array
		if( index + 1 < iFormIdMapArray.Count() )
			{
			iFormIdMapArray.Insert(newItemId,index + 1 );
			}
		else
			{
			iFormIdMapArray.Append(newItemId);
			}

		//Update the arrray which holds the CEikEdwin ids...
		iFormIdEdwinMapArray.Append(newItemId);


        CCoeControl* control = CreateLineByTypeL(newFieldPtr, ActivePageId(),
            newItemId , EEikCtFlPtEd, NULL ) ;
        CEikEdwin* edwin = STATIC_CAST( CEikEdwin*, control );

        CEikFloatingPointEditor* fpEditor = STATIC_CAST( 
        CEikFloatingPointEditor*, control );
        fpEditor->SetMinMax( 0, 9e99 );
        edwin->ConstructL( EEikEdwinNoHorizScrolling | EEikEdwinResizable, 10, 12, 10 );
        edwin->SetAknEditorNumericKeymap( EAknEditorConverterNumberModeKeymap );

        edwin->SetAknEditorAllowedInputModes( EAknEditorNumericInputMode );

        edwin->SetAknEditorInputMode( EAknEditorNumericInputMode );
        edwin->SetAknEditorSpecialCharacterTable( 0 );

        edwin->CreateTextViewL();
        Line( newItemId )->ActivateL();

        // Set the editor's value at last and use a number instead of the fixed character.
        SetFloatEditorValueL( IdOfFocusControl(), 1 );

		*iModelChanged = ETrue;
		}	
		CleanupStack::PopAndDestroy(); // foreignText HBufC
	}


void CCnvRatesForm::DeleteCurrencyL()
	{


	TInt LineForDeletion = IdOfFocusControl();

	TInt indexOfCurrency = iFormIdMapArray.Find(LineForDeletion);
	TUint index = indexOfCurrency; //To remove warnings


	if( index >= iCategory.BaseUnit())
		{
		indexOfCurrency++;
		}

	const MDesCArray* names = iCategory.MdcArray();

		TBuf<16> currentName;
	currentName.Copy( names->MdcaPoint( indexOfCurrency ).Left( MAXCURRENCYNAMELENGTH ) );

	HBufC* confirmDelete = StringLoader::LoadLC(
		R_QTN_CNV_CONFIRM_DELETE,currentName);

	CAknQueryDialog* dlg = CAknQueryDialog::NewL();
	dlg->SetPromptL( *confirmDelete);

	if( dlg->ExecuteLD( R_CNV_CONFIRMATION_QUERY ) )
		{	
		SetFormFlag( EDeleting, ETrue );

		if (LineForDeletion)
			{
			iCategory.DeleteCurrencyL( indexOfCurrency );
			DeleteLine(LineForDeletion);
			//SetFormFlag( EUnsavedEdit, ETrue );
			TInt index = iFormIdMapArray.Find(LineForDeletion);
			iFormIdMapArray.Remove(index);

			if( index  == iFormIdMapArray.Count())
				{
				iLastUsedEditorId = iFormIdMapArray[index - 1];
				}
			else
				{
				iLastUsedEditorId = iFormIdMapArray[index];
				}
			}

		DrawNow();
		SetFormFlag( EDeleting, EFalse );
		*iModelChanged = ETrue;
		}
	CleanupStack::PopAndDestroy();	// confirmDelete;
	}

//This function launches a dialog to get the new currency name.
//Note can also be used for Rename Currency, but will be made use, afte the  raised for that has been fixed
TCurrencyType  CCnvRatesForm::NewCurrencyNameDialogL( TDes& aField, TBool& aNameEntered,  TBool aIsAddNewCurrency )
	{	
	TBool nameOk( EFalse );	
	TCurrencyType type = ENameChanged;
  //TInt index = IdOfFocusControl() - 1;
  TInt controlId = IdOfFocusControl();
  TInt index = iFormIdMapArray.Find( controlId ); //get the index where the id i
  TUint uintindex = index ;                    //To remove warnings
    if ( uintindex  >= iCategory.BaseUnit() )     //get correct index
        {
        index++;
        }

	const MDesCArray* names = iCategory.MdcArray();
	do
		{
		CAknTextQueryDialog* dlg =
			new( ELeave ) CAknTextQueryDialog( aField );
		CleanupStack::PushL(dlg);

				if( dlg->ExecuteLD( R_CNV_UNITNAME_QUERY ) != EAknSoftkeyOk )
					{
					aNameEntered = EFalse;
					CleanupStack::Pop(dlg);	// dlg
					return type;
					}
		CleanupStack::Pop(dlg);	// dlg

		// Search through all the names to see if the new name already exists
		TInt len( names->MdcaCount() );
		TBool nameExists( EFalse );
        //Set the index larger than all when a new is added, make it compare with 
        //all of the item to judge if the name is already used
        if ( aIsAddNewCurrency )
            {
            index = len;
            }
		
		for( TInt i = 0; i < len; i ++ )
			{
			if( names->MdcaPoint( i ).Compare( aField ) == 0 )
				{
                if ( i != index )
                    {
				    nameExists = ETrue;
				    break;
					}
				}
			}

		if( nameExists )
			{
			// display error
			HBufC* label = StringLoader::LoadLC(
				R_QTN_FLDR_NAME_ALREADY_USED, aField );
			CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
			note->ExecuteLD( *label );
			CleanupStack::PopAndDestroy();	// label;
			}
		else
			{
			nameOk = ETrue;
			}
		}

	while ( !nameOk );
	
	HBufC* homeText = StringLoader::LoadLC( R_QTN_CNV_HOME,iEikonEnv );	
	HBufC* foreignText = StringLoader::LoadLC( R_QTN_CNV_FOREIGN,iEikonEnv );
	TBuf<16> defaultTextPtr;
	TBuf<16> homeTextPtr;
	TInt equal;
	
	defaultTextPtr.Copy(foreignText->Left(MAXCURRENCYNAMELENGTH));
	homeTextPtr.Copy(homeText->Left(MAXCURRENCYNAMELENGTH));
	equal = defaultTextPtr.Compare(aField);
	if(equal==0)
		type = EForeign;
	
	equal = homeTextPtr.Compare(aField);
	if(equal==0)
		type = EHome;

	aNameEntered = nameOk;
	CleanupStack::PopAndDestroy(); // foreignText HBufC
	CleanupStack::PopAndDestroy(); // HomeText HBufC		

	return type;
	}
//
//ShowInvalidValueNoteL(TInt aEditorId)
//
void CCnvRatesForm::ShowInvalidValueNoteL(TInt aEditorId, TBool aSetValue)
	{
	HBufC* label = iCoeEnv->AllocReadResourceLC(
		R_CNV_MAINFORM_INVALIDVALUE );
	CAknErrorNote* note = new( ELeave ) CAknErrorNote(ETrue);
	note->ExecuteLD( *label );
	CleanupStack::PopAndDestroy();	// label;
	if ( aSetValue )
		{
		SetFloatEditorValueL( aEditorId, 0.0 ); 
		}
	}

//
//IsVaildNumberEntered(TInt aEditorId)
//
TBool CCnvRatesForm::IsVaildNumberEntered(TInt aEditorId)
	{

	CEikFloatingPointEditor* editor =
		static_cast< CEikFloatingPointEditor* >( Control( aEditorId ) );

	if( editor->TextLength() == 0 )
		{
		return EFalse;
		}	

	TReal real;
	CAknNumericEdwin::TValidationStatus validationStatus = editor->GetValueAsReal(real);

  // The enter number is not valid if GetValueAsReal function's return value is not EValueValid 
	if( validationStatus != CAknNumericEdwin::EValueValid /* && validationStatus != CAknNumericEdwin::EValueTooLarge */ )
		{
		return EFalse;
		}
	return ETrue;

	}


TBool CCnvRatesForm::OkToExitL(TInt aButtonId)
	{
	//Check for validity of the rates entered
	TInt aControlId( IdOfFocusControl() );			
	if(!IsVaildNumberEntered(aControlId))
		{
		ShowInvalidValueNoteL(aControlId, ETrue );
		}
	//Check which one of the two buttons'Options' or 'MSK' is pressed
	if (aButtonId == EAknSoftkeyOptions && iEKeyOk)
		{
		//Make the variable False
		iEKeyOk = EFalse;
		return EFalse;
		}
	else
		// Call the Base Class OkToExitL()
		return CAknForm::OkToExitL(aButtonId);

	}
//
//OfferKeyEventL
//
TKeyResponse CCnvRatesForm::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
	
	// Make the variable 'iEKeyOk' to True if and only if MSK is pressed.
	if(aKeyEvent.iScanCode == EStdKeyDevice3 && aType == EEventKeyDown)
		{
		iEKeyOk = ETrue;
		}
	if(aKeyEvent.iScanCode == EStdKeyDevice0 && aType == EEventKeyDown && iEKeyOk)
		{
		iEKeyOk = EFalse;
		}

	
	TInt controlId1( IdOfFocusControl() );
	if(aType == EEventKeyDown)
	{		
		if((aKeyEvent.iScanCode == EStdKeyDevice0)
		 ||(aKeyEvent.iScanCode == EStdKeyDevice1)||(aKeyEvent.iScanCode == EStdKeyDevice3) )
			{			
			if(!IsVaildNumberEntered(controlId1))
				{				
				iError=ETrue;
				return EKeyWasConsumed;
				}
		   	 }
	 }	
			
	if(aType == EEventKeyUp && iError)	
	{
		if((aKeyEvent.iScanCode == EStdKeyDevice0)
		 ||(aKeyEvent.iScanCode == EStdKeyDevice1)||(aKeyEvent.iScanCode == EStdKeyDevice3))
			{
			ShowInvalidValueNoteL(controlId1, ETrue); 
			iError = EFalse;
			return EKeyWasConsumed;
			}
	}

    // Return EKeyWasNotConsumed if OfferKeyEventL() does not handle any event.	
    if ( aType != EEventKey )
        {
        return EKeyWasNotConsumed;
        }

    if ( ( aKeyEvent.iScanCode == EStdKeyDownArrow ) || ( aKeyEvent.iScanCode == EStdKeyUpArrow ) )
        {			
        TInt controlId( IdOfFocusControl() );			
        if ( !IsVaildNumberEntered( controlId ) )
            {
            ShowInvalidValueNoteL( controlId, ETrue);
            return EKeyWasConsumed;
            }
        }	
    return CAknForm::OfferKeyEventL( aKeyEvent, aType );
}

// ----------------------------------------------------
// CCnvRatesForm::IsDigit
// Converts the digit from any digit mode (devangiri or arabic) 
// to western digit and checks whether it is a valid digit or not.
// ----------------------------------------------------
//
TBool CCnvRatesForm::IsDigit(TUint aChar)
	{
	TBuf<1> digit;
	digit.Append(aChar);
	
	AknTextUtils::ConvertDigitsTo(digit , EDigitTypeWestern);
	
	TChar ch = digit[0];
	return ch.IsDigit();
	}


// ----------------------------------------------------
// CCnvRatesForm::PrepareForFocusTransitionL
// Used for touch UI to display the invalid note if number 
// is invalid
// ----------------------------------------------------
//
void CCnvRatesForm::PrepareForFocusTransitionL()
    {
    // Check if touch is enabled or not
    if ( !AknLayoutUtils::PenEnabled() )
        {
        CAknForm::PrepareForFocusTransitionL();
        return;
        }

    // Get the focus of the current control line
    TInt controlId( IdOfFocusControl() );
    // Check for validity of the rate entered
    if (!IsVaildNumberEntered( controlId ) )
        {
         ShowInvalidValueNoteL(controlId, EFalse);
        }

    // Delete the radix point if it is excrescent
    else
        {
        CEikFloatingPointEditor* editor = 
           static_cast<CEikFloatingPointEditor*> ( Control( controlId ) );
        TBuf< KEditorTextMaxLength > editorText;
        editor->GetText( editorText );
        TInt editorTextLength = editorText.Length();
        TLocale local;
        if ( editorText[ editorTextLength - 1 ] == local.DecimalSeparator() )
            {
            editorText.Delete( editorTextLength - 1 , 1 );
            editor->SetTextL( &editorText );
            }
        }
    CAknForm::PrepareForFocusTransitionL();
    }


// ----------------------------------------------------
// CCnvRatesForm::ActivateL
// Sets the control as ready to be drawn.
// ----------------------------------------------------
//    
void CCnvRatesForm::ActivateL()
    {
    CAknForm::ActivateL();
    CEikFloatingPointEditor* editor =
        static_cast< CEikFloatingPointEditor* >( Control( IdOfFocusControl() ) );
    editor->SetCursorPosL( editor->Text()->DocumentLength(), ETrue );
    }
//End of file


