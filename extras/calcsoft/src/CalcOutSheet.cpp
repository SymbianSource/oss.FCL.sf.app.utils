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
* Description:  The OutputSheet sub-pane class, CCalcOutputSheet.
*                Derived from CCoeControl.
*                The CCalcOutputSheet displays histroy data.
*                History data is scroll up and down.
*
*/



// INCLUDE FILES
#include    <AknUtils.h>
#include    <Calcsoft.rsg>
#include    <applayout.cdl.h>
#include 	<AknsDrawUtils.h>
#include	<layoutmetadata.cdl.h>
#include	"CalcEnv.h"
#include    "CalcCont.h"    
#include    "CalcOutSheet.h"
#include    "CalcDoc.h"
#include    "CalcEditline.h"
#include    "CalcAppUi.h"
#include    "CalcHistory.h"
#include    "CalcDrawingConst.laf"
#include "fbs.h"
#include <AknIconUtils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknFepInternalCRKeys.h>


#include <eiksbfrm.h> 
#include <eikscrlb.h>

//  LOCAL CONSTANTS AND MACROS  

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CCalcOutputSheet* CCalcOutputSheet::NewL(
                    CCalcContainer* aContainer)
    {
    CCalcOutputSheet* self = new (ELeave) CCalcOutputSheet();
    CleanupStack::PushL(self);
    self->ConstructL(aContainer);
    CleanupStack::Pop(self); 
    return self;
    }

// Destructor
CCalcOutputSheet::~CCalcOutputSheet()
    {
    delete iOpSheetContext;
    if(iOperatorLayout)
	    {
	    iOperatorLayout->Reset();
	    delete iOperatorLayout;	
	    }
	if(iOperandLayout)
	    {
	    iOperandLayout->Reset();
	    delete iOperandLayout;	
	    }
	if(iEqualLineLayout)
	    {
	    iEqualLineLayout->Reset();
	    delete iEqualLineLayout;	
	    }
	if(iResultsLineLayout)
	    {
	    iResultsLineLayout->Reset();
	    delete iResultsLineLayout;	
	    }
	if (iCRKey)
		{
		delete iCRKey;
		iCRKey = NULL;
		}
		
	//Delete the scrollbar frame
	if(iSBFrame)
	    {
	    delete iSBFrame;
	    iSBFrame = NULL;
	    }
    }

// ---------------------------------------------------------
// CCalcOutputSheet::ScrollToBottomL
// Scroll to bottom line.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::ScrollToBottomL()
    {
    iScrollOffset = 0;
    
    //If scrollbar is present, update the scrollbar as well
     if( AknLayoutUtils::PenEnabled()  )
        {
          UpdateScrollIndicatorL();
        }
    DrawNow();
    }

// ---------------------------------------------------------
// CCalcOutputSheet::NotifyClearHistory
// Called after clearing calculation history.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::NotifyClearHistoryL()
    {
    iScrollOffset = 0;
    if( AknLayoutUtils::PenEnabled()  )
    {
    	UpdateScrollIndicatorL();	
    }
    DrawNow();
    }
        

