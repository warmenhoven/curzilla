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
#include "nsWindow.h"

nsWindow::nsWindow()
{
	DBG(("nsWindow::nsWindow()\n"));
	mActivatePending  = PR_FALSE;
	mIsTopLevel       = PR_FALSE;
	mListenForResizes = PR_FALSE;
}

nsWindow::~nsWindow()
{
	DBG(("nsWindow::~nsWindow()\n"));
}

NS_IMPL_ISUPPORTS_INHERITED0(nsWindow, nsBaseWidget)

NS_IMETHODIMP
nsWindow::Create(nsIWidget        *aParent,
                 const nsRect     &aRect,
                 EVENT_CALLBACK    aHandleEventFunction,
                 nsIDeviceContext *aContext,
                 nsIAppShell      *aAppShell,
                 nsIToolkit       *aToolkit,
                 nsWidgetInitData *aInitData)
{
	return CreateWindow(aParent, nsnull, aRect, aHandleEventFunction,
			    aContext, aAppShell, aToolkit, aInitData);
}

NS_IMETHODIMP
nsWindow::Create(nsNativeWidget    aParent,
                 const nsRect     &aRect,
                 EVENT_CALLBACK    aHandleEventFunction,
                 nsIDeviceContext *aContext,
                 nsIAppShell      *aAppShell,
                 nsIToolkit       *aToolkit,
                 nsWidgetInitData *aInitData)
{
	return CreateWindow(nsnull, aParent, aRect, aHandleEventFunction,
			    aContext, aAppShell, aToolkit, aInitData);
}

