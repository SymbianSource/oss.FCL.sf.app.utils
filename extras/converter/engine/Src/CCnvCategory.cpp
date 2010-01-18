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
*	  This is the implementation of the class defined in CCnvCategory.h
*
*/



#include <barsread.h>	// TResourceReader
#include <e32math.h>	// Math::Round()
#include <s32strm.h>	// RReadStream, RWriteStream
#include <Converter.rsg>
#include "CCnvCategory.h"

CCnvCategory::~CCnvCategory()
	{
	iUnitArray.Reset();
	iTempUintForRestoreArray.Reset();
	}

CCnvCategory* CCnvCategory::NewLC( TResourceReader& aReader , TBool aIsCurrencyCategory)
	{
	CCnvCategory* self = new( ELeave ) CCnvCategory;
	CleanupStack::PushL( self );
	self->ConstructFromResourceL( aReader ,aIsCurrencyCategory );

	return self;
	}

void CCnvCategory::ConstructFromResourceL( TResourceReader& aReader, TBool aIsCurrencyCategory)  
	{
	// read the category parameters
	iName.Copy( aReader.ReadTPtrC() );
	TInt isChineseorder = aReader.ReadInt8();
	iBaseUnitIndex = aReader.ReadInt8();


	if( isChineseorder == 1)
		{
		switch( User::Language() )
			{
			case ELangHongKongChinese:
			case ELangTaiwanChinese:
			case ELangPrcChinese:
				iDestinationUnitIndex = aReader.ReadInt8();
				iSourceUnitIndex = aReader.ReadInt8();		
				break;
			default:
				iSourceUnitIndex = aReader.ReadInt8();
				iDestinationUnitIndex = aReader.ReadInt8();
				break;
			}
		}
	else
		{
		iSourceUnitIndex = aReader.ReadInt8();
		iDestinationUnitIndex = aReader.ReadInt8();
		}
	
	TBool isTemperatureCategory( EFalse );
	if( aReader.ReadInt8() != 0 )
		{
		isTemperatureCategory = ETrue;
		}

	// start reading the unit array
	TInt numUnits = aReader.ReadInt16();

	TCnvUnit unit;
	for( TInt i = 0; i < numUnits; i++ )
		{
		unit.ReadFromResource( aReader, isTemperatureCategory, aIsCurrencyCategory );
		iUnitArray.AppendL( unit );
		}
	}

TInt CCnvCategory::MdcaCount() const
	{
	return iUnitArray.Count();
	}

// Returns the name of the unit at index 'aIndex'
TPtrC CCnvCategory::MdcaPoint( TInt aIndex ) const
	{
	return TPtrC( iUnitArray.At( aIndex ).iName );
	}

TReal CCnvCategory::Convert( TUint aSourceUnit, TUint aDestinationUnit,
							const TReal& aAmount, TUint aDecimals ) const
	{
	const TCnvUnit& srcUnit = iUnitArray.At( aSourceUnit );
	const TCnvUnit& dstUnit = iUnitArray.At( aDestinationUnit );

	TReal result( ( aAmount - srcUnit.iOffset ) / srcUnit.iFactor
		* dstUnit.iFactor + dstUnit.iOffset );
	Math::Round( result, result, aDecimals );
	return result;
	}

void CCnvCategory::GetDefaultUnits( TUint& aSourceUnit,
								   TUint& aDestinationUnit ) const
	{
	aSourceUnit = iSourceUnitIndex;
	aDestinationUnit = iDestinationUnitIndex;
	}

void CCnvCategory::SetDefaultUnits( TUint aSourceUnit,
								   TUint aDestinationUnit )
	{
	iSourceUnitIndex = aSourceUnit;
	iDestinationUnitIndex = aDestinationUnit;
	}

TUint CCnvCategory::BaseUnit() const
	{
	return iBaseUnitIndex;
	}

void CCnvCategory::SetBaseUnit( const TUint& aBaseUnit )
	{
	iBaseUnitIndex = aBaseUnit;
	}

const TDes& CCnvCategory::Name() const
	{
	return iName;
	}

TCnvUnit& CCnvCategory::Unit( TUint aIndex )
	{
	return iUnitArray.At( aIndex );
	}

// Store the data in a stream.
void CCnvCategory::ExternalizeL( RWriteStream& aStream ) const
	{
	aStream.WriteUint8L( iBaseUnitIndex );
	aStream.WriteUint8L( iSourceUnitIndex );
	aStream.WriteUint8L( iDestinationUnitIndex );

	TUint len( iUnitArray.Count() );
	aStream.WriteUint8L( iUnitArray.Count() );
	for( TUint i = 0; i < len; i ++)
		{
		iUnitArray.At( i ).ExternalizeL( aStream );
		}
	}