// ---------------------------------------------------------
// CCalcOutputSheet::CanUp
// Check whether scroll-up can be done or not.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCalcOutputSheet::CanUp() const
    {
    // The number of lines of the history which a document has is got.
    TInt historyLine(iCalcHistory->Count());
    if ( (iCalcOutputSheetLines + iScrollOffset) < historyLine )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------
// CCalcOutputSheet::CanDown
// Check whether scroll-down can be done or not.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCalcOutputSheet::CanDown() const
    {
    // The number of lines of the history which a document has is got.
    return (iScrollOffset != 0);
    }


// ---------------------------------------------------------
// CCalcOutputSheet::ScrollUp
// history is scrolled up.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::ScrollUp()
    {
   
    if (CanUp())
        {
        iScrollOffset++;
        DrawNow();
        }
    }

// ---------------------------------------------------------
// CCalcOutputSheet::ScrollDown
// history is scrolled down.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::ScrollDown()
    {

    if (CanDown())
        {
        iScrollOffset--;
        DrawNow();
        }
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCalcOutputSheet::CCalcOutputSheet() 
    {
    }

// default constructor can leave.
void CCalcOutputSheet::ConstructL
        (CCalcContainer* aContainer) 
                     
    {
    SetContainerWindowL(*aContainer);

    iCalcContainer = aContainer;
    
    iCalcAppEnv = CCalcAppEnv::Static();
    iCalcPaper = iCalcAppEnv->PaperBitmap();
	iCalcAppEnv->ScalablePaperBitmaps(iCalcScalablePaper);
#ifdef __SCALABLE_ICONS
	iCalcResultsLine = iCalcAppEnv->ResultsLineBitmap();
#endif

    CCalcAppUi* appui = iCalcAppEnv->AppUi();
    CCalcDocument* calcDocument = 
                   STATIC_CAST(CCalcDocument*, appui->Document());
    iCalcHistory = calcDocument->History(); 
    TAknLayoutScalableParameterLimits listLimits;
    listLimits = AknLayoutScalable_Apps::list_calc_item_pane_ParamLimits();
    iCalcOutputSheetLines   = (listLimits.LastRow()-listLimits.FirstRow()) + 1;
    iOpSheetContext = CAknsBasicBackgroundControlContext::NewL(KAknsIIDQsnFrCalcPaper , TRect(0,0,0,0), EFalse);
    iOperatorLayout = new(ELeave)CArrayFixFlat<TAknLayoutText>(iCalcOutputSheetLines);
    iOperandLayout = new(ELeave)CArrayFixFlat<TAknLayoutText>(iCalcOutputSheetLines);
    iResultsLineLayout = new(ELeave)CArrayFixFlat<TAknLayoutRect>(iCalcOutputSheetLines-1);
    iEqualLineLayout = new(ELeave)CArrayFixFlat<TAknLayoutRect>(iCalcOutputSheetLines-1);
    iUiLanguage = User::Language();
    }

// ---------------------------------------------------------
// CCalcOutputSheet::SizeChanged
// Set size of control.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::SizeChanged()
    {
    TRAP_IGNORE( SizeChangedOutSheetL() );
    }

// ---------------------------------------------------------
// CCalcOutputSheet::SizeChanged
// Set size of control.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::SizeChangedOutSheetL()
    {
    TRect parentRect(iCalcContainer->Rect());
    
    TAknLayoutScalableParameterLimits listLimits;
    if(AknLayoutUtils::PenEnabled())
    {
        listLimits = AknLayoutScalable_Apps::list_calc_item_pane_ParamLimits(enTouch_enabled);    
    }
    else
    {
        listLimits = AknLayoutScalable_Apps::list_calc_item_pane_ParamLimits(enTouch_disabled);    
    }
    iCalcOutputSheetLines   = (listLimits.LastRow()-listLimits.FirstRow()) + 1;
    iOperatorLayout->ResizeL(iCalcOutputSheetLines);
	iOperandLayout->ResizeL(iCalcOutputSheetLines);
	iResultsLineLayout->ResizeL(iCalcOutputSheetLines-1);
	iEqualLineLayout->ResizeL(iCalcOutputSheetLines-1);
	
    iOperatorLayout->Reset();
	iOperandLayout->Reset();
	iResultsLineLayout->Reset();
	iEqualLineLayout->Reset();
    
    TAknLayoutRect rectParent;
	TAknWindowLineLayout layoutOfParentResultsLine;
	TAknLayoutRect rectListCalcPane;
	
 
    
    if( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
		{
			TAknWindowLineLayout  calcPaperPane;
			
			//For Touch UI Layouts, different configuration
			if( AknLayoutUtils::PenEnabled() )
	        {
	            if (Layout_Meta_Data::IsLandscapeOrientation())
	            {
	                calcPaperPane = AknLayoutScalable_Apps::calc_paper_pane(enTouch_with_lsc).LayoutLine();
	            }
	            else
	            {
	                calcPaperPane = AknLayoutScalable_Apps::calc_paper_pane(enTouch_with_prt).LayoutLine();
	            }
	        }
	        else
	        {
		        calcPaperPane = AknLayoutScalable_Apps::calc_paper_pane(enTouch_disabled).LayoutLine();
	        }
			TAknLayoutRect rectcalcPaperPane;	
			rectcalcPaperPane.LayoutRect(parentRect,calcPaperPane);
			TAknWindowLineLayout layoutListCalcPane;
			if( AknLayoutUtils::PenEnabled() )
	        {
	            layoutListCalcPane = AknLayoutScalable_Apps::list_calc_pane(enTouch_enabled).LayoutLine();
	        }
	        else
	        {
	            layoutListCalcPane = AknLayoutScalable_Apps::list_calc_pane(enTouch_disabled).LayoutLine();
	        }
			
			rectListCalcPane.LayoutRect(rectcalcPaperPane.Rect(),layoutListCalcPane);
		}
    
	TInt nTimes=(iCalcOutputSheetLines-1);
    for (TInt cnt(0); cnt < iCalcOutputSheetLines; cnt++)
        {
        if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
		{
		if(AknLayoutUtils::PenEnabled())
		{
			if (Layout_Meta_Data::IsLandscapeOrientation())
			{
				layoutOfParentResultsLine =  AknLayoutScalable_Apps::list_calc_item_pane(nTimes,enTouch_enabled).LayoutLine();	
			}
			else
			{
				layoutOfParentResultsLine =  AknLayoutScalable_Apps::list_calc_item_pane(nTimes,enTouch_enabled).LayoutLine();
			}
	
		}
		else
		{
		    layoutOfParentResultsLine =  AknLayoutScalable_Apps::list_calc_item_pane(nTimes,enTouch_disabled).LayoutLine();    
		}
		
		
		rectParent.LayoutRect(rectListCalcPane.Rect(),layoutOfParentResultsLine) ;
			
		TAknLayoutText operatorLayout;
		TAknLayoutText operendLayout;
		if(AknLayoutUtils::PenEnabled())
		{
		    operatorLayout.LayoutText(rectParent.Rect(),AknLayoutScalable_Apps::list_calc_item_pane_t2(enTouch_with_prt).LayoutLine(),iCalcAppEnv->OutSheetOperatorFont()); 
		    iOperatorLayout->AppendL(operatorLayout);
		
		    operendLayout.LayoutText(rectParent.Rect(),AknLayoutScalable_Apps::list_calc_item_pane_t1(enTouch_with_lsc).LayoutLine(),iCalcAppEnv->OutSheetOperatorFont()); 
		    iOperandLayout->AppendL(operendLayout);
		}
		else
		{
		    operatorLayout.LayoutText(rectParent.Rect(),AknLayoutScalable_Apps::list_calc_item_pane_t2(enTouch_disabled).LayoutLine(),iCalcAppEnv->OutSheetOperatorFont()); 
		    iOperatorLayout->AppendL(operatorLayout);
		
		    operendLayout.LayoutText(rectParent.Rect(),AknLayoutScalable_Apps::list_calc_item_pane_t1(enTouch_disabled).LayoutLine(),iCalcAppEnv->OutSheetOperatorFont()); 
		    iOperandLayout->AppendL(operendLayout);    
		}
		
	  // aCustomFont
     --nTimes;
		
		}
	  
	  else
	  {
	  	  
	    
	  //Scalable UI.

	  	TAknLayoutText operatorLayout;
		TAknLayoutText operendLayout;
	  	operatorLayout.LayoutText(parentRect,AppLayout::Calculator_texts_Line_8(nTimes),iCalcAppEnv->OutSheetOperatorFont()); // aCustomFont
	    iOperatorLayout->AppendL(operatorLayout);        

		operendLayout.LayoutText(parentRect,AppLayout::Calculator_texts_Line_7(nTimes), NULL);
		iOperandLayout->AppendL(operendLayout);            
     --nTimes;
	  }
    }
    
    //Scalable UI.
	nTimes = iCalcOutputSheetLines - 2;	  
    for (TInt ii(0); ii < (iCalcOutputSheetLines - 1); ii++)
        {


		if( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
			{
#ifdef __SCALABLE_ICONS		
			layoutOfParentResultsLine =  AknLayoutScalable_Apps::list_calc_item_pane(nTimes).LayoutLine();

			rectParent.LayoutRect(rectListCalcPane.Rect(),layoutOfParentResultsLine) ;
			TAknWindowLineLayout   layoutOfResultsLine = AknLayoutScalable_Apps::list_calc_item_pane_g1().LayoutLine(); 
			TAknLayoutRect lineLayout;
			lineLayout.LayoutRect(rectParent.Rect(),layoutOfResultsLine);
			iResultsLineLayout->AppendL(lineLayout);
#else
			TAknLayoutRect lineLayout;
			lineLayout.LayoutRect(parentRect,AppLayout::Calculator_elements_Line_3(nTimes)); 
			iEqualLineLayout->AppendL(lineLayout);
#endif
			}
			else
			{
			TAknLayoutRect lineLayout;
			lineLayout.LayoutRect(parentRect,AppLayout::Calculator_elements_Line_3(nTimes)); 
			iEqualLineLayout->AppendL(lineLayout);
			}
			
		--nTimes;
        }

    TAknLayoutRect  paperRect;
	if( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
			{
			TAknLayoutRect rectCalcPaperPane;	
			TAknLayoutRect rectbgCalcPaperPane;
			//Different configuration for Touch Layouts
			if( AknLayoutUtils::PenEnabled() )
			{
					//In landscape mode different configuration is needed
			     if (Layout_Meta_Data::IsLandscapeOrientation())
			     {
			        rectCalcPaperPane.LayoutRect(parentRect,AknLayoutScalable_Apps::calc_paper_pane(enTouch_with_lsc).LayoutLine());  
			     }
			     else
			     {
			        rectCalcPaperPane.LayoutRect(parentRect,AknLayoutScalable_Apps::calc_paper_pane(enTouch_with_prt).LayoutLine());      
			     }
			     rectbgCalcPaperPane.LayoutRect(rectCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane(enTouch_enabled).LayoutLine());
			}
			else
			{
			    rectCalcPaperPane.LayoutRect(parentRect,AknLayoutScalable_Apps::calc_paper_pane(enTouch_disabled).LayoutLine());    
			    rectbgCalcPaperPane.LayoutRect(rectCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane().LayoutLine());
			}
			
			iPaperPaneRect  = rectCalcPaperPane;
			
			
			iCalcPaperLayout[0].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g1().LayoutLine());
			iCalcPaperLayout[1].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g2().LayoutLine());
			iCalcPaperLayout[2].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g3().LayoutLine());
			iCalcPaperLayout[3].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g4().LayoutLine());
		    iCalcPaperLayout[4].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g5().LayoutLine());
			iCalcPaperLayout[5].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g6().LayoutLine());
			iCalcPaperLayout[6].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g7().LayoutLine());
    		iCalcPaperLayout[7].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::bg_calc_paper_pane_g8().LayoutLine());
			iCalcPaperLayout[8].LayoutRect(rectbgCalcPaperPane.Rect(),AknLayoutScalable_Apps::calc_bg_paper_pane_g9().LayoutLine());
			}
			
	   iOpSheetContext->SetRect(Rect());
		
		//Only for touch UI, scrollbars are present
	   if( AknLayoutUtils::PenEnabled()  )
        {
          UpdateScrollIndicatorL();
        }
        else
        {
        if(iSBFrame)
        {
            delete(iSBFrame);
            iSBFrame = NULL;
        }
        
        }
		
		}


