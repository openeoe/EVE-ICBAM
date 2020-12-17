include ../../MakeVars

#
# What to build
#


TARGETS = \
	inspreddat

SRCS = \
	main.c         \
	oracle.c       \
	rproc.c

CTAGSRCS = \
	main.c         \
	oracle.pc      \
	rproc.c

DBSRCS = \
	oracle.pc

OBJS = \
	$(SRCS:.c=.o)

DSRCS = \
	$(DBSRCS:%.pc=%.c) $(DBSRCS:%.pc=%.lis)

#
# Structure Information
#

TOPDIR = ../..
LIBDIR = ../../../$(OSLIBDIR)

SNMPLIBS = \
	$(LIBDIR)/libltp.a     \
	$(LIBDIR)/libscript.a  \
	$(LIBDIR)/libscriptE.a \
	$(LIBDIR)/libarlcmpt.a \
	$(LIBDIR)/libarl.a     \
	$(LIBDIR)/libmibtree.a \
	$(LIBDIR)/libsnmp.a    \
	$(LIBDIR)/libarl.a

INC_DIRS = \
	-I$(TOPDIR)/ltp                 \
	-I$(TOPDIR)/snmp/include        \
	-I$(TOPDIR)/snmp/arl            \
	-I$(TOPDIR)/snmp/arl/compat     \
	-I$(TOPDIR)/snmp/script/lib     \
	-I$(TOPDIR)/snmp/script/lib/ext \
	-I/usr/include/pgsql            \
	-I.

#
# Build Options
#

#
# Version Definitions
#
V1  = -DSR_SNMPv1
V2C = -DSR_SNMPv2c
V2  = -DSR_SNMPv2
EXPORT = -DSR_NO_PRIVACY
USER_AP_SERVICE = -DSR_USER_AP_SERVICE
VER_DEFS = $(V1) $(V2C) $(V2) $(EXPORT)

#
# Compile Options
#

LIGHT =
DEBUG = -g
DEBUGFLAGS =
SR_MIB_TABLE_HASHING = -DSR_MIB_TABLE_HASHING
PROXY =
SETS = -DSETS

#
# Transports
#

IP_TRANSPORT = -DSR_IP
IPX_TRANSPORT =
TRANSPORTS = $(IP_TRANSPORT) $(IPX_TRANSPORT)

DEFS = \
	$(OS) \
	$(OSTYPE) \
	$(DEBUG) \
	$(LIGHT) \
	$(EXTRA_DEFS) \
	$(VER_DEFS) \
	$(USER_AP_SERVICE) \
	$(TRANSPORTS) \
	-DWANT_ALL_ERRORS

COPTIONS = $(DEFS) $(SYSOPTS)
CFLAGS = $(DEBUGFLAGS) $(COPTIONS) $(INC_DIRS) $(GLOBAL_CFLAGS) $(DATABASE_CFLAGS)
LINTFLAGS = $(COPTIONS) $(INC_DIRS)
LDFLAGS =
ECPGLDFLAGS = -lecpg -lpq -lpgtypes -ldl -lm

.SUFFIXES: .pc .c
.pc.c:
	$(ECPG) $(ECPGFLAGS) $<

all: $(TARGETS)

$(TARGETS): $(SRCS) $(OBJS) $(SNMPLIBS)
	$(CC) -o $(TARGETS) $(OBJS) $(ECPGLDFLAGS) $(SNMPLIBS) $(SOCKETLIBS) $(LIBS)

clean:
	-$(RM) $(OBJS) $(TARGETS) tags core $(DSRCS) *~ \#*

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<
