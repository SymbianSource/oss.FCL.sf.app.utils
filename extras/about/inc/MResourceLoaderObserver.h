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
* Description: Observer for Resource loader.
*
*/

#ifndef MRESOURCELOADEROBSERVER_H_
#define MRESOURCELOADEROBSERVER_H_

/*
 * The observer for CAboutResourceLoader.
 */
class MResourceLoaderObserver
    {
public:
    /*
     * Handle items loaded event. The items loaded event will be emit 
     * when fixed items loaded.
     * @aError KErrNone if there is no error occurs, 
     * otherwise return system wide error code.
     */
    virtual void HandleItemsLoadedL( TInt aError ) = 0;
    
    /*
     * Handle the resouce loading completed event.
     * @aResourceId the id for the resource which has been loaded.
     * @aError KErrNone if there is no error occurs, 
     * otherwise retrurn system wide error code.
     */
    virtual void HandleResourceLoadCompletedL( const TInt aResourceId, TInt aError ) = 0;
    };

#endif /* MRESOURCELOADEROBSERVER_H_ */
