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
*     This class is a data container of unit conversion parameters
*     for a single unit.
*     It is able to construct itself from a resource file.
*
*/



#ifndef __TCNVUNIT_H__
#define __TCNVUNIT_H__

// INCLUDES
#include <e32base.h>
#include "CCnvConverter.h"
// FORWARD DECLARATIONS
class TResourceReader;
class RWriteStream;

// CONSTANTS
const TInt KMaxUnitNameLength = 36;

// CLASS DEFINITION
/**
* This class is a data container of unit conversion parameters
* for a single unit.
* It is able to construct itself from a resource file.
*/
class TCnvUnit
	{
	public: // Constructors

		TCnvUnit();

	public:	// new methods.

		/**
		* Constructs this unit from a CNV_UNIT or CNV_UNIT_TEMPERATURE
		* resource.
		* @param aReader A resource reader object initialised to a resource.
		* @param aIsTemperature EFalse if we are going to read a CNV_UNIT
		*        resource, ETrue if a CNV_UNIT_TEMPERATURE.
		*/
		void ReadFromResource( TResourceReader& aReader,
							   TBool aIsTemperature = EFalse, TBool aIsCurrencyUnit =EFalse);

		/**
		* Stores the member variables to the given stream.
		* @param aStream Stream for writing.
		*/
		void ExternalizeL( RWriteStream& aStream ) const;

		/**
		* Loads the member variables from the given stream.
		* @param aStream Stream for reading.
		*/
		void InternalizeL( RReadStream& aStream , TBool aIsCurrencyCategory = EFalse);

	public: // data

		/**
		* The factor for converting this unit to the base unit.
		*/
		TReal iFactor;

		/**
		* The offset for converting this unit to the base unit.
		* Used only with temperature units.
		*/
		TReal iOffset;

		/**
		* Minimum allowable unit amount in conversion.
		* Used only with temperature units.
		*/
		TReal iMinimum;

		/**
		* Unit name. Max lengh of 35 used in unit name queries.
		*/
		TBuf< KMaxUnitNameLength > iName;

		/**
		* Id of the unit. Used to identify unit among all other units.
		* Unique.
		*/
		TUint iId;

//	private: // data

		/**
		* ETrue if this unit defines a temperature, EFalse otherwise.
		*/
		TBool iIsTemperatureUnit;
		TBool iIsCurrencyUnit;
		TCurrencyType iNameType;
	};

#endif // __TCNVUNIT_H__
