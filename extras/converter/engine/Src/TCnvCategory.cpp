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
*	  This is the implementation of the class defined in TCnvCategory.h
*
*/



#include "CCnvCategory.h"
#include "TCnvUnit.h"
#include "TCnvCategory.h"


EXPORT_C TCnvCategory::TCnvCategory() :
	iCategory( NULL ), iDesCArray( NULL ),
		iCurrentValueIndex( 0 ), iIsQueryValueOnly( EFalse )
		{
		}

TCnvCategory::TCnvCategory( CCnvCategory* aCategory ) :
	iCategory( aCategory ), iDesCArray( aCategory ),
		iCurrentValueIndex( 0 ), iIsQueryValueOnly( EFalse )
		{
		}

TCnvCategory::TCnvCategory( MDesCArray* aDesCArray ): 
	iCategory( NULL ), iDesCArray( aDesCArray ), 
		iCurrentValueIndex( 0 ), iIsQueryValueOnly( ETrue )
		{
		}

EXPORT_C TReal TCnvCategory::Convert( TUint aSourceUnit,
	TUint aDestinationUnit,
	const TReal& aAmount,
	TUint aDecimals ) const
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_Convert ) );
	return iCategory->Convert( aSourceUnit, aDestinationUnit,
		aAmount, aDecimals );
	}

EXPORT_C TReal TCnvCategory::UnitFactor( TUint aUnit ) const
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_UnitFactor ) );
	return iCategory->Unit( aUnit ).iFactor; 
	}

EXPORT_C TReal TCnvCategory::UnitMinimum( TUint aUnit ) const
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_UnitMinimum ) );
	return iCategory->Unit( aUnit ).iMinimum; 
	}

EXPORT_C void TCnvCategory::SetUnitFactor( TUint aUnit,
	const TReal& aNewFactor )
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_SetUnitFactor ) );
	iCategory->Unit( aUnit ).iFactor = aNewFactor;
	}

EXPORT_C void TCnvCategory::SetUnitName( TUint aUnit, const TDesC& aNewName, TCurrencyType aType )
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_SetUnitName ) );
	iCategory->Unit( aUnit ).iName.Copy( aNewName );
	iCategory->Unit( aUnit ).iNameType = aType;
	}

EXPORT_C TUint TCnvCategory::BaseUnit() const
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_BaseUnit ) );
	return iCategory->BaseUnit();
	}

EXPORT_C void TCnvCategory::SetBaseUnit( TUint aBaseUnit )
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_SetBaseUnit ) );
	iCategory->SetBaseUnit( aBaseUnit );
	iCategory->Unit( aBaseUnit ).iFactor = 1.0;
	}

EXPORT_C void TCnvCategory::GetDefaultUnits( TUint& aSourceUnit,
	TUint& aDestinationUnit ) const
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText, EPanicPreCond_GetDefaultUnits ) );
	iCategory->GetDefaultUnits( aSourceUnit, aDestinationUnit );
	}

EXPORT_C void TCnvCategory::SetDefaultUnits( TUint aSourceUnit,
	TUint aDestinationUnit )
	{
	__ASSERT_DEBUG( iCategory, User::Panic(KPanicText,  EPanicPreCond_SetDefaultUnits ) );
	iCategory->SetDefaultUnits( aSourceUnit, aDestinationUnit );
	}

EXPORT_C const MDesCArray* TCnvCategory::MdcArray() const
	{
	return iDesCArray;
	}

EXPORT_C HBufC* TCnvCategory::CurrentValueTextLC()
	{
	__ASSERT_DEBUG( iDesCArray, User::Panic(KPanicText, EPanicPreCond_CurrentValueTextLC ) );
	TPtrC ptr = iDesCArray->MdcaPoint( iCurrentValueIndex );
	HBufC* buf = HBufC::NewLC( ptr.Length() );
	buf->operator=( ptr );
	return buf;
	}

EXPORT_C TInt TCnvCategory::CurrentValueIndex() const
	{
	return iCurrentValueIndex;
	}

EXPORT_C void TCnvCategory::SetCurrentValueIndex( const TInt aIndex )
	{
	iCurrentValueIndex = aIndex;
	}

EXPORT_C void TCnvCategory::AddNewCurrencyL( TInt aControlId, TDesC& aNewName, TReal& aNewFactor, TCurrencyType aType )
	{
	iCategory->AddNewCurrencyL( aControlId, aNewName, aNewFactor, aType );
	}

EXPORT_C void TCnvCategory::DeleteCurrencyL(TInt aCurrencyId)
	{
	iCategory->DeleteCurrency( aCurrencyId);
	}

EXPORT_C void TCnvCategory::CreateTempUnitArrayForRestore()
	{
	iCategory->CreateTempUnitArrayForRestore();
	}
EXPORT_C void TCnvCategory::RestoreUnitArray()
	{
	iCategory->RestoreUnitArray();
	}

// End of file
