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
#include "nsWidgetsCID.h"
#include "nsAppShell.h"
#include "nsBaseWidget.h"
#include "nsLookAndFeel.h"
#include "nsWindow.h"
#include "nsTransferable.h"
#include "nsClipboardHelper.h"
#include "nsHTMLFormatConverter.h"
#include "nsClipboard.h"
#include "nsSound.h"
#ifdef IBMBIDI
#include "nsBidiKeyboard.h"
#endif

NS_GENERIC_FACTORY_CONSTRUCTOR(nsWindow)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsChildWindow)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsAppShell)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsLookAndFeel)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsTransferable)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsClipboardHelper)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsHTMLFormatConverter)
NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsClipboard, Init)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSound)
#ifdef IBMBIDI
NS_GENERIC_FACTORY_CONSTRUCTOR(nsBidiKeyboard)
#endif

static const nsModuleComponentInfo components[] =
{
	{
		"Curses Window",
		NS_WINDOW_CID,
		"@mozilla.org/widget/window/curses;1",
		nsWindowConstructor
	},
	{
		"Curses Child Window",
		NS_CHILD_CID,
		"@mozilla.org/widgets/child_window/curses;1",
		nsChildWindowConstructor
	},
	{
		"Curses AppShell",
		NS_APPSHELL_CID,
		"@mozilla.org/widget/appshell/curses;1",
		nsAppShellConstructor
	},
	{
		"Curses Look And Feel",
		NS_LOOKANDFEEL_CID,
		"@mozilla.org/widget/lookandfeel;1",
		nsLookAndFeelConstructor
	},
	{
		"Curses Sound",
		NS_SOUND_CID,
		"@mozilla.org/sound;1",
		nsSoundConstructor
	},
	{
		"Transferable",
		NS_TRANSFERABLE_CID,
		"@mozilla.org/widget/transferable;1",
		nsTransferableConstructor
	},
	{
		"Gtk Clipboard",
		NS_CLIPBOARD_CID,
		"@mozilla.org/widget/clipboard;1",
		nsClipboardConstructor
	},
	{
		"Clipboard Helper",
		NS_CLIPBOARDHELPER_CID,
		"@mozilla.org/widget/clipboardhelper;1",
		nsClipboardHelperConstructor
	},
	{
		"HTML Format Converter",
		NS_HTMLFORMATCONVERTER_CID,
		"@mozilla.org/widget/htmlformatconverter;1",
		nsHTMLFormatConverterConstructor
	},
#ifdef IBMBIDI
	{
		"Bidi Keyboard",
		NS_BIDIKEYBOARD_CID,
		"@mozilla.org/widget/bidikeyboard;1",
		nsBidiKeyboardConstructor
	},
#endif // IBMBIDI
};

NS_IMPL_NSGETMODULE(nsWidgetCursesModule, components)
