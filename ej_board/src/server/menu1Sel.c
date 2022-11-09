#include <stdio.h>
#include "array.h"
#include "server.h"

int menu1Sel(LPDATA vuser, int sd)
{
	char key[20];
	LPMANAGER user = (LPMANAGER)vuser;
	
	sprintf(key,"%d",sd);
	hashRemoveKey(user->curUserListHT,key);
	return 1;
}

