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
*      This is the implementation of the class defined in CCnvMainForm.h
*
*/



#include <barsread.h>
#include <eikenv.h>
#include <eikappui.h>
#include <eikfpne.h>
#include <avkon.hrh>
#include <AknPopupField.h>
#include <aknnotewrappers.h>
#include <eikmenup.h>
#include <s32file.h>
#include <ErrorUI.h>
#include <featmgr.h>

#include <hlplch.h>
#include <csxhelp/cnv.hlp.hrh>

#include <CCnvConverter.h>
#include <TCnvCategory.h>
#include <Converter.rsg>
#include "Converter.hrh"
#include "CCnvApplication.h"
#include "CCnvMainForm.h"
#include "CCnvRatesForm.h"

#include <aknnumed.h> 
#include <f32file.h>
#include <e32std.h>

_LIT( KCnvPlusSign, "+" );
_LIT( KCnvMinusSign, "-" );
_LIT( KCnvExponentSign, "E" );
_LIT( KCnvDotSign, "." );

_LIT( KCnvMinusInTheMiddle, "?*-?*" );
_LIT( KCnvPlusInTheMiddle, "?*+?*" );
_LIT( KCnvExponentInTheMiddle, "?*E?*" );
_LIT( KCnvExponentInTheMiddleAndEnd, "?*E?*E?" );

	_LIT( KConverterFilename,"UnitConverter.ini");
	_LIT(KDelimiter, ":");

CCnvMainForm::CCnvMainForm( TBool &aAlreadyDead )
: iIsFullyConstructed( EFalse ),
iHasNotifiedDiscFull( EFalse ),
iLastUsedEditorId( EConverterEditor1 ),
iHistoryTypeCategory( -1 ),
iHistoryUnit1Category( -1 ),
iHistoryUnit2Category( -1 ),
iAlreadyDead( aAlreadyDead )
	{
	}

CCnvMainForm::~CCnvMainForm()
	{
	if( iIsFullyConstructed )
		{
		SaveState();
		}
	delete iModel;
	delete iConversionTimer;
	delete iErrorUI;
	iAlreadyDead = ETrue;

	//remove the edwin observers
	CEikFloatingPointEditor* editor1 =
		static_cast< CEikFloatingPointEditor* >( Control( EConverterEditor1 ) );
	editor1->RemoveEdwinObserver(this);

	CEikFloatingPointEditor* editor2 =
		static_cast< CEikFloatingPointEditor* >( Control( EConverterEditor2 ) );	
	editor2->RemoveEdwinObserver(this);

	}

void CCnvMainForm::ConstructL()
	{
	CAknDialog::ConstructL( R_CNV_MAINFORM_MENUBAR );

	iErrorUI = CErrorUI::NewL( *iCoeEnv );

	TResourceReader reader;
	iEikonEnv->CreateResourceReaderLC( reader, R_CNV_MODEL );
	iModel = CCnvConverter::NewL( reader );
	CleanupStack::PopAndDestroy(); // reader

	iConversionTimer = CPeriodic::NewL( CActive::EPriorityStandard );

	iCurrencyCategoryId = iModel->CurrencyCategoryId();
	iIsFullyConstructed = ETrue;
	isEdwinUpdateText = EFalse;
	
		TFileName path;
    	iEikonEnv->FsSession().PrivatePath(path);
    	iEikonEnv->FsSession().MkDirAll(path);
  
	}

void CCnvMainForm::GetHelpContext( TCoeHelpContext& aContext ) const
	{
	aContext.iMajor = KUidCnv;
	aContext.iContext = KCNV_HLP_MAIN_STATE;
	}

// Handle commands from Options menu
void CCnvMainForm::ProcessCommandL( TInt aCommandId )
	{
    // this hides the options menu etc.
    CAknForm::ProcessCommandL( aCommandId );

	switch( aCommandId )
		{
		case ECmdSetType:
			{
			CmdSetTypeL();
			break;
			}

		case ECmdSetUnit:		// FLOWTHROUGH
		case ECmdSetCurrency:
			{
			CmdSetUnitL();
			break;
			}

		case ECmdRates:
			{
			CmdRatesL();
			break;
			}
		case EAknCmdHelp:
			{
			iEikonEnv->EikAppUi()->HandleCommandL( EAknCmdHelp );
			break;
			}
		case EEikCmdExit:
			{
			OkToExitL( EAknSoftkeyExit );
			break;
			}

		default:
			{
			break;
			}
		}
	}

