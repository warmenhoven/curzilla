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

#ifndef nsFontMetricsCurses_h__
#define nsFontMetricsCurses_h__

#include "nsIFontMetrics.h"
#include "nsIFontEnumerator.h"
#include "nsFont.h"
#include "nsString.h"
#include "nsUnitConversion.h"
#include "nsIDeviceContext.h"
#include "nsDeviceContext.h"
#include "nsCRT.h"
#include "nsCOMPtr.h"
#include "nsCursesGfxLog.h"

class nsFontMetricsCurses : public nsIFontMetrics
{
	public:
		nsFontMetricsCurses();
		virtual ~nsFontMetricsCurses();

		NS_DECL_AND_IMPL_ZEROING_OPERATOR_NEW

		NS_DECL_ISUPPORTS

		NS_IMETHOD  Init(const nsFont& aFont, nsIAtom* aLangGroup,
				 nsIDeviceContext* aContext);

		NS_IMETHOD  GetLangGroup(nsIAtom** aLangGroup);

		NS_IMETHODIMP  Destroy()
		{
			LOG(("nsFontMetricsCurses::Destroy()\n"));
			mDeviceContext = nsnull;
			return NS_OK;
		}

		inline NS_IMETHODIMP  GetXHeight(nscoord& aResult)
		{
			LOG(("nsFontMetricsCurses::GetXHeight()\n"));
			aResult = mXHeight;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetSuperscriptOffset(nscoord& aResult)
		{
			LOG(("nsFontMetricsCurses::GetSuperscriptOffset()\n"));
			aResult = mSuperscriptOffset;
			return NS_OK;
		}
		inline NS_IMETHOD  GetSubscriptOffset(nscoord& aResult)
		{
			LOG(("nsFontMetricsCurses::GetSubscriptOffset()\n"));
			aResult = mSubscriptOffset;
			return NS_OK;
		}
		inline NS_IMETHOD  GetStrikeout(nscoord& aOffset, nscoord& aSize)
		{
			LOG(("nsFontMetricsCurses::GetStrikeout()\n"));
			aOffset = mStrikeoutOffset;
			aSize = mStrikeoutSize;
			return NS_OK;
		}
		inline NS_IMETHOD  GetUnderline(nscoord& aOffset, nscoord& aSize)
		{
			LOG(("nsFontMetricsCurses::GetUnderline()\n"));
			aOffset = mUnderlineOffset;
			aSize = mUnderlineSize;
			return NS_OK;
		}

		inline NS_IMETHODIMP  GetHeight(nscoord &aHeight)
		{
			LOG(("nsFontMetricsCurses::GetHeight()\n"));
			aHeight = mHeight;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetNormalLineHeight(nscoord &aHeight)
		{
			LOG(("nsFontMetricsCurses::GetNormalLineHeight()\n"));
			aHeight = mEmHeight + mLeading;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetLeading(nscoord &aLeading)
		{
			LOG(("nsFontMetricsCurses::GetLeading()\n"));
			aLeading = mLeading;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetEmHeight(nscoord &aHeight)
		{
			LOG(("nsFontMetricsCurses::GetEmHeight()\n"));
			aHeight = mEmHeight;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetEmAscent(nscoord &aAscent)
		{
			LOG(("nsFontMetricsCurses::GetEmAscent()\n"));
			aAscent = mEmAscent;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetEmDescent(nscoord &aDescent)
		{
			LOG(("nsFontMetricsCurses::GetEmDescent()\n"));
			aDescent = mEmDescent;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetMaxHeight(nscoord &aHeight)
		{
			LOG(("nsFontMetricsCurses::GetMaxHeight()\n"));
			aHeight = mMaxHeight;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetMaxAscent(nscoord &aAscent)
		{
			LOG(("nsFontMetricsCurses::GetMaxAscent()\n"));
			aAscent = mMaxAscent;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetMaxDescent(nscoord &aDescent)
		{
			LOG(("nsFontMetricsCurses::GetMaxDescent()\n"));
			aDescent = mMaxDescent;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetMaxAdvance(nscoord &aAdvance)
		{
			LOG(("nsFontMetricsCurses::GetMaxAdvance()\n"));
			aAdvance = mMaxAdvance;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetAveCharWidth(nscoord &aAveCharWidth)
		{
			DBG(("nsFontMetricsCurses::GetAveCharWidth()\n"));
			aAveCharWidth = mAveCharWidth;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetFont(const nsFont *&aFont)
		{
			LOG(("nsFontMetricsCurses::GetFont()\n"));
			aFont = mFont;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetFontHandle(nsFontHandle &aHandle)
		{
			LOG(("nsFontMetricsCurses::GetFontHandle()\n"));
			aHandle = (nsFontHandle) mFontHandle;
			return NS_OK;
		}
		inline NS_IMETHODIMP  GetSpaceWidth(nscoord &aSpaceWidth)
		{
			LOG(("nsFontMetricsCurses::GetSpaceWidth()\n"));
			aSpaceWidth = mSpaceWidth;
			return NS_OK;
		}

	protected:
		void RealizeFont();

		nsIDeviceContext	*mDeviceContext;
		nsFont			*mFont;
		char			*mFontHandle;
		nscoord			mHeight;
		nscoord			mAscent;
		nscoord			mDescent;
		nscoord			mLeading;
		nscoord			mEmHeight;
		nscoord			mEmAscent;
		nscoord			mEmDescent;
		nscoord			mMaxHeight;
		nscoord			mMaxAscent;
		nscoord			mMaxDescent;
		nscoord			mMaxAdvance;
		nscoord			mXHeight;
		nscoord			mSuperscriptOffset;
		nscoord			mSubscriptOffset;
		nscoord			mStrikeoutSize;
		nscoord			mStrikeoutOffset;
		nscoord			mUnderlineSize;
		nscoord			mUnderlineOffset;
		nscoord			mSpaceWidth;
		nscoord			mAveCharWidth;

		nsCOMPtr<nsIAtom>	mLangGroup;
};

class nsFontEnumeratorCurses : public nsIFontEnumerator
{
	public:
		nsFontEnumeratorCurses();
		NS_DECL_ISUPPORTS
		NS_DECL_NSIFONTENUMERATOR
};

#endif
