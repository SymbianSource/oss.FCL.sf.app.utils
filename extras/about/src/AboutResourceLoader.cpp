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
* Description: Resource loader for About in order to asynchronous load resource.
*
*/

#include "AboutResourceLoader.h"
#include "MResourceLoaderObserver.h"    // For MResourceLoaderObserver
#include "AboutResource.h"              // For CAboutResource
#include <StringLoader.h>               // For StringLoader
#include <coneresloader.h>              // For RConeResourceLoader
#include <eikenv.h>                     // For CEikonEnv
#include <StringLoader.h>               // For StringLoader
#include <f32file.h>
#include <BAUTILS.H>

// CONSTANTS
const TInt KDefaultResourceId( -1 ); // Default resource id.
// The count for the resouce that to be loaded in one time.
const TInt KDefaultItemLoadingCount( 10 );

// ---------------------------------------------------------
// CAboutResourceLoader::CAboutResourceLoader()
//
// ---------------------------------------------------------
CAboutResourceLoader::CAboutResourceLoader( CEikonEnv* aEikonEnv,
                                    MResourceLoaderObserver* aObserver ) 
                     : CActive( EPriorityLow ),
                       iEnv( aEikonEnv ),
                       iResourceId( KDefaultResourceId ),
                       iObserver( aObserver )
    {
    
    }

