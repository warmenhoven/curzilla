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

#include "nsRenderingContextCurses.h"
#include "nsCursesGfxLog.h"

PRLogModuleInfo *gGfxLog = PR_NewLogModule("Gfx");

NS_IMPL_ISUPPORTS1(nsRenderingContextCurses, nsIRenderingContext)

nsRenderingContextCurses::nsRenderingContextCurses()
{
	LOG(("nsRenderingContextCurses::nsRenderingContextCurses()\n"));
	mP2T = 1.0f;
	mFontMetrics = nsnull;
	mClipRegion = nsnull;
	mContext = nsnull;
	mOffscreenSurface = nsnull;
	mSurface = nsnull;
	mTranMatrix = nsnull;

	PushState();
}

nsRenderingContextCurses::~nsRenderingContextCurses()
{
	LOG(("nsRenderingContextCurses::~nsRenderingContextCurses()\n"));
	if (mTranMatrix)
		delete mTranMatrix;
	NS_IF_RELEASE(mOffscreenSurface);
	NS_IF_RELEASE(mFontMetrics);
	NS_IF_RELEASE(mContext);
}

NS_IMETHODIMP
nsRenderingContextCurses::Init(nsIDeviceContext* aContext,
			       nsIWidget *aWindow)
{
	LOG(("nsRenderingContextCurses::Init()\n"));
	mContext = aContext;
	NS_IF_ADDREF(mContext);
	mSurface = new nsDrawingSurfaceCurses();
	mOffscreenSurface = mSurface;
	NS_ADDREF(mSurface);
	return CommonInit();
}

NS_IMETHODIMP
nsRenderingContextCurses::Init(nsIDeviceContext* aContext,
			       nsIDrawingSurface* aSurface)
{
	LOG(("nsRenderingContextCurses::Init()\n"));
	mContext = aContext;
	NS_IF_ADDREF(mContext);
	mSurface = (nsDrawingSurfaceCurses *)aSurface;
	NS_ADDREF(mSurface);
	mOffscreenSurface = mSurface;
	return CommonInit();
}

