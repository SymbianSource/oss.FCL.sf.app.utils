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
* Description:  Implemented all inline functions for CCalcContainer.  
*
*/


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCalcContainer::FuncmapSubPane
// Return pointer of FunctionMap subpane class
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CCalcFuncmapSubPane* CCalcContainer::FuncmapSubPane() const
    {
    return iFuncmapPane;
    }

// ---------------------------------------------------------
// CCalcContainer::EditorPane
// Return pointer of editor subpane.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CCalcEditorSubPane* CCalcContainer::EditorPane() const
    {
    return iEditorPane;
    }

// ---------------------------------------------------------
// CCalcContainer::OutputSheet
// Return pointer of OutputSheet.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CCalcOutputSheet* CCalcContainer::OutputSheet() const
    {
    return iSheetPane;
    }

// ---------------------------------------------------------
// CCalcContainer::View
// Return pointer of CCalcView.
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline CCalcView* CCalcContainer::View() const
    {
    return iView;
    }


	//EISkin2.6
// ---------------------------------------------------------
// CCalcContainer::FunctionSubPane
// Gets the function subpane when the skin changes.
// ---------------------------------------------------------
//
inline CCalcFuncmapSubPane* CCalcContainer::FunctionSubPane()const
{
	return iFuncmapPane;
}
// End of File