// Takes any action required when the current line is changed to aControlId.
void CCnvMainForm::LineChangedL( TInt aControlId )
	{

		CEikButtonGroupContainer& cba = ButtonGroupContainer();
		cba.SetCommandSetL(R_CNV_MAINFORM_SOFTKEYS);
		
	if( ( aControlId == EConverterEditor1 ) ||
		( aControlId == EConverterEditor2 ) )
		{
		CEikFloatingPointEditor* editor =
			static_cast< CEikFloatingPointEditor* >( Control( aControlId ) );

		// don't clear the selection if there is only '0' in the editor
		TBuf<KEditorTextMaxLength> editorText;
		editor->GetText(editorText);

		SynchronousConversionL();
		//        iActiveEditorChanged = EFalse;
		}
	else if(aControlId == EConverterUnit1Popup || 
		aControlId == EConverterUnit2Popup)
		{
			
									
		// if focus is moved to unit popup from amount field
		// then try to do the conversion.
		// IsEditorValueValidL shows invalid value note if value is not valid
		TBool editorValueValid = IsEditorValueValidL(iLastUsedEditorId);
		if (editorValueValid && AreRatesValidL(ETrue))
			{
			SynchronousConversionL();
			}
		}
	if(aControlId ==  EConverterUnit1Popup || 
		aControlId == EConverterUnit2Popup || 
		aControlId == EConverterTypePopup)
		{
		//Makes the MSK visible
	
			cba.MakeCommandVisible(EAknSoftkeyChange,ETrue);
	
				
		}
		else if(aControlId ==EConverterEditor1 ||
				aControlId ==EConverterEditor2) 
		{
			
		
		//makes the MSK invisible
	
		cba.MakeCommandVisible(EAknSoftkeyChange,EFalse);

		}
	 cba.DrawNow();
	}

void CCnvMainForm::DynInitMenuPaneL( TInt aResourceId,
                                    CEikMenuPane* aMenuPane )
    {
    // When click on LSK(option), the invalid value should be set to 0.
    TInt controlId( IdOfFocusControl() );
    if ( ( controlId == EConverterEditor1 ) || ( controlId == EConverterEditor2 ) )
    	  {
        if ( EditorValueCausesHold( controlId ) || !IsVaildNumberEntered( controlId ) )
            {
            SetFloatEditorValueL( controlId, 0.0 );
            }
        SynchronousConversionL( EFalse );
    	}

    TUint currentType( iTypeCategory.CurrentValueIndex() );

	if( aResourceId == R_CNV_MAINFORM_MENUPANE )
		{
		if (!FeatureManager::FeatureSupported(KFeatureIdHelp))
			{
			aMenuPane->SetItemDimmed(EAknCmdHelp, ETrue);
			}

		if( currentType == iCurrencyCategoryId )
			{
			aMenuPane->SetItemDimmed( ECmdRates, EFalse ) ;
			}

		switch( IdOfFocusControl() )
			{
			case EConverterUnit1Popup:
			case EConverterUnit2Popup:
				{
				if( currentType == iCurrencyCategoryId )
					{
					aMenuPane->SetItemDimmed( ECmdSetCurrency, EFalse ) ;
					}
				else
					{
					aMenuPane->SetItemDimmed( ECmdSetUnit, EFalse ) ;
					}
				break;
				}
			default:
				{
				break;
				}
			}
		if ( iTypePopupCtrl->SelectionMode() ==  CAknPopupField::EAknPopupFieldSelectionListMode ||
				iUnit1PopupCtrl->SelectionMode() == CAknPopupField::EAknPopupFieldSelectionListMode ||
				iUnit2PopupCtrl->SelectionMode() == CAknPopupField::EAknPopupFieldSelectionListMode)
			{
			aMenuPane->SetItemDimmed( ECmdSetCurrency, ETrue );
			aMenuPane->SetItemDimmed( ECmdSetType, ETrue );
			aMenuPane->SetItemDimmed( ECmdSetUnit, ETrue );
			aMenuPane->SetItemDimmed( ECmdRates, ETrue );
			}
		}
	}

// Handles events to the three pop lists
void CCnvMainForm::HandleControlEventL( CCoeControl* /*aControl*/,
									   TCoeEvent aEventType )
	{
	// We are interested only about EEventStateChanged events
	if( aEventType != EEventStateChanged )
		{
		return;
		}

	TInt controlId( IdOfFocusControl() );

	if( !HasReallyChanged( controlId ) )
		{
		return;
		}

	switch( controlId )
		{
		case EConverterTypePopup:
			{
			HandleCategoryChangedL();
			break;
			}
		case EConverterUnit1Popup:
		case EConverterUnit2Popup:
			{
			HandleUnitChangedL();
			break;
			}
		//These have been moved to OfferKeyEventL
		case EConverterEditor1:
		case EConverterEditor2:
			{
			iLastUsedEditorId = controlId;
			if( (iActiveEditorChanged)&&
				( isEdwinNavigated == EFalse || isEdwinUpdateText )  ) // Is there any navigation (or selection) 
				//happening on the editor
				{
				AsynchronousConversion();
				}
			isEdwinNavigated = EFalse; //make the boolean false
			iActiveEditorChanged = ETrue;
			break;
			}

		default:
			{
			break;
			}
		}
	}

TBool CCnvMainForm::SaveFormData()
	{
	// Always allow exit (data is saved in the destructor)
	return ETrue;
	}

