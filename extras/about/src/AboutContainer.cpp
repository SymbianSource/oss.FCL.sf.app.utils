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
#include "AboutResourceLoader.h"
#include "AboutResource.h"
#include <layoutmetadata.cdl.h>
#include <about.rsg>
#include <about_oss_text.rsg>
#include <about_oss_part_text.rsg>
#include <about_oss_other_text.rsg>
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
#include <AknsBasicBackgroundControlContext.h>
#include <AknsConstants.h>
#include <AknDef.h>
#include <about.mbg>
#include <StringLoader.h>
#include <featmgr.h>

// CONSTANTS
const TInt KAboutOSSIndex = 14; // For adding blank break between OSS Text and real Text

// The index of dolby-plus�s information in rss file and is used to decide whether to load dolby-plus�s information in about application
const TInt KAboutDDPlusInfoIndex = 6;

_LIT( KAboutPanicCategory, "About" );
// Resource files for about application loading real data because this rsc file must be loaded by yourself
_LIT( KRSCDirAndFileFirst, "Z:\\resource\\apps\\about_oss_text.rsc" );
_LIT( KRSCDirAndFile, "Z:\\resource\\apps\\about_oss_part_text.rsc" );
_LIT( KRSCDirAndFileAdd, "Z:\\resource\\apps\\about_oss_other_text.rsc" );


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

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CAboutContainer::CAboutContainer()
// -----------------------------------------------------------------------------
CAboutContainer::CAboutContainer()
    {
    }

// -----------------------------------------------------------------------------
// CAboutContainer::ConstructL()
// -----------------------------------------------------------------------------
void CAboutContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    iScrollBarDragged = EFalse;
    iBreakFlag = EFalse;
    iSkinContext = NULL;
    iText = NULL;
    iImages = NULL;
    iScreenStarts = NULL;
    iSBFrame = NULL;
    FeatureManager::InitializeLibL();
    iLoader = CAboutResourceLoader::NewL( iEikonEnv, this );
    CalculateL(aRect); 
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CAboutContainer::~CAboutContainer()
// -----------------------------------------------------------------------------
CAboutContainer::~CAboutContainer()
    {
    FeatureManager::UnInitializeLib();
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
    
    if ( iLoader )
        {
        iLoader->Cancel();
        delete iLoader;
        iLoader = NULL;
        }
    
    }

// -----------------------------------------------------------------------------
// CAboutContainer::SizeChanged()
// -----------------------------------------------------------------------------
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
		{
		//Draw the skin background
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

            // Draw javalogo on Screeen (PictureType: svg)
            gc.BitBltMasked( position, image->Bitmap(), aRect, 
                    image->BitmapMask(), ETrue );
            }
        }
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

		switch( aItem )
			{
			// Between About real text and oss text
			case KAboutOSSIndex:
				{
				iBreakFlag = ETrue;
				break;
				}
			default:
				break;
			}
        if(!line->Length())
            {
            iText->AppendL( NULL );
			
            CleanupStack::PopAndDestroy(line);  // line
            }
        else
            {
            if( iBreakFlag && ( numLines - 1 ) == i )
            	{
                iText->AppendL( line );
                // Get the text's lines' count for set break
                TInt textCount = iText->Count();
                TInt count = iLinesPerScreen - ( textCount % ( iLinesPerScreen - 1 ) ) - 1;
                if( iLinesPerScreen == count + 1 )
                	{
                	// When the last line has content, we should set count zero 
                	count = 0;
                	}
                for( TInt j = 0; j < count; j++ )
                	{
                	iText->AppendL( NULL );
                	}
                CleanupStack::Pop( line );  // line
                iBreakFlag = EFalse;
            	}
            else
            	{
                iText->AppendL( line );
                CleanupStack::Pop( line );  // line
            	}
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
        if ( ( lastLine == screenStart + 1 ) && ( aItem == iLoader->ItemCount() - 1 ) )
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

    // If flag is ETrue, the file type is bmp 
    CAboutImage* image =
    CAboutImage::NewLC( aFileName, aBitmapId, firstLineOfImage, iBaseLineDelta,
            EFalse );

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
                            // Do nothing if the scroll bar is on the bottom, 
                            // when pressing arrow down.
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

// -----------------------------------------------------------------------------
// CAboutContainer::CalculateL()
// -----------------------------------------------------------------------------
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
    TInt resourceId = 0;
    resourceId = R_ABOUT_MAIN_TEXT;
    iResourcePath.Zero();
    iLoader->SetResourceId( resourceId, ETrue, iResourcePath );
    iLoader->StartL();

#ifdef __ABOUT_USE_TEST_RESOURCE__
    if ( testResourceFileOffset )
        {
        iCoeEnv->DeleteResourceFile( testResourceFileOffset );
        }
#endif

    SetRect( aRect );  
    UpdateScrollIndicatorL();
    }

