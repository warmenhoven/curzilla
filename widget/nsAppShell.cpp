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
#include "nsIEventQueueService.h"
#include "nsIServiceManager.h"
#include "plhash.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static PRBool sInitialized = PR_FALSE;
static PLHashTable *sQueueHashTable = nsnull;
static PLHashTable *sCountHashTable = nsnull;

PRLogModuleInfo *gWidgetLog = nsnull;

#define NUMBER_HASH_KEY(_num) ((PLHashNumber) _num)

static PLHashNumber
IntHashKey(PRInt32 key)
{
	return NUMBER_HASH_KEY(key);
}

nsAppShell::nsAppShell(void)
{
	if (!gWidgetLog)
		gWidgetLog = PR_NewLogModule("Widget");
	mQuitNow = 0;
	DBG(("nsAppShell::nsAppShell()\n"));
}

nsAppShell::~nsAppShell(void)
{
	DBG(("nsAppShell::~nsAppShell()\n"));
}

NS_IMPL_ISUPPORTS1(nsAppShell, nsIAppShell)

NS_IMETHODIMP
nsAppShell::Create(int *argc, char **argv)
{
	DBG(("nsAppShell::Create()\n"));

	if (sInitialized)
		return NS_OK;

	sInitialized = PR_TRUE;

	initscr();
	keypad(stdscr, TRUE);
	nonl();
	raw();
	noecho();
	mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

	if (has_colors())
		start_color();

	return NS_OK;
}

static PRIntn
build_fdSet(PLHashEntry *he, PRIntn i, void *arg)
{
	int *nFDs = (int *)arg;
	nFDs[i] = (int)(he->key);
	return NS_OK;
}

NS_IMETHODIMP
nsAppShell::Run(void)
{
	DBG(("nsAppShell::Run()\n"));

	if (!mEventQueue)
		Spinup();

	if (!mEventQueue)
		return NS_ERROR_NOT_INITIALIZED;

	int curs_state = curs_set(0);
	refresh();

	INF(("nsAppShell entering inf. loop"));
	while (!mQuitNow) {
		int *nFDs = (int *)(malloc(sizeof(int) * sQueueHashTable->nentries));
		PL_HashTableEnumerateEntries(sQueueHashTable, build_fdSet, nFDs);

		fd_set set;
		int maxFd = 0;
		FD_ZERO(&set);
		FD_SET(0, &set);
		for (unsigned int i = 0; i < sQueueHashTable->nentries; i++) {
			FD_SET(nFDs[i], &set);
			if (nFDs[i] > maxFd)
				maxFd = nFDs[i];
		}

		select(maxFd + 1, &set, NULL, NULL, NULL);

		if (FD_ISSET(0, &set)) {
			int c = getch();
			if (c == 3 || c == 4) {
				free(nFDs);
				break;
			}
		}
		for (unsigned int i = 0; i < sQueueHashTable->nentries; i++) {
			if (FD_ISSET(nFDs[i], &set)) {
				nsIEventQueue *eventQueue =
					(nsIEventQueue *)(PL_HashTableLookup(sQueueHashTable,
									     (void *)(nFDs[i])));
				if (eventQueue) {
					INF(("nsAppShell Processing Pending Events"));
					eventQueue->ProcessPendingEvents();
				}
			}
		}

		free(nFDs);
	}
	INF(("nsAppShell::Run() exits\n"));

	Spindown();

	curs_set(curs_state);
	endwin();
	mQuitNow = 0;
	return NS_OK;
}

NS_IMETHODIMP
nsAppShell::Spinup(void)
{
	DBG(("nsAppShell::Spinup()\n"));
	nsresult rv = NS_OK;

	// get the event queue service
	nsCOMPtr <nsIEventQueueService> eventQService =
		do_GetService(NS_EVENTQUEUESERVICE_CONTRACTID, &rv);

	if (NS_FAILED(rv)) {
		NS_WARNING("Failed to get event queue service");
		return rv;
	}

	// get the event queue for this thread
	rv = eventQService->GetThreadEventQueue(NS_CURRENT_THREAD,
						getter_AddRefs(mEventQueue));

	// if we got an event queue, just use it
	if (mEventQueue)
		goto done;

	// otherwise creaet a new event queue for the thread
	rv = eventQService->CreateThreadEventQueue();
	if (NS_FAILED(rv)) {
		NS_WARNING("Could not create the thread event queue");
		return rv;
	}

	// ask again for the event queue now that we have create one.
	rv = eventQService->GetThreadEventQueue(NS_CURRENT_THREAD,
						getter_AddRefs(mEventQueue));

done:
	ListenToEventQueue(mEventQueue, PR_TRUE);

	return rv;
}

NS_IMETHODIMP
nsAppShell::Spindown(void)
{
	DBG(("nsAppShell::Spindown()\n"));
	if (mEventQueue) {
		ListenToEventQueue(mEventQueue, PR_FALSE);
		mEventQueue->ProcessPendingEvents();
		mEventQueue = nsnull;
	}
	return NS_OK;
}

NS_IMETHODIMP
nsAppShell::ListenToEventQueue(nsIEventQueue *aQueue, PRBool aListen)
{
	if (!sQueueHashTable)
		sQueueHashTable = PL_NewHashTable(3, (PLHashFunction)IntHashKey,
						  PL_CompareValues,
						  PL_CompareValues, 0, 0);
	if (!sCountHashTable)
		sCountHashTable = PL_NewHashTable(3, (PLHashFunction)IntHashKey,
						  PL_CompareValues,
						  PL_CompareValues, 0, 0);

	PRInt32 key = aQueue->GetEventQueueSelectFD();
	INF(("nsAppShell::ListenToEventQueue(%p(%d), %d)\n", aQueue, key, aListen));

	/* add a listener */
	if (aListen) {
		/* only add if we arn't already in the table */
		if (!PL_HashTableLookup(sQueueHashTable, (void *)key)) {
			PL_HashTableAdd(sQueueHashTable, (void *)(key), (void *)(aQueue));
		}
		/* bump up the count */
		int count = (int)(PL_HashTableLookup(sCountHashTable, (void *)(key)));
		PL_HashTableAdd(sCountHashTable, (void *)(key), (void *)(count+1));
	} else {
		/* remove listener */
		int count = (int)(PL_HashTableLookup(sCountHashTable, (void *)(key)));
		if (count - 1 == 0) {
			PL_HashTableRemove(sQueueHashTable, (void *)(key));
			PL_HashTableRemove(sCountHashTable, (void *)(key));
		} else {
			PL_HashTableAdd(sCountHashTable, (void *)(key), (void *)(count-1));
		}
	}

	return NS_OK;
}

NS_IMETHODIMP
nsAppShell::GetNativeEvent(PRBool &aRealEvent, void * &aEvent)
{
	DBG(("nsAppShell::GetNativeEvent()\n"));
	aRealEvent = PR_FALSE;
	aEvent = 0;

	return NS_OK;
}

NS_IMETHODIMP
nsAppShell::DispatchNativeEvent(PRBool aRealEvent, void *aEvent)
{
	DBG(("nsAppShell::DispatchNativeEvent()\n"));
	return NS_OK;
}

NS_IMETHODIMP
nsAppShell::Exit(void)
{
	DBG(("nsAppShell::Exit()\n"));
	mQuitNow = 1;
	return NS_OK;
}
