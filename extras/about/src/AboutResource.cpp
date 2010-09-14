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

#include "AboutResource.h"


// CONSTANTS
const TInt KDefaultResourceIndex( -1 ); // Default resource index.
const TInt KDefaultBitmapId( -1 ); // Default bitmap id.

// ---------------------------------------------------------
// CAboutResource::CAboutResource()
//
// ---------------------------------------------------------
CAboutResource::CAboutResource():iBitmapId( KDefaultBitmapId ),
                                 iResourceItem( NULL ),
                                 iResourceIndex( KDefaultResourceIndex )
    {
    iResourceType = EAboutTextItem;
    }

// ---------------------------------------------------------
// CAboutResource::~CAboutResource()
//
// ---------------------------------------------------------
CAboutResource::~CAboutResource()
    {
    if ( iResourceItem )
        {
        delete iResourceItem;
        iResourceItem = NULL;
        }
    }

// ---------------------------------------------------------
// CAboutResource::NewLC()
//
// ---------------------------------------------------------
CAboutResource* CAboutResource::NewLC()
    {
    CAboutResource* self = new ( ELeave ) CAboutResource();
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------
// CAboutResource::NewL()
//
// ---------------------------------------------------------
CAboutResource* CAboutResource::NewL()
    {
    CAboutResource* self = CAboutResource::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

// ---------------------------------------------------------
// CAboutResource::ResourceIndex()
//
// ---------------------------------------------------------
TInt CAboutResource::ResourceIndex()
    {
    return iResourceIndex;
    }

// ---------------------------------------------------------
// CAboutResource::SetResourceIndex()
//
// ---------------------------------------------------------
void CAboutResource::SetResourceIndex( const TInt aIndex )
    {
    iResourceIndex = aIndex;
    }

// ---------------------------------------------------------
// CAboutResource::BitmapId()
//
// ---------------------------------------------------------
TInt CAboutResource::BitmapId()
    {
    return iBitmapId;
    }

// ---------------------------------------------------------
// CAboutResource::SetBitmapId()
//
// ---------------------------------------------------------
void CAboutResource::SetBitmapId( const TInt aBitmapId )
    {
    iBitmapId = aBitmapId;
    }

// ---------------------------------------------------------
// CAboutResource::ResourceType()
//
// ---------------------------------------------------------
TAboutItemType CAboutResource::ResourceType()
    {
    return iResourceType;
    }

// ---------------------------------------------------------
// CAboutResource::SetResourceType()
//
// ---------------------------------------------------------
void CAboutResource::SetResourceType( TAboutItemType aType )
    {
    iResourceType = aType;
    }

// ---------------------------------------------------------
// CAboutResource::ResourceItem()
//
// ---------------------------------------------------------
HBufC* CAboutResource::ResourceItem()
    {
    return iResourceItem;
    }

// ---------------------------------------------------------
// CAboutResource::SetResourceItem()
//
// ---------------------------------------------------------
void CAboutResource::SetResourceItem( HBufC* aItem )
    {
    iResourceItem = aItem;
    }