// -----------------------------------------------------------------------------
// CAboutContainer::HandleResourceChange()
// -----------------------------------------------------------------------------
 void CAboutContainer::HandleResourceChange(TInt aType)
 {

switch ( aType )
  {
  case KEikDynamicLayoutVariantSwitch :
       {
            TRAP_IGNORE( CalculateL( iAvkonAppUi->ClientRect() ) );
       DrawNow();
       }
       break;
  case KEikMessageUnfadeWindows :
  case KAknsMessageSkinChange :
       {
            // window owning scrollbar
            TRAP_IGNORE( iSBFrame->CreateDoubleSpanScrollBarsL( ETrue, EFalse ) ); 
       iSBFrame->SetTypeOfVScrollBar(CEikScrollBarFrame::EDoubleSpan);
            TRAP_IGNORE( iSBFrame->SetScrollBarVisibilityL( 
                    CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn ) );
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
void CAboutContainer::HandleScrollEventL( CEikScrollBar* aScrollBar, 
        TEikScrollEvent aEventType )
{
    //Only on page up/down,scroll up/down and drag events
    if((aEventType == EEikScrollPageDown) || (aEventType == EEikScrollPageUp) || 
       (aEventType == EEikScrollThumbDragVert) || (aEventType == EEikScrollUp) ||
       ( aEventType == EEikScrollDown ) )
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

// ---------------------------------------------------------
// CAboutContainer::HandleItemsLoadedL()
// Handle resource load Items event.
// ---------------------------------------------------------
void CAboutContainer::HandleItemsLoadedL( TInt aError )
    {
    // Panic if error occurred.
    if ( aError != KErrNone )
        {
        User::Panic( KAboutPanicCategory, aError );
        }
    // Display loaded texts
    RPointerArray<CAboutResource>* items = iLoader->LoadedResourceItems();
    if ( items )
        {
        for ( TInt i = 0; i < items->Count(); i++ )
            {
            CAboutResource* item = ( *items )[i];
            if ( item->ResourceType() == EAboutTextItem )
                {
                if ( R_ABOUT_MAIN_TEXT == iLoader->CurrentResourceId() )
                    {
                    // Since the description of this DD-plus in the sixth item(in rss file)
                    // Here we need to check if it supports DD-plus
                    if ( KAboutDDPlusInfoIndex == item->ResourceIndex() )
                        {
                        // To determine whether the dolby exists by flag KFeatureIdFfVideoAudioBranding
                        if ( FeatureManager::FeatureSupported( KFeatureIdFfVideoAudioBranding ) )
                            {
                            SetTextL( *item->ResourceItem(), item->ResourceIndex() );
                            }
                        }
                    else
                        {
                        SetTextL( *item->ResourceItem(), item->ResourceIndex() );
                        }
                    if ( iLoader->ItemCount() == item->ResourceIndex() + 1 )
                        {
                        iFinalCount = iLoader->ItemCount() + 1;
                        }
                    }
                else
                    {
                    SetTextL( *item->ResourceItem(), iFinalCount );
                    iFinalCount++;
                    }
                }
            else if( item->ResourceType() == EAboutImageItem )
                {
                SetImageL( *item->ResourceItem(), item->BitmapId() );
                }
            }
        iLoader->ReleaseLoadedItems();
        UpdateScrollIndicatorL();
        DrawDeferred();
        }
    }

// ---------------------------------------------------------
// CAboutContainer::HandleResourceLoadCompletedL()
// Handle resource load Completed event.
// ---------------------------------------------------------
void CAboutContainer::HandleResourceLoadCompletedL( 
                                TInt aResourceId, TInt aError )
    {
    // Panic if error occurred.
    if ( aError != KErrNone )
        {
        User::Panic( KAboutPanicCategory, aError );
        }
    
    // R_ABOUT_OSS_TEXT_ADD is the last one we loaded, 
    // do nothing here when it loaded.
    if ( R_ABOUT_OSS_TEXT_ADD == aResourceId )
        {
        return;
        }
    
    // Load other resources including R_ABOUT_OSS_TEXT and R_ABOUT_OSS_TEXT_ADD
    TInt resourceId( aResourceId );
    if( R_ABOUT_MAIN_TEXT == resourceId )
        {
        // Load the R_ABOUT_OSS_TEXT_FIRST
        // Set the resource which you want to load next
        resourceId = R_ABOUT_OSS_TEXT_FIRST;
        // Set the resource's path
        iResourcePath.Copy( KRSCDirAndFileFirst );
        }
    else if ( R_ABOUT_OSS_TEXT_FIRST == resourceId )
        {
        // Load the R_ABOUT_OSS_TEXT
        resourceId = R_ABOUT_OSS_TEXT;
        iResourcePath.Copy( KRSCDirAndFile );
        }
    else
        {
        // Load the R_ABOUT_OSS_TEXT_ADD
        resourceId = R_ABOUT_OSS_TEXT_ADD;
        iResourcePath.Copy( KRSCDirAndFileAdd );
        }
    
    // Load to complete a resource and continue to load next resources
    iLoader->SetResourceId( resourceId, EFalse, iResourcePath );
    iLoader->StartL();
    }

// End of File
