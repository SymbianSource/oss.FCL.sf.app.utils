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
*     This class is a data container of unit categories.
*     It is able to construct itself from a resource file.
*     This class implements the Model Instantiation API.
*
*/



#ifndef __CCNVCONVERTER_H__
#define __CCNVCONVERTER_H__

// INCLUDES
#include <e32base.h>
#include <bamdesca.h>

// FORWARD DECLARATIONS
class CCnvCategory;
class TCnvCategory;
class RWriteStream;
class TResourceReader;

// CLASS DEFINITION
/**
* This class is a data container of unit categories.
* It is able to construct itself from a resource file.
* This class implements the Model Instantiation API.
*/
enum TCurrencyType
{
	ENotApplicable,
	ENameChanged,
	EHome,
	EForeign
};

class CCnvConverter : public CBase, public MDesCArray
	{
	public:	// Constructors and destructor.

		/**
		* Destructor.
		*/
		IMPORT_C ~CCnvConverter();

		/**
		* Two-phased constructor
		* @param aReader A resource reader object initialised to a resource.
		*/
		IMPORT_C static CCnvConverter* NewL( TResourceReader& aReader );

	private: // Constructors

		/**
		* Default constructor.
		* 1 = minimum array granularity
		*/
		inline CCnvConverter() : iCategoryArray( 1 ) {};

		/**
		* Constructs the model a CNV_MODEL resource.
		* @param aReader A resource reader object initialised to a resource.
		*/
		void ConstructFromResourceL( TResourceReader& aReader );

	public: // from MDesCArray (need not to be imported, vftable takes care..)

		/**
		* Returns the number of unit categories.
		* @return number of items.
		*/
		TInt MdcaCount() const;

		/**
		* Returns a pointer to a unit category name specified by aIndex.
		* @param aIndex Defines the point in the array that is 
		*        queried for the name.
		* @return Pointer to category name.
		*/
		TPtrC MdcaPoint( TInt aIndex ) const;
 
	public: // new methods

		/**
		* Returns a category accessor for the requested category.
		* @param aCategory Category accessor placeholder.
		* @param aCategoryIndex Category identifier.
		*/
		IMPORT_C void GetCategoryAccessor( TCnvCategory& aCategory,
										   TUint aCategoryIndex );
		/**
		* Return a category accessor for the category list.
		* @param aCategory Category accessor placeholder.
		*/
		IMPORT_C void GetCategorylistAccessor( TCnvCategory& aCategory );

		/**
		* Returns the index of the currency category.
		* @return category index.
		*/
		IMPORT_C TUint CurrencyCategoryId() const;

		/**
		* Stores the category array to the given stream.
		* @param aStream Stream for writing.
		*/
		IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

		/**
		* Loads the category array from the given stream.
		* @param aStream Stream for reading.
		*/
		IMPORT_C void InternalizeL( RReadStream& aStream );

	private: // new methods

		/**
		* Skips zero or more category structures.
		* @param aCount Skips this many categories
		* @param aReader A resource reader object initialised to a resource.
		*/
		void SkipCategoriesL( TInt aCount, TResourceReader& aReader );

	private: // data

		/**
		* The running version number of the model data (structure and content)
		*/
		TUint iModelVersion;

		/**
		* The index of the category containing currencies (in the array).
		*/
		TUint iCurrencyCategory;
		
		/**
		* The array for categories in this model. Owned.
		*/
		CArrayPtrSeg< CCnvCategory > iCategoryArray;

	};

#endif // __CCNVCONVERTER_H__

