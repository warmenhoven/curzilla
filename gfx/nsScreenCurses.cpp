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

#include "nsScreenCurses.h"
#include "nsCursesGfxLog.h"

#include <curses.h>

nsScreenCurses::nsScreenCurses()
{
	DBG(("nsScreenCurses::nsScreenCurses()\n"));
}

nsScreenCurses::~nsScreenCurses()
{
	DBG(("nsScreenCurses::~nsScreenCurses()\n"));
}

NS_IMPL_ISUPPORTS1(nsScreenCurses, nsIScreen)

NS_IMETHODIMP
nsScreenCurses::GetPixelDepth(PRInt32 *aPixelDepth)
{
	DBG(("nsScreenCurses::GetPixelDepth()\n"));
	*aPixelDepth = 8;
	return NS_OK;
}

NS_IMETHODIMP
nsScreenCurses::GetColorDepth(PRInt32 *aColorDepth)
{
	DBG(("nsScreenCurses::GetColorDepth()\n"));
	return GetPixelDepth(aColorDepth);
}

NS_IMETHODIMP
nsScreenCurses::GetRect(PRInt32 *outLeft, PRInt32 *outTop, PRInt32 *outWidth, PRInt32 *outHeight)
{
	DBG(("nsScreenCurses::GetRect()\n"));
	*outTop = 0;
	*outLeft = 0;
	*outWidth = COLS * 5;
	*outHeight = LINES * 8;
	return NS_OK;
}

NS_IMETHODIMP
nsScreenCurses::GetAvailRect(PRInt32 *outLeft, PRInt32 *outTop, PRInt32 *outWidth, PRInt32 *outHeight)
{
	DBG(("nsScreenCurses::GetAvailRect()\n"));
	*outTop = 0;
	*outLeft = 0;
	*outWidth = COLS * 5;
	*outHeight = LINES * 8;
	return NS_OK;
}