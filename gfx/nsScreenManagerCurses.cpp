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
 * The Initial Developer of the Original Code Eric Warmenhoven
 * <eric@warmenhoven.org>.  Portions created by the Initial Developer
 * are Copyright (C) 2003 the Initial Developer. All Rights Reserved.
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

#include "nsScreenManagerCurses.h"
#include "nsScreenCurses.h"
#include "nsCursesGfxLog.h"

nsScreenManagerCurses::nsScreenManagerCurses()
{
	DBG(("nsScreenManagerCurses::nsScreenManagerCurses()\n"));
	NS_INIT_ISUPPORTS();
	mScreen = nsnull;
}

nsScreenManagerCurses::~nsScreenManagerCurses()
{
	DBG(("nsScreenManagerCurses::~nsScreenManagerCurses()\n"));
}

NS_IMPL_ISUPPORTS1(nsScreenManagerCurses, nsIScreenManager)

NS_IMETHODIMP
nsScreenManagerCurses::ScreenForRect(PRInt32 inLeft, PRInt32 inTop, PRInt32 inWidth,
				     PRInt32 inHeight, nsIScreen **outScreen)
{
	DBG(("nsScreenManagerCurses::ScreenForRect()\n"));
	return GetPrimaryScreen(outScreen);
}

NS_IMETHODIMP
nsScreenManagerCurses::GetPrimaryScreen(nsIScreen * *aPrimaryScreen)
{
	DBG(("nsScreenManagerCurses::GetPrimaryScreen()\n"));
	*aPrimaryScreen = mScreen = nsnull;
	return NS_OK;

	if (!mScreen) {
		mScreen = new nsScreenCurses();
		if (!mScreen)
			return NS_ERROR_OUT_OF_MEMORY;
	}
	*aPrimaryScreen = mScreen;

	return NS_OK;
}

NS_IMETHODIMP
nsScreenManagerCurses::GetNumberOfScreens(PRUint32 *aNumberOfScreens)
{
	DBG(("nsScreenManagerCurses::GetNumberOfScreens()\n"));
	*aNumberOfScreens = 1;
	return NS_OK;
}
