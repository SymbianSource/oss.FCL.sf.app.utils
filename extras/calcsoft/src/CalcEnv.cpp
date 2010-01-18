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
* Description:  Global data class, CCalcAppEnv, derived from CBase
*                This class maintain global data. e.g font, bitmap.
*
*/



// INCLUDE FILES
#include <barsread.h> // resource reader
#include <eikapp.h>
#include <eikenv.h>
#include <eikrutil.h> 
#include <fbs.h>    // Fonts
#include <AknUtils.h>

//Extended Icon Skin Implementation
#include  <AknsConstants.h>
#include <AknsUtils.h>
#include <AknsItemID.h>

#include <Calcsoft.rsg>
#include <calcsoft.mbg>
#include "CalcEnv.h"
#include "CalcAppUi.h"

//  LOCAL CONSTANTS AND MACROS  

// ================= MEMBER FUNCTIONS =======================
// Two-phased constructor.
CCalcAppEnv* CCalcAppEnv::NewL
(CCalcAppUi* aAppUi)    
	{
	CCalcAppEnv* self = new (ELeave) CCalcAppEnv(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructAppEnvL();
	CleanupStack::Pop(self);
	// Set to TLS(Thread Local Storage).
	UserSvr::DllSetTls(67338721, self); 

	return self;     	
	}

void CCalcAppEnv::InitializeAppUI(CCalcAppUi* aAppUi)
	{
	iAppUi = aAppUi;
	}


// Destructor
CCalcAppEnv::~CCalcAppEnv()
	{
	// Text
	delete iEditorOperArray;
	delete iOutSheetOperArray;
#ifdef __SCALABLE_ICONS
	delete  iResultsLineBitmap;
#endif


	// Bitmap
    	delete iDisplaySideLBitmap;
		delete iDisplayCenterBitmap;
		delete iDisplaySideRBitmap;
		
	delete iPaperBitmap;
	for(TInt Cnt=0;Cnt<9;Cnt++)
		{
	delete iScalablePaperBitmap[Cnt];
		} 
    
	// font 
	// free font for number and operator
	CBitmapDevice* device = iEnv->ScreenDevice();
	device->ReleaseFont(iNumberShadowFont);
	device->ReleaseFont(iNumberFont);
	device->ReleaseFont(iOperatorShadowFont);
	device->ReleaseFont(iOperatorFont);
	device->ReleaseFont(iOutSheetOperatorFont);

   
	//Extended Icon Skin Implementation
		iAknsConstArray.Close();
		iAknsConstArrayForPaper.Close();
	}


// ---------------------------------------------------------
// CCalcAppEnv::Static
// Return value of thread-local storage.
// This value is set pointer of CCalcAppEnc class in CCalcAppEnv::NewL().  
// (other items were commented in a header).  
// ---------------------------------------------------------
//
CCalcAppEnv* CCalcAppEnv::Static()
	{
	return REINTERPRET_CAST(CCalcAppEnv*, UserSvr::DllTls(67338721)); 
	}


// C++ default constructor can NOT contain any code, that
// might leave.
//

CCalcAppEnv::CCalcAppEnv()
	{
	}
	

CCalcAppEnv::CCalcAppEnv
(CCalcAppUi* aAppUi)  
: iAppUi(aAppUi)
	{
	}

// Second-phase constructor
void CCalcAppEnv::ConstructAppEnvL()
	{
	iEnv = CEikonEnv::Static();

	// Read char code for operator.
	TResourceReader editOperRr;
	iEnv ->CreateResourceReaderLC(
		editOperRr, R_CALC_EDITOR_OPERATOR_ARRAY);
	TResourceReader osheetOperRr;
	iEnv->CreateResourceReaderLC(
		osheetOperRr, R_CALC_OUTSHEET_OPERATOR_ARRAY);

	editOperRr.ReadInt16(); // Read count of items
	osheetOperRr.ReadInt16(); // Read count of items

	iEditorOperArray = 
		new (ELeave) CDesCArrayFlat(KCalcCountOfOperatorType);
	iOutSheetOperArray = 
		new (ELeave) CDesCArrayFlat(KCalcCountOfOperatorType);

	TChar editorOper;
	TChar outSheetOper;

	for (TInt cnt(0); cnt < KCalcCountOfOperatorType; cnt++)
		{
		editorOper = editOperRr.ReadUint16();
		outSheetOper = osheetOperRr.ReadUint16();
		TBuf<1> editorOperBuf;
		TBuf<1> outSheetOperBuf; 
		editorOperBuf.Append(editorOper);
		outSheetOperBuf.Append(outSheetOper);
		iEditorOperArray->AppendL(editorOperBuf); 
		iOutSheetOperArray->AppendL(outSheetOperBuf);
		}

	CleanupStack::PopAndDestroy(2); // editOperRr, osheetOperRr


	// TLocale::DecimalSeparator() must be used when decimal separator is get.  
	TLocale locale;
	iDecimalSeparator = locale.DecimalSeparator();

	TBuf<1> buf;
	iEnv->ReadResourceL(buf, R_CALC_MINUS_INDICATOR);
	iMinusIndicator = buf[0];
	iEnv->ReadResourceL(iMemoryIndicator, R_CALC_STTUS_SAVED_TO_MEM);
	iEnv->ReadResourceL(iMemoryIndicatorShadow, R_CALC_STTUS_SAVED_TO_MEM_SHA);

	CEikApplication* app = iEnv->EikAppUi()->Application();
	TFileName bmpPath(app->BitmapStoreName());

	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	CFbsBitmap* bitmap = NULL;
	CFbsBitmap* mask	= NULL;

	//Ext Icon Skinning implementation for Display
	
	AknsUtils::CreateIconL(skin,KAknsIIDQsnFrCalcDisplaySideL,bitmap,mask,bmpPath,
		EMbmCalcsoftQsn_fr_calc_display_side_l, EMbmCalcsoftQsn_fr_calc_display_side_l_mask);
		
		iDisplaySideLBitmap = CGulIcon::NewL(bitmap, mask);
			bitmap = NULL;
			mask	= NULL;
		
		
	AknsUtils::CreateIconL(skin,KAknsIIDQsnFrCalcDisplayCenter,bitmap,mask,bmpPath,
		EMbmCalcsoftQsn_fr_calc_display_center, EMbmCalcsoftQsn_fr_calc_display_center_mask);
			
		iDisplayCenterBitmap = CGulIcon::NewL(bitmap, mask);
			bitmap = NULL;
			mask	= NULL;
		
	AknsUtils::CreateIconL(skin,KAknsIIDQsnFrCalcDisplaySideR,bitmap,mask,bmpPath,
		EMbmCalcsoftQsn_fr_calc_display_side_r, EMbmCalcsoftQsn_fr_calc_display_side_r_mask);
			
		iDisplaySideRBitmap = CGulIcon::NewL(bitmap, mask);

	//Ext Icon Skinning implementation for Paper
	bitmap = NULL;
	mask	= NULL;
	AknsUtils::CreateIconL(skin,KAknsIIDQgnGrafCalcPaper,bitmap,mask,bmpPath,
		EMbmCalcsoftQgn_graf_calc_paper,EMbmCalcsoftQgn_graf_calc_paper_mask);
	iPaperBitmap = CGulIcon::NewL(bitmap, mask);

	bitmap = NULL;
	mask	= NULL;

#ifdef __SCALABLE_ICONS
	AknsUtils::CreateIconL(skin,KAknsIIDQgnGrafCalcResultsLine, bitmap,mask,bmpPath,
		EMbmCalcsoftQgn_graf_calc_results_line,EMbmCalcsoftQgn_graf_calc_results_line_mask);
	iResultsLineBitmap = CGulIcon::NewL(bitmap, mask);
#endif
	bitmap = NULL;
	mask	= NULL; 
	iIsSkinChanged = EFalse;
	UpdateAknConstArrayforPaper();
	LoadPaperBitmapsL();

	UpdateAknConstArray();

	LoadFuncMapBitmapL();


	}


// ---------------------------------------------------------
// CCalcAppEnv::LoadPaperBitmapsL
// Load bitmaps for Paper Pane.  
// (other items were commented in a header).  
// ---------------------------------------------------------
//
 void CCalcAppEnv::LoadPaperBitmapsL()
	{
	CEikApplication* app = iEnv->EikAppUi()->Application();
	TFileName bmpPath(app->BitmapStoreName());
	TInt startBitmapId = EMbmCalcsoftQsn_fr_calc_paper_center;
	TInt startMaskId = EMbmCalcsoftQsn_fr_calc_paper_center_mask;
	TInt startIndex=0;
	TInt count = 9;
	TInt bmpId(startBitmapId);
	TInt maskId(startMaskId);

	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	CFbsBitmap* bitmap = NULL;
	CFbsBitmap* mask	= NULL; 

	for (TInt cnt(startIndex); cnt < (count + startIndex); cnt++)
		{

		AknsUtils::CreateIconL(skin, (iAknsConstArrayForPaper)[cnt],bitmap,mask,
					bmpPath, bmpId, maskId);

		if(!iIsSkinChanged)
			{
			iScalablePaperBitmap[cnt] = CGulIcon::NewL(bitmap, mask);
			}
		else
			{
			iScalablePaperBitmap[cnt]->SetBitmap(bitmap);
			iScalablePaperBitmap[cnt]->SetMask(mask);
			}
		bmpId = bmpId + 2;
		maskId = maskId + 2;
		}
	}

// ---------------------------------------------------------
// CCalcAppEnv::LoadFuncMapBitmapL
// Load bitmaps for FunctionMap SubPane.  
// (other items were commented in a header).  
// ---------------------------------------------------------
//
void CCalcAppEnv::LoadFuncMapBitmapL()
	{

	CEikApplication* app = iEnv->EikAppUi()->Application();
	TFileName bmpPath(app->BitmapStoreName());

	if(iIsSkinChanged) //EISkin2.6
		{
		MAknsSkinInstance* skin = AknsUtils::SkinInstance();
		CFbsBitmap* bitmap = NULL;
		CFbsBitmap* mask	= NULL;

		//Ext Icon Skinning implementation for Display
		
	AknsUtils::CreateIconL(skin,KAknsIIDQsnFrCalcDisplaySideL,bitmap,mask,bmpPath,
		EMbmCalcsoftQsn_fr_calc_display_side_l, EMbmCalcsoftQsn_fr_calc_display_side_l_mask);
		
		iDisplaySideLBitmap->SetBitmap(bitmap);
		iDisplaySideLBitmap->SetMask(mask);
		
			bitmap = NULL;
			mask	= NULL;
		
		
	AknsUtils::CreateIconL(skin,KAknsIIDQsnFrCalcDisplayCenter,bitmap,mask,bmpPath,
		EMbmCalcsoftQsn_fr_calc_display_center, EMbmCalcsoftQsn_fr_calc_display_center_mask);
			
		iDisplayCenterBitmap->SetBitmap(bitmap);
		iDisplayCenterBitmap->SetMask(mask);
		
			bitmap = NULL;
			mask	= NULL;
		
	AknsUtils::CreateIconL(skin,KAknsIIDQsnFrCalcDisplaySideR,bitmap,mask,bmpPath,
		EMbmCalcsoftQsn_fr_calc_display_side_r, EMbmCalcsoftQsn_fr_calc_display_side_r_mask);
			
	iDisplaySideRBitmap->SetBitmap(bitmap);
	iDisplaySideRBitmap->SetMask(mask);
			
		//Ext Icon Skinning implementation for Paper
		bitmap = NULL;
		mask	= NULL;
		AknsUtils::CreateIconL(skin,KAknsIIDQgnGrafCalcPaper,bitmap,mask,bmpPath,
			EMbmCalcsoftQgn_graf_calc_paper,EMbmCalcsoftQgn_graf_calc_paper_mask);
	
		iPaperBitmap->SetBitmap(bitmap);
		iPaperBitmap->SetMask(mask);
		bitmap = NULL;
		mask	= NULL;
#ifdef __SCALABLE_ICONS
		AknsUtils::CreateIconL(skin,KAknsIIDQgnGrafCalcResultsLine, bitmap,mask,bmpPath,
		EMbmCalcsoftQgn_graf_calc_results_line,EMbmCalcsoftQgn_graf_calc_results_line_mask);
		
		iResultsLineBitmap->SetBitmap(bitmap);
		iResultsLineBitmap->SetMask(mask);
#endif
		bitmap = NULL;
		mask	= NULL;
		LoadPaperBitmapsL();
		}

	if(!iIsSkinChanged)
	{
		
	
		if( AknLayoutUtils::PenEnabled() )
	    {
	            
	        
	        LoadBitmapForFuncMapL(
			EMbmCalcsoftQgn_indi_calc_cancel,
	    	EMbmCalcsoftQgn_indi_calc_cancel_mask,
		    0, 
		    KCountOfButtonsTouch, iUnpressedButtonBmp, ENORMAL);
	    }

		else
	    {
	        LoadBitmapForFuncMapL(
			EMbmCalcsoftQgn_indi_calc_button_plus,
	    	EMbmCalcsoftQgn_indi_calc_button_plus_mask,
		    0, 
		    KCountOfButtons, iUnpressedButtonBmp, ENORMAL);
		    
		    LoadBitmapForFuncMapL(
			EMbmCalcsoftQgn_indi_calc_button_plus_pressed,
	    	EMbmCalcsoftQgn_indi_calc_button_plus_pressed_mask,
		    0, 
		    KCountOfButtons, iPressedButtonBmp, EPRESSED);
		    
	    }	
	}

	}

// ---------------------------------------------------------
// CCalcAppEnv::UpdateAknConstArrayForFuncmap
// Update array for function map.  
// (other items were commented in a header).  
// ---------------------------------------------------------
//
void CCalcAppEnv::UpdateAknConstArrayForFuncMap()
    {
    UpdateAknConstArray();
    }
	    
// ---------------------------------------------------------
// CCalcAppEnv::LoadBitmapForFuncMapButtonL
// Load bitmaps for FunctionMap button.  
// (other items were commented in a header).  
// ---------------------------------------------------------
//

void CCalcAppEnv::LoadBitmapForFuncMapL(TInt aStartBitmapId, TInt aStartMaskId, TInt aStartIndex,
	TInt aCount, CGulIcon* aBitmapArray[], ButtonType aType)
	{
	CEikApplication* app = iEnv->EikAppUi()->Application();
	TFileName bmpPath(app->BitmapStoreName());
	TInt bmpId(aStartBitmapId);
	TInt maskId(aStartMaskId);

	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	CFbsBitmap* bitmap = NULL;
	CFbsBitmap* mask	= NULL;

    // Get the text color of theme
    TRgb iconColor;
    AknsUtils::GetCachedColor( skin, iconColor, 
        KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG29 );
    
	for (TInt cnt(aStartIndex); cnt < (aCount + aStartIndex); cnt++)
		{
		switch(aType)
			{
			case EPRESSED:
				{
    				if(!AknLayoutUtils::PenEnabled() )
    				{
    				    AknsUtils::CreateIconL(skin,(iAknsConstArray)[cnt + KCalcCountOfButtonPressed],bitmap,mask,
    					bmpPath, bmpId, maskId);    
    					
    				}
				}
			break;
			case ENORMAL:
				{
				    if(!AknLayoutUtils::PenEnabled() )
				    {
				        AknsUtils::CreateIconL(skin,(iAknsConstArray)[cnt],bitmap,mask,
					    bmpPath, bmpId, maskId);        
				    }
                    else
                    {
                        AknsUtils::CreateIconL(skin,(iAknsConstArray)[cnt]/*KAknsIIDQgnIndiCalcAdd*/,bitmap,mask,
                        bmpPath, bmpId, maskId);
                        // Set the color of icons
                        AknIconUtils::SetIconColor( bitmap, iconColor );
                    }
				
				}
			break;
			case EACTIVE_INACTIVE:
				{
				AknsUtils::CreateIconL(skin,(iAknsConstArray)[cnt + KCalcCountOfButtonInactive],bitmap,mask,
					bmpPath, bmpId, maskId);
				}
			break;
			default:
				break;

			}

		CGulIcon* icon = CGulIcon::NewL(bitmap, mask);
		aBitmapArray[cnt] = icon;
			
		bmpId = bmpId + 2;
		maskId = maskId + 2;
		}

	}

//---------------------------------------------------------
// CCalcAppEnv::UpdateAknConstArrayforPaper
//Updates the array with avkon skin constant Ids for paper bitmaps.   
// ---------------------------------------------------------
//
   void CCalcAppEnv::UpdateAknConstArrayforPaper()
	{
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperCenter);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperCornerTl);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperCornerTr);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperCornerBl);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperCornerBr);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperSideT);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperSideB);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperSideL);
	iAknsConstArrayForPaper.Append(KAknsIIDQsnFrCalcPaperSideR); 
	
	}