// ---------------------------------------------------------
// CAboutResourceLoader::NewLC()
//
// ---------------------------------------------------------
CAboutResourceLoader* CAboutResourceLoader::NewLC( CEikonEnv* aEikonEnv, 
                                        MResourceLoaderObserver* aObserver )
    {
    CAboutResourceLoader* self = new ( ELeave ) CAboutResourceLoader( 
                                                    aEikonEnv, aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CAboutResourceLoader::NewL()
//
// ---------------------------------------------------------
CAboutResourceLoader* CAboutResourceLoader::NewL( CEikonEnv* aEikonEnv, 
                                        MResourceLoaderObserver* aObserver )
    {
    CAboutResourceLoader* self = CAboutResourceLoader::NewLC( 
                                            aEikonEnv, aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

// ---------------------------------------------------------
// CAboutResourceLoader::ConstructL()
//
// ---------------------------------------------------------
void CAboutResourceLoader::ConstructL()
    {
    CActiveScheduler::Add(this); // Add to scheduler
    iLoadedItem = new(ELeave) RPointerArray<CAboutResource>();
    iLoader = new(ELeave) RConeResourceLoader( *iEnv );
    }

// ---------------------------------------------------------
// CAboutResourceLoader::CAboutResourceLoader()
// 
// ---------------------------------------------------------
CAboutResourceLoader::~CAboutResourceLoader()
    {
    Cancel(); // Cancel any request, if outstanding
    if ( iLoadedItem )
        {
        iLoadedItem->ResetAndDestroy();
        delete iLoadedItem;
        iLoadedItem = NULL;
        }
    if ( iResourceBuffer )
        {
        delete iResourceBuffer;
        iResourceBuffer = NULL;
        }
    if ( iLoader )
        {
        iLoader->Close();
        delete iLoader;
        iLoader = NULL;
        }
    }

// ---------------------------------------------------------
// CAboutResourceLoader::DoCancel()
// 
// ---------------------------------------------------------
void CAboutResourceLoader::DoCancel()
    {
    iItemIterator = 0;
    // Delete the resource buffer.
    if ( iResourceBuffer )
        {
        delete iResourceBuffer;
        iResourceBuffer = NULL;
        }
    
    if ( iLoader )
        {
        iLoader->Close();
        }
    }

// ---------------------------------------------------------
// CAboutResourceLoader::StartL()
// 
// ---------------------------------------------------------
void CAboutResourceLoader::StartL()
    {
    // Leave with KErrGeneral if the resource id is not set.
    if ( iResourceId == KDefaultResourceId )
        {
        User::Leave( KErrGeneral );
        }
    
    Cancel();
    
    iItemIterator = 0;
    ReleaseLoadedItems();
    
    if ( !iLoaded )
        {
        // Init the rsc file path
        TFileName filePath( iResourcePath );
        BaflUtils::NearestLanguageFile( iEnv->FsSession(), filePath );
        iLoader->Close();
        iLoader->OpenL( filePath );
        }
    
    if ( iResourceBuffer )
        {
        delete iResourceBuffer;
        iResourceBuffer = NULL;
        }
    
    iResourceBuffer = iEnv->AllocReadResourceAsDes8LC( iResourceId );
    iReader.SetBuffer( iResourceBuffer );
    iItemCount = iReader.ReadInt16();
    CleanupStack::Pop(); // iResourceBuffer
    
    Activate();
    }

// ---------------------------------------------------------
// CAboutResourceLoader::RunL()
// 
// ---------------------------------------------------------
void CAboutResourceLoader::RunL()
    {
    TInt err = KErrNone;
    for ( ; iItemIterator < iItemCount; iItemIterator++ )
        {
        CAboutResource* aboutResource = CAboutResource::NewL();
        CleanupStack::PushL( aboutResource );
        TInt type = iReader.ReadInt8();
        
        // Load Text
        if ( type == EAboutTextItem )
            {
            HBufC* resourceItem( NULL );
            // There are two different text, one is direct output and another needs format
            if ( iReader.ReadInt8() == EAboutUpdateTime )
                {
                // About text needs format
                TTime time;
                time.UniversalTime();
                TDateTime currentTime = time.DateTime();
                resourceItem = StringLoader::LoadLC( iReader.ReadInt32(), 
                                            currentTime.Year(), CEikonEnv::Static() );
                }
            else
                {
                // Direct output about text
                resourceItem = iEnv->AllocReadResourceLC( iReader.ReadInt32() );
                }
            // Set the loaded item
            aboutResource->SetResourceItem( resourceItem );
            // Set the loaded item
            aboutResource->SetResourceIndex( iItemIterator );
            iLoadedItem->AppendL( aboutResource );
            CleanupStack::Pop( 2 ); // resourceItem and aboutResource
            }
        // Load image.
        else if ( type == EAboutImageItem )
            {
            TPtrC imageFile = iReader.ReadTPtrC();
            aboutResource->SetResourceItem( imageFile.AllocL() );
            aboutResource->SetBitmapId( iReader.ReadInt16() );
            aboutResource->SetResourceType( EAboutImageItem );
            iLoadedItem->AppendL( aboutResource );
            CleanupStack::Pop(); // aboutResource
            }
        else
            {
            err = KErrNotSupported;
            CleanupStack::PopAndDestroy(); // aboutResource
            break;
            }
        
        // Ten items per loading and calculate the number of loaded
        if ( ( iItemIterator % KDefaultItemLoadingCount ) 
                            == ( KDefaultItemLoadingCount - 1 ) )
            {
            // Once the value of iItemIterator > 10, we should break
            iItemIterator++;
            break;
            }
        }
    
    // Notify observer.
    if ( err == KErrNone && iItemIterator < iItemCount )
        {
        // Ten items loading over, Notify HandleItemsLoadedL observer
        iObserver->HandleItemsLoadedL( err );
        Activate();
        }
    else
        {
        // Once there is not enough ten items, need run this function again
        iObserver->HandleItemsLoadedL( err );
        // One resource loading over, Notify HandleResourceLoadCompletedL observer
        iObserver->HandleResourceLoadCompletedL( iResourceId, err );
        }
    }

// ---------------------------------------------------------
// CAboutResourceLoader::RunError()
//
// ---------------------------------------------------------
TInt CAboutResourceLoader::RunError( TInt aError )
    {
    return aError;
    }

// ---------------------------------------------------------
// CAboutResourceLoader::Activate()
//
// ---------------------------------------------------------
void CAboutResourceLoader::Activate()
    {
    TRequestStatus* status = &iStatus;
    *status = KRequestPending;
    User::RequestComplete( status, KErrNone );
    SetActive();
    }

// ---------------------------------------------------------
// CAboutResourceLoader::SetResourceId()
//
// ---------------------------------------------------------
void CAboutResourceLoader::SetResourceId( const TInt aResourceId, 
                        const TBool aLoaded, const TDesC& aResourcePath )
    {
    iResourceId = aResourceId;
    iLoaded = aLoaded;
    iResourcePath.Copy( aResourcePath );
    }

// ---------------------------------------------------------
// CAboutResourceLoader::CurrentResourceId()
//
// ---------------------------------------------------------
TInt CAboutResourceLoader::CurrentResourceId()
    {
    return iResourceId;
    }

// ---------------------------------------------------------
// CAboutResourceLoader::LoadedResourceItems()
//
// ---------------------------------------------------------
RPointerArray<CAboutResource>* CAboutResourceLoader::LoadedResourceItems() const
    {
    return iLoadedItem;
    }

// ---------------------------------------------------------
// CAboutResourceLoader::RleaseLoadedItems()
//
// ---------------------------------------------------------
void CAboutResourceLoader::ReleaseLoadedItems()
    {
    iLoadedItem->ResetAndDestroy();
    }

// ---------------------------------------------------------
// CAboutResourceLoader::ItemCount()
//
// ---------------------------------------------------------
TInt CAboutResourceLoader::ItemCount() const
    {
    return iItemCount;
    }
