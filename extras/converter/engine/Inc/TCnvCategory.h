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
*     This class can be seen as a view to a subset (one unit category) of
*     the data stored in the data model.
*	  It does not own or copy any model data.
*     This class implements the Model Access API.
*
*/



#ifndef __TCNVCATEGORY_H__
#define __TCNVCATEGORY_H__

// INCLUDES
#include <e32base.h>
#include <AknQueryValue.h>
#include "CCnvConverter.h"
// FORWARD DECLARATIONS
class CCnvCategory;
class MDesCArray;

enum TPanicCode
	{
	EPanicPreCond_Convert = 1,
	EPanicPreCond_UnitFactor,
	EPanicPreCond_UnitMinimum,
	EPanicPreCond_SetUnitFactor,
	EPanicPreCond_SetUnitName,
	EPanicPreCond_BaseUnit,
	EPanicPreCond_SetBaseUnit,
	EPanicPreCond_GetDefaultUnits,
	EPanicPreCond_SetDefaultUnits,
	EPanicPreCond_CurrentValueTextLC
	};

_LIT(KPanicText, "TCnvCategory");

// CLASS DEFINITION
/**
* This class can be seen as a view to a subset (one unit category) of
* the data stored in the data model. It does not own or copy any model data.
* This class implements the Model Access API.
*/
class TCnvCategory : public MAknQueryValue
	{
	public: // Constructors.

 		/**
		* Default constructor.
		*/
		IMPORT_C TCnvCategory();

 		/**
		* Constructor.
		*/
		TCnvCategory( CCnvCategory* aCategory );

		/**
		* Constructor.
		*/
		TCnvCategory( MDesCArray* aDesCArray );

	public: // from MAknQueryValue

		/**
		* Returns a descriptor array interface to the category data.
		* @return Text array accessor.
		*/
		IMPORT_C const MDesCArray* MdcArray() const;

		/**
		* Returns the text associated with the current selection.
		* @return currently selected item's text.
		*/
		IMPORT_C HBufC* CurrentValueTextLC();

		/**
		* Returns the index of the current selection. 
		* @return currently selected item.
		*/
		IMPORT_C TInt CurrentValueIndex() const;

		/**
		* Set the index of the current selection. 
		* @param aIndex Newly selected item.
		*/
		IMPORT_C void SetCurrentValueIndex( const TInt aIndex );

	public: // new methods

		/**
		* Calculates the conversion from source unit to destination unit.
		* @param aSourceUnit Index of the source unit.
		* @param aDestinationUnit Index of the destination unit.
		* @param aAmount Convert this many source units.
		* @return Result of conversion, rounded to aDecimals decimal places.
		*/
		IMPORT_C TReal Convert( TUint aSourceUnit, TUint aDestinationUnit,
								const TReal& aAmount,
								TUint aDecimals = 5 ) const;

		/**
		* Returns the factor (rate) of the specified unit.
		* @param unit index (identifier).
		*/
		IMPORT_C TReal UnitFactor( TUint aUnit ) const;

		/**
		* Returns the minimum allowable amount for the specified unit.
		* @param unit index (identifier).
		*/
		IMPORT_C TReal UnitMinimum( TUint aUnit ) const;

		/**
		* Changes the factor (rate) of the specified unit.
		* @param aUnit index (identifier).
		* @param aNewFactor new factor for the unit.
		*/
		IMPORT_C void SetUnitFactor( TUint aUnit, const TReal& aNewFactor );

		/**
		* Changes the name of the specified unit.
		* @param aUnit index (identifier).
		* @param aNewName new name for the unit.
		*/
		IMPORT_C void SetUnitName( TUint aUnit, const TDesC& aNewName , TCurrencyType aType);

		/**
		* Returns the index of the base unit.
		* @return base unit index.
		*/
		IMPORT_C TUint BaseUnit() const;

		/**
		* Changes the index of the base unit.
		* @param aBaseUnit index.
		*/
		IMPORT_C void SetBaseUnit( TUint aBaseUnit );

		/**
		* Returns the default unit indices.
		* @param aSourceUnit Index placeholder of the source unit.
		* @param aDestinationUnit Index placeholder of the destination unit.
		*/
		IMPORT_C void GetDefaultUnits( TUint& aSourceUnit,
									   TUint& aDestinationUnit ) const;

		/**
		* Sets the default unit indices.
		* @param aSourceUnit Index of the new source unit.
		* @param aDestinationUnit Index of the new destination unit.
		*/
		IMPORT_C void SetDefaultUnits( TUint aSourceUnit,
									   TUint aDestinationUnit );
		/**
		*Deletes a particular currency  from the list.
		*@param aControlId indicates which currency has to be deleted.
		*/
		IMPORT_C void AddNewCurrencyL(TInt aControlId, TDesC& aNewName , TReal& aNewFactor, TCurrencyType aType);
		/**
		*Adds a new field at a given position(focus)
		*@param aControlId indicates the currenct focus and the new currency 
		*		should be added accordingly
		*@param aNewName indicates the name of the currecy being added
		*@param aNewFactor indicates the factor of the currency that will be added
		*/
		IMPORT_C void DeleteCurrencyL( TInt aCurrencyId );
        
        /**
        *Create a temp array to save the previous unit for restore
        *
        */
        IMPORT_C void CreateTempUnitArrayForRestore();

        /**
        *restore the unit array that not be modified
        *
        */
        IMPORT_C void RestoreUnitArray();

    private: // from MAknQueryValue, empty implementations, not to be used!
			 // inlined to avoid exporting these.

		inline TBool CreateEditorL() { return EFalse; }
		inline void SetQueryMode( MAknQueryValue::TMode ) {};
		inline void SetSettingPageResourceIds( TInt, TInt ) {};
		inline void SetAutoAppend( TBool ) {};	
		inline void AppendValueIfNewL() {};

	private: // data

		/**
		* Pointer to the category object this class accesses. Not owned.
		*/
		CCnvCategory* iCategory;

		/**
		* Pointer to the name array. Not owned. (MAknQueryValue interface).
		*/
		MDesCArray* iDesCArray;

		/**
		* Index of the currently selected value (MAknQueryValue interface).
		*/
		TUint iCurrentValueIndex;

		/**
		* ETrue if the instance can be used only as a MAknQueryValue.
		* It is the case when this class is
		* constructed by TCnvCategory( MDesCArray* aDesCArray ) 
		*/
		TBool iIsQueryValueOnly;

	};

#endif // __TCNVCATEGORY_H__