TBool CCnvMainForm::OkToExitL( TInt aButtonId )
	{
	TBool status( CAknDialog::OkToExitL( aButtonId ) );
	TInt controlId( IdOfFocusControl() );
	if( status && ( aButtonId == EAknSoftkeyExit ) )
		{
		iEikonEnv->EikAppUi()->HandleCommandL( EAknCmdExit );
		}
	else if(aButtonId == EAknSoftkeyChange)
		{
		if (controlId == EConverterTypePopup)
		{
		CmdSetTypeL();
		status = EFalse;
		}
	else if ((controlId == EConverterUnit1Popup) ||(controlId == EConverterUnit2Popup))
		{
		CmdSetUnitL();
		status = EFalse;
		}
		}
	else
		{
		status = EFalse;
		}
	return status;
	}

void CCnvMainForm::PreLayoutDynInitL()
	{
	// Store pointers to the popup list controls
	// (makes further access easier/faster)
	iTypePopupCtrl =
		static_cast< CAknPopupField* >( Control( EConverterTypePopup ) );
	iUnit1PopupCtrl =
		static_cast< CAknPopupField* >( Control( EConverterUnit1Popup ) );
	iUnit2PopupCtrl =
		static_cast< CAknPopupField* >( Control( EConverterUnit2Popup ) );


	// Set values for the unit category popup control
	iModel->GetCategorylistAccessor( iTypeCategory );
	iTypePopupCtrl->SetQueryValueL( &iTypeCategory );

	// Load user selections from file (if any)
	LoadStateL();

	// Set initial values for the popup lists
	HandleCategoryChangedL();

	InitialiseChangeHistory();

	if (iUnit1Category.UnitFactor(iUnit1Category.CurrentValueIndex()) != 0.0 &&
		iUnit2Category.UnitFactor(iUnit2Category.CurrentValueIndex()) != 0.0)
		{
		SynchronousConversionL();
		}
	
	//Add wdwin observers for the two editors so that they can capture the Navigation 
	//events and not do asynchronous conversion
	CEikFloatingPointEditor* editor1 =
		static_cast< CEikFloatingPointEditor* >( Control( EConverterEditor1 ) );	

	CEikFloatingPointEditor* editor2 =
		static_cast< CEikFloatingPointEditor* >( Control( EConverterEditor2 ) );	

	editor1->AddEdwinObserverL(this);
	editor2->AddEdwinObserverL(this);


	}

TBool CCnvMainForm::HasReallyChanged( TInt controlId )
	{
	TBool changed( EFalse );

	switch( controlId )
		{
        case EConverterTypePopup:
            {
            if( iTypeCategory.CurrentValueIndex() != iHistoryTypeCategory )
                {
                changed = ETrue;
                iHistoryTypeCategory = iTypeCategory.CurrentValueIndex();

                // renew iHistoryUnit1Category and iHistoryUnit2Category
                iModel->GetCategoryAccessor( iUnit1Category, iHistoryTypeCategory );
                TUint HistoryUnit1Category;
                TUint HistoryUnit2Category;
                iUnit1Category.GetDefaultUnits( HistoryUnit1Category , HistoryUnit2Category );
                iHistoryUnit1Category = HistoryUnit1Category;
                iHistoryUnit2Category = HistoryUnit2Category;
                }
            break;
			}
		case EConverterUnit1Popup:
			{
			if( iUnit1Category.CurrentValueIndex() != iHistoryUnit1Category )
				{
				changed = ETrue;
				iHistoryUnit1Category = iUnit1Category.CurrentValueIndex();
				}
			break;
			}
		case EConverterUnit2Popup:
			{
			if( iUnit2Category.CurrentValueIndex() != iHistoryUnit2Category )
				{
				changed = ETrue;
				iHistoryUnit2Category = iUnit2Category.CurrentValueIndex();
				}
			break;
			}

		case EConverterEditor1:
		case EConverterEditor2:
			{
			changed = ETrue;
			break;
			}

		default:
			{
			break;
			}
		}

	return changed;
	}

void CCnvMainForm::InitialiseChangeHistory()
	{
	iHistoryTypeCategory = iTypeCategory.CurrentValueIndex();
	iHistoryUnit1Category = iUnit1Category.CurrentValueIndex();
	iHistoryUnit2Category = iUnit2Category.CurrentValueIndex();
	}

void CCnvMainForm::HandleCategoryChangedL()
	{
	TInt categoryId = iTypeCategory.CurrentValueIndex();
	iModel->GetCategoryAccessor( iUnit1Category, categoryId );
	iModel->GetCategoryAccessor( iUnit2Category, categoryId );

	TUint srcUnit, dstUnit;
	iUnit1Category.GetDefaultUnits( srcUnit, dstUnit );
	iUnit1Category.SetCurrentValueIndex( srcUnit );
	iUnit2Category.SetCurrentValueIndex( dstUnit );

	// Set arrays to popup lists and redraw them
	iUnit1PopupCtrl->SetQueryValueL( &iUnit1Category );
	iUnit2PopupCtrl->SetQueryValueL( &iUnit2Category );
	iUnit1PopupCtrl->DrawNow();
	iUnit2PopupCtrl->DrawNow();

	// Reset amount fields
	TReal zero( 0.0 );
	SetFloatingPointEditorValueL( EConverterEditor1, &zero );
	SetFloatingPointEditorValueL( EConverterEditor2, &zero );

    // when category's type is temperature the editor's value can't simply set to zero
	AsynchronousConversion();
    
	iLastUsedEditorId = EConverterEditor1;
	}

