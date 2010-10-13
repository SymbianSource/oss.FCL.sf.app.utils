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
#include <gulicon.h>
#include "MResourceLoaderObserver.h"

// FORWARD DECLARATIONS
class CEikScrollBarFrame;
class CAboutImage;
class CFont;
class TRect;
class TBidiText;
class CAknsBasicBackgroundControlContext;
class CAboutResourceLoader;


// CLASS DECLARATION

/**
*  CAboutContainer  container control class.
*  The container for About. It handle the system event. 
*  And it draws correct texts and images to screen base on the scroll bar.
*
*/

class CAboutContainer : public CCoeControl,
                        public MEikScrollBarObserver,
                        public MResourceLoaderObserver
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

    private: //From MResourceLoaderObserver
        void HandleItemsLoadedL( TInt aError );
        void HandleResourceLoadCompletedL( const TInt aResourceId, TInt aError );
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
        
        //Owned: Resource loader.
        CAboutResourceLoader* iLoader;
        
        // Judge the break between "real" about box language and the full OSS texts 
        TBool iBreakFlag;
        
        // Store the resource path for load the file
        TFileName iResourcePath;
        
        // Store the the count of the loaded content's items
        TInt iFinalCount;
    };

#endif

// End of File
