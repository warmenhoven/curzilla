/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim:expandtab:shiftwidth=4:tabstop=4:
 */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code mozilla.org code.
 *
 * The Initial Developer of the Original Code Christopher Blizzard
 * <blizzard@mozilla.org>.  Portions created by the Initial Developer
 * are Copyright (C) 2001 the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsClipboard.h"
#include "nsAppShell.h"

nsClipboard::nsClipboard()
{
	DBG(("nsClipboard::nsClipboard()\n"));
}

nsClipboard::~nsClipboard()
{
	DBG(("nsClipboard::~nsClipboard()\n"));
}

NS_IMPL_ISUPPORTS1(nsClipboard, nsIClipboard)

nsresult
nsClipboard::Init(void)
{
	DBG(("nsClipboard::Init()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsClipboard::SetData(nsITransferable *aTransferable,
                     nsIClipboardOwner *aOwner, PRInt32 aWhichClipboard)
{
	DBG(("nsClipboard::SetData()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsClipboard::GetData(nsITransferable *aTransferable, PRInt32 aWhichClipboard)
{
	DBG(("nsClipboard::GetData()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsClipboard::EmptyClipboard(PRInt32 aWhichClipboard)
{
	DBG(("nsClipboard::EmptyClipboard()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsClipboard::HasDataMatchingFlavors(nsISupportsArray *aFlavorList,
                                    PRInt32 aWhichClipboard, PRBool *_retval)
{
	DBG(("nsClipboard::HasDataMatchingFlavors()\n"));
	*_retval = PR_FALSE;
	return NS_OK;
}

NS_IMETHODIMP
nsClipboard::SupportsSelectionClipboard(PRBool *_retval)
{
	DBG(("nsClipboard::SupportsSelectionClipboard()\n"));
	*_retval = PR_FALSE;
	return NS_OK;
}