// ---------------------------------------------------------
// CCalcOutputSheet::UpdateScrollIndicatorL
// Creates and updates the scrollbar
// (other items were commented in a header).
// ---------------------------------------------------------
void CCalcOutputSheet::UpdateScrollIndicatorL()
    {
    	
    	if( !AknLayoutUtils::PenEnabled()  )
    	{
    		return;
    	}
    	
        TAknLayoutRect rectCalcScrollPane;	

        //Get the layout information for the scrollbar
        if( AknLayoutUtils::PenEnabled() )
        {
         if (Layout_Meta_Data::IsLandscapeOrientation())
         {
            rectCalcScrollPane.LayoutRect(iPaperPaneRect.Rect(),AknLayoutScalable_Apps::scroll_pane_cp024().LayoutLine());  
         }
         else
         {
            rectCalcScrollPane.LayoutRect(iPaperPaneRect.Rect(),AknLayoutScalable_Apps::scroll_pane_cp024().LayoutLine());      
         }

        }
				//Create scrollbar if not created already
        if ( !iSBFrame )
        {
        //Make this class as the observer class as well
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
        
        //By default span is one if only one page is present
        if(iCalcHistory->Count() < iCalcOutputSheetLines)
        {
            vSbarModel.iScrollSpan = 1;    
            vSbarModel.iThumbPosition = 0;    
            
        }
        else
        {
        		//span calculator if no of lines exceeds one page
            vSbarModel.iScrollSpan = 1 + iCalcHistory->Count() - iCalcOutputSheetLines;  
        }

        iPrevThumbPosition = vSbarModel.iThumbPosition = vSbarModel.iScrollSpan - 1;
        vSbarModel.iThumbSpan = 1;

        TRect clientRect( iAvkonAppUi->ClientRect() );
        TRect rec(rectCalcScrollPane.Rect()); 

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

        iSBFrame->TileL(&hDsSbarModel, &vDsSbarModel, clientRect, rec, layout);               
        iSBFrame->SetVFocusPosToThumbPos(vDsSbarModel.FocusPosition());
        }  
        else
        {
        iSBFrame->TileL( &hSbarModel, &vSbarModel, clientRect, rec, layout );
        iSBFrame->SetVFocusPosToThumbPos( vSbarModel.iThumbPosition );
        }
        }
        

