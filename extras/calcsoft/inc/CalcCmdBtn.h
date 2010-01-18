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
* Description:  The Function-Map's button class, CCalcCommandButton
*                Derived from CEikCommandButtonBase
*                The CCalcCommandButton is flat button.
*                In making disable, it calls SetDimmed() function.
*
*/


#ifndef     CALCCMDBTN_H
#define     CALCCMDBTN_H


#include <coecntrl.h>
#include <aknutils.h>

//  CLASS DEFINITIONS 
class CCalcFuncmapSubPane;

/**
CCalcCommandButton : The button class for FunctionMap of a calculator
*/

class CCalcCommandButton 
      : public CCoeControl
    {
    public:
        enum TState  // Button state for bitmap.
            {
            ENormal,    // Button is normal.
            EPressed,   // Button is pushed.
            EDimmed     // Button is dimmed.
            };

    public: // Constructors and destructor 
        /**
        * Two-phased constructor.
        * @param aFuncmapSubPane : CCalcFuncmapSubPane pointer 
        * @param aUnpressedBmp : Bitmap for unpressed button
        * @param aPressedBmp : Bitmap for pressed button
        * @param aInactiveBmp : Bitmap for inactive button
        */
        static CCalcCommandButton* NewL(
            CCalcFuncmapSubPane* aFuncmapSubPane,
            const CGulIcon* aUnpressedBmp,
            const CGulIcon* aPressedBmp,
            const CGulIcon* aInactiveBmp); 
        
        /**
        * Destructor.
        */
        virtual ~CCalcCommandButton();
        
        
    public: // New functions
        /**
        * Button state is set and re-draw.
        * @param aState : State of a button (ENormal || EPressed || ESelected)
        */
        void SetStateAndRedraw(const TState& aState);

        /**
        * Set flag which button is selected or not, and redraw.
        * @param aIsSelected : Button is selected or not.
        */
        void SetIsSelectedAndRedraw(TBool aIsSelected);

        /**
        * Return Current state of a button.
        * @return Current state of a button (ENormal || EPressed || ESelected)
        */
        TState State() const;

		/**
		 *Sets iUnpressedButton, iPressedButton, iInactiveButton bitmap arrays to the current bitmaps.
		 * @param aUnpressedButtonIcon: Icon for Unpressed button 
         * @param aPressedButtonIcon :  Icon for  pressed button
         * @param aInactiveButtonIcon : Icon for Inactive button        
		*/
		void SetBitmapIcons(CGulIcon* aUnpressedButtonIcon,
                    CGulIcon* aPressedButtonIcon,
                    CGulIcon* aInactiveButtonIcon);
        /**
        * Gets the button layout
        * Used for touch support.
        */
        TRect ButtonLayout();
    
    private: // New functions
        /**
        * C++ default constructor.
        */
        CCalcCommandButton();
       
        /**
        * Second-phase constructor.
        * @param aFuncmapSubPane : CCalcFuncmapSubPane pointer 
        * @param aUnpressedBmp : Bitmap for unpressed button
        * @param aPressedBmp : Bitmap for pressed button
        * @param aInactiveBmp : Bitmap for inactive button
        */
        void ConstructL(
                CCalcFuncmapSubPane* aFuncmapSubPane,
                const CGulIcon* aUnpressedBmp,
                const CGulIcon* aPressedBmp,
                const CGulIcon* aInactiveBmp); 
        
        
    private:  // Functions from base classes
        /**
        * From CCoeControl : Data of control is set.
        */
        void SizeChanged();

        /**
        * From CCoeControl : Draw a button
        * @param aRect : Size of a button
        */
        void Draw(const TRect& aRect) const;

        
    private:  // Data
        TState  iState;      // Current state of a button
        TBool   iIsSelected; // If Button selected TRUE
        const CGulIcon*   iUnpressedButton;   // Unpressed button bitmap
        const CGulIcon*   iPressedButton;     // Pressed button bitmap
        const CGulIcon*   iInactiveButton;    // Inactive button bitmap
        TAknLayoutRect    iLayoutRect;        // rectangle of button 
    };


#endif      //  CALCCMDBTN_H

// End of File