NS_IMETHODIMP
nsRenderingContextCurses::CommonInit()
{
	LOG(("nsRenderingContextCurses::CommonInit()\n"));
	mP2T = mContext->DevUnitsToAppUnits();
	float app2dev;
	app2dev = mContext->AppUnitsToDevUnits();
	mTranMatrix->AddScale(app2dev, app2dev);
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetHints(PRUint32& aResult)
{
	LOG(("nsRenderingContextCurses::GetHints()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::LockDrawingSurface(PRInt32 aX, PRInt32 aY,
					     PRUint32 aWidth, PRUint32 aHeight,
					     void **aBits, PRInt32 *aStride,
					     PRInt32 *aWidthBytes, PRUint32 aFlags)
{
	LOG(("nsRenderingContextCurses::LockDrawingSurface()\n"));
	return mSurface->Lock(aX, aY, aWidth, aHeight, aBits, aStride, aWidthBytes, aFlags);
}

NS_IMETHODIMP
nsRenderingContextCurses::UnlockDrawingSurface(void)
{
	LOG(("nsRenderingContextCurses::UnlockDrawingSurface()\n"));
	return mSurface->Unlock();
}

NS_IMETHODIMP
nsRenderingContextCurses::SelectOffScreenDrawingSurface(nsIDrawingSurface* aSurface)
{
	LOG(("nsRenderingContextCurses::SelectOffScreenDrawingSurface()\n"));
	if (aSurface == nsnull)
		mSurface = mOffscreenSurface;
	else
		mSurface = (nsDrawingSurfaceCurses *)aSurface;
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetDrawingSurface(nsIDrawingSurface* *aSurface)
{
	LOG(("nsRenderingContextCurses::GetDrawingSurface()\n"));
	*aSurface = mSurface;
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::Reset()
{
	LOG(("nsRenderingContextCurses::Reset()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetDeviceContext(nsIDeviceContext *&aContext)
{
	DBG(("nsRenderingContextCurses::GetDeviceContext()\n"));
	NS_IF_ADDREF(mContext);
	aContext = mContext;
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::PushState(void)
{
	LOG(("nsRenderingContextCurses::PushState()\n"));

	if (mTranMatrix == nsnull)
		mTranMatrix = new nsTransform2D();
	else
		mTranMatrix = new nsTransform2D(mTranMatrix);

	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::PopState()
{
	LOG(("nsRenderingContextCurses::PopState()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::IsVisibleRect(const nsRect& aRect,
					PRBool &aVisible)
{
	LOG(("nsRenderingContextCurses::IsVisibleRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetClipRect(nsRect &aRect, PRBool &aClipValid)
{
	LOG(("nsRenderingContextCurses::GetClipRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::SetClipRect(const nsRect& aRect,
				      nsClipCombine aCombine)
{
	LOG(("nsRenderingContextCurses::SetClipRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::SetClipRegion(const nsIRegion& aRegion,
					nsClipCombine aCombine)
{
	LOG(("nsRenderingContextCurses::SetClipRegion()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::CopyClipRegion(nsIRegion &aRegion)
{
	LOG(("nsRenderingContextCurses::CopyClipRegion()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetClipRegion(nsIRegion **aRegion)
{
	LOG(("nsRenderingContextCurses::GetClipRegion()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::SetColor(nscolor aColor)
{
	LOG(("nsRenderingContextCurses::SetColor()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetColor(nscolor &aColor) const
{
	LOG(("nsRenderingContextCurses::GetColor()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::SetFont(const nsFont& aFont, nsIAtom* aLangGroup)
{
	LOG(("nsRenderingContextCurses::SetFont()\n"));
	nsCOMPtr<nsIFontMetrics> newMetrics;
	nsresult rv = mContext->GetMetricsFor(aFont, aLangGroup, *getter_AddRefs(newMetrics));
	if (NS_SUCCEEDED(rv)) {
		rv = SetFont(newMetrics);
	}
	if (rv != NS_OK)
		LOG(("SetFont() not OK!\n"));
	return rv;
}

NS_IMETHODIMP
nsRenderingContextCurses::SetFont(nsIFontMetrics *aFontMetrics)
{
	LOG(("nsRenderingContextCurses::SetFont()\n"));
	NS_IF_RELEASE(mFontMetrics);
	mFontMetrics = NS_REINTERPRET_CAST(nsIFontMetrics *, aFontMetrics);
	NS_IF_ADDREF(mFontMetrics);

	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::SetLineStyle(nsLineStyle aLineStyle)
{
	LOG(("nsRenderingContextCurses::SetLineStyle()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetLineStyle(nsLineStyle &aLineStyle)
{
	LOG(("nsRenderingContextCurses::GetLineStyle()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetFontMetrics(nsIFontMetrics *&aFontMetrics)
{
	DBG(("nsRenderingContextCurses::GetFontMetrics()\n"));
	NS_IF_ADDREF(mFontMetrics);
	aFontMetrics = mFontMetrics;
	if (!mFontMetrics)
		WRN(("nsRenderingContextCurses::GetFontMetrics() returns nsnull!!!\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::Translate(nscoord aX, nscoord aY)
{
	LOG(("nsRenderingContextCurses::Translate()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::Scale(float aSx, float aSy)
{
	LOG(("nsRenderingContextCurses::Scale()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetCurrentTransform(nsTransform2D *&aTransform)
{
	LOG(("nsRenderingContextCurses::GetCurrentTransform()\n"));
	aTransform = mTranMatrix;
	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::CreateDrawingSurface(const nsRect &aBounds,
					       PRUint32 aSurfFlags,
					       nsIDrawingSurface* &aSurface)
{
	LOG(("nsRenderingContextCurses::CreateDrawingSurface()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DestroyDrawingSurface(nsIDrawingSurface* aDS)
{
	LOG(("nsRenderingContextCurses::DestroyDrawingSurface()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawLine(nscoord aX0, nscoord aY0, nscoord aX1, nscoord aY1)
{
	LOG(("nsRenderingContextCurses::DrawLine()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawStdLine(nscoord aX0, nscoord aY0, nscoord aX1, nscoord aY1)
{
	LOG(("nsRenderingContextCurses::DrawStdLine()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nsRenderingContextCurses::DrawPolyline(const nsPoint aPoints[], PRInt32 aNumPoints)
{
	LOG(("nsRenderingContextCurses::DrawPolyline()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawRect(const nsRect& aRect)
{
	LOG(("nsRenderingContextCurses::DrawRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawRect(nscoord aX, nscoord aY, nscoord aWidth, nscoord aHeight)
{
	LOG(("nsRenderingContextCurses::DrawRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::FillRect(const nsRect& aRect)
{
	LOG(("nsRenderingContextCurses::FillRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::FillRect(nscoord aX, nscoord aY, nscoord aWidth, nscoord aHeight)
{
	LOG(("nsRenderingContextCurses::FillRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::InvertRect(const nsRect& aRect)
{
	LOG(("nsRenderingContextCurses::InvertRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::InvertRect(nscoord aX, nscoord aY, nscoord aWidth, nscoord aHeight)
{
	LOG(("nsRenderingContextCurses::InvertRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawPolygon(const nsPoint aPoints[], PRInt32 aNumPoints)
{
	LOG(("nsRenderingContextCurses::DrawPolygon()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::FillPolygon(const nsPoint aPoints[], PRInt32 aNumPoints)
{
	LOG(("nsRenderingContextCurses::FillPolygon()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawEllipse(const nsRect& aRect)
{
	LOG(("nsRenderingContextCurses::DrawEllipse()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawEllipse(nscoord aX, nscoord aY, nscoord aWidth, nscoord aHeight)
{
	LOG(("nsRenderingContextCurses::DrawEllipse()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::FillEllipse(const nsRect& aRect)
{
	LOG(("nsRenderingContextCurses::FillEllipse()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::FillEllipse(nscoord aX, nscoord aY, nscoord aWidth, nscoord aHeight)
{
	LOG(("nsRenderingContextCurses::FillEllipse()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawArc(const nsRect& aRect,
				  float aStartAngle, float aEndAngle)
{
	LOG(("nsRenderingContextCurses::DrawArc()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawArc(nscoord aX, nscoord aY,
				  nscoord aWidth, nscoord aHeight,
				  float aStartAngle, float aEndAngle)
{
	LOG(("nsRenderingContextCurses::DrawArc()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::FillArc(const nsRect& aRect,
				  float aStartAngle, float aEndAngle)
{
	LOG(("nsRenderingContextCurses::FillArc()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nsRenderingContextCurses::FillArc(nscoord aX, nscoord aY,
				  nscoord aWidth, nscoord aHeight,
				  float aStartAngle, float aEndAngle)
{
	LOG(("nsRenderingContextCurses::FillArc()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetWidth(char aC, nscoord &aWidth)
{
	return GetWidth(&aC, 1, aWidth);
}

NS_IMETHODIMP
nsRenderingContextCurses::GetWidth(PRUnichar aC, nscoord& aWidth,
				   PRInt32* aFontID)
{
	return GetWidth(&aC, 1, aWidth, aFontID);
}

NS_IMETHODIMP
nsRenderingContextCurses::GetWidth(const nsString& aString,
				   nscoord& aWidth, PRInt32* aFontID)
{
	return GetWidth(aString.get(), aString.Length(), aWidth, aFontID);
}

NS_IMETHODIMP
nsRenderingContextCurses::GetWidth(const char* aString, nscoord& aWidth)
{
	return GetWidth(aString, strlen(aString), aWidth);
}

NS_IMETHODIMP
nsRenderingContextCurses::GetWidth(const char* aString, PRUint32 aLength,
				   nscoord& aWidth)
{
	LOG(("nsRenderingContextCurses::GetWidth()\n"));
	if (aLength == 0) {
		aWidth = 0;
		return NS_OK;
	}

	if (aString == NULL)
		return NS_ERROR_FAILURE;

	aWidth = 6 * aLength;

	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetWidth(const PRUnichar* aString, PRUint32 aLength,
				   nscoord& aWidth, PRInt32* aFontID)
{
	LOG(("nsRenderingContextCurses::GetWidth()\n"));
	if (aLength == 0) {
		aWidth = 0;
		return NS_OK;
	}

	if (aString == NULL)
		return NS_ERROR_FAILURE;

	if (aFontID != nsnull)
		*aFontID = 0;

	aWidth = 6 * aLength;

	return NS_OK;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetTextDimensions(const char* aString, PRUint32 aLength,
					    nsTextDimensions& aDimensions)
{
	DBG(("nsRenderingContextCurses::GetTextDimensions()\n"));
	mFontMetrics->GetMaxAscent(aDimensions.ascent);
	mFontMetrics->GetMaxDescent(aDimensions.descent);
	return GetWidth(aString, aLength, aDimensions.width);
}

NS_IMETHODIMP
nsRenderingContextCurses::GetTextDimensions(const PRUnichar* aString,
					    PRUint32 aLength,
					    nsTextDimensions& aDimensions,
					    PRInt32* aFontID)
{
	DBG(("nsRenderingContextCurses::GetTextDimensions()\n"));
	mFontMetrics->GetMaxAscent(aDimensions.ascent);
	mFontMetrics->GetMaxDescent(aDimensions.descent);

	NS_ConvertUCS2toUTF8    theUnicodeString (aString, aLength);
	const char *s = theUnicodeString.get();
	return GetWidth( s, strlen(s), aDimensions.width );
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawString(const char *aString, PRUint32 aLength,
				     nscoord aX, nscoord aY,
				     const nscoord* aSpacing)
{
	LOG(("nsRenderingContextCurses::DrawString(%d, %d, %s)\n", aX, aY, aString));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawString(const PRUnichar* aString, PRUint32 aLength,
				     nscoord aX, nscoord aY,
				     PRInt32 aFontID,
				     const nscoord* aSpacing)
{
	LOG(("nsRenderingContextCurses::DrawString(%d, %d, %s)\n", aX, aY, aString));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::DrawString(const nsString& aString,
				     nscoord aX, nscoord aY,
				     PRInt32 aFontID,
				     const nscoord* aSpacing)
{
	LOG(("nsRenderingContextCurses::DrawString(%d, %d, %s)\n", aX, aY, aString));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::CopyOffScreenBits(nsIDrawingSurface* aSrcSurf,
					    PRInt32 aSrcX, PRInt32 aSrcY,
					    const nsRect &aDestBounds,
					    PRUint32 aCopyFlags)
{
	LOG(("nsRenderingContextCurses::CopyOffScreenBits()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::RetrieveCurrentNativeGraphicData(PRUint32 * ngd)
{
	LOG(("nsRenderingContextCurses::RetrieveCurrentNativeGraphicData()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

#ifdef MOZ_MATHML

NS_IMETHODIMP
nsRenderingContextCurses::GetBoundingMetrics(const char*        aString,
					     PRUint32           aLength,
					     nsBoundingMetrics& aBoundingMetrics)
{
	LOG(("nsRenderingContextCurses::GetBoundingMetrics()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetBoundingMetrics(const PRUnichar*   aString,
					     PRUint32           aLength,
					     nsBoundingMetrics& aBoundingMetrics,
					     PRInt32*           aFontID)
{
	LOG(("nsRenderingContextCurses::GetBoundingMetrics()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

#endif /* MOZ_MATHML */

/*
NS_IMETHODIMP
nsRenderingContextCurses::DrawImage(imgIContainer *aImage,
				    const nsRect * aSrcRect, const nsPoint * aDestPoint)
{
	LOG(("nsRenderingContextCurses::DrawImage()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}
*/

NS_IMETHODIMP
nsRenderingContextCurses::DrawScaledImage(imgIContainer *aImage,
					  const nsRect * aSrcRect, const nsRect * aDestRect)
{
	LOG(("nsRenderingContextCurses::DrawScaledImage()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::GetBackbuffer(const nsRect &aRequestedSize, const nsRect &aMaxSize,
					nsIDrawingSurface* &aBackbuffer)
{
	LOG(("nsRenderingContextCurses::GetBackbuffer()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsRenderingContextCurses::ReleaseBackbuffer(void)
{
	LOG(("nsRenderingContextCurses::ReleaseBackbuffer()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}
