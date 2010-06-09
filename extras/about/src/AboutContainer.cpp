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
* Description: application container control.  
*
*/


// INCLUDE FILES

#include "AboutContainer.h"
#include "AboutImage.h"
#include "about.hrh"
#include "about_test.hrh"
#include <layoutmetadata.cdl.h>
#include <about.rsg>
#include <eiksbfrm.h>
#include <eikscrlb.h>
#include <eikrted.h>
#include <txtrich.h>
#include <barsread.h>
#include <eikenv.h>
#include <aknenv.h>
#include <AknUtils.h>
#include <aknconsts.h>
#include <txtfrmat.h>
#include <AknBidiTextUtils.h>
#include <data_caging_path_literals.hrh>
#include <f32file.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <aknappui.h>
#include <AknsDrawUtils.h> 
#include    <AknsBasicBackgroundControlContext.h>
#include    <AknsConstants.h>
#include <AknDef.h>
#include <about.mbg>
#include <StringLoader.h> 

// CONSTANTS

_LIT( KAboutPanicCategory, "About" );

enum TAboutPanic
    {
    EAboutNotSupported = 0
    };

/**
* SCROLLING:
*
* Scrolling is implemented according to the following rules
* when scrolling down:
*
* (same screens are shown again when scrolling up)
*
* - If the last line in previous screen was a text line,
*   next screen is started with that line, ie. the last
*   line is shown again in the next screen.
*
* - If previous screen ended with an image that did not
*   fully fit, next screen is started with that image so
*   that it can be seen as a whole.
*
* - If previous screen ended with an image that fit there fully,
*   next screen is started with next text line or image.
*/

// constructors

CAboutContainer::CAboutContainer():iNumItem( 0 )
    {
    }

void CAboutContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();

    iScrollBarDragged = EFalse;
	iSkinContext = NULL;
	iText = NULL;
	iImages = NULL;
	iScreenStarts = NULL;
	iSBFrame = NULL;
    CalculateL(aRect); 
    ActivateL();
    }

// destructor

CAboutContainer::~CAboutContainer()
    {
	delete iSkinContext;
    delete iSBFrame;
    delete iScreenStarts;
    delete iIcon;

    if ( iText )
        {
        iText->ResetAndDestroy();
        delete iText;
        }

    if ( iImages )
        {
        iImages->ResetAndDestroy();
        delete iImages;
        }
    }

void CAboutContainer::SizeChanged()
    {
    TRect parentRect(Rect());
	if (iSkinContext)
        {
        iSkinContext->SetRect(parentRect);
        }
	}
// -----------------------------------------------------------------------------
// CAboutContainer::Draw()
// -----------------------------------------------------------------------------