void CCnvMainForm::HandleUnitChangedL()
	{
	iUnit1Category.SetDefaultUnits( iUnit1Category.CurrentValueIndex(),
		iUnit2Category.CurrentValueIndex() );
	SynchronousConversionL();
	}

TBool CCnvMainForm::AreRatesValidL(TBool aShowNote)
	{
	TInt numInvalid( 0 );
	if( iUnit1Category.UnitFactor( iUnit1Category.CurrentValueIndex() )
		== 0.0 )
		{
		numInvalid++;
		SetFloatEditorValueL( EConverterEditor1, 0.0 );
		}
	if( iUnit2Category.UnitFactor( iUnit2Category.CurrentValueIndex() )
		== 0.0 )
		{
		numInvalid++;
		SetFloatEditorValueL( EConverterEditor2, 0.0 );
		}

	if(numInvalid)
		{
		CancelConversion();

		if (aShowNote)
			{
			TBool isPlural( EFalse );
			if( numInvalid > 1 )
				{
				isPlural = ETrue;
				}

			TInt resourceId(R_CNV_MAINFORM_NORATE );
			if( isPlural )
				{
				resourceId = R_CNV_MAINFORM_NORATE_PLURAL;
				}
			HBufC* label = iCoeEnv->AllocReadResourceLC( resourceId );
			CAknErrorNote* note = new( ELeave ) CAknErrorNote;
			note->ExecuteLD( *label );
			CleanupStack::PopAndDestroy();	// label;
			}
		return EFalse;
		}
	else
		{
		return ETrue;
		}
	}

void CCnvMainForm::ShowInvalidValueNoteL(TInt aEditorId)
	{
	CancelConversion();
	
	SetFloatEditorValueL( aEditorId, 0.0 );
	
	HBufC* label = iCoeEnv->AllocReadResourceLC(
		R_CNV_MAINFORM_INVALIDVALUE );
	CAknErrorNote* note = new( ELeave ) CAknErrorNote;
	note->ExecuteLD( *label );
	CleanupStack::PopAndDestroy();	// label;

	SynchronousConversionL();
	}

TBool CCnvMainForm::IsEditorValueValidL( TUint aEditorId )
	{
	TReal amount( FloatingPointEditorValue( aEditorId ) );

	TReal minimum;
	if( aEditorId == EConverterEditor1 )
		{
		minimum = iUnit1Category.UnitMinimum(
			iUnit1Category.CurrentValueIndex() );
		}
	else
		{
		minimum = iUnit2Category.UnitMinimum(
			iUnit2Category.CurrentValueIndex() );
		}

	if( amount < minimum )
		{
		ShowInvalidValueNoteL(aEditorId);
		return EFalse;
		}
	else
		{
		return ETrue;
		}
	}

TBool CCnvMainForm::EditorValueCausesHold( TUint aEditorId )
	{
	CEikFloatingPointEditor* editor =
		static_cast< CEikFloatingPointEditor* >( Control( aEditorId ) );	

	if( editor->TextLength() == 0 )
		{
		return ETrue;
		}

    TBuf< 12 > content;
    TLocale locale;

    // Not all the countries use '.' to express radix point, some use ','.
    // So use locale.DecimalSeparator() to get local symbol.
    TBuf<1> decimal;
    decimal.Append( locale.DecimalSeparator() );
    editor->Text()->Extract( content, 0, 12 );
    TInt contentLength( content.Length() );
    if ( !content.Compare( KCnvPlusSign ) )
        {
        return ETrue;
        }
    else if ( !content.Compare( KCnvMinusSign ) )
        {
        return ETrue;
        }

    else if ( !content.Compare( decimal ) )
        {
        return ETrue;
        }
	else if( contentLength > 1 && 
		content[ contentLength - 1 ] == KCnvExponentSign()[ 0 ] &&
		IsDigit( content[ contentLength - 2 ] ) &&
		content.Match( KCnvExponentInTheMiddle ) == KErrNotFound &&
		content.Match( KCnvMinusInTheMiddle ) == KErrNotFound &&
		content.Match( KCnvPlusInTheMiddle ) == KErrNotFound)
		{
		// If content ends with <digit>E with no minus, plus or exponent
		// sign in the middle
		return ETrue;
		}
	else if( contentLength > 2 &&
		( content[ contentLength - 1 ] == KCnvMinusSign()[ 0 ] ||
		content[ contentLength - 1 ] == KCnvPlusSign()[ 0 ] ) &&
		content[ contentLength - 2 ] == KCnvExponentSign()[ 0 ] &&
		IsDigit( content[ contentLength - 3 ] ) &&
		content.Match( KCnvExponentInTheMiddleAndEnd ) == KErrNotFound &&
		content.Match( KCnvMinusInTheMiddle ) == KErrNotFound &&
		content.Match( KCnvPlusInTheMiddle ) == KErrNotFound )
		{
		// If content ends with <digit>E[+|-] with no minus, plus or exponent
		// sign in the middle
		return ETrue;
		}
	else if(contentLength > 12 &&
		content.Locate(locale.DecimalSeparator()) == contentLength - 1)
		{
		// content ends with decimal separator and there is no other
		// decimal separator
		return ETrue;
		}


	return EFalse;
	}

