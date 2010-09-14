/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Resource for About resource loader.
*
*/

#ifndef ABOUTRESOURCE_H
#define ABOUTRESOURCE_H

// INCLUDES
#include "about.hrh"
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CAboutResource
 *  This class's main purpose is transfer data between container and loader
 */
class CAboutResource : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CAboutResource();

    /**
     * Two-phased constructor.
     */
    static CAboutResource* NewL();

    /**
     * Two-phased constructor.
     */
    static CAboutResource* NewLC();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CAboutResource();

public:

    /**
     * Get the index number of resource
     * @return get the the count of the resource item
     */
    TInt ResourceIndex();
    
    /**
     * Set the index number of rsource
     * @param aIndex the index which you want to set
     */
    void SetResourceIndex( const TInt aIndex );

    /**
     * Get the image's id
     * @return  get the pic's id which you want to display
     */
    TInt BitmapId();

    /**
     * Set the loaded image's id
     * @param aBitmapId set the pic's id which you want to display
     */
    void SetBitmapId( const TInt aBitmapId );

    /**
     * Get the string's type which need to be loaded 
     * @return TAboutItemType get the resource type defineed in .rh file
     */
    TAboutItemType ResourceType();

    /**
     * Set the string's type which need to be loaded 
     * @param aType set the resource type
     */
    void SetResourceType( const TAboutItemType aType );

    /**
     * Get about text or about image path
     * @return HBufC a buffer for transter data
     */
    HBufC* ResourceItem();

    /**
     * Set about text or about image path
     * @param aItem a buffer for transter data
     */
    void SetResourceItem( HBufC* aItem );
    
private:

    // The index of the resource
    TInt iResourceIndex;
    
    // Id for the bitmap
    TInt iBitmapId;
    
    // Resource item content
    HBufC* iResourceItem;
    
    // Resource item type
    TAboutItemType iResourceType;
    };

#endif // ABOUTRESOURCE_H