void CAboutContainer::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	if (iSkinContext)
		{//Draw the skin background
		AknsDrawUtils::Background(
			skin, iSkinContext, this, gc, aRect);
		}
	else
		{//  clear the area
		gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
		gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
		gc.Clear( aRect );
		}

    // draw text
    gc.UseFont( iFont );
    // index of the first line on the screen in the text array
    TInt firstLine = 0 ;

    // The value of firstLine can not be the same.
    if ( !iScrollBarDragged )
        {
        if ( ( iScreenStarts ) && (iScreenStarts->Count() >= iCurrentScreen ) )
            {
    	    firstLine = ( ( *iScreenStarts )[ iCurrentScreen ] ); 	
            }        
        }
    else
        {                    
        if ( iCurrentScreen <= ( ( *iScreenStarts )[ iScreenStarts->Count() - 1 ] ) )
            {
            firstLine = iCurrentScreen ;
            }
        }
    // index of the last line on the screen in the text array
    TInt lastLine( firstLine + iLinesPerScreen - 1 );

    gc.SetBrushStyle( CGraphicsContext::ENullBrush );
    TPoint position( iTopBaseLineX, iTopBaseLineY );
    TPoint topLeft;
    TSize rectSize( iLineWidth, iBaseLineDelta +iFont->DescentInPixels() );

    for ( TInt index = firstLine ;
          index < iText->Count() && index <= lastLine ;
          index++, position.iY += iBaseLineDelta )
        {
        HBufC* text = (*iText)[ index ];

        if ( text )
            {
            topLeft = TPoint( position.iX, position.iY-iBaseLineDelta );
			TRgb color;			
			TInt error = AknsUtils::GetCachedColor( skin, 
							color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6 );
			
			if(error == KErrNone)
				{
				gc.SetPenColor(color);
				}		
            gc.DrawText( *text,
                         TRect( topLeft, rectSize ),
                         iBaseLineDelta,
                         iTextAlign );
            }
        }

    gc.DiscardFont();

    // draw images

    for ( TInt i = 0 ; i < iImages->Count() ; i++ )
        {
        CAboutImage* image = (*iImages)[ i ];

        // If part of the image resides in visible lines, draw it.
        if ( image->StartLine() <= lastLine && image->EndLine() >= firstLine )
            {
            position.SetXY( iTopBaseLineX, iTopBaseLineY );
            position.iY += ( image->StartLine() - firstLine ) * iBaseLineDelta;

            position.iY -= iBaseLineDelta - iFont->DescentInPixels();
            // Now iY is the top line of rectangle where the picture is
            // centered in.
            position.iY += ( (image->Lines()+1) * iBaseLineDelta -
                             iFont->HeightInPixels() -
                             image->HeightInPixels() ) / 2;

            // If text is right-aligned, also align images to the right.

            if ( iTextAlign == CGraphicsContext::ERight )
                {
                position.iX += ( iLineWidth - image->WidthInPixels() );
                }

            gc.BitBlt( position, image->Bitmap(), aRect );
            }
        }
     /*   if(iCurrentScreen == 0)
    {
        TAknWindowLineLayout area = AknLayoutScalable_Apps::mce_icon_pane().LayoutLine();
 	    TAknLayoutRect layoutRect;
 	    TRect iconrect = Rect();
 	    iconrect.iTl.iX = position.iX;
	    layoutRect.LayoutRect(iconrect, area);
  	    layoutRect.DrawImage(gc, iIcon->Bitmap(),iIcon->Mask());
        
    }*/

    }

// -----------------------------------------------------------------------------
// CAboutContainer::ActivateL()
// -----------------------------------------------------------------------------

void CAboutContainer::ActivateL()
    {
    CCoeControl::ActivateL();
    UpdateScrollIndicatorL();
    }

// -----------------------------------------------------------------------------
// CAboutContainer::SetTextL()
// -----------------------------------------------------------------------------

void CAboutContainer::SetTextL( const TDesC& aText , const TInt aItem )
    {
    CArrayFix<TPtrC>* wrappedArray =
        new( ELeave ) CArrayFixFlat<TPtrC>( 10 );

    CleanupStack::PushL( wrappedArray );

    HBufC* dataToDestroy =
        AknBidiTextUtils::ConvertToVisualAndWrapToArrayL(
            aText, iLineWidth, *iFont, *wrappedArray
        );

    TInt numLines( wrappedArray->Count() );
    for ( TInt i = 0 ; i < numLines ; i++ )
        {
        HBufC* line = (*wrappedArray)[i].AllocLC();

        if(!line->Length())
            {
            iText->AppendL( NULL );
			
            CleanupStack::PopAndDestroy(line);  // line
            }
        else
            {
            iText->AppendL( line );
            CleanupStack::Pop(line);  // line
            }
        }
	iText->AppendL( NULL );

    // If the last char was newline, add one extra, since
    // wrapping automatically removes it.
    if ( aText[ aText.Length() - 1 ] == '\n' )
        {
        iText->AppendL( NULL );
		}

    CleanupStack::PopAndDestroy(wrappedArray); // wrappedArray
    delete dataToDestroy;

    // update screen scrolling info array

    TInt lastLine( iText->Count() - 1 );
    TInt screenStart( (*iScreenStarts)[ iScreenStarts->Count() - 1 ] );

    TBool firstNewScreenHandled( EFalse );

    while ( lastLine >= screenStart + iLinesPerScreen )
        {
        if ( !firstNewScreenHandled && iDoNotShowLastLineAgain )
            {
            screenStart++;
            firstNewScreenHandled = ETrue;
            }

        screenStart += iLinesPerScreen - 1;
        iScreenStarts->AppendL( screenStart );
        if ( ( lastLine == screenStart + 1 ) && ( aItem == iNumItem - 1 ) )
        	{
        	iScreenStarts->Delete( iScreenStarts->Count() - 1 );
        	}
        }

    // if text, last line is shown again in next screen
    iDoNotShowLastLineAgain = EFalse;
    }

