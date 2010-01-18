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
*     An AVKON form class for changing unit names and factors (rates).
*     Used with currency units, but is not limited to them.
*     Implements the 'Rates State' of the application.
*
*/



#ifndef __CCNVRATESFORM_H__
#define __CCNVRATESFORM_H__

// INCLUDES
#include <AknForm.h>
#include "CCnvConverter.h"
// FORWARD DECLARATIONS
class TCnvCategory;

// CLASS DEFINITION
/**
* An AVKON form class for changing unit names and factors (rates).
* Used with currency units, but is not limited to them.
* Implements the 'Rates State' of the application.
*/
class CCnvRatesForm : public CAknForm
	{

	public: // Constructors and destructor

		/**
		* Constructor.
		* @param aWantsRelaunch Indicates to the caller if we want
		*        to be relaunched.
		* @param aShouldExit Indicates to the caller if the application should
		*        exit after this form has been closed.
		* @param aModelChanged Indicates to the caller that the model data
		*		 has changed and needs saving to permanent storage.
		* @param aCategory Unit category containing currencies.
		*/

		CCnvRatesForm( TBool* aWantsRelaunch, TBool* aShouldExit,
			TBool* aModelChanged, TCnvCategory& aCategory );

		/**
		* Destructor.
		*/
		~CCnvRatesForm();

		/**
		* 2nd phase constructor.
		*/
		void ConstructL();

	public: // from CCoeControl

		/**
		* Specifies the help context for this form.
		* @param aContext the context IDs are placed here
		*/
		void GetHelpContext( TCoeHelpContext& aContext ) const;

	public: // from MEikCommandObserver

		/**
		* Handles menu commands.
		* @param aCommandId Command identifier.
		*/
		void ProcessCommandL( TInt aCommandId );

	public:	// from MEikDialogPageObserver

		/**
		* Takes any action required when the current line
		* is changed to aControlId.
		* @param aControlId Control identifier.
		*/
		void LineChangedL( TInt aControlId );

	public:	// from MEikMenuObserver

		/**
		* Dynamically initialises a menu pane. The Uikon framework calls this
		* function, immediately before the menu pane is activated.
		* @param aResourceId Resource ID identifying a menu pane to initialise
		* @param aMenuPane The in-memory representation of the menu pane.
		*/
		void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

	protected: // from CAknForm
	
		/**
		* Saves form data to model.
		* @return ETrue if the editable state can be left
		*         (always in this case).
		*/
		void DoNotSaveFormDataL();
		/**
		* Saves form data to model.
		* @return ETrue if the editable state can be left
		*         (always in this case).
		*/
		TBool SaveFormDataL();
		/**
		* Tests if it is OK to exit the dialog.
		* @param aButtonId The ID of the button that was activated.
		* @return ETrue to validate and exit the dialog,
		*         EFalse to keep the dialog active.
		*/
		TBool OkToExitL(TInt aButtonId);
		/**
		* Sets the focus to the topmost line of the form.
		* when the dialog is activated.
		*/
		void SetInitialCurrentLine();
		
		/**
		* For Touch Ui implementation
		* To prepare for focus transition from the current line.
		*/
		void PrepareForFocusTransitionL();

	protected: // from CEikDialog

		/**
		* Pre-layout dialog initialisation.
		* Populates from with floating point editor controls.
		*/
		void PreLayoutDynInitL();

	private: // Constructors and destructor.

		/**
		* Default constructor, empty implementation.
		*/
		CCnvRatesForm();
		
		/**
		* returns ETrue if the the character is an integer
		*/
		TBool IsDigit(TUint aChar);

	private: // new methods

		/**
		* Handles the ECmdChangeCurrencyName command.
		*/
		void CmdChangeCurrencyNameL();

		/**
		* Handles the ECmdSetAsBaseCurrency command.
		*/
		void CmdSetAsBaseCurrencyL();

		/**
		* Changes the application title pane text to default.
		* Non-leaving.
		*/
		void SetDefaultTitle();

		/**
		* Changes the application title pane text.
		* @param aNonDefault EFalse = set to default.
		*/
		void SetTitleL( TBool aNonDefault );

		/**
		* Handles the Key events for the rates form
		* @param aKeyEvent : Key event information
		*        aType : EEventKeyDown | EEventKey | EEventKeyUp
		* @return : EKeyWasConsumed if keyevent is consumed.   
		*           EKeyWasNotConsumed if keyevent is not consumed.
		*/
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
			TEventCode aType);


	private: // data

		/**
		* Pointer to the flag in the caller object.
		* Used to indicate if we want to be relaunched after exit.
		*/
		TBool* iWantsRelaunch;

		/**
		* Pointer to the flag in the caller object.
		* Used to indicate if the application should close after form exit.
		*/
		TBool* iShouldExit;

		/**
		* Shows an error note for invalid value situation
		* @param aEditorId Id of the editor that holds the invalid value
		* @param aSetValue its value is ETrue or EFalse,
		*        if aSetValue equals EFlase that does not set FloatEditor Value
		*/
		void ShowInvalidValueNoteL(TInt aEditorId, TBool aSetValue);

		/**
		* Pointer to the flag in the caller object.
		* Used to indicate if the model data has changed and needs
		* saving to permanent storage.
		*/
		TBool* iModelChanged;

		/**
		* Model accessor for the units displayed in this form.
		*/
		TCnvCategory iCategory;

	private:
		/**
		*Implements the dialog to add a new currency OR rename a currency
		*@param aNewname indicates the text that should appear in the dialog as default
		*@param aPrompt indicates that title of the dialog
		*@param aIsAddNewCurrency if add a new currency aIsAddNewCurrency is ETrue,otherwise EFalsse   
		*/
		TCurrencyType NewCurrencyNameDialogL(TDes& aNewName,TBool& aNameEntered,TBool aIsAddNewCurrency = EFalse );
		/**
		* Adds new currency...
		*/
		void AddNewCurrencyL();
		/**
		* Deletes the Currency (which is having focus)
		*/
		void DeleteCurrencyL();

		/**
		*Checks if the number entered in the editor field is a valid number
		*@param aEditroId : Id of the editor that needs to be evaluated
		*@return : EFalse if the editor value is not a vaild number	
		*		 : ETrue if the number is a valid number
		*/
		TBool IsVaildNumberEntered(TInt aEditorId);
		
		/**
		*Sets the control as ready to be drawn.
		*/		
		void ActivateL();
		
		RArray<TInt> iFormIdMapArray; //Array which has the ids of the form..
		RArray<TInt> iFormIdEdwinMapArray; //Array which has the ids of the CEikEdwin editors in the form
		/*
		* Last active editor line.
		*/
        TInt iLastUsedEditorId;
		TBool iEKeyOk;
		TBool iError;

    TUint iSrcUnitIndexForRestore;
    TUint iDesUnitIndexForRestore;
    TInt iHistoryBaseUnit;

	};

#endif // __CCNVRATESFORM_H__

