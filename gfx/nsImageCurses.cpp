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

#include "nsImageCurses.h"
#include "nsRenderingContextCurses.h"
#include "nsCursesGfxLog.h"

NS_IMPL_ISUPPORTS1(nsImageCurses, nsIImage)

nsImageCurses::nsImageCurses()
{
	LOG(("nsImageCurses::nsImageCurses()\n"));
	mImageBits = nsnull;
	mTrueAlphaBits = nsnull;
	mAlphaBits = nsnull;
	mWidth = 0;
	mHeight = 0;
	mRowBytes = 0;
	mSizeImage = 0;
	mAlphaRowBytes = 0;
	mTrueAlphaRowBytes = 0;
	mAlphaDepth = 0;
	mTrueAlphaDepth = 0;
	mDepth = 0;
}

nsImageCurses::~nsImageCurses()
{
	LOG(("nsImageCurses::~nsImageCurses()\n"));
	if(nsnull != mImageBits) {
		delete[] mImageBits;
		mImageBits = nsnull;
	}

	if (nsnull != mAlphaBits) {
		delete[] mAlphaBits;
		mAlphaBits = nsnull;
	}

	if (nsnull != mTrueAlphaBits) {
		delete[] mTrueAlphaBits;
		mTrueAlphaBits = nsnull;
	}
}

nsresult
nsImageCurses::Init(PRInt32 aWidth, PRInt32 aHeight,
		    PRInt32 aDepth, nsMaskRequirements aMaskRequirements)
{
	LOG(("nsImageCurses::Init()\n"));
	mWidth = aWidth;
	mHeight = aHeight;
	mDepth = aDepth;

	mRowBytes = (mWidth * mDepth) >> 5;

	if (((PRUint32)mWidth * mDepth) & 0x1F)
		mRowBytes++;
	mRowBytes <<= 2;

	mSizeImage = mRowBytes * mHeight;

	mImageBits = (PRUint8*) new PRUint8[mSizeImage];

	switch(aMaskRequirements) {
	case nsMaskRequirements_kNeeds8Bit:
		mTrueAlphaRowBytes = aWidth;
		mTrueAlphaDepth = 8;

		// 32-bit align each row
		mTrueAlphaRowBytes = (mTrueAlphaRowBytes + 3) & ~0x3;
		mTrueAlphaBits = new PRUint8[mTrueAlphaRowBytes * aHeight];
		memset(mTrueAlphaBits, 0, mTrueAlphaRowBytes*aHeight);

		// FALL THROUGH

	case nsMaskRequirements_kNeeds1Bit:
		mAlphaRowBytes = (aWidth + 7) / 8;
		mAlphaDepth = 1;

		// 32-bit align each row
		mAlphaRowBytes = (mAlphaRowBytes + 3) & ~0x3;

		mAlphaBits = new PRUint8[mAlphaRowBytes * aHeight];
		memset(mAlphaBits, 0, mAlphaRowBytes*aHeight);
		break;

	default:
		break; // avoid compiler warning
	}

	if (aMaskRequirements == nsMaskRequirements_kNeeds8Bit)
		mAlphaDepth = 0;

	return NS_OK;
}

PRInt32
nsImageCurses::GetBytesPix()
{
	LOG(("nsImageCurses::GetBytesPix()\n"));
	return (mDepth / 8);
}

PRInt32
nsImageCurses::GetHeight()
{
	LOG(("nsImageCurses::GetHeight()\n"));
	return mHeight;
}

PRInt32
nsImageCurses::GetWidth()
{
	LOG(("nsImageCurses::GetWidth()\n"));
	return mWidth;
}

PRUint8 *
nsImageCurses::GetBits()
{
	LOG(("nsImageCurses::GetBits()\n"));
	return mImageBits;
}

void *
nsImageCurses::GetBitInfo()
{
	LOG(("nsImageCurses::GetBitInfo()\n"));
	return nsnull;
}

PRBool
nsImageCurses::GetIsRowOrderTopToBottom()
{
	LOG(("nsImageCurses::GetIsRowOrderTopToBottom()\n"));
	return PR_TRUE;
}

PRInt32
nsImageCurses::GetLineStride()
{
	LOG(("nsImageCurses::GetLineStride()\n"));
	return mRowBytes;
}

nsColorMap *
nsImageCurses::GetColorMap()
{
	LOG(("nsImageCurses::GetColorMap()\n"));
	return nsnull;
}

PRBool
nsImageCurses::GetHasAlphaMask()
{
	LOG(("nsImageCurses::GetHasAlphaMask()\n"));
	return (mAlphaBits != nsnull);
}

PRUint8 *
nsImageCurses::GetAlphaBits()
{
	LOG(("nsImageCurses::GetAlphaBits()\n"));
	if (mTrueAlphaBits)
		return mTrueAlphaBits;
	else
		return mAlphaBits;
}

PRInt32
nsImageCurses::GetAlphaLineStride()
{
	LOG(("nsImageCurses::GetAlphaLineStride()\n"));
	if (mTrueAlphaBits)
		return mTrueAlphaRowBytes;
	else
		return mAlphaRowBytes;
}

PRInt8
nsImageCurses::GetAlphaDepth()
{
	LOG(("nsImageCurses::GetAlphaDepth()\n"));
	if (mTrueAlphaBits)
		return mTrueAlphaDepth;
	else
		return mAlphaDepth;
}

void
nsImageCurses::ImageUpdated(nsIDeviceContext *aContext,
			  PRUint8 aFlags,
			  nsRect *aUpdateRect)
{
	LOG(("nsImageCurses::ImageUpdated()\n"));
}

NS_IMETHODIMP
nsImageCurses::Draw(nsIRenderingContext &aContext, nsIDrawingSurface* aSurface,
		  PRInt32 aSX, PRInt32 aSY, PRInt32 aSWidth, PRInt32 aSHeight,
		  PRInt32 aDX, PRInt32 aDY, PRInt32 aDWidth, PRInt32 aDHeight)
{
	LOG(("nsImageCurses::Draw()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nsImageCurses::Draw(nsIRenderingContext &aContext,
                 nsIDrawingSurface* aSurface,
                 PRInt32 aX, PRInt32 aY,
                 PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsImageCurses::Draw()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsImageCurses::DrawTile(nsIRenderingContext &aContext,
		      nsIDrawingSurface* aSurface,
		      PRInt32 aSXOffset, PRInt32 aSYOffset,
		      PRInt32 aPadX, PRInt32 aPadY,
		      const nsRect &aTileRect)
{
	LOG(("nsImageCurses::DrawTile()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult
nsImageCurses::Optimize(nsIDeviceContext* aContext)
{
	LOG(("nsImageCurses::Optimize()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsImageCurses::LockImagePixels(PRBool aMaskPixels)
{
	LOG(("nsImageCurses::LockImagePixels()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsImageCurses::UnlockImagePixels(PRBool aMaskPixels)
{
	LOG(("nsImageCurses::UnlockImagePixels()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsImageCurses::DrawToImage(nsIImage* aDstImage,
			 nscoord aDX, nscoord aDY,
			 nscoord aDWidth, nscoord aDHeight)
{
	LOG(("nsImageCurses::DrawToImage()\n"));
	return NS_OK;
}
