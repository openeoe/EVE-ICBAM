#ifndef __INSFORE_H_
#define __INSFORE_H_

#include <stdio.h>
#include <time.h>

#include "ltpType.h"
#include "ltpByteArr.h"
#include "ltpTclHash.h"
#include "ltpSock.h"
#include "ltpCon.h"
#include "ltpServer.h"
#include "ltpProc.h"
#include "ltpLog.h"
#include "ltpUtil.h"
#include "util.h"
#include "properties.h"

#define NELEMS(arr)		(sizeof(arr) / sizeof(arr[0]))

#define ID_LEN          32
#define NAME_LEN        64
#define DESCR_LEN       256
#define TIME_LEN        14
#define IPADDR_LEN      15

#define INC_BLOCK       1024

enum {
    CTX_POLLNODE,
    CTX_POLLDEVICE,
    CTX_IFCONFIG,
    CTX_TRAP,
    CTX_POPIP,
    CTX_MLMTOOL,
    CTX_AUTODISCOVERY,
    CTX_AUTOTOPOLOGY
} SCRIPT_CONTEXT;


#define TIME_SCALE      100

typedef struct _RESINFO {
	char containment[256];
	char sub_inst[256];
} RESINFO;

#endif