//---------------------------------------------------------
// CCalcAppEnv::UpdateAknConstArray
//Updates the array with avkon skin constant Ids for bitmaps.   
// ---------------------------------------------------------
//
void CCalcAppEnv::UpdateAknConstArray()
    {
    iAknsConstArray.Reset();
    if( AknLayoutUtils::PenEnabled() )
    {
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcCancel );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcSeven );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcFour );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcOne );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcZero );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcSquareroot );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcEight );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcFive );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcTwo );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcSeparator );  
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcPercent );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcNine );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcSix );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcThree );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcSign );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcDivide );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcMultiply );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcSubtract );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcAdd );
    iAknsConstArray.Append( KAknsIIDQgnIndiCalcEquals );
    }
    else
    {
    //Akn Const for +, -, *, /....etc 0-9
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonPlus);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonMinus);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonMultiply);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonDivide);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSign);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonEquals);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSquareroot);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonPercent);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonUp);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonDown);


    //Akn Const for pressed +, -, *, /....etc  10-19
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonPlusPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonMinusPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonMultiplyPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonDividePressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSignPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonEqualsPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSquarerootPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonPercentPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonUpPressed);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonDownPressed);

    //Akn Const for Active/ Inactive icons 20-24
    /*iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSignInactive);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSignInactive);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonSquarerootInactive);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonPercentInactive);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonUpInactive);
    iAknsConstArray.Append(KAknsIIDQgnIndiCalcButtonDownInactive);*/
    //}
    }
    }   
//EISkin2.6
//-----------------------------------------------------------
//CCalcAppEnv::LoadFuncMapBitmapSkinChangeL
//Loads the function map Icons when the skin changes
//-----------------------------------------------------------
//
void CCalcAppEnv::LoadFuncMapBitmapSkinChangeL()
	{
	iIsSkinChanged = ETrue;
	LoadFuncMapBitmapL();
	}


//-----------------------------------------------------------
//CCalcAppEnv::SetSkinChangedValue
//Sets if the skin is changed or not
//-----------------------------------------------------------
	
void CCalcAppEnv::SetSkinChangedValue(TBool aValue)
{
	iIsSkinChanged = aValue;
}
//  End of File  