void CCnvMainForm::CmdRatesL()
	{
	TBool wantsRelaunch;
	TBool shouldExit( EFalse );
	TBool needsSave( EFalse );
	do
		{
		wantsRelaunch = EFalse;
		TCnvCategory currencies;
		iModel->GetCategoryAccessor( currencies, iCurrencyCategoryId );

		CCnvRatesForm* rates = new( ELeave ) CCnvRatesForm( &wantsRelaunch,
			&shouldExit, &needsSave, currencies);

		CleanupStack::PushL( rates );
		rates->ConstructL();
		CleanupStack::Pop(); // rates
		rates->ExecuteLD( R_CNV_RATESFORM );
		}
	while( wantsRelaunch );

	if( needsSave )
		{
		SaveState();
		}

	if( !shouldExit )
		{
		// Reset the form (there might be changes in unit names..)
		HandleCategoryChangedL();
		
		DrawNow();
		}
	else
		{
		TryExitL( EAknSoftkeyExit );
		}
	}

void CCnvMainForm::CmdSetTypeL()
	{
	TryChangeFocusToL( EConverterTypePopup );
	iTypePopupCtrl->ActivateSelectionListL();
	}

void CCnvMainForm::CmdSetUnitL()
	{
	switch ( IdOfFocusControl() )
		{
		case EConverterUnit1Popup:
			{
			iUnit1PopupCtrl->ActivateSelectionListL();							
			break;
			}

		case EConverterUnit2Popup:
			{
			iUnit2PopupCtrl->ActivateSelectionListL();
			break;
			}

		default:
			{
			break;
			}
		}
	}

void CCnvMainForm::LoadStateL()
	{
	RFileReadStream in;
	
		TFileName filePath;
		TBuf<1> tempDes;
		TChar driveChar;
    	User::LeaveIfError(iEikonEnv->FsSession().PrivatePath(filePath));
    	iEikonEnv->FsSession().DriveToChar(KDefaultDrive, driveChar);
    	tempDes.Append(driveChar);
    	filePath.Insert(0,KDelimiter);
    	filePath.Insert(0,tempDes);
    	filePath.Append(KConverterFilename);
  		
		TInt err( in.Open( iEikonEnv->FsSession(), filePath,
			EFileRead ) );
	
	if( !err )
		{
		TRAP( err, iModel->InternalizeL( in ) );
		if( !err )
			{
			TRAP( err, iTypeCategory.SetCurrentValueIndex( in.ReadInt8L() ) );
			}
		in.Close();
		}
	}

void CCnvMainForm::SaveState()
	{
	RFileWriteStream out;
	
		TFileName filePath;
		TBuf<1> tempDes;
		TChar driveChar;
    	TRAPD(errd, iEikonEnv->FsSession().PrivatePath(filePath));
    	iEikonEnv->FsSession().DriveToChar(KDefaultDrive, driveChar);
    	tempDes.Append(driveChar);
    	if(!errd)
    	{
    		
    	filePath.Insert(0,KDelimiter);
    	filePath.Insert(0,tempDes);
    	filePath.Append(KConverterFilename);
    	}
    	
		TInt err( out.Replace( iEikonEnv->FsSession(), filePath,
			EFileWrite ) );
	
	if( !err )
		{
		TRAP( err, iModel->ExternalizeL( out ) );
		if( !err )
			{
			TRAP( err, out.WriteInt8L( iTypeCategory.CurrentValueIndex() ) );
			}
		out.Close();
		}

	if( err && !iHasNotifiedDiscFull )
		{
		TRAP_IGNORE(iErrorUI->ShowGlobalErrorNoteL( err ) );
		iHasNotifiedDiscFull = ETrue;
		}
	}