// Recover the data from a stream.
void CCnvCategory::InternalizeL( RReadStream& aStream, TBool aIsCurrencyCategory )
	{
	TBuf<16> currencyNames[3];
	
	iBaseUnitIndex = aStream.ReadUint8L();
	iSourceUnitIndex = aStream.ReadUint8L();
	iDestinationUnitIndex = aStream.ReadUint8L();

	TUint len( aStream.ReadUint8L() );
	
	//The ini file exists. So delete the existing iUnitArray and load it again 
	//from the Configuration Settings file
	
	//Trying to use enum values as array indexes
	currencyNames[1] = iUnitArray.At(0).iName; // Qtn_Cnv_Home
	currencyNames[2] = iUnitArray.At(1).iName; // Qtn_Cnv_Foreign
	
	iUnitArray.Delete(0,iUnitArray.Count());	

	for( TUint i = 0; i < len; i ++)
		{
		//Change: Loads the currencies directly from ini file. no searching..
		//		TInt index;
		TCnvUnit unit;
		unit.InternalizeL( aStream , aIsCurrencyCategory);

		iUnitArray.AppendL(unit);
		}
		
	TCurrencyType type;
	
	for( TUint in = 0; in < len; in++)
		{
			type = iUnitArray.At(in).iNameType;
			if(type == EHome)
				iUnitArray.At(in).iName = currencyNames[1];
			else if(type == EForeign)
				iUnitArray.At(in).iName = currencyNames[2];
		}
	}


void CCnvCategory::AddNewCurrencyL( TInt aCurrencyId, TDesC& aNewName, TReal& aNewFactor, TCurrencyType aType)
	{
	TCnvUnit unit;
	TUint curencyId = aCurrencyId; // To remove the warnings
	unit.iName  = aNewName;	
	unit.iFactor = aNewFactor;
	unit.iNameType = aType;
	unit.iIsCurrencyUnit = ETrue;
	//To calculate the id for the new currency added.
	//will run thru the ids, gets the largest id and increments it and assigns to the new unit.
	//Make sure that the ids for the currencies have the largest possible value in the hrh file.
	TUint id;
	id = iUnitArray.At(0).iId;
	for(TInt i = 1; i < iUnitArray.Count(); i++)
		{
		if(id < iUnitArray.At(i).iId)
			{
			id = iUnitArray.At(i).iId;
			}
		}
	id++;
	unit.iId = id;

	iUnitArray.InsertL( curencyId, unit );
	//New element has been added. So set the base source and destination currencies accordingly
	if( iBaseUnitIndex >= curencyId )
		{		
		iBaseUnitIndex++; 
		}
	if( iSourceUnitIndex >= curencyId )
		{		
		iSourceUnitIndex++; 
		}
	if( iDestinationUnitIndex >= curencyId )
		{		
		iDestinationUnitIndex++; 
		}

	}



void CCnvCategory::DeleteCurrency(TInt aCurrencyId)
	{
	iUnitArray.Delete(aCurrencyId);

	TUint unsignedCurrencyId = aCurrencyId; //to remove warnings

	//An element has been deleted. So set the base source and destination currencies accordingly
	if(iBaseUnitIndex >= unsignedCurrencyId   )
		{
		if( iBaseUnitIndex > 0)
			iBaseUnitIndex--;
		}
	if(iSourceUnitIndex >= unsignedCurrencyId   )
		{
		if( iSourceUnitIndex > 0)
			iSourceUnitIndex--;
		}
	if(iDestinationUnitIndex >= unsignedCurrencyId   )
		{
		if( iDestinationUnitIndex > 0)
			iDestinationUnitIndex--;
		}

	}


void CCnvCategory::CreateTempUnitArrayForRestore()
	{
	iTempUintForRestoreArray.Reset();
	for ( TInt i = 0; i < iUnitArray.Count() ; i++ )
		{
		TRAP_IGNORE( iTempUintForRestoreArray.AppendL( iUnitArray.At( i ) ) );
		}
	}

void CCnvCategory::RestoreUnitArray()
	{
	iUnitArray.Reset();
	for ( TInt i = 0 ; i < iTempUintForRestoreArray.Count() ; i++ )
		{
		TRAP_IGNORE( iUnitArray.AppendL( iTempUintForRestoreArray.At( i ) ) );
		}
	}

// End of File
