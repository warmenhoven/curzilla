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

#include "nsRegionCurses.h"
#include "nsCursesGfxLog.h"
#include "nsMemory.h"

nsRegionCurses::nsRegionCurses()
{
	LOG(("nsRegionCurses::nsRegionCurses()\n"));
	size = 0;
	numRects = 0;
	rects = nsnull;
	extents.x = 0;
	extents.y = 0;
	extents.width = 0;
	extents.height = 0;
}

nsRegionCurses::~nsRegionCurses()
{
	LOG(("nsRegionCurses::~nsRegionCurses()\n"));
	if (rects)
		delete [] rects;
	rects = nsnull;
}

NS_IMPL_ISUPPORTS1(nsRegionCurses, nsIRegion)

nsresult
nsRegionCurses::Init(void)
{
	LOG(("nsRegionCurses::Init()\n"));
	size = 0;
	numRects = 0;
	if (rects)
		delete [] rects;
	rects = nsnull;
	extents.x = 0;
	extents.y = 0;
	extents.width = 0;
	extents.height = 0;
	return NS_OK;
}

void
nsRegionCurses::SetTo(const nsIRegion &aRegion)
{
	LOG(("nsRegionCurses::SetTo()\n"));
	nsRegionCurses *pRegion = (nsRegionCurses *)&aRegion;
	size = pRegion->numRects;
	numRects = pRegion->numRects;
	extents = pRegion->extents;
	if (pRegion->numRects) {
		rects = new nsRect[pRegion->numRects];
		memcpy(rects, pRegion->rects, sizeof (nsRect) * pRegion->numRects);
	}
}

void
nsRegionCurses::SetTo(PRInt32 aX, PRInt32 aY, PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsRegionCurses::SetTo()\n"));
	size = 1;
	numRects = 1;
	rects = new nsRect[1];
	extents.x = rects[0].x = aX;
	extents.y = rects[0].y = aY;
	extents.width = rects[0].width = aWidth;
	extents.height = rects[0].height = aHeight;
}

void
nsRegionCurses::Intersect(const nsIRegion &aRegion)
{
	LOG(("nsRegionCurses::Intersect()\n"));
}

void
nsRegionCurses::Intersect(PRInt32 aX, PRInt32 aY, PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsRegionCurses::Intersect()\n"));
}

void
nsRegionCurses::Union(const nsIRegion &aRegion)
{
	LOG(("nsRegionCurses::Union(Region)\n"));
	if (!numRects) {
		SetTo(aRegion);
		return;
	}

	nsRegionCurses *pRegion = (nsRegionCurses *)&aRegion;

	if ((numRects == 1) &&
		(extents.x <= pRegion->extents.x) &&
		(extents.y <= pRegion->extents.y) &&
		(extents.width >= pRegion->extents.width) &&
		(extents.height >= pRegion->extents.height)) {
		return;
	}

	if ((pRegion->numRects == 1) &&
		(extents.x >= pRegion->extents.x) &&
		(extents.y >= pRegion->extents.y) &&
		(extents.width <= pRegion->extents.width) &&
		(extents.height <= pRegion->extents.height)) {
		SetTo(aRegion);
		return;
	}

}

void
nsRegionCurses::Union(PRInt32 aX, PRInt32 aY, PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsRegionCurses::Union()\n"));
	if (size && numRects) {
		if (aWidth > 0 && aHeight > 0) {
			nsRegionCurses tmp;
			tmp.SetTo(aX, aY, aWidth, aHeight);
			Union(tmp);
		}
	} else {
		SetTo(aX, aY, aWidth, aHeight);
	}
}

void
nsRegionCurses::Subtract(const nsIRegion &aRegion)
{
	LOG(("nsRegionCurses::Subtract()\n"));
}

void
nsRegionCurses::Subtract(PRInt32 aX, PRInt32 aY, PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsRegionCurses::Subtract()\n"));
}

PRBool
nsRegionCurses::IsEmpty(void)
{
	LOG(("nsRegionCurses::IsEmpty()\n"));
	if (numRects == 0)
		return PR_TRUE;
	else
		return PR_FALSE;
}

PRBool
nsRegionCurses::IsEqual(const nsIRegion &aRegion)
{
	LOG(("nsRegionCurses::IsEqual()\n"));
	return PR_TRUE;
}

void
nsRegionCurses::GetBoundingBox(PRInt32 *aX, PRInt32 *aY, PRInt32 *aWidth, PRInt32 *aHeight)
{
	LOG(("nsRegionCurses::GetBoundingBox()\n"));
	*aX = extents.x;
	*aY = extents.y;
	*aWidth = extents.width;
	*aHeight = extents.height;
}

void
nsRegionCurses::Offset(PRInt32 aXOffset, PRInt32 aYOffset)
{
	LOG(("nsRegionCurses::Offset()\n"));
}

PRBool
nsRegionCurses::ContainsRect(PRInt32 aX, PRInt32 aY, PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsRegionCurses::ContainsRect()\n"));
	return PR_FALSE;
}

NS_IMETHODIMP
nsRegionCurses::GetRects(nsRegionRectSet **aRects)
{
	LOG(("nsRegionCurses::GetRects()\n"));
	nsRegionRectSet *retval;
	nsRegionRect *regionrect;

	*aRects = nsnull;

	if (!numRects) {
		retval = (nsRegionRectSet *)nsMemory::Alloc(sizeof(nsRegionRectSet));
		if (!retval)
			return NS_ERROR_OUT_OF_MEMORY;

		retval->mNumRects = 0;
		retval->mRectsLen = 0;
		retval->mArea = 0;

		*aRects = retval;

		return NS_OK;
	}

	retval = (nsRegionRectSet *)
		nsMemory::Alloc(sizeof(nsRegionRectSet) +
						(sizeof(nsRegionRect) * (numRects - 1)));
	if (!retval)
		return NS_ERROR_OUT_OF_MEMORY;

	regionrect = &retval->mRects[0];
	retval->mNumRects = numRects;
	retval->mRectsLen = numRects;

	int currect = 0;
	while (currect < numRects) {
		regionrect->x = rects[currect].x;
		regionrect->y = rects[currect].y;
		regionrect->width = rects[currect].width;
		regionrect->height = rects[currect].height;

		retval->mArea += rects[currect].width * rects[currect].height;

		currect++;
		regionrect++;
	}

	*aRects = retval;
	return NS_OK;
}

NS_IMETHODIMP
nsRegionCurses::FreeRects(nsRegionRectSet *aRects)
{
	LOG(("nsRegionCurses::FreeRects()\n"));
	if (nsnull != aRects)
		nsMemory::Free(aRects);

	return NS_OK;
}

NS_IMETHODIMP
nsRegionCurses::GetNativeRegion(void *&aRegion) const
{
	LOG(("nsRegionCurses::GetNativeRegion()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRegionCurses::GetRegionComplexity(nsRegionComplexity &aComplexity) const
{
	LOG(("nsRegionCurses::GetRegionComplexity()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRegionCurses::GetNumRects(PRUint32 *aRects) const
{
	LOG(("nsRegionCurses::GetNumRects()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}
