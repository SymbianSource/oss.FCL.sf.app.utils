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
*	  This is the implementation of the class defined in TCnvUnit.h
*
*/



#include <barsread.h>
#include <s32strm.h>

#include "TCnvUnit.h"

TCnvUnit::TCnvUnit() : 
	iFactor( 0 ), iOffset( 0 ), 
	iMinimum( 0 ), iId( 0 ), 
	iIsTemperatureUnit( EFalse ),
	iIsCurrencyUnit ( EFalse )
	{
	}

// Read the data from a resource. If this is a temperature unit we have
// a couple extra parameters to read.
void TCnvUnit::ReadFromResource( TResourceReader& aReader,
								 TBool aIsTemperature, TBool aIsCurrencyUnit )
	{
	// This flag has been done to avoid storing the two additional TReals in
	// resources and streams (they are valid only for temperature units).
	iIsTemperatureUnit = aIsTemperature;
	iIsCurrencyUnit = aIsCurrencyUnit;

	iName.Copy( aReader.ReadTPtrC() );
	iFactor = aReader.ReadReal64();
	iId = aReader.ReadUint16();

	if( iIsTemperatureUnit )
		{
		iOffset = aReader.ReadReal64();
		iMinimum = aReader.ReadReal64();
		}
	
	if(iIsCurrencyUnit)	
		{
		TUint nameType (aReader.ReadUint8());
		iNameType = (TCurrencyType)nameType;
		}
	}

// Store the data in a stream.
void TCnvUnit::ExternalizeL( RWriteStream& aStream ) const
	{
	aStream.WriteUint16L( iName.Length() );
	aStream.WriteL( iName );

	aStream.WriteReal64L( iFactor );
	
	aStream.WriteUint16L( iId );

	aStream.WriteUint8L( iIsTemperatureUnit );
	if( iIsTemperatureUnit )
		{
		aStream.WriteReal64L( iOffset );
		aStream.WriteReal64L( iMinimum );
		}
		
	aStream.WriteUint8L( iIsCurrencyUnit );
	if(iIsCurrencyUnit)
		{
		aStream.WriteUint8L( iNameType );
		}
	}

// Recover the data from a stream.
void TCnvUnit::InternalizeL( RReadStream& aStream, TBool aIsCurrencyCategory )
	{
	iIsCurrencyUnit = aIsCurrencyCategory;
	TUint nameLength( aStream.ReadUint16L() );
	aStream.ReadL( iName, nameLength );

	iFactor = aStream.ReadReal64L();

	iId = aStream.ReadUint16L();

	iIsTemperatureUnit = aStream.ReadUint8L();

	if( iIsTemperatureUnit )
		{
		iOffset = aStream.ReadReal64L();
		iMinimum = aStream.ReadReal64L();
		}
	
	iIsCurrencyUnit = aStream.ReadUint8L();
	
	if(iIsCurrencyUnit)
		{
		TUint nameType (aStream.ReadUint8L());
		iNameType = (TCurrencyType)nameType;
		}
	}


// End of file