void CCnvMainForm::SynchronousConversionL( TBool aShowRateInvalideNote )
    {
    // Don't report an "invalid value" error. Cancel conversion.
    // if the field is empty (i.e. the user has used delete/backspace) etc.
    if( EditorValueCausesHold( iLastUsedEditorId ) )
        {
        // if one Editor is blank, set the other to 0.
        CEikFloatingPointEditor* editor =
        		static_cast< CEikFloatingPointEditor* >( Control( iLastUsedEditorId ) );
    	if( editor->TextLength() == 0 )
    		{
            if( iLastUsedEditorId == EConverterEditor1)
            	{
            	SetFloatEditorValueL( EConverterEditor2, 0.0 );
            	}
            if( iLastUsedEditorId == EConverterEditor2)
            	{
            	SetFloatEditorValueL( EConverterEditor1, 0.0 );
            	}
    		}
        return;
        }

    if ( !IsVaildNumberEntered( iLastUsedEditorId ) )
        {
        ShowInvalidValueNoteL( iLastUsedEditorId );
        }


    // PrepareForFocusLossL leaves if the component is not able to
    // parse the content. We trap it so that IsEditorValueValidL()
    // can check and report the situation correctly.
    TRAP_IGNORE( Control( iLastUsedEditorId )->PrepareForFocusLossL() );
    TInt CurrentControlId( IdOfFocusControl() );
    if( CurrentControlId == EConverterUnit1Popup )
    	{
    	IsEditorValueValidL( EConverterEditor1 );
    	}
    else if (CurrentControlId == EConverterUnit2Popup)
    	{
    	IsEditorValueValidL( EConverterEditor2 );
    	}
    else if (CurrentControlId == EConverterEditor2 )
        	{
        	if ( !IsVaildNumberEntered( EConverterEditor2 ) )
        		{
        		iLastUsedEditorId = CurrentControlId;
        		ShowInvalidValueNoteL( EConverterEditor2 );
        		}
        	}
    else if (CurrentControlId == EConverterEditor1 )
        	{
        	if ( !IsVaildNumberEntered( EConverterEditor1 ) )
        		{
        		iLastUsedEditorId = CurrentControlId;
        		ShowInvalidValueNoteL( EConverterEditor1 );
        		}
        	}
    TReal result( 0.0 );

    // send an argument to notify the AreRatesValidL() display a note dialog or not
    if (IsEditorValueValidL(iLastUsedEditorId))
		{
		if (AreRatesValidL(aShowRateInvalideNote))
			{
			TReal amount(FloatingPointEditorValue(iLastUsedEditorId));
			TUint unit1(iUnit1Category.CurrentValueIndex());
			TUint unit2(iUnit2Category.CurrentValueIndex());

			//        RAPD( ignored, Control( iLastUsedEditorId )->PrepareForFocusLossL() );


			// do not reset the touch UI to standby state
			// CCoeEnv::Static()->SyncNotifyFocusObserversOfChangeInFocus();


			// Do the calculation and change the value for the related edit field      
			ValueCalculationL(unit1, unit2, amount);

			CEikFloatingPointEditor* editor =
					static_cast<CEikFloatingPointEditor*> (Control(
							iLastUsedEditorId));
			TBuf<KEditorTextMaxLength> editorText;
			editor->GetText(editorText);

			// Use local value for different area
			TBuf<1> localeZero;
			localeZero.AppendNum(0);
			AknTextUtils::LanguageSpecificNumberConversion(localeZero);
			if (editorText.Length() > 1 && editorText[0] == localeZero[0])
				{
				TCursorSelection sel = editor->Selection();
				TInt removedZeroes = 0;
				// remove all leading zeroes
				while (editorText.Length() > 0 && editorText[0]
						== localeZero[0])
					{
					editorText.Delete(0, 1);
					++removedZeroes;
					}
				// if too many zeroes removed, insert one.
				TLocale locale;
				if (editorText.Length() == 0 || TChar(editorText[0])
						== locale.DecimalSeparator())
					{
					editorText.Insert(0, localeZero);
					--removedZeroes;
					}
				editor->SetTextL(&editorText);

				// adjust the selection accroding to removed zeroes
				TInt anchor =
						(sel.iAnchorPos - removedZeroes > 0) ? sel.iAnchorPos
								- removedZeroes : 0;
				TInt cursor =
						(sel.iCursorPos - removedZeroes > 0) ? sel.iCursorPos
								- removedZeroes : 0;
				editor->SetSelectionL(cursor, anchor);
            
            if ( !IsVaildNumberEntered( iLastUsedEditorId ) )
                   {
                   ShowInvalidValueNoteL( iLastUsedEditorId );
                   }
				// notify the touch window
				editor->SetCursorPosL(cursor, ETrue);
				editor->DrawNow();
				}
			}
		}
    }

void CCnvMainForm::AsynchronousConversion()
	{
	CancelConversion();
	TCallBack cb( TimerCallback, this );
	iConversionTimer->Start( 750000, 0, cb );
	}

void CCnvMainForm::CancelConversion()
	{
	iConversionTimer->Cancel();
	}

TInt CCnvMainForm::TimerCallback( TAny* aMainForm )
	{

	CCnvMainForm* form = reinterpret_cast< CCnvMainForm* >( aMainForm );

	TRAP_IGNORE(form->SynchronousConversionL() );

	form->CancelConversion();

	return KErrNone;
	}



