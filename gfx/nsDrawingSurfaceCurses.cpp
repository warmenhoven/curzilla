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

#include "nsDrawingSurfaceCurses.h"
#include "nsCursesGfxLog.h"

NS_IMPL_ISUPPORTS1(nsDrawingSurfaceCurses, nsIDrawingSurface)

nsDrawingSurfaceCurses::nsDrawingSurfaceCurses()
{
	DBG(("nsDrawingSurfaceCurses::nsDrawingSurfaceCurses()\n"));
}

nsDrawingSurfaceCurses::~nsDrawingSurfaceCurses()
{
	LOG(("nsDrawingSurfaceCurses::~nsDrawingSurfaceCurses()\n"));
}

/**
 * Lock a rect of a drawing surface and return a
 * pointer to the upper left hand corner of the
 * bitmap.
 * @param  aX x position of subrect of bitmap
 * @param  aY y position of subrect of bitmap
 * @param  aWidth width of subrect of bitmap
 * @param  aHeight height of subrect of bitmap
 * @param  aBits out parameter for upper left hand
 *         corner of bitmap
 * @param  aStride out parameter for number of bytes
 *         to add to aBits to go from scanline to scanline
 * @param  aWidthBytes out parameter for number of
 *         bytes per line in aBits to process aWidth pixels
 * @return error status
 *
 **/
NS_IMETHODIMP
nsDrawingSurfaceCurses::Lock(PRInt32 aX, PRInt32 aY,
			  PRUint32 aWidth, PRUint32 aHeight,
			  void **aBits, PRInt32 *aStride,
			  PRInt32 *aWidthBytes, PRUint32 aFlags)
{
	LOG(("nsDrawingSurfaceCurses::Lock()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDrawingSurfaceCurses::Unlock(void)
{
	LOG(("nsDrawingSurfaceCurses::Unlock()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDrawingSurfaceCurses::GetDimensions(PRUint32 *aWidth, PRUint32 *aHeight)
{
	LOG(("nsDrawingSurfaceCurses::GetDimensions()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDrawingSurfaceCurses::IsOffscreen(PRBool *aOffScreen)
{
	LOG(("nsDrawingSurfaceCurses::IsOffscreen()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDrawingSurfaceCurses::IsPixelAddressable(PRBool *aAddressable)
{
	LOG(("nsDrawingSurfaceCurses::IsPixelAddressable()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDrawingSurfaceCurses::GetPixelFormat(nsPixelFormat *aFormat)
{
	LOG(("nsDrawingSurfaceCurses::GetPixelFormat()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}
