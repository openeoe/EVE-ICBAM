#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mktrain.h"

extern Properties* gconf;

int getAutoDiscovery(LtpCon* conn);
int getInterfaceList(LtpCon* conn);

LtpProcList proc_list[] = {
	{ "GET_AD"		, getAutoDiscovery	,			RETURN_SYNC	},
	{ 0				, 0					,			0			}
};


int getAutoDiscovery(LtpCon* conn)
{
	FieldArr* farr = conn->farr;

	farr->ret = NULL;

	if (farr->ret)
		return (1);
	else
		return (-1);
}

