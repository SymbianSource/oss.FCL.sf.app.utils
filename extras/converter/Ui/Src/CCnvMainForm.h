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
*     An AVKON form class containing popup fields and floating point editors.
*     Implements the 'Main State' of the application.
*
*/



#ifndef __CCNVMAINFORM_H__
#define __CCNVMAINFORM_H__

// INCLUDES
#include <AknForm.h>
#include <TCnvCategory.h>

//Select Edwin
#include <eikedwob.h>


// FORWARD DECLARATIONS
class CCnvConverter;
class CAknPopupField;
class CErrorUI;

// CLASS DEFINITION
/**
* An AVKON form class containing popup fields and floating point editors.
* Implements the 'Main State' of the application.
*/
class CCnvMainForm : public CAknForm , public MEikEdwinObserver
	{

	public: // constructors and destructor

		/**
		* Default constructor.
		*/
		CCnvMainForm( TBool &aAlreadyDead );

		/**
		* Destructor.
		*/
		~CCnvMainForm();

		/**
		* 2nd phase constructor.
		*/
		void ConstructL();

	private: // constructors

		/**
		* Default constructor. Not implemented.
		*/
		CCnvMainForm();

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
		* Takes any action required when the current line is
		* changed to aControlId.
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

	protected: // from MCoeControlObserver

		/**
		* Handles an event originating from one of the form's controls.
		* @param aControl The control that reported the event.
		* @param aEventType The type of the reported event.
		*/
		void HandleControlEventL( CCoeControl* aControl,
			TCoeEvent aEventType );

	protected: // from CAknForm

		/**
		* Saves form popup field states to a file (not implemented, yet).
		* @return ETrue if the editable state can be left
		*         (always in this case).
		*/
		TBool SaveFormData();

        /**
        * Deal with that the field is empty,when move focus to other field.
        * The Uikon framework calls this function,immediately before focus move. 
        */
        void PrepareForFocusTransitionL();
	protected: // from CEikDialog

		/**
		* Tests if it is OK to exit the dialog.
		* @param aButtonId The ID of the button that was activated.
		* @return ETrue to validate and exit the dialog,
		*         EFalse to keep the dialog active.
		*/
		TBool OkToExitL( TInt aButtonId );

		/**
		* Pre-layout dialog initialisation.
		*/
		void PreLayoutDynInitL();

	private: // new methods
		/**
		* returns ETrue if the the character is an integer
		*/
		TBool IsDigit(TUint aChar);

		/**
		* Checks if a form control has REALLY changed (uses history knowledge)
		* Form controls report bogus state changed events, this is used to
		* filter them out.
		* @param controlId Form ID of the control to be checked
		* @return ETrue if the specified control's state has changed
		*/
		TBool HasReallyChanged( TInt controlId );

		/**
		* Initialised change history (for HasReallyChanged)
		*/
		void InitialiseChangeHistory();

		/**
		* Called when the user has changed
		* the unit type popup field selection.
		*/
		void HandleCategoryChangedL();

		/**
		* Called when the user has changed
		* either of the unit popup field selections.
		*/
		void HandleUnitChangedL();

		/**
		* Checks if the unit rates (factors) are defined in the model
		* for the currently selected unit type.
		* @param aShowNote If ETrue "rates missing note" is shown, if
		*                  rates are not valid.
		* @return ETrue if rates are valid, EFalse otherwise.
		*/
		TBool AreRatesValidL(TBool aShowNote);

		/**
		* Checks if a floating point editor's value is within allowable range.
		* @param aEditorId Resource ID of the editor control to be checked.
		* @return ETrue If value is in range, EFalse otherwise.
		*/
		TBool IsEditorValueValidL( TUint aEditorId );

		/**
		* Checks if a floating point editor's value is such that no conversion
		* should be made and no "invalid value" error should be reported.
		* @param aEditorId Resource ID of the editor control to be checked.
		* @return ETrue Indicates that conversion should not proceed.
		*/
		TBool EditorValueCausesHold( TUint aEditorId );

		/**
		* Handles the ECmdRates command. Launches the Rates Form.
		*/
		void CmdRatesL();

		/**
		* Handles the ECmdSetType command.
		*/
		void CmdSetTypeL();

		/**
		* Handles the ECmdSetUnit command.
		*/
		void CmdSetUnitL();

		/**
		* Tells the model to load its state from a file.
		*/
		void LoadStateL();

		/**
		* Tells the model to save its state to a file.
		* Displays an error note if there is not enough space in permanent
		* storage.
		*/
		void SaveState();

		/**
		* Calculates and displays the unit conversion immediately.
		*/
		void SynchronousConversionL( TBool aShowRateInvalidNote = ETrue );

		/**
		* Calculates and displays the unit conversion after 0.75 seconds.
		*/
		void AsynchronousConversion();

		/**
		* Cancels a queued asynchronous conversion request, if any.
		*/
		void CancelConversion();

		/**
		* Called by the conversion timer after the 0.75 second timeout
		* has exipred. Calls SynchronousConversion() to do
		* the actual conversion.
		* @param aMainForm Pointer to the CCnvMainForm instance.
		*/
		static TInt TimerCallback( TAny* aMainForm );

		/**
		* Shows an error note for invalid value situation
		* @param aEditorId Id of the editor that holds the invalid value
		*/
		void ShowInvalidValueNoteL(TInt aEditorId);

		/**
		* From CCoeControl processing of a key event
		* @param aKeyEvent : Key event information
		*        aType : EEventKeyDown | EEventKey | EEventKeyUp
		* @return : EKeyWasConsumed if keyevent is consumed.   
		*           EKeyWasNotConsumed if keyevent is not consumed.
		* Handling of the keys is taken care for the editor fields
		*/
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
			TEventCode aType);

		/**
		*Checks if the number entered in the editor field is a valid number
		*@param aEditroId : Id of the editor that needs to be evaluated
		*@return : EFalse if the editor value is not a vaild number	
		*		 : ETrue if the number is a valid number
		*/
		TBool IsVaildNumberEntered(TInt aEditorId);

		/**
		*From MEikEdwinObserver. handles the Navigation on the editor control. 
		*/
		void HandleEdwinEventL(CEikEdwin* aEdwin,TEdwinEvent aEventType);
		
		/**
		*Do the calculation and change the value for the related edit field
		*/
		void ValueCalculationL( TUint aUnit1Popup, TUint aUnit2Popup, TReal aMount );

	private: // data

		/**
		* Pointer to our model. Owned.
		*/
		CCnvConverter* iModel;

		/**
		* Model accessor for unit types.
		*/
		TCnvCategory iTypeCategory;

		/**
		* Model accessor for source units.
		*/
		TCnvCategory iUnit1Category;

		/**
		* Model accessor for destination units.
		*/
		TCnvCategory iUnit2Category;

		/**
		* Pointer to the Unit Type popup field. Not owned.
		*/
		CAknPopupField* iTypePopupCtrl;

		/**
		* Pointer to the source Unit popup field. Not owned.
		*/
		CAknPopupField* iUnit1PopupCtrl;

		/**
		* Pointer to the destination Unit popup field. Not owned.
		*/
		CAknPopupField* iUnit2PopupCtrl;

		/**
		* Id of the currency category in the model.
		*/
		TUint iCurrencyCategoryId;

		/**
		* ETrue if this instance is fully constructed, EFalse otherwise.
		*/
		TBool iIsFullyConstructed;

		/**
		* ETrue if the user has already been notified that the disk is full.
		*/
		TBool iHasNotifiedDiscFull;

		/**
		* ETrue if the iLastUsedEditorId has changed after the focus
		* has entered the editor.
		*/
		TBool iActiveEditorChanged;

		/**
		* Pointer to the timer used for asynchronous conversions
		* (which need a0.75 second delay..)
		*/
		CPeriodic* iConversionTimer;

		/**
		* The id of the unit amount field that the user has modified last.
		*/
		TInt iLastUsedEditorId;

		/**
		* The id of the line that was last active in the form.
		*/
		TInt iLastActiveEditorLine;

		/**
		* The previous type selection.
		*/
		TInt iHistoryTypeCategory;

		/**
		* The previous source unit selection.
		*/
		TInt iHistoryUnit1Category;

		/**
		* The previous destination unit selection.
		*/
		TInt iHistoryUnit2Category;

		/**
		* Pointer to the Error UI object. Owned
		* Used for displaying the Disk Full error message.
		*/
		CErrorUI* iErrorUI;

		/**
		* Reference to a flag in this form's owner.
		* Used to indicate that we have been deleted by someone
		* (could be the app framework.. <sigh>)
		*/
		TBool& iAlreadyDead;	

		TKeyEvent iKeyEvent;

		TBool isEdwinNavigated;
		TBool isEdwinUpdateText;

	};

#endif // __CCNVMAINFORM_H__

