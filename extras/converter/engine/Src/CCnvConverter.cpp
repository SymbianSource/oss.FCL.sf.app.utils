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
* Description:  
*	  This is the implementation of the class defined in CCnvConverter.h
*
*/



#include <s32strm.h>
#include <barsread.h>

#include "CCnvConverter.h"
#include "CCnvCategory.h"
#include "TCnvCategory.h"


EXPORT_C CCnvConverter::~CCnvConverter()
	{
	iCategoryArray.ResetAndDestroy();
	}

EXPORT_C CCnvConverter* CCnvConverter::NewL( TResourceReader& aReader )
	{
	CCnvConverter* self = new( ELeave ) CCnvConverter;
	CleanupStack::PushL( self );
	self->ConstructFromResourceL( aReader );
	CleanupStack::Pop();	// self;

	return self;
	}

void CCnvConverter::ConstructFromResourceL( TResourceReader& aReader )
	{
	iModelVersion = aReader.ReadInt16(); 
	iCurrencyCategory = aReader.ReadInt16();
iCurrencyCategory = iCurrencyCategory ;

	TUint numCategories( aReader.ReadInt16() );

	for( TUint i = 0; i < numCategories ; i++ )
		{
		if(i == iCurrencyCategory)
			{
			CCnvCategory* category = CCnvCategory::NewLC( aReader, ETrue ); // Load Currency
			iCategoryArray.AppendL( category );
			CleanupStack::Pop();	// category
			}
		else
			{
			CCnvCategory* category = CCnvCategory::NewLC( aReader );
			iCategoryArray.AppendL( category );
			CleanupStack::Pop();	// category
			}
		}
	}

TInt CCnvConverter::MdcaCount() const
	{
	return iCategoryArray.Count();
	}

TPtrC CCnvConverter::MdcaPoint( TInt aIndex ) const
	{
	return TPtrC( iCategoryArray.At( aIndex )->Name() );
	}

EXPORT_C void CCnvConverter::GetCategoryAccessor( TCnvCategory& aCategory,
												 TUint aCategoryIndex )
	{
	aCategory = TCnvCategory( iCategoryArray.At( aCategoryIndex ) );
	}

EXPORT_C void CCnvConverter::GetCategorylistAccessor(
	TCnvCategory& aCategory )
	{
	aCategory = TCnvCategory( this );
	}

EXPORT_C TUint CCnvConverter::CurrencyCategoryId() const
	{
	return iCurrencyCategory;
	}

// Store the data in a stream.
EXPORT_C void CCnvConverter::ExternalizeL( RWriteStream& aStream ) const
	{
	// Write the current model version
	aStream.WriteUint32L( iModelVersion );

	// Write the current model language variant
	aStream.WriteUint32L( User::Language() );

	// Save out the currency category as a whole
	CCnvCategory* category = iCategoryArray.At( iCurrencyCategory );
	category->ExternalizeL( aStream );

	// Save
	TUint len( iCategoryArray.Count() );
	for( TUint i = 0; i < len; i++ )
		{
		TUint aSrc( 0 ), aDst( 0 );
		category = iCategoryArray.At( i );
		category->GetDefaultUnits( aSrc, aDst );
		aStream.WriteUint8L( aSrc );
		aStream.WriteUint8L( aDst );
		}
	}

// Recover the data from a stream.
EXPORT_C void CCnvConverter::InternalizeL( RReadStream& aStream )
	{
	TUint version( aStream.ReadUint32L() );
	if( version != iModelVersion )
		{
		User::Leave( KErrCorrupt );
		}
	TInt language( aStream.ReadUint32L() );
	language = language; // suppress compiler warnings
	/*if( language != User::Language() )
	{
	User::Leave( KErrCorrupt );
	}
	*/
	// Read in the currency category as a whole
	CCnvCategory* category = iCategoryArray.At( iCurrencyCategory );
	category->InternalizeL( aStream );

	TUint len( iCategoryArray.Count() );
	for( TUint i = 0; i < len; i++ )
		{
		TUint src( aStream.ReadUint8L() );
		TUint dst( aStream.ReadUint8L() );
		category = iCategoryArray.At( i );
		category->SetDefaultUnits( src, dst );
		}
	}


void CCnvConverter::SkipCategoriesL( TInt aCount, TResourceReader& aReader )
	{
	for( TInt i = 0; i < aCount; i++ )
		{
		CCnvCategory::NewLC( aReader );
		CleanupStack::PopAndDestroy();
		}
	}

// End of File
