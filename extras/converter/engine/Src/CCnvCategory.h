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
*     for a single unit category. It creates and owns the units in this
*     category. It is able to construct itself from a resource file.
*
*/



#ifndef __CCNVCATEGORY_H__
#define __CCNVCATEGORY_H__

// INCLUDES
#include <e32base.h>
#include <bamdesca.h>
#include "TCnvUnit.h"

// FORWARD DECLARATIONS
class TResourceReader;
class RWriteStream;

// CONSTANTS
const TInt KMaxNameLength = 32;

// CLASS DEFINITION
/**
* This class is a data container of unit conversion parameters
* for a single unit category. It creates and owns the units in this category.
* It is able to construct itself from a resource file.
*/
class CCnvCategory : public CBase, public MDesCArray
	{
	public:	// Constructors and destructor

		/**
		* Destructor.
		*/
		~CCnvCategory();

		/**
		* Two-phased constructor
		* @param aReader A resource reader object initialised to a resource.
		*/
		static CCnvCategory* NewLC( TResourceReader& aReader, TBool aIsCurrencyCategory = EFalse );

	private: // Constructors

		/**
		* Default constructor. Inlined to save a few bytes
		* 2 = array granularity ( balanced between memory usage and speed )
		*/
		inline CCnvCategory() : iUnitArray( 2 ) , iTempUintForRestoreArray( 2 ) {};

		/**
		* Constructs this category from a CNV_CATEGORY resource.
		* @param aReader A resource reader object initialised to a resource.
		*/
		void ConstructFromResourceL( TResourceReader& aReader , TBool aIsCurrencyCategory = EFalse);

	public: // from MDesCArray

		/**
		* Returns the number of items (units) in this category.
		* @return number of items.
		*/
		TInt MdcaCount() const;

		/**
		* Returns a pointer to a unit name specified by aIndex.
		* @param aIndex Defines the point in the array that is 
		*        queried for the name.
		* @return Pointer to unit name.
		*/
		TPtrC MdcaPoint( TInt aIndex ) const;
  
	public:	// new methods

		/**
		* Calculates the conversion from source unit to destination unit.
		* @param aSourceUnit Index of the source unit.
		* @param aDestinationUnit Index of the destination unit.
		* @param aAmount Convert this many source units.
		* @return Result of conversion, rounded to aDecimals decimal places.
		*/
		TReal Convert( TUint aSourceUnit, TUint aDestinationUnit,
					   const TReal& aAmount, TUint aDecimals ) const;

		/**
		* Returns the default unit indices.
		* @param aSourceUnit Index placeholder of the source unit.
		* @param aDestinationUnit Index placeholder of the destination unit.
		*/
		void GetDefaultUnits( TUint& aSourceUnit,
							  TUint& aDestinationUnit ) const;

		/**
		* Sets the default unit indices.
		* @param aSourceUnit Index of the new source unit.
		* @param aDestinationUnit Index of the new destination unit.
		*/
		void SetDefaultUnits( TUint aSourceUnit, TUint aDestinationUnit );

		/**
		* Returns the index of the base unit.
		* @return base unit index.
		*/
		TUint BaseUnit() const;

		/**
		* Changes the index of the base unit.
		* @param aIndex new base unit index.
		*/
		void SetBaseUnit( const TUint& aIndex );

		/**
		* Returns a pointer to this category's name.
		* @return the category name.
		*/
		const TDes& Name() const;

		/**
		* Returns a reference to the Unit object identified by aIndex.
		* @return the requested Unit object.
		*/
		TCnvUnit& Unit( TUint aIndex );

		/**
		* Stores the unit array to the given stream.
		* @param aStream Stream for writing.
		*/
		void ExternalizeL( RWriteStream& aStream ) const;

		/**
		* Loads the unit array from the given stream.
		* @param aStream Stream for reading.
		*/
		void InternalizeL( RReadStream& aStream , TBool aIsCurrencyCategory = EFalse);
		
		/**
		* create a unit array copy
		* 
		*/
		void CreateTempUnitArrayForRestore();
		
		/**
		* restore the unit array
		* 
		*/
		void RestoreUnitArray();

	private: // data

		/**
		* Index of the default source unit  (in the unit array).
		*/
		TUint iSourceUnitIndex;

		/**
		* Index of the default destination unit (in the unit array).
		*/
		TUint iDestinationUnitIndex;

		/**
		* Index of the base unit (in the unit array).
		*/
		TUint iBaseUnitIndex;

		/**
		* Name of this category.
		*/
		TBuf< KMaxNameLength > iName;

		/**
		* The array for units in this category. Owned.
		*/
		CArrayFixSeg< TCnvUnit > iUnitArray;
		
		// Save unit for restore
		CArrayFixSeg< TCnvUnit > iTempUintForRestoreArray;

	public:
		/**
		*Deletes a particular currency  from the list.
		*@param aControlId indicates which currency has to be deleted.
		*/
		void DeleteCurrency(TInt aCurrencyId);
		
		/**
		*Adds a new field at a given position(focus)
		*@param aControlId indicates the currenct focus and the new currency 
		*		should be added accordingly
		*@param aNewName indicates the name of the currecy being added
		*@param aNewFactor indicates the factor of the currency that will be added
		*/
		void AddNewCurrencyL( TInt aCurrencyId, TDesC& aNewName, TReal& aNewFactor , TCurrencyType aType);

	};

#endif // __CCNVCATEGORY_H__
