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

#include "nsIServiceManager.h"
#include "nsDeviceContextCurses.h"
#include "nsCursesGfxLog.h"

nsDeviceContextCurses::nsDeviceContextCurses() : DeviceContextImpl()
{
	DBG(("nsDeviceContextCurses::nsDeviceContextCurses()\n"));
	mTwipsToPixels = 1.0;
	mPixelsToTwips = 1.0;
	mDepth = 0 ;

	mWidthFloat = 0.0f;
	mHeightFloat = 0.0f;
	mWidth = -1;
	mHeight = -1;
}

nsDeviceContextCurses::~nsDeviceContextCurses()
{
	DBG(("nsDeviceContextCurses::~nsDeviceContextCurses()\n"));
}

NS_IMETHODIMP
nsDeviceContextCurses::Init(nsNativeWidget aNativeWidget)
{
	DBG(("nsDeviceContextCurses::Init()\n"));
	if (!mScreenManager)
		mScreenManager = do_GetService("@mozilla.org/gfx/screenmanager;1");
	if (!mScreenManager) {
		return NS_ERROR_FAILURE;
	}


	nsCOMPtr<nsIScreen> screen = nsnull;
	mScreenManager->GetPrimaryScreen(getter_AddRefs(screen));
	if (screen) {
		PRInt32 x, y, width, height, depth;
		screen->GetAvailRect(&x, &y, &width, &height);
		screen->GetPixelDepth(&depth);
		mWidthFloat = float(width);
		mHeightFloat = float(height);
		mDepth = NS_STATIC_CAST(PRUint32, depth);
	}

	mNativeWidget = aNativeWidget;

	DeviceContextImpl::CommonInit();

	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::CreateRenderingContext(nsIRenderingContext *&aContext)
{
	DBG(("nsDeviceContextCurses::CreateRenderingContext()\n"));
#ifdef NS_PRINT_PREVIEW
	// Defer to Alt when there is one
	if (mAltDC && ((mUseAltDC & kUseAltDCFor_CREATERC_PAINT) || (mUseAltDC & kUseAltDCFor_CREATERC_REFLOW))) {
		return mAltDC->CreateRenderingContext(aContext);
	}
#endif

	nsIRenderingContext *pContext;
	nsresult             rv;
	nsDrawingSurfaceCurses *surf;

	// to call init for this, we need to have a valid nsDrawingSurfaceCurses created
	pContext = new nsRenderingContextCurses();

	if (nsnull != pContext) {
		NS_ADDREF(pContext);

		// create the nsDrawingSurfaceCurses
		surf = new nsDrawingSurfaceCurses();

		if (surf)
			rv = pContext->Init(this, surf);
		else
			rv = NS_ERROR_OUT_OF_MEMORY;
	} else
		rv = NS_ERROR_OUT_OF_MEMORY;

	if (NS_OK != rv) {
		NS_IF_RELEASE(pContext);
	}

	aContext = pContext;

	return rv;
}

NS_IMETHODIMP
nsDeviceContextCurses::CreateRenderingContextInstance(nsIRenderingContext *&aContext)
{
	DBG(("nsDeviceContextCurses::CreateRenderingContextInstance()\n"));
	nsCOMPtr<nsIRenderingContext> renderingContext = new nsRenderingContextCurses();
	if (!renderingContext)
		return NS_ERROR_OUT_OF_MEMORY;

	aContext = renderingContext;
	NS_ADDREF(aContext);

	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::SupportsNativeWidgets(PRBool &aSupportsWidgets)
{
	DBG(("nsDeviceContextCurses::SupportsNativeWidgets()\n"));
	//XXX it is very critical that this not lie!! MMP
	// read the comments in the mac code for this
	//
	// We return true here because any time any thing changes,
	// update_panels() and doupdate() get called, redrawing it all. This
	// effectively means that widgets paint themselves (even though that's
	// not technically true).
	//
	// XXX
	aSupportsWidgets = PR_TRUE;

	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::GetScrollBarDimensions(float &aWidth, float &aHeight) const
{
	LOG(("nsDeviceContextCurses::GetScrollBarDimensions()\n"));
	float scale;
	GetCanonicalPixelScale(scale);
	aWidth = 6 * mPixelsToTwips * scale;
	aHeight = 6 * mPixelsToTwips * scale;

	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::GetSystemFont(nsSystemFontID aID, nsFont *aFont) const
{
	LOG(("nsDeviceContextCurses::GetSystemFont()\n"));
	aFont->style       = NS_FONT_STYLE_NORMAL;
	aFont->weight      = NS_FONT_WEIGHT_NORMAL;
	aFont->decorations = NS_FONT_DECORATION_NONE;
	aFont->size        = 8;
	aFont->name.Assign(NS_LITERAL_STRING("TextFont"));
	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::ConvertPixel(nscolor aColor, PRUint32 & aPixel)
{
	LOG(("nsDeviceContextCurses::ConvertPixel()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nsDeviceContextCurses::CheckFontExistence(const nsString& aFontName)
{
	LOG(("nsDeviceContextCurses::CheckFontExistence()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDeviceContextCurses::GetDeviceSurfaceDimensions(PRInt32 &aWidth, PRInt32 &aHeight)
{
	LOG(("nsDeviceContextCurses::GetDeviceSurfaceDimensions()\n"));
#ifdef NS_PRINT_PREVIEW
	// Defer to Alt when there is one
	if (mAltDC && (mUseAltDC & kUseAltDCFor_SURFACE_DIM)) {
		return mAltDC->GetDeviceSurfaceDimensions(aWidth, aHeight);
	}
#endif

	if (mWidth == -1)
		mWidth = NSToIntRound(mWidthFloat * mDevUnitsToAppUnits);

	if (mHeight == -1)
		mHeight = NSToIntRound(mHeightFloat * mDevUnitsToAppUnits);

	aWidth = mWidth;
	aHeight = mHeight;

	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::GetRect(nsRect &aRect)
{
	LOG(("nsDeviceContextCurses::GetRect()\n"));
	// if we have an initialized widget for this device context, use it
	// to try and get real screen coordinates.
	if (mNativeWidget) {
		int x = 0, y = 0, width = 0, height = 0;
		nsCOMPtr<nsIScreen> screen;
		mScreenManager->ScreenForRect(x, y, width, height, getter_AddRefs(screen));
		if (screen) {
			screen->GetRect(&aRect.x, &aRect.y, &aRect.width, &aRect.height);
			aRect.x = NSToIntRound(mDevUnitsToAppUnits * aRect.x);
			aRect.y = NSToIntRound(mDevUnitsToAppUnits * aRect.y);
			aRect.width = NSToIntRound(mDevUnitsToAppUnits * aRect.width);
			aRect.height = NSToIntRound(mDevUnitsToAppUnits * aRect.height);
		}
	} else {
		PRInt32 width, height;
		GetDeviceSurfaceDimensions(width, height);
		aRect.x = 0;
		aRect.y = 0;
		aRect.width = width;
		aRect.height = height;
	}
	return NS_OK;
}


NS_IMETHODIMP
nsDeviceContextCurses::GetClientRect(nsRect &aRect)
{
	LOG(("nsDeviceContextCurses::GetClientRect()\n"));
	return GetRect(aRect);
}

NS_IMETHODIMP
nsDeviceContextCurses::GetDeviceContextFor(nsIDeviceContextSpec *aDevice,
					   nsIDeviceContext *&aContext)
{
	LOG(("nsDeviceContextCurses::GetDeviceContextFor()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDeviceContextCurses::BeginDocument(PRUnichar * aTitle, PRUnichar* aPrintToFileName,
				     PRInt32 aStartPage, PRInt32 aEndPage)
{
	LOG(("nsDeviceContextCurses::BeginDocument()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::EndDocument(void)
{
	LOG(("nsDeviceContextCurses::EndDocument()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::AbortDocument(void)
{
	LOG(("nsDeviceContextCurses::AbortDocument()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::BeginPage(void)
{
	LOG(("nsDeviceContextCurses::BeginPage()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::EndPage(void)
{
	LOG(("nsDeviceContextCurses::EndPage()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsDeviceContextCurses::GetDepth(PRUint32& aDepth)
{
	LOG(("nsDeviceContextCurses::GetDepth()\n"));
	aDepth = mDepth;
	return NS_OK;
}
