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
* Description:  An App UI class, CCalcAppUi, derived from CAknViewAppUi
*
*/


#ifndef     CALCAPPUI_H
#define     CALCAPPUI_H


//  INCLUDES
#include <aknViewAppUi.h>


//  FORWARD DECLARATIONS
class   CCalcAppEnv;
class   CEnvironmentChangeNotifier;

//  CLASS DEFINITIONS 

/**
CCalcAppUi : 'AppUi' class
*/

class   CCalcAppUi
      :public CAknViewAppUi

    {
    public:     // Constructors and destructor
        /**
        * C++ default constructor.
        */
        CCalcAppUi();
        
        /**
        * Destructor.
        */
        virtual ~CCalcAppUi();

    public: // New functions
        /**
        * Exit Calculator application.
        */
        void ExitCalculator();

		/**
		* Returns ETrue value when the skin change event has occured.
		*/
		inline TBool IsSkinChanged()const;

		/**
		* Sets the value to ETrue when the skin changes
		* @param aETrue : Boolean value to set the iIsSkinChanged variable to ETtrue.
        */
		inline void SetSkinChanged(TBool aETrue); 

    private: // New functions
        /**
        * This function is called when locale is changed.
        * @param aThisPtr : Specified in TCallback constructor.
        * @return EFalse
        */
        static TInt LocaleCallback(TAny* aThisPtr);

        /**
        * Handle locale change event.
        */
        void HandleLocaleChangeEvent();
        
    private: // Functions from base classes
        /**
        * From CEikAppUi, default constructor
        */
        void ConstructL(); 

        /**
        * From CEikAppUi, Handle Exit command.
        * Other command is handled in CCalcView::HandleCommandL().
        * @param : aCommand : command ID
        */
        void HandleCommandL(TInt aCommand);

		/**
		*To handle the event when the skin changes.
		*@param aType :This is used to check whether the skin changes or not.
        */
		void HandleResourceChangeL(TInt aType);

		/**
		*Handles events sent to the application by the window server.
		*@param aEvent : window server event.
		*@param aDestination : The control associated with the event. 
		**/
        void HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination );
              
    private:    // Data
        CCalcAppEnv* iCalcAppEnv; // Calculator environment
        CEnvironmentChangeNotifier* iLocaleChangeNotifier;
                                        // For locale change notifier
		TBool			iIsSkinChanged; // For Skin change notification
    };

#include"CalcAppUi.inl"
#endif      //  CALCAPPUI_H
            
// End of File