NS_IMETHODIMP
nsWindow::CreateWindow(nsIWidget       *aParent,
		       nsNativeWidget   aNativeParent,
		       const nsRect    &aRect,
		       EVENT_CALLBACK   aHandleEventFunction,
		       nsIDeviceContext *aContext,
		       nsIAppShell      *aAppShell,
		       nsIToolkit       *aToolkit,
		       nsWidgetInitData *aInitData)
{
	DBG(("nsWindow::Create()\n"));

	nsIWidget *baseParent =
		aInitData && (aInitData->mWindowType == eWindowType_dialog ||
			      aInitData->mWindowType == eWindowType_toplevel ||
			      aInitData->mWindowType == eWindowType_invisible ) ?
		nsnull : aParent;

	DBG(("%p is the parent of %p\n", aParent, this));
	BaseCreate(baseParent, aRect, aHandleEventFunction, aContext,
		   aAppShell, aToolkit, aInitData);

	if (aParent || aNativeParent || (aInitData && aInitData->mListenForResizes))
		mListenForResizes = PR_TRUE;

	mBounds = aRect;
	mParent = aParent;
	if (aInitData != nsnull)
		mWindowType = aInitData->mWindowType;

	switch (mWindowType) {
	case eWindowType_dialog:
	case eWindowType_popup:
	case eWindowType_toplevel:
	case eWindowType_invisible:
		mIsTopLevel = PR_TRUE;
		break;
	case eWindowType_child:
	default:
		mIsTopLevel = PR_FALSE;
	}

	Resize(mBounds.width, mBounds.height, PR_FALSE);

	/*
	nsGUIEvent event(NS_MOVE, this);
	event.point.x = mBounds.x;
	event.point.y = mBounds.y;

	nsEventStatus status;
	DispatchEvent(&event, status);
	*/

	mActivatePending = PR_TRUE;

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::Destroy(void)
{
	DBG(("nsWindow::Destroy()\n"));
	OnDestroy();
	return NS_OK;
}

nsIWidget *
nsWindow::GetParent(void)
{
	DBG(("nsWindow::GetParent()\n"));
	nsIWidget *retval;
	retval = mParent;
	NS_IF_ADDREF(retval);
	return retval;
}

NS_IMETHODIMP
nsWindow::SetModal(PRBool aModal)
{
	LOG(("nsWindow::SetModal()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::IsVisible(PRBool & aState)
{
	LOG(("nsWindow::IsVisible()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::ConstrainPosition(PRBool aAllowSlop, PRInt32 *aX, PRInt32 *aY)
{
	LOG(("nsWindow::ConstrainPosition()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::Move(PRInt32 aX, PRInt32 aY)
{
	LOG(("nsWindow::Move()\n"));
	mBounds.x = aX;
	mBounds.y = aY;
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::SetSizeMode(PRInt32 aMode)
{
	LOG(("nsWindow::SetSizeMode()\n"));
	return nsBaseWidget::SetSizeMode(aMode);
}

NS_IMETHODIMP
nsWindow::Enable(PRBool aState)
{
	LOG(("nsWindow::Enable()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::IsEnabled(PRBool *aState)
{
	LOG(("nsWindow::IsEnabled()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

void
nsWindow::OnDestroy(void)
{
	LOG(("nsWindow::OnDestroy()\n"));
	nsBaseWidget::OnDestroy();
	mParent = nsnull;

	nsGUIEvent event;
	nsEventStatus status;
	InitGUIEvent(event, NS_DESTROY);
	DispatchEvent(&event, status);
}

NS_IMETHODIMP
nsWindow::SetFocus(PRBool aRaise)
{
	LOG(("nsWindow::SetFocus()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::SetForegroundColor(const nscolor &aColor)
{
	LOG(("nsWindow::SetForegroundColor()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::SetBackgroundColor(const nscolor &aColor)
{
	DBG(("nsWindow::SetBackgroundColor()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

nsIFontMetrics*
nsWindow::GetFont(void)
{
	LOG(("nsWindow::GetFont()\n"));
	return nsnull;
}

NS_IMETHODIMP
nsWindow::SetFont(const nsFont &aFont)
{
	LOG(("nsWindow::SetFont()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::SetCursor(nsCursor aCursor)
{
	DBG(("nsWindow::SetCursor(%d)\n", aCursor));
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::Validate()
{
	LOG(("nsWindow::Validate()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::Invalidate(PRBool aIsSynchronous)
{
	LOG(("nsWindow::Invalidate()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::Invalidate(const nsRect &aRect,
                     PRBool        aIsSynchronous)
{
	LOG(("nsWindow::Invalidate(%d, %d, %d, %d)\n",
	     aRect.x, aRect.y, aRect.width, aRect.height));

	nsRect rect(aRect.x, aRect.y, aRect.width, aRect.height);
	nsEventStatus status;
	nsPaintEvent paint;
	InitPaintEvent(paint);
	paint.point.x = aRect.x;
	paint.point.y = aRect.y;
	paint.rect = &rect;
	paint.region = nsnull;
	paint.renderingContext = GetRenderingContext();
	DispatchEvent(&paint, status);

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::InvalidateRegion(const nsIRegion* aRegion,
                           PRBool           aIsSynchronous)
{
	LOG(("nsWindow::InvalidateRegion()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::Update()
{
	LOG(("nsWindow::Update()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::SetColorMap(nsColorMap *aColorMap)
{
	LOG(("nsWindow::SetColorMap()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::Scroll(PRInt32  aDx,
                 PRInt32  aDy,
                 nsRect  *aClipRect)
{
	LOG(("nsWindow::Scroll()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::ScrollWidgets(PRInt32 aDx,
                        PRInt32 aDy)
{
	LOG(("nsWindow::ScrollWidgets()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::ScrollRect(nsRect  &aSrcRect,
                     PRInt32  aDx,
                     PRInt32  aDy)
{
	LOG(("nsWindow::ScrollRect()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

void*
nsWindow::GetNativeData(PRUint32 aDataType)
{
	DBG(("nsWindow::GetNativeData()\n"));
	switch (aDataType) {
	case NS_NATIVE_WIDGET:
		return (void *)this;
	}
	WRN(("nsWindow::GetNativeData(): unhandled type %d!\n", aDataType));
	return nsnull;
}

NS_IMETHODIMP
nsWindow::SetBorderStyle(nsBorderStyle aBorderStyle)
{
	LOG(("nsWindow::SetBorderStyle()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::SetTitle(const nsAString& aTitle)
{
	DBG(("nsWindow::SetTitle()\n"));
	NS_ConvertUCS2toUTF8 utf8title(aTitle);
	mvaddstr(0, COLS - strlen((const char *)utf8title.get()) - 1, utf8title.get());
	refresh();
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::SetIcon(const nsAString& aIconSpec)
{
	DBG(("nsWindow::SetIcon()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::SetMenuBar(nsIMenuBar * aMenuBar)
{
	LOG(("nsWindow::SetMenuBar()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::ShowMenuBar(PRBool aShow)
{
	DBG(("nsWindow::ShowMenuBar()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::WidgetToScreen(const nsRect& aOldRect, nsRect& aNewRect)
{
	LOG(("nsWindow::WidgetToScreen()\n"));
	aNewRect.x = aOldRect.x;
	aNewRect.y = aOldRect.y;
	aNewRect.width = aOldRect.width;
	aNewRect.height = aOldRect.height;
	return NS_OK;
}

NS_IMETHODIMP
nsWindow::ScreenToWidget(const nsRect& aOldRect, nsRect& aNewRect)
{
	LOG(("nsWindow::ScreenToWidget()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::BeginResizingChildren(void)
{
	LOG(("nsWindow::BeginResizingChildren()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::EndResizingChildren(void)
{
	LOG(("nsWindow::EndResizingChildren()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::EnableDragDrop(PRBool aEnable)
{
	DBG(("nsWindow::EnableDragDrop()\n"));
	return NS_OK;
}

void
nsWindow::ConvertToDeviceCoordinates(nscoord &aX,
                                     nscoord &aY)
{
	LOG(("nsWindow::ConvertToDeviceCoordinates()\n"));
}

NS_IMETHODIMP
nsWindow::CaptureMouse(PRBool aCapture)
{
	LOG(("nsWindow::CaptureMouse()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::CaptureRollupEvents(nsIRollupListener *aListener,
                              PRBool             aDoCapture,
                              PRBool             aConsumeRollupEvent)
{
	LOG(("nsWindow::CaptureRollupEvents()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::GetAttention(PRInt32 aCycleCount)
{
	LOG(("nsWindow::GetAttention()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::HideWindowChrome(PRBool aShouldHide)
{
	LOG(("nsWindow::HideWindowChrome()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

void
nsWindow::InitGUIEvent(nsGUIEvent &aEvent, PRUint32 aMsg)
{
	DBG(("nsWindow::InitGUIEvent()\n"));
	memset(&aEvent, 0, sizeof(nsGUIEvent));
	aEvent.eventStructType = NS_GUI_EVENT;
	aEvent.message = aMsg;
	aEvent.widget = NS_STATIC_CAST(nsIWidget *, this);
}

void
nsWindow::InitSizeEvent(nsSizeEvent &aEvent)
{
	DBG(("nsWindow::InitSizeEvent()\n"));
	memset(&aEvent, 0, sizeof(nsSizeEvent));
	aEvent.eventStructType = NS_SIZE_EVENT;
	aEvent.message = NS_SIZE;
	aEvent.widget = NS_STATIC_CAST(nsIWidget *, this);
}

void
nsWindow::InitPaintEvent(nsPaintEvent &aEvent)
{
	DBG(("nsWindow::InitPaintEvent()\n"));
	memset(&aEvent, 0, sizeof(nsPaintEvent));
	aEvent.eventStructType = NS_PAINT_EVENT;
	aEvent.message = NS_PAINT;
	aEvent.widget = NS_STATIC_CAST(nsIWidget *, this);
}

NS_IMETHODIMP
nsWindow::DispatchResizeEvent(nsRect &aRect, nsEventStatus &aStatus)
{
	DBG(("nsWindow::DispatchResizeEvent()\n"));
	nsSizeEvent event;
	InitSizeEvent(event);

	event.windowSize = &aRect;
	event.point.x = aRect.x;
	event.point.y = aRect.y;
	event.mWinWidth = aRect.width;
	event.mWinHeight = aRect.height;

	DispatchEvent(&event, aStatus);

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::DispatchEvent(nsGUIEvent *aEvent,
			nsEventStatus &aStatus)
{
	DBG(("nsWindow::DispatchEvent()\n"));
	aStatus = nsEventStatus_eIgnore;

	// hold a widget reference while we dispatch this event
	NS_ADDREF(aEvent->widget);

	if (this == aEvent->widget) {
		DBG(("this %p event type %u message %u\n",
		       this, aEvent->eventStructType, aEvent->message));
	} else {
		DBG(("this %p event widget %p type %u message %u\n",
		       this, aEvent->widget, aEvent->eventStructType, aEvent->message));
	}

	// send it to the standard callback
	if (mEventCallback)
		aStatus = (* mEventCallback)(aEvent);

	// dispatch to event listener if event was not consumed
	// this is really only for mouse events... ?
	if ((aStatus != nsEventStatus_eIgnore) && mEventListener)
		aStatus = mEventListener->ProcessEvent(*aEvent);

	NS_IF_RELEASE(aEvent->widget);

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::Show(PRBool aState)
{
	DBG(("nsWindow::Show(%d), %d\n", aState, mActivatePending));
	nsEventStatus status;

	if (!aState)
		return NS_OK; /* XXX */

	if (mIsTopLevel) {
		if (mActivatePending) {
			mActivatePending = PR_FALSE;

			nsSizeModeEvent event(NS_SIZEMODE, this);
			event.mSizeMode = nsSizeMode_Normal;
			DispatchEvent(&event, status);

			nsGUIEvent gui;
			InitGUIEvent(gui, NS_MOVE);
			gui.point.x = mBounds.x;
			gui.point.y = mBounds.y;
			DispatchEvent(&gui, status);

			InitGUIEvent(gui, NS_GOTFOCUS);
			DispatchEvent(&gui, status);

			InitGUIEvent(gui, NS_ACTIVATE);
			DispatchEvent(&gui, status);
		} else {
			nsRect rect(mBounds.x, mBounds.y, mBounds.width, mBounds.height);

			nsPaintEvent paint;
			InitPaintEvent(paint);
			paint.point.x = mBounds.x;
			paint.point.y = mBounds.y;
			paint.rect = &rect;
			paint.region = nsnull;
			paint.renderingContext = GetRenderingContext();
			DispatchEvent(&paint, status);
		}
	} else if (mParent) {
		nsRect rect(mBounds.x, mBounds.y, mBounds.width, mBounds.height);

		nsPaintEvent paint;
		InitPaintEvent(paint);
		paint.point.x = mBounds.x;
		paint.point.y = mBounds.y;
		paint.rect = &rect;
		paint.region = nsnull;
		paint.renderingContext = GetRenderingContext();
		DispatchEvent(&paint, status);
	}

	nsIWidget *child = mFirstChild;
	while (child) {
		child->Show(aState);
		child = child->GetNextSibling();
	}

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::Resize(PRInt32 aWidth, PRInt32 aHeight, PRBool aRepaint)
{
	LOG(("nsWindow::Resize()\n"));
	mBounds.width = aWidth;
	mBounds.height = aHeight;

	if (mIsTopLevel || mListenForResizes) {
		nsRect rect(mBounds.x, mBounds.y, aWidth, aHeight);
		nsEventStatus status;
		DispatchResizeEvent(rect, status);
	}

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::Resize(PRInt32 aX, PRInt32 aY, PRInt32 aWidth, PRInt32 aHeight,
                       PRBool aRepaint)
{
	LOG(("nsWindow::Resize()\n"));
	mBounds.x = aX;
	mBounds.y = aY;
	mBounds.width = aWidth;
	mBounds.height = aHeight;

	if (mIsTopLevel || mListenForResizes) {
		nsRect rect(mBounds.x, mBounds.y, aWidth, aHeight);
		nsEventStatus status;
		DispatchResizeEvent(rect, status);
	}

	return NS_OK;
}

NS_IMETHODIMP
nsWindow::GetPreferredSize(PRInt32 &aWidth, PRInt32 &aHeight)
{
	LOG(("nsWindow::GetPreferredSize()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsWindow::SetPreferredSize(PRInt32 aWidth, PRInt32 aHeight)
{
	LOG(("nsWindow::SetPreferredSize()\n"));
	return NS_ERROR_NOT_IMPLEMENTED;
}

// nsChildWindow class

nsChildWindow::nsChildWindow()
{
	DBG(("nsChildWindow::nsChildWindow()\n"));
}

nsChildWindow::~nsChildWindow()
{
	DBG(("nsChildWindow::~nsChildWindow()\n"));
}