// ----------------------------------------------------
// CCnvMainForm::OfferKeyEventL
// Keyevent process.
// ----------------------------------------------------
//
TKeyResponse CCnvMainForm::OfferKeyEventL(const TKeyEvent& aKeyEvent,
										  TEventCode aType)
	{

	if ( !IsFocused() )
	{
	   if (aKeyEvent.iCode == EKeyEscape)
		   {
		   return CAknForm::OfferKeyEventL( aKeyEvent, aType );
		   }
		return EKeyWasNotConsumed;
	}
	
	TInt controlId( IdOfFocusControl() );

	TBool hashKeyPressed = EFalse;
  
    // Use decimalChar instead of 46
    TLocale locale;
    TChar decimalChar = locale.DecimalSeparator();
    if ( aKeyEvent.iScanCode == EStdKeyHash || decimalChar == TChar( aKeyEvent.iCode ) )
        { 
        hashKeyPressed = ETrue;
        }
		
	if( hashKeyPressed )
		{
            // If inputting through VKB or HWR, the event can be done only when aType is equal to EEventKey.
            // Besides, only when inputting the dot in VKB or HWR, aKeyEvent.iScanCode is equal to decimalChar.
            TBool VKBdotKeyPressed = EFalse; 
            if ( ( aKeyEvent.iScanCode == decimalChar ) )
            {
                VKBdotKeyPressed = ETrue;
            }
            if ( ( aType == EEventKey) && ( !VKBdotKeyPressed ) )
			{
				return EKeyWasConsumed;
			}
			else if( aType == EEventKeyUp)
			{
				TKeyEvent keyevent( aKeyEvent );
				aType = EEventKey;
                keyevent.iCode = decimalChar;
				keyevent.iModifiers = 0;
				keyevent.iScanCode = 0;
				keyevent.iRepeats = 0;
				return  static_cast< CEikFloatingPointEditor* >( Control( controlId ) )
				->OfferKeyEventL(keyevent,aType);
			}
			else
			{//
			}
		}
	
	//Not a key event this case handles the menu and exit keys 
	//when an invalid value is entered in the amount field
	if(aType == EEventKeyDown) 
		{
		if( ( EConverterEditor1 == controlId )||
			( EConverterEditor2 == controlId ) )
			{

			if( (aKeyEvent.iScanCode == EStdKeyDevice0) ||//menu Key
				(aKeyEvent.iScanCode == EStdKeyDevice1) )//Exit key
				{
				if( EditorValueCausesHold( controlId ) ||
					!IsVaildNumberEntered( controlId ) )
					{
					SetFloatEditorValueL( controlId, 0.0);
					}
				//return EKeyWasNotConsumed;
				}
			}
		}

    if ( aType != EEventKey )
        {
        // Return EKeyWasNotConsumed if OfferKeyEventL() does not handle any event.	
        return EKeyWasNotConsumed;
        } 	

	if( ( EConverterEditor1 == controlId )||
		( EConverterEditor2 == controlId ) )
		{	
		//Just to see that -9E-99 doesnt appear when an invalid number is editor is 
		//entered in the editor		

		if(aKeyEvent.iScanCode == EStdKeyUpArrow ||
			aKeyEvent.iScanCode == EStdKeyDownArrow )
			{
			CancelConversion();
			if( EditorValueCausesHold( controlId ) )
				{
				SetFloatEditorValueL( controlId, 0.0);
				}
			if(	!IsVaildNumberEntered( controlId ) )
				{
				ShowInvalidValueNoteL( controlId );
				}		
			}

		iLastUsedEditorId = controlId;

		//For arrow keys asynchronous conversion not required
		// if (iActiveEditorChanged)
		// {
		/*if( ( aKeyEvent.iScanCode != EStdKeyRightArrow )&&
		( aKeyEvent.iScanCode != EStdKeyLeftArrow )&&
		( aKeyEvent.iScanCode != EStdKeyUpArrow )&&
		( aKeyEvent.iScanCode != EStdKeyDownArrow )&&
		( isEditKey == EFalse ) )
		{
		AsynchronousConversion();			
		}*/
		//}

		//iActiveEditorChanged = ETrue;

		}
	return CAknForm::OfferKeyEventL(aKeyEvent, aType);

	}


// ----------------------------------------------------
// CCnvMainForm::IsVaildNumberEntered
// Checks if a valid number is entered in the editor field and returns
// ETrue if a vaild number is entered and
// EFalse if an invalid number is entered
// ----------------------------------------------------
//

TBool CCnvMainForm::IsVaildNumberEntered( TInt aEditorId )
    {
    CEikFloatingPointEditor* editor =
        static_cast< CEikFloatingPointEditor* >( Control( aEditorId ) );

    if ( editor->TextLength() == 0 )
        {
        return EFalse;
        }	

    TReal real;
    CAknNumericEdwin::TValidationStatus validationStatus = editor->GetValueAsReal( real );

    // The enter number is not valid if GetValueAsReal function's return value is not EValueValid 
    if ( validationStatus != CAknNumericEdwin::EValueValid /*&& validationStatus != CAknNumericEdwin::EValueTooLarge*/ )
        {
        return EFalse;
        }
    return ETrue;
    }



// ----------------------------------------------------
// CCnvMainForm::HandleEdwinEventL
// Checks the Navigation events on the editor 
// ----------------------------------------------------
//

void CCnvMainForm::HandleEdwinEventL(CEikEdwin* /*aEdwin*/,TEdwinEvent aEventType)
	{
	if(EEventNavigation == aEventType)
		{
		isEdwinNavigated = ETrue;
		}
	 if( aEventType == EEventTextUpdate )
			{
			isEdwinUpdateText = ETrue;
			}
	
	}

