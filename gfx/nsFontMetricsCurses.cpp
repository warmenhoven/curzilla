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

#include "nsQuickSort.h"
#include "nsIServiceManager.h"
#include "nsFontMetricsCurses.h"
#include "nsHashtable.h"
#include "nsIPref.h"
#include "nsReadableUtils.h"
#include "nsCursesGfxLog.h"

nsFontMetricsCurses::nsFontMetricsCurses()
{
	LOG(("nsFontMetricsCurses::nsFontMetricsCurses()\n"));
	NS_INIT_ISUPPORTS();
	mDeviceContext = nsnull;
	mFont = nsnull;

	mHeight = 10;
	mAscent = 7;
	mDescent = 2;
	mLeading = 1;
	mEmHeight = 10;
	mEmAscent = 7;
	mEmDescent = 2;
	mMaxHeight = 10;
	mMaxAscent = 7;
	mMaxDescent = 2;
	mMaxAdvance = 0;
	mXHeight = 10;
	mSuperscriptOffset = 0;
	mSubscriptOffset = 0;
	mStrikeoutSize = 0;
	mStrikeoutOffset = 0;
	mUnderlineSize = 10;
	mUnderlineOffset = 0;
	mSpaceWidth = 5;
	mAveCharWidth = 5;
}

nsFontMetricsCurses::~nsFontMetricsCurses()
{
	LOG(("nsFontMetricsCurses::~nsFontMetricsCurses()\n"));
	if (nsnull != mFont) {
		delete mFont;
		mFont = nsnull;
	}
	if (mFontHandle)
		free (mFontHandle);
	if (mDeviceContext) {
		// Notify our device context that owns us so that it can update its font cache
		mDeviceContext->FontMetricsDeleted(this);
		mDeviceContext = nsnull;
	}
}

NS_IMPL_ISUPPORTS1(nsFontMetricsCurses, nsIFontMetrics)

static PRBool
FontEnumCallback(const nsString& aFamily, PRBool aGeneric, void *aData)
{
	LOG(("nsFontMetricsCurses::FontEnumCallback()\n"));
	INF(("font = '%s'\n", NS_LossyConvertUCS2toASCII(aFamily).get()));

	return PR_FALSE;
}

NS_IMETHODIMP
nsFontMetricsCurses::Init(const nsFont& aFont, nsIAtom* aLangGroup, nsIDeviceContext* aContext)
{
	LOG(("nsFontMetricsCurses::Init()\n"));
	mFont = new nsFont(aFont);
	mLangGroup = aLangGroup;
	mDeviceContext = aContext;

	mFont->EnumerateFamilies(FontEnumCallback, this);
	return NS_OK;
}

void
nsFontMetricsCurses::RealizeFont()
{
	LOG(("nsFontMetricsCurses::RealizeFont()\n"));
}

NS_IMETHODIMP
nsFontMetricsCurses::GetLangGroup(nsIAtom** aLangGroup)
{
	LOG(("nsFontMetricsCurses::GetLangGroup()\n"));
	if (!aLangGroup)
		return NS_ERROR_NULL_POINTER;

	*aLangGroup = mLangGroup;
	NS_IF_ADDREF(*aLangGroup);

	return NS_OK;
}

// The Font Enumerator
nsFontEnumeratorCurses::nsFontEnumeratorCurses()
{
	LOG(("nsFontEnumeratorCurses::FontEnumeratorCurses()\n"));
	NS_INIT_ISUPPORTS();
}

NS_IMPL_ISUPPORTS1(nsFontEnumeratorCurses, nsIFontEnumerator)

NS_IMETHODIMP
nsFontEnumeratorCurses::EnumerateAllFonts(PRUint32* aCount, PRUnichar*** aResult)
{
	LOG(("nsFontEnumeratorCurses::EnumerateAllFonts()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsFontEnumeratorCurses::EnumerateFonts(const char* aLangGroup, const char* aGeneric,
				     PRUint32* aCount, PRUnichar*** aResult)
{
	LOG(("nsFontEnumeratorCurses::EnumerateFonts()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsFontEnumeratorCurses::HaveFontFor(const char* aLangGroup, PRBool* aResult)
{
	LOG(("nsFontEnumeratorCurses::HaveFontFor()\n"));
	NS_ENSURE_ARG_POINTER(aResult);
	*aResult = PR_TRUE; // always return true for now.
	return NS_OK;
}

NS_IMETHODIMP
nsFontEnumeratorCurses::GetDefaultFont(const char *aLangGroup, const char *aGeneric,
				     PRUnichar **aResult)
{
	LOG(("nsFontEnumeratorCurses::GetDefaultFont()\n"));
	NS_ENSURE_ARG_POINTER(aResult);
	*aResult = nsnull;
	return NS_OK;
}

NS_IMETHODIMP
nsFontEnumeratorCurses::UpdateFontList(PRBool *updateFontList)
{
	LOG(("nsFontEnumeratorCurses::UpdateFontList()\n"));
	*updateFontList = PR_FALSE; // always return false for now
	return NS_OK;
}
