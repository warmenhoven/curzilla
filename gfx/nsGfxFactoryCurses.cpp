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

#include "nsIGenericFactory.h"
#include "nsGfxCIID.h"

#include "nsBlender.h"
#include "nsFontMetricsCurses.h"
#include "nsRenderingContextCurses.h"
#include "nsScreenManagerCurses.h"
#include "nsScriptableRegion.h"
#include "nsDeviceContextCurses.h"
#include "nsFontList.h"
#include "gfxImageFrame.h"

#include "nsRegionCurses.h"
#include "nsImageCurses.h"

#include "nsCursesGfxLog.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(nsFontMetricsCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsDeviceContextCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsRenderingContextCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsBlender)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsImageCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsRegionCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsFontEnumeratorCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsFontList)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsScreenManagerCurses)
NS_GENERIC_FACTORY_CONSTRUCTOR(gfxImageFrame)

static nsresult nsScriptableRegionConstructor(nsISupports *aOuter, REFNSIID aIID, void **aResult)
{
	LOG(("nsScriptableRegionConstructor()\n"));
	nsresult rv;

	nsIScriptableRegion *inst = nsnull;

	if (!aResult) {
		rv = NS_ERROR_NULL_POINTER;
		return rv;
	}
	*aResult = nsnull;
	if (aOuter) {
		rv = NS_ERROR_NO_AGGREGATION;
		return rv;
	}
	// create an nsRegionCurses and get the scriptable region from it
	nsCOMPtr <nsIRegion> rgn;
	NS_NEWXPCOM(rgn, nsRegionCurses);
	nsCOMPtr<nsIScriptableRegion> scriptableRgn;
	if (rgn != nsnull) {
		scriptableRgn = new nsScriptableRegion(rgn);
		inst = scriptableRgn;
	}
	if (!inst) {
		rv = NS_ERROR_OUT_OF_MEMORY;
		return rv;
	}
	NS_ADDREF(inst);
	// release our variable above now that we have created our owning
	// reference - we don't want this to go out of scope early!
	scriptableRgn = nsnull;
	rv = inst->QueryInterface(aIID, aResult);
	NS_RELEASE(inst);

	return rv;
}

static const nsModuleComponentInfo components[] =
{
	{
		"Curses Font Metrics",
		NS_FONT_METRICS_CID,
		"@mozilla.org/gfx/fontmetrics;1",
		nsFontMetricsCursesConstructor
	},

	{
		"Curses Device Context",
		NS_DEVICE_CONTEXT_CID,
		"@mozilla.org/gfx/devicecontext;1",
		nsDeviceContextCursesConstructor
	},

	{
		"Curses Rendering Context",
		NS_RENDERING_CONTEXT_CID,
		"@mozilla.org/gfx/renderingcontext;1",
		nsRenderingContextCursesConstructor
	},

	{
		"Curses Image",
		NS_IMAGE_CID,
		"@mozilla.org/gfx/image;1",
		nsImageCursesConstructor
	},

	{
		"Curses Region",
		NS_REGION_CID,
		"@mozilla.org/gfx/region/curses;1",
		nsRegionCursesConstructor
	},

	{
		"Scriptable Region",
		NS_SCRIPTABLE_REGION_CID,
		"@mozilla.org/gfx/region;1",
		nsScriptableRegionConstructor
	},

	{
		"Blender",
		NS_BLENDER_CID,
		"@mozilla.org/gfx/blender;1",
		nsBlenderConstructor
	},

	{
		"Curses Font Enumerator",
		NS_FONT_ENUMERATOR_CID,
		"@mozilla.org/gfx/fontenumerator;1",
		nsFontEnumeratorCursesConstructor
	},

	{
		"Font List",
		NS_FONTLIST_CID,
		NS_FONTLIST_CONTRACTID,
		nsFontListConstructor
	},

	{
		"Curses Screen Manager",
		NS_SCREENMANAGER_CID,
		"@mozilla.org/gfx/screenmanager;1",
		nsScreenManagerCursesConstructor
	},

	{
		"windows image frame",
		GFX_IMAGEFRAME_CID,
		"@mozilla.org/gfx/image/frame;2",
		gfxImageFrameConstructor
	},
};

NS_IMPL_NSGETMODULE(nsGfxCursesModule, components)
