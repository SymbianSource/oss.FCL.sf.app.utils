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
* Description:  Declares container control for application.
*
*/


#ifndef ABOUTCONTAINER_H
#define ABOUTCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <gdi.h>  
#include <eiksbobs.h> 
// FORWARD DECLARATIONS
class CEikScrollBarFrame;
class CAboutImage;
class CFont;
class TRect;
class TBidiText;
class   CAknsBasicBackgroundControlContext;
#include <gulicon.h>

// CLASS DECLARATION

/**
*  CAboutContainer  container control class.
*  
*/
//About container derived from scrollbarobserver to get events on scrollbar movements
class CAboutContainer : public CCoeControl, public MEikScrollBarObserver
    {
    public: // Constructors and destructor
        CAboutContainer();
        void ConstructL( const TRect& aRect );

		void CalculateL(const TRect& aRect);
		void HandleResourceChange(TInt aType);
        /**
        * From MEikScrollBarObserver
        */
        void HandleScrollEventL(CEikScrollBar* aScrollBar,TEikScrollEvent aEventType); 

        ~CAboutContainer();

    private: // from CCoeControl
		
		/**
        * From CCoeControl : Control size is set.
        */
        void SizeChanged();

        void Draw( const TRect& aRect ) const;
        void ActivateL();
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aModifiers );

		TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
    private: // new functions

        void SetTextL( const TDesC& aText , const TInt aItem );
        void SetImageL( const TDesC& aFileName, TInt aBitmapId );
        void UpdateScrollIndicatorL();

        // text wrapping

    private: // Data

        CArrayPtr<HBufC>* iText;
        CArrayPtr<CAboutImage>* iImages;
        CArrayFixFlat<TInt>* iScreenStarts;
        TInt iCurrentScreen;
        TBool iDoNotShowLastLineAgain;
        CGraphicsContext::TTextAlign iTextAlign;
        CEikScrollBarFrame* iSBFrame;
        const CFont* iFont; // not owned
        TInt iLineWidth;
        TInt iBaseLineDelta;
		TInt iTopBaseLineX;
		TInt iTopBaseLineY;
		TInt iLinesPerScreen;
		CAknsBasicBackgroundControlContext* iSkinContext; // skin data
		CGulIcon* iIcon;
		
        // Judge whether scroll bar is dragged. 
        TBool iScrollBarDragged;
        TInt iNumItem;
    };

#endif

// End of File