// -----------------------------------------------------------------------------
// CAboutContainer::SetImageL()
// -----------------------------------------------------------------------------

void CAboutContainer::SetImageL( const TDesC& aFileName, TInt aBitmapId )
    {
    TInt firstLineOfImage( iText->Count() );

    CAboutImage* image =
    CAboutImage::NewLC( aFileName, aBitmapId, firstLineOfImage, iBaseLineDelta );

    // new lines to make room for the picture

    for ( TInt i = 0 ; i < image->Lines() ; i++ )
        {
        iText->AppendL( NULL );
        }

    iImages->AppendL( image );
    CleanupStack::Pop(image); // image

    // update screen scrolling info array

    TInt lastLineOfImage( iText->Count() - 1 );
    TInt screenStart( (*iScreenStarts)[ iScreenStarts->Count() - 1 ] );

    TBool firstNewScreenHandled( EFalse );

    // If the image was not fully shown in the first screen,
    // start the next screen with the image.

    if ( firstLineOfImage < screenStart + iLinesPerScreen &&
         lastLineOfImage >= screenStart + iLinesPerScreen )
        {
        screenStart = firstLineOfImage;
        iScreenStarts->AppendL( screenStart );
        firstNewScreenHandled = ETrue;
        }

    while ( lastLineOfImage >= screenStart + iLinesPerScreen )
        {
        if ( !firstNewScreenHandled && iDoNotShowLastLineAgain )
            {
            screenStart++;
            firstNewScreenHandled = ETrue;
            }

        screenStart += iLinesPerScreen - 1;
        iScreenStarts->AppendL( screenStart );
        }

    if ( lastLineOfImage == screenStart + iLinesPerScreen - 1 )
        {
        iDoNotShowLastLineAgain = ETrue;
        }
    else
        {
        iDoNotShowLastLineAgain = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CAboutContainer::OfferKeyEventL()
// -----------------------------------------------------------------------------

TKeyResponse CAboutContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                              TEventCode aType )
    {
    // Operation is different in dragging and thumbing.
    if ( ( iScrollBarDragged ) && ( aType == EEventKey ) )
        {
        for( TInt i=0; i<iScreenStarts->Count(); i++ )
            {
            if ( iCurrentScreen <= ( ( *iScreenStarts )[i] ) ) 
                {
                // if last state is dragging, value of iCurrentScreen should be adjusted.
                switch ( aKeyEvent.iCode )
                    {
                    case EKeyUpArrow:
                        {
                        iScrollBarDragged = EFalse;
                        // Do nothing if the scroll bar is on the top, when pressing arrow up.
                        if ( i != 0 )
                            {
                            iCurrentScreen =  ( i - 1 );
                            }
                        DrawNow();
                        UpdateScrollIndicatorL();
                        break;
                        }
                    case EKeyDownArrow:
                        {
                        iScrollBarDragged = EFalse;
                        if ( ( iCurrentScreen == ( *iScreenStarts )[i] )
                            // Do nothing if the scroll bar is on the bottom, when pressing arrow down.
                            && ( i != ( iScreenStarts->Count() - 1 ) ) )
                            {
                            iCurrentScreen = ( i + 1 );
                            }
                       	else
                            {
                            iCurrentScreen = i;
                            }
                        DrawNow();
                        UpdateScrollIndicatorL();
                        break;
                        }
                    default:
                        break;
                    }
                return EKeyWasConsumed;
                }
            } 
        }
        
    if ( aType == EEventKey && iScreenStarts->Count() > 1 )
        {
        switch ( aKeyEvent.iCode )
            {
            case EKeyUpArrow:
                {
                if ( iCurrentScreen > 0 )
                    {
                    iCurrentScreen--;
                    DrawNow();
                    UpdateScrollIndicatorL();
                    }
                }
                break;

            case EKeyDownArrow:
                {
                if ( iCurrentScreen < iScreenStarts->Count() - 1 )
                    {
                    iCurrentScreen++;
                    DrawNow();
                    UpdateScrollIndicatorL();
                    }
                }
                break;

            default:
                break;
            }
        return EKeyWasConsumed;
        }

    // When pressing the Send Key and the event's type is not EEvenKey return EKeyWasNotConsumed
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CAboutContainer::UpdateScrollIndicatorL()
// -----------------------------------------------------------------------------

void CAboutContainer::UpdateScrollIndicatorL()
 {
    if ( iScreenStarts->Count() <= 1 )
    {
        return;
    }

    if ( !iSBFrame )
    {
        iSBFrame = new( ELeave ) CEikScrollBarFrame( this,this, ETrue );
        
		// Decide which type of scrollbar is to be shown
        CAknAppUi* appUi = iAvkonAppUi;
		if (AknLayoutUtils::DefaultScrollBarType(appUi) == CEikScrollBarFrame::EDoubleSpan)
		{
			// For EDoubleSpan type scrollbar
			iSBFrame->CreateDoubleSpanScrollBarsL(ETrue, EFalse); // non-window owning scrollbar            
			iSBFrame->SetTypeOfVScrollBar(CEikScrollBarFrame::EDoubleSpan);
		}
		else
		{
			// For EArrowHead type scrollbar
			iSBFrame->SetTypeOfVScrollBar(CEikScrollBarFrame::EArrowHead);
		}  
		iSBFrame->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
    }

	TEikScrollBarModel hSbarModel;
    TEikScrollBarModel vSbarModel;

    // The member of vSbarModel shoul be changed because the state when dragging scroll bar shoul be considered.
    TInt spanAdjust = iLinesPerScreen - ( iText->Count() - ( *iScreenStarts )[ iScreenStarts->Count() - 1 ] + 1 );
    if ( iScrollBarDragged )
    {
    vSbarModel.iThumbPosition = iCurrentScreen;
    }
    else
    {
        vSbarModel.iThumbPosition = iCurrentScreen * ( iLinesPerScreen - 1 );
    }
    vSbarModel.iScrollSpan = iText->Count() + spanAdjust;
    vSbarModel.iThumbSpan = ( iLinesPerScreen - 1 );
	
	TRect rect(Rect());  
    TEikScrollBarFrameLayout layout;
	layout.iTilingMode = TEikScrollBarFrameLayout::EInclusiveRectConstant;

    if (iSBFrame->TypeOfVScrollBar() == CEikScrollBarFrame::EDoubleSpan)
    {    
        // For EDoubleSpan type scrollbar
        if (vSbarModel.iThumbPosition + vSbarModel.iThumbSpan > vSbarModel.iScrollSpan)
        {
            // Not let scrollbar values overflow
            vSbarModel.iThumbPosition = vSbarModel.iScrollSpan - vSbarModel.iThumbSpan;
        }
		
        TAknDoubleSpanScrollBarModel hDsSbarModel(hSbarModel);
        TAknDoubleSpanScrollBarModel vDsSbarModel(vSbarModel);
       
        // When resolution is changed, changing the drawing area of layout for the text.
        TRect currentRect( iAvkonAppUi->ClientRect() );
        iSBFrame->TileL(&hDsSbarModel, &vDsSbarModel, currentRect, rect, layout);        
        iSBFrame->SetVFocusPosToThumbPos(vDsSbarModel.FocusPosition());
	}  
	else
	{
		iSBFrame->TileL( &hSbarModel, &vSbarModel, rect, rect, layout );
		iSBFrame->SetVFocusPosToThumbPos( vSbarModel.iThumbPosition );
	}
}


// End of File

void CAboutContainer::CalculateL(const TRect& aRect)
    {
    TRect rect( 0, 0, 0, 0 );
    iCurrentScreen = 0;
	
    if ( iSkinContext )
        {
        delete iSkinContext;
        iSkinContext = NULL;
        }  
	
    // Temporary rect is passed. Correct rect is set in SizeChanged.
    iSkinContext = CAknsBasicBackgroundControlContext::NewL(
    KAknsIIDQsnBgAreaMain , rect, EFalse );

    iFont = AknLayoutUtils::FontFromId( EAknLogicalFontSecondaryFont );

    // Calculate various text positioning parameters
    iBaseLineDelta = iFont->HeightInPixels() * 4 / 3;

    TInt mainPaneWidth( aRect.iBr.iX - aRect.iTl.iX );
    TInt mainPaneHeight( aRect.iBr.iY - aRect.iTl.iY );
    // Line width is 87% of client rect, horizontal margins 13%
    iLineWidth = mainPaneWidth * 87 / 100;
    iTopBaseLineX = ( mainPaneWidth - iLineWidth ) / 2;

    // top margin is 6.5% of the client rect
    TInt topMargin = mainPaneHeight * 65 / 1000;    
    iTopBaseLineY = topMargin + iFont->AscentInPixels();

    // minimum bottom margin is 3% of the client rect
    TInt bottomMargin = mainPaneHeight * 3 / 100;
    iLinesPerScreen =
        ( mainPaneHeight - topMargin - bottomMargin ) / iBaseLineDelta;
    
    TInt uiLanguage = User::Language();
    
    if( uiLanguage == ELangArabic || uiLanguage == ELangFarsi || 
        uiLanguage == ELangHebrew || uiLanguage == ELangUrdu )
    	{
        iTextAlign = CGraphicsContext::ERight;
    	}
    else
    	{
        iTextAlign = CGraphicsContext::ELeft;
    	}

   
    if ( iText )
        {
        iText->ResetAndDestroy();
        delete iText;
        iText = NULL;
        }

    // Every text line on screen is one entry in this array
    iText = new( ELeave ) CArrayPtrFlat<HBufC>( 20 );

    if ( iImages )
        {
        iImages->ResetAndDestroy();
        delete iImages;
        iImages = NULL;
        }

    // Every image on screen is one entry in this array
    iImages = new( ELeave ) CArrayPtrFlat<CAboutImage>( 1 );

    if ( iScreenStarts )
        {
        delete iScreenStarts;
        iScreenStarts = NULL;
        }
    // This array contains indices for lines that start the subsequent
    // screens, for custom scrolling
    iScreenStarts = new( ELeave ) CArrayFixFlat<TInt>( 5 );
    // Initialisation: first screen starts at line 0.
    iScreenStarts->AppendL( 0 );

    // Read text and image items to be shown on the screen from a resource file.
#ifdef __ABOUT_USE_TEST_RESOURCE__
    #include <about_test.rsg>
    // test resource
    _LIT(KDirAndFile, "Z:about_test.rsc");
    TParse* fp = new(ELeave) TParse ();
    fp->Set (KDirAndFile, &KDC_APP_RESOURCE_DIR, NULL);
    TBuf<254> KAboutTestResourceFileName;
    KAboutTestResourceFileName.Copy(fp ->FullName());
    delete fp;
    fp=NULL;

    TFileName fileName( KAboutTestResourceFileName );
    BaflUtils::NearestLanguageFile( iEikEnv->FsSession(), filename );
    TInt testResourceFileOffset = iCoeEnv->AddResourceFileL( fileName );
    TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, R_ABOUT_TEST_MAIN_TEXT );
#else
    // real resource
    TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, R_ABOUT_MAIN_TEXT );
#endif
    
    CFbsBitmap* bitmap,*mask;
    bitmap = NULL;
    mask= NULL;
    TFileName bmpPath;
    _LIT( KMbmFileName, "Z:about.mif" );

    TParse* fpMbm = new(ELeave) TParse ();
    fpMbm->Set ( KMbmFileName, &KDC_APP_BITMAP_DIR, NULL );
    bmpPath.Copy( fpMbm ->FullName() );
    delete fpMbm;
    fpMbm=NULL;

    TResourceReader readerSVG;                   
    iEikonEnv->CreateResourceReaderLC( readerSVG, R_ABOUT_SVG_TEXT );
    TInt svgId = readerSVG.ReadInt16();
    TInt svgIdmask = readerSVG.ReadInt16();
    TInt height = readerSVG.ReadInt16();
    TInt width = readerSVG.ReadInt16();
    CleanupStack::PopAndDestroy();      // readerSVG
	  
    MAknsSkinInstance* skinInstance = AknsUtils::SkinInstance();
    AknsUtils::CreateIconL( skinInstance, KAknsIIDQgnMenuSmsvoLst, bitmap, mask, bmpPath, svgId, svgIdmask );	
    //AknsUtils::CreateIconL( skinInstance, KAknsIIDQgnMenuSmsvoLst, bitmap, bmpPath, EMbmAboutQgn_graf_java_logo );	
    if ( iIcon )
        {
        delete( iIcon );
        iIcon = NULL;
        }
    iIcon = CGulIcon::NewL( bitmap,mask );
    
    TRect rect1; 
   	rect1.iTl.iX = 0;
   	rect1.iTl.iY = 0;
   	rect1.iBr.iX = height;
   	rect1.iBr.iY = width;
	  AknIconUtils::SetSize( iIcon->Bitmap(), rect1.Size() ); 
	  AknIconUtils::SetSize( iIcon->Mask(), rect1.Size() );

    TInt lines( bitmap->SizeInPixels().iHeight +
                  KImageTopMargin + 
                  KImageBottomMargin +
                  ( iBaseLineDelta - 1 ) / iBaseLineDelta );

/*        for(TInt i =0; i< 2; i++)
		{
		    iText->AppendL(NULL);    
		}*/
    TInt NumItem( reader.ReadInt16() );
    iNumItem = NumItem;

    for ( TInt i = 0 ; i < iNumItem ; i++ )
        {
        TInt type = reader.ReadInt8();

        if ( type == EAboutTextItem )    
            { 
            HBufC* text;  
            if ( reader.ReadInt8() == EAboutUpdateTime )
                {  
                TTime time;
                time.UniversalTime();
                TDateTime currentTime = time.DateTime();
                text = StringLoader::LoadLC( reader.ReadInt32(), 
                                            currentTime.Year(), CEikonEnv::Static() );
                }
            else
                {
                text = iEikonEnv->AllocReadResourceLC( reader.ReadInt32() );
                }
            SetTextL( *text , i );
            CleanupStack::PopAndDestroy( text );
            }
        else if ( type == EAboutImageItem )
            {
            TPtrC bitmapFile = reader.ReadTPtrC();
            TInt bitmapId = reader.ReadInt16();
            SetImageL( bitmapFile, bitmapId );
            }
        else
            {
            User::Panic( KAboutPanicCategory, EAboutNotSupported );
            }
        }

    CleanupStack::PopAndDestroy(); // reader

#ifdef __ABOUT_USE_TEST_RESOURCE__
    if ( testResourceFileOffset )
        {
        iCoeEnv->DeleteResourceFile( testResourceFileOffset );
        }
#endif

    SetRect( aRect );  
    UpdateScrollIndicatorL();
    }


 void CAboutContainer::HandleResourceChange(TInt aType)
 {

switch ( aType )
  {
  case KEikDynamicLayoutVariantSwitch :
       {
       TRAP_IGNORE(CalculateL(iAvkonAppUi->ClientRect()));   
       DrawNow();
       }
       break;
  case KEikMessageUnfadeWindows :
  case KAknsMessageSkinChange :
       {
       TRAP_IGNORE(iSBFrame->CreateDoubleSpanScrollBarsL(ETrue, EFalse)); // window owning scrollbar            
       iSBFrame->SetTypeOfVScrollBar(CEikScrollBarFrame::EDoubleSpan);
 	   TRAP_IGNORE(iSBFrame->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn));
	   TRAP_IGNORE(UpdateScrollIndicatorL());
       }
        
  default:
       {
       CCoeControl::HandleResourceChange(aType);
       }
	   break;
  }
}  
// ---------------------------------------------------------
// CAboutContainer::MopSupplyObject()
// Pass skin information if need.
// ---------------------------------------------------------

TTypeUid::Ptr CAboutContainer::MopSupplyObject(TTypeUid aId)
    {
    if (aId.iUid == MAknsControlContext::ETypeId && iSkinContext)
        {
        return MAknsControlContext::SupplyMopObject(aId, iSkinContext);
        }

    return CCoeControl::MopSupplyObject(aId);
    }

// ---------------------------------------------------------
// CAboutContainer::HandleScrollEventL()
// Capture Touch Events on the Scroll Bar
// ---------------------------------------------------------
void CAboutContainer::HandleScrollEventL(CEikScrollBar* aScrollBar,TEikScrollEvent aEventType)
{
    //Only on page up/down,scroll up/down and drag events
    if((aEventType == EEikScrollPageDown) || (aEventType == EEikScrollPageUp) || 
       (aEventType == EEikScrollThumbDragVert) || (aEventType == EEikScrollUp) ||
       (aEventType == EEikScrollDown))
    {
        //Get the current position from the scroll bar
        iCurrentScreen = aScrollBar->ThumbPosition();
        
        // Reset flag when dragging scroll bar
        iScrollBarDragged = ETrue;
        //Refresh now
        DrawNow();
        UpdateScrollIndicatorL();
    }
    
}
