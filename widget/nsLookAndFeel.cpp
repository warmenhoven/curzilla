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

#include "nsAppShell.h"
#include "nsLookAndFeel.h"

#include <curses.h>

static nscolor
CURSES_COLOR_TO_NS_RGB(short c)
{
	short r, g, b;
	color_content(c, &r, &g, &b);
	return ((nscolor) NS_RGB(r >> 8, g >> 8, b >> 8));
}

nsLookAndFeel::nsLookAndFeel() : nsXPLookAndFeel()
{
	LOG(("nsLookAndFeel::nsLookAndFeel()\n"));
}

nsLookAndFeel::~nsLookAndFeel()
{
	LOG(("nsLookAndFeel::~nsLookAndFeel()\n"));
}

nsresult nsLookAndFeel::NativeGetColor(const nsColorID aID, nscolor& aColor)
{
	LOG(("nsLookAndFeel::NativeGetColor()\n"));
	switch (aID) {
	case eColor_WindowBackground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_WindowForeground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_WidgetBackground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_WidgetForeground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_WidgetSelectBackground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_WidgetSelectForeground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_Widget3DHighlight:
		aColor = NS_RGB(0xa0,0xa0,0xa0);
		break;
	case eColor_Widget3DShadow:
		aColor = NS_RGB(0x40,0x40,0x40);
		break;
	case eColor_TextBackground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_TextForeground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_TextSelectBackground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_TextSelectForeground:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

		// css2  http://www.w3.org/TR/REC-CSS2/ui.html#system-colors
	case eColor_activeborder:
		// active window border
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_activecaption:
		// active window caption background
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_appworkspace:
		// MDI background color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_background:
		// desktop background
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_captiontext:
		// text in active window caption, size box, and scrollbar arrow box (!)
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_graytext:
		// disabled text
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_highlight:
		// background of selected item
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_highlighttext:
		// text of selected item
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_inactiveborder:
		// inactive window border
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_inactivecaption:
		// inactive window caption
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_inactivecaptiontext:
		// text in inactive window caption
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_infobackground:
		// tooltip background color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_infotext:
		// tooltip text color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_menu:
		// menu background
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor_menutext:
		// menu text
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor_scrollbar:
		// scrollbar gray area
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_threedface:
	case eColor_buttonface:
		// 3-D face color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_buttontext:
		// text on push buttons
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;

	case eColor_buttonhighlight:
		// 3-D highlighted edge color
	case eColor_threedhighlight:
		// 3-D highlighted outer edge color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_threedlightshadow:
		// 3-D highlighted inner edge color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_buttonshadow:
		// 3-D shadow edge color
	case eColor_threedshadow:
		// 3-D shadow inner edge color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_threeddarkshadow:
		// 3-D shadow outer edge color
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_window:
	case eColor_windowframe:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;

	case eColor_windowtext:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;

		// from the CSS3 working draft (not yet finalized)
		// http://www.w3.org/tr/2000/wd-css3-userint-20000216.html#color

	case eColor__moz_field:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor__moz_fieldtext:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor__moz_dialog:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_BLACK);
		break;
	case eColor__moz_dialogtext:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	case eColor__moz_dragtargetzone:
		aColor = CURSES_COLOR_TO_NS_RGB(COLOR_WHITE);
		break;
	default:
		/* default color is BLACK */
		aColor = 0;
		return NS_ERROR_FAILURE;
	}

	return NS_OK;
}

NS_IMETHODIMP nsLookAndFeel::GetMetric(const nsMetricID aID, PRInt32 & aMetric)
{
	LOG(("nsLookAndFeel::GetMetric()\n"));
	nsresult res = nsXPLookAndFeel::GetMetric(aID, aMetric);
	if (NS_SUCCEEDED(res))
		return res;
	switch (aID) {
	case eMetric_WindowTitleHeight:
		aMetric = 0;
		break;
	case eMetric_WindowBorderWidth:
		// XXXldb Why is this commented out?
		//    aMetric = mStyle->klass->xthickness;
		break;
	case eMetric_WindowBorderHeight:
		// XXXldb Why is this commented out?
		//    aMetric = mStyle->klass->ythickness;
		break;
	case eMetric_Widget3DBorder:
		// XXXldb Why is this commented out?
		//    aMetric = 4;
		break;
	case eMetric_TextFieldHeight:
		aMetric = 6;
		break;
	case eMetric_TextFieldBorder:
		aMetric = 0;
		break;
	case eMetric_TextVerticalInsidePadding:
		aMetric = 0;
		break;
	case eMetric_TextShouldUseVerticalInsidePadding:
		aMetric = 0;
		break;
	case eMetric_TextHorizontalInsideMinimumPadding:
		aMetric = 18;
		break;
	case eMetric_TextShouldUseHorizontalInsideMinimumPadding:
		aMetric = 0;
		break;
	case eMetric_ButtonHorizontalInsidePaddingNavQuirks:
		aMetric = 12;
		break;
	case eMetric_ButtonHorizontalInsidePaddingOffsetNavQuirks:
		aMetric = 6;
		break;
	case eMetric_CheckboxSize:
		aMetric = 18;
		break;
	case eMetric_RadioboxSize:
		aMetric = 18;
		break;
	case eMetric_ListShouldUseHorizontalInsideMinimumPadding:
		aMetric = 18;
		break;
	case eMetric_ListHorizontalInsideMinimumPadding:
		aMetric = 18;
		break;
	case eMetric_ListShouldUseVerticalInsidePadding:
		aMetric = 0;
		break;
	case eMetric_ListVerticalInsidePadding:
		aMetric = 0;
		break;
	case eMetric_CaretBlinkTime:
		aMetric = 500;
		break;
	case eMetric_ShowCaretDuringSelection:
		aMetric = 0;
		break;
	case eMetric_SelectTextfieldsOnKeyFocus:
		// Select textfield content when focused by kbd
		// used by nsEventStateManager::sTextfieldSelectModel
		aMetric = 1;
		break;
	case eMetric_SubmenuDelay:
		aMetric = 200;
		break;
	case eMetric_MenusCanOverlapOSBar:
		// we want XUL popups to be able to overlap the task bar.
		aMetric = 1;
		break;
	case eMetric_DragFullWindow:
		aMetric = 1;
		break;
	case eMetric_DragThresholdX:
	case eMetric_DragThresholdY:
		aMetric = 0;
		break;
	case eMetric_ScrollArrowStyle:
		aMetric = eMetric_ScrollArrowStyleSingle;
		break;
	case eMetric_ScrollSliderStyle:
		aMetric = eMetric_ScrollThumbStyleProportional;
		break;
	case eMetric_TreeOpenDelay:
		aMetric = 1000;
		break;
	case eMetric_TreeCloseDelay:
		aMetric = 1000;
		break;
	case eMetric_TreeLazyScrollDelay:
		aMetric = 150;
		break;
	case eMetric_TreeScrollDelay:
		aMetric = 100;
		break;
	case eMetric_TreeScrollLinesMax:
		aMetric = 3;
		break;
	default:
		aMetric = 0;
		return NS_ERROR_FAILURE;
	}

	return NS_OK;
}

NS_IMETHODIMP nsLookAndFeel::GetMetric(const nsMetricFloatID aID,
                                       float & aMetric)
{
	LOG(("nsLookAndFeel::GetMetric(float)\n"));
	nsresult res = nsXPLookAndFeel::GetMetric(aID, aMetric);
	if (NS_SUCCEEDED(res))
		return res;
	return NS_ERROR_NOT_IMPLEMENTED;
}
