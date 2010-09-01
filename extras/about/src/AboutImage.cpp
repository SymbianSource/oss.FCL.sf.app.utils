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
#include <aknenv.h>
#include <AknUtils.h>
#include <aknconsts.h>
#include <AknBidiTextUtils.h>
#include <AknsDrawUtils.h> 
#include <AknsBasicBackgroundControlContext.h>
#include <AknsConstants.h>
#include <AknDef.h>


const TInt QtnJavaLogoWidth  = 46;
const TInt QtnJavaLogoHeight = 70;

// ================= MEMBER FUNCTIONS ==========================================

CAboutImage::CAboutImage()
    {
    }

CAboutImage::~CAboutImage()
    {
    delete iBitmap;
    delete iBitmapMask;
    }

CAboutImage* CAboutImage::NewLC( const TDesC& aFileName,
                                 TInt aBitmapId,
                                 TInt aStartLine,
                                 TInt aBaseLineDelta,
                                 TBool aTypeFlag )
    {
    CAboutImage* self = new( ELeave ) CAboutImage();
    CleanupStack::PushL( self );

    if( aTypeFlag )
    	{
    	// bmp files
	self->iBitmap  = AknIconUtils::CreateIconL(aFileName, aBitmapId );
    	}
    else
    	{
    	// svg files
    	MAknsSkinInstance* skinInstance = AknsUtils::SkinInstance();
    	AknsUtils::CreateIconL( skinInstance, KAknsIIDQgnMenuSmsvoLst, self->iBitmap, self->iBitmapMask, 
					aFileName, aBitmapId, aBitmapId + 1 );
    	AknIconUtils::SetSize( self->iBitmap, TSize( QtnJavaLogoWidth, QtnJavaLogoHeight ) );
    	AknIconUtils::SetSize( self->iBitmapMask, TSize( QtnJavaLogoWidth, QtnJavaLogoHeight ) );
    	}
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

// -----------------------------------------------------------------------------
// CAboutImage::BitmapMask()
// -----------------------------------------------------------------------------

const CFbsBitmap* CAboutImage::BitmapMask() const
	{
	return iBitmapMask;
	}
// End of File  