// ---------------------------------------------------------
// CCalcOutputSheet::Draw
// Drawing frame and history.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::Draw
        (const TRect& aRect) const
    {
    
    CWindowGc& gc = SystemGc();
	gc.Clear(aRect);
    gc.SetClippingRect(aRect);
    TRect rect(Rect());
    

    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
    AknsDrawUtils::Background( AknsUtils:: SkinInstance(), AknsDrawUtils::ControlContext(this), this, gc, rect );
        
           
    gc.SetBrushStyle(CGraphicsContext::ENullBrush);
    gc.SetPenStyle(CGraphicsContext::ENullPen);
	//set the size of the bitmap. SVG-T upgrade.
	CAknsItemData* idata = NULL;    				
    if (skin)
    	{
        idata = skin->GetCachedItemData( KAknsIIDQsnFrCalcPaper);
    	}
    	
    if ( idata && idata->Type() == EAknsITImageTable )
    	{
            if ( AknLayoutUtils::PenEnabled() ) 
            {//when touch input,the paper become bigger,so it needs only one paper 
                AknIconUtils::SetSize(iCalcScalablePaper[0]->Bitmap(),iCalcPaperLayout[0].Rect().Size(),EAspectRatioNotPreserved); 
                AknIconUtils::SetSize(iCalcScalablePaper[0]->Mask(),iCalcPaperLayout[0].Rect().Size(),EAspectRatioNotPreserved);
                iCalcPaperLayout[0].DrawImage(gc, iCalcScalablePaper[0]->Bitmap(),iCalcScalablePaper[0]->Mask());	
            }
            else
            {
                for(TInt Cnt=0;Cnt < 9; Cnt++)
            	{
                AknIconUtils::SetSize(iCalcScalablePaper[Cnt]->Bitmap(),iCalcPaperLayout[Cnt].Rect().Size(),EAspectRatioNotPreserved); 
            	AknIconUtils::SetSize(iCalcScalablePaper[Cnt]->Mask(),iCalcPaperLayout[Cnt].Rect().Size(),EAspectRatioNotPreserved);
            	iCalcPaperLayout[Cnt].DrawImage(gc, iCalcScalablePaper[Cnt]->Bitmap(),iCalcScalablePaper[Cnt]->Mask());
                }
            }
  		}
    else
        {
            // Make outsheet displayed in Jingmask is the same size as in other themes
            // when there is a scrollbar on outsheet.
            if ( AknLayoutUtils::PenEnabled() )
                {
                AknsDrawUtils::Background( AknsUtils:: SkinInstance(), iOpSheetContext, this, gc, iCalcPaperLayout[0].Rect() );
                }
            else
                {
                AknsDrawUtils::Background( AknsUtils:: SkinInstance(), iOpSheetContext, this, gc, rect );
                }	
        }
  		
	
    for (TInt loop(iCalcOutputSheetLines - 1); loop >= 0; loop--)
        {
        const TCalcEditLine& history = (*iCalcHistory)[loop + iScrollOffset];
        TCalcEditLine::TCalcOperatorType operatorType(
            history.Operator());
        
        if (loop < iCalcOutputSheetLines - 1)
            {
			// When an operator is Equal, a line is drawn at an upper end.
			if (TCalcEditLine::ECalcEqual == operatorType)
				{
					TRgb ResultsLineolor;
                    TInt error1 = AknsUtils::GetCachedColor( skin, ResultsLineolor, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG29);
                    if(error1 == KErrNone)
                    {
                        AknIconUtils::SetIconColor(iCalcResultsLine->Bitmap(),ResultsLineolor);
                    }
				if( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
					{
#ifdef __SCALABLE_ICONS
					TAknLayoutRect& lineLayout = iResultsLineLayout->At(loop);
					AknIconUtils::SetSize(iCalcResultsLine->Bitmap(),lineLayout.Rect().Size(),EAspectRatioNotPreserved);
					AknIconUtils::SetSize(iCalcResultsLine->Mask(),lineLayout.Rect().Size(),EAspectRatioNotPreserved);
					lineLayout.DrawImage(gc, iCalcResultsLine->Bitmap(),iCalcResultsLine->Mask()); 
#else
					TAknLayoutRect& equalLineLayout= iEqualLineLayout->At(loop);
					TRect rectLine = equalLineLayout.Rect();
					equalLineLayout.DrawRect(gc);
#endif
					}
				else 
					{ 
					TAknLayoutRect& equalLineLayout= iEqualLineLayout->At(loop);
					TRect rectLine = equalLineLayout.Rect();
					equalLineLayout.DrawRect(gc);
					}
				}
            }
            TRgb color;
            TInt error = AknsUtils::GetCachedColor( skin, color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG29);
			if(error == KErrNone)
			{
				gc.SetPenColor(color);
			}
			
		//change TBuf<10> to TBuf<KCalcMaxNumberWidth>
       TBuf<KCalcMaxNumberWidth> keyvalue;
       
       keyvalue.Append(history.NumberString());
       AknTextUtils::LanguageSpecificNumberConversion  (keyvalue);
        // An operand's drawing
		TAknLayoutText& operendLayout = iOperandLayout->At(loop);

                    if ( iUiLanguage == ELangFarsi || iUiLanguage == ELangHindi || 
                        iUiLanguage == ELangUrdu || iUiLanguage == ELangArabic )
                        {
                         if( iUiLanguage == ELangUrdu || iUiLanguage == ELangArabic )
		         {
			    	TInt len = keyvalue.Length();
				    TInt pos = KErrNotFound;
				    TBuf<1> ch;
				    ch.Append(iCalcAppEnv->MinusIndicator());
				    if( (pos = keyvalue.Locate(iCalcAppEnv->MinusIndicator())) != KErrNotFound &&
				         pos == keyvalue.LocateReverse(iCalcAppEnv->MinusIndicator()))
				    {
				     keyvalue.Delete( pos, 1 );
				     keyvalue.Insert( 0, ch );	 
				    }
		         }
			operendLayout.DrawText(gc,keyvalue,EFalse,color);
		    }
		else
			{
			operendLayout.DrawText(gc,history.NumberString(),EFalse,color);	
			}
        // An operator's drawing
        TPtrC operatorString(
            iCalcAppEnv->OutSheetOperator(operatorType));
		TAknLayoutText& operatorLayout = iOperatorLayout->At(loop);
		operatorLayout.DrawText(gc, operatorString , EFalse, color);
        }
        
        
    }

// ---------------------------------------------------------
// CCalcOutputSheet::HandleScrollEventL
// Handles the events from the scrollbar
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType)
    {
          //Only on page up/down,scroll up/down and drag events
    if((aEventType == EEikScrollPageDown) || (aEventType == EEikScrollPageUp) || 
       (aEventType == EEikScrollThumbDragVert) || (aEventType == EEikScrollUp) ||
       (aEventType == EEikScrollDown))
       
        {
           //Get the current position from the scroll bar
           
           if(iPrevThumbPosition >aScrollBar->ThumbPosition() )
           {
           		  //Move the scroll bar n times up
                while(iPrevThumbPosition != aScrollBar->ThumbPosition())
                {
                    ScrollUp();
                    iPrevThumbPosition--;
                }
           }
           else
           {
                //Move the scroll bar n times down
                while( iPrevThumbPosition != aScrollBar->ThumbPosition() )
                {
                    ScrollDown();
                    iPrevThumbPosition++;
                }
           }

           DrawNow();
        }
    }
    
// ---------------------------------------------------------
// CCalcOutputSheet::HandleResourceChange
// Handles the events that needs to be taken care after resource change
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::HandleResourceChange(TInt aType)
{
    TRAP_IGNORE(HandleResourceChangeOutSheetL(aType) );
    }

// ---------------------------------------------------------
// CCalcOutputSheet::HandleResourceChangeOutSheetL
// Handles the events that needs to be taken care after resource change
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCalcOutputSheet::HandleResourceChangeOutSheetL(TInt aType)

{
  
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        if ( AknLayoutUtils::PenEnabled() )
            {
            ScrollToBottomL();
            }
        }
	if(aType == KAknsMessageSkinChange)
	{
		if( AknLayoutUtils::PenEnabled()  )
        {
        	delete(iSBFrame);
    		iSBFrame = NULL;
	
		    UpdateScrollIndicatorL();
        }
	}
}
//  End of File  