// ----------------------------------------------------
// CCnvMainForm::IsDigit
// Converts the digit from any digit mode (devangiri or arabic) 
// to western digit and checks whether it is a valid digit or not.
// ----------------------------------------------------
//
TBool CCnvMainForm::IsDigit(TUint aChar)
    {
    TBuf<1> digit;
    digit.Append(aChar);
    
    AknTextUtils::ConvertDigitsTo(digit , EDigitTypeWestern);
    
    TChar ch = digit[0];
    return ch.IsDigit();
    }
    
// ----------------------------------------------------
// CCnvMainForm::PrepareForFocusTransitionL
// The Uikon framework calls this function immediately before focus move.
// ----------------------------------------------------
//    
void CCnvMainForm::PrepareForFocusTransitionL() 
    {
    TInt controlId( IdOfFocusControl() );
    if ( ( EConverterEditor1 == controlId ) || ( EConverterEditor2 == controlId ) )
        {

        // record the last use editor id
        iLastUsedEditorId = controlId;
        CancelConversion();
        if ( EditorValueCausesHold( controlId ) )
            {
            SetFloatEditorValueL( controlId, 0.0 );
            }

        // Delete the radix point when there is no number behind it.
        else if ( IsVaildNumberEntered( controlId ) )
            {
            CEikFloatingPointEditor* editor =
                static_cast<CEikFloatingPointEditor*> ( Control( controlId ) ); 
            TBuf<KEditorTextMaxLength> editorText;
            editor->GetText( editorText );
            TLocale locale;
            TInt editorLength = editorText.Length();

            // Check if the last character is radix point.
            if ( editorText[ editorLength - 1 ] == locale.DecimalSeparator() )
                {
                editorText.Delete( editorLength - 1, 1 );
                editor->SetTextL( &editorText );
                }
            }
        }
    }

// ----------------------------------------------------
// CCnvMainForm::ValueCalculation
// Do the calculation and change the value for the related edit field
// ----------------------------------------------------
//        
void CCnvMainForm::ValueCalculationL( TUint aUnit1Popup, TUint aUnit2Popup, TReal aMount )
    {
    TReal result( 0.0 );
    // Use "LastUnit1" and "LastUnit2" to insure that the second amount don't change if the last used editor 
    // is the second and the category's units do not change.
    static TInt LastUnit1 = -1;
    static TInt LastUnit2 = -1;
    
    // According to the UI Spec:
    // 1. If the first conversion unit field is changed, a new conversion is calculated to the 
    // second amount field of units, or vice versa.
    // 2. If the first edit field is changed, a new conversion is calculated to the second 
    // edit field, or vice versa.
    if ( iLastUsedEditorId == EConverterEditor1 )
        {
        //the first unit is changed
        if ( ( ( IdOfFocusControl() != EConverterEditor1 ) && ( LastUnit1 != aUnit1Popup ) )
         || ( IdOfFocusControl() == EConverterTypePopup ) )
            {
            aMount = FloatingPointEditorValue( EConverterEditor1 );
            result = iUnit1Category.Convert( aUnit1Popup, aUnit2Popup, aMount );
            SetFloatEditorValueL( EConverterEditor2, result );
            }
        //the second unit is changed
        else if ( ( IdOfFocusControl() != EConverterEditor1 ) && ( LastUnit2 != aUnit2Popup ) )
            {
            aMount = FloatingPointEditorValue( EConverterEditor2 );
            result = iUnit1Category.Convert( aUnit2Popup, aUnit1Popup, aMount );
            SetFloatEditorValueL( EConverterEditor1, result );           	
            }
        //the first amount field is changed
        else
            {        
            result = iUnit1Category.Convert( aUnit1Popup, aUnit2Popup, aMount );
            SetFloatEditorValueL( EConverterEditor2, result );
            }
        }
    else
        {
        //the first unit is changed
        if ( ( ( IdOfFocusControl() != EConverterEditor2 ) && ( LastUnit1 != aUnit1Popup ) )
           || ( IdOfFocusControl() == EConverterTypePopup ))
            {
            aMount = FloatingPointEditorValue( EConverterEditor1 );
            result = iUnit1Category.Convert( aUnit1Popup, aUnit2Popup, aMount );
            SetFloatEditorValueL( EConverterEditor2, result );
            }
        //the second unit is changed
        else if ( ( IdOfFocusControl() != EConverterEditor2 ) && ( LastUnit2 != aUnit2Popup ) )
        	{
            aMount = FloatingPointEditorValue( EConverterEditor2 );
            result = iUnit1Category.Convert( aUnit2Popup, aUnit1Popup, aMount );
            SetFloatEditorValueL( EConverterEditor1, result );           	
        	}
        //the second amount field is changed
        else
            {
            result = iUnit1Category.Convert( aUnit2Popup, aUnit1Popup, aMount );
            SetFloatEditorValueL( EConverterEditor1, result );
            } 
        }

    // record units of the category at this time.
    LastUnit1 = aUnit1Popup;
    LastUnit2 = aUnit2Popup;	
    }
//End of file
