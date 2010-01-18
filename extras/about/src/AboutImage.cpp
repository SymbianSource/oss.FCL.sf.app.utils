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
* Description: offer bitmap for application.
*
*/


// INCLUDE FILES
#include <fbs.h>
#include "AboutImage.h"
#include <AknIconUtils.h>

// ================= MEMBER FUNCTIONS ==========================================

CAboutImage::CAboutImage()
    {
    }

CAboutImage::~CAboutImage()
    {
    delete iBitmap;
    }

CAboutImage* CAboutImage::NewLC( const TDesC& aFileName,
                                 TInt aBitmapId,
                                 TInt aStartLine,
                                 TInt aBaseLineDelta )
    {
    CAboutImage* self = new( ELeave ) CAboutImage();
    CleanupStack::PushL( self );

    /*self->iBitmap = new( ELeave ) CFbsBitmap;
    self->iBitmap->Load( aFileName, aBitmapId );*/

	self->iBitmap  = AknIconUtils::CreateIconL(aFileName, aBitmapId );
    self->iStartLine = aStartLine;

    // enough lines so that image and margins fit in them.

    TInt lines( ( self->HeightInPixels() + 
                  KImageTopMargin + 
                  KImageBottomMargin +
                  aBaseLineDelta - 1 ) / aBaseLineDelta );

    self->iEndLine = aStartLine + lines - 1;
    return self;
    }

// -----------------------------------------------------------------------------
// CAboutImage::HeightInPixels()
// -----------------------------------------------------------------------------

TInt CAboutImage::HeightInPixels() const
    {
    return iBitmap->SizeInPixels().iHeight;
    }

// -----------------------------------------------------------------------------
// CAboutImage::WidthInPixels()
// -----------------------------------------------------------------------------

TInt CAboutImage::WidthInPixels() const
    {
    return iBitmap->SizeInPixels().iWidth;
    }

// -----------------------------------------------------------------------------
// CAboutImage::StartLine()
// -----------------------------------------------------------------------------

TInt CAboutImage::StartLine() const
    {
    return iStartLine;
    }

// -----------------------------------------------------------------------------
// CAboutImage::EndLine()
// -----------------------------------------------------------------------------

TInt CAboutImage::EndLine() const
    {
    return iEndLine;
    }

// -----------------------------------------------------------------------------
// CAboutImage::Lines()
// -----------------------------------------------------------------------------

TInt CAboutImage::Lines() const
    {
    return iEndLine - iStartLine + 1;
    }

// -----------------------------------------------------------------------------
// CAboutImage::Bitmap()
// -----------------------------------------------------------------------------

const CFbsBitmap* CAboutImage::Bitmap() const
    {
    return iBitmap;
    }

// End of File  
