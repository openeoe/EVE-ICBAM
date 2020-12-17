#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>

#include "mktrain.h"


extern int loginOracle(char* site, char* dbname, char* user, char* passwd);
extern int dump_traffic(char* db_dir, int in_out, char* today, char* fromdate);
extern int dump_resource_mem(char* db_dir, char* today, char* fromdate);
extern int dump_resource_swap(char* db_dir, char* today, char* fromdate);
extern int dump_resource_fsusage(char* db_dir, char* today, char* fromdate);
extern int dump_environment_temp(char* db_dir, char* today, char* fromdate);
extern int dump_apc_cpu(char* db_dir, char* today, char* fromdate);
extern int dump_apc_mem(char* db_dir, char* today, char* fromdate);
extern int dump_apcluster_const(char* db_dir, char* today, char* fromdate);

void init_STATDATA();
void init_RAWDATA(char* fromdate);
time_t str2time(char* strtime);
char* time2str(time_t ctime, char* buf);

Properties* gconf;
RAWDATA gdat[NUM_RAWDATA];
float gavg[24];
int gnum[24];


int main(int argc, char* argv[])
{
	int c = 0;
	int err = 0;
	int len = 0;
	int help = 0;
	int period = 0;

	time_t ctime = 0;
	struct tm* tp = NULL;

	char today[65];
	char fromdate[65];

	char* conf = NULL;
	char* attr = NULL;
	char* fdate = NULL;
	char* db_user = NULL;
	char* db_passwd = NULL;
	char* db_name = NULL;
	char* db_period = NULL;
	char* db_dir = NULL;

	/* Usage */
	while((c = getopt(argc, argv, "hfc:a:")) != EOF) {
		switch(c) {
		case 'c': conf = strdup(optarg); break; 
		case 'a': attr = strdup(optarg); break;
		case 'f': fdate = strdup(optarg); break;
		case 'h': help = 1; break;
		default : err = 1; break;
		}
	}

	if (err > 0 || help > 0 || conf == NULL || attr == NULL) {
		fprintf(stderr, "\n");
		fprintf(stderr, "Usage: %s [-c config] [-f date] [-a in|out|mem|swap|temp|apccpu|apcmem|const]\n", argv[0]);
		fprintf(stderr, "   -c config  : <config> config_file\n"); 
		fprintf(stderr, "   -f date    : <date>   from date\n"); 
		fprintf(stderr, "   -a in      : <in>     traffic in usage\n"); 
		fprintf(stderr, "   -a out     : <out>    traffic out usage\n"); 
		fprintf(stderr, "   -a mem     : <memory> memory usage\n"); 
		fprintf(stderr, "   -a swap    : <swap>   swap usage\n"); 
		fprintf(stderr, "   -a fs      : <filesystem> file system usage\n"); 
		fprintf(stderr, "   -a temp    : <temp>   temperature\n"); 
		fprintf(stderr, "   -a apccpu  : <apccpu> apc cpu usage\n"); 
		fprintf(stderr, "   -a apcmem  : <apcmem> apc memory usage\n"); 
		fprintf(stderr, "   -a const   : <const>  connected stations\n"); 
		fprintf(stderr, "\n");
		exit(1);
	}

	gconf = new_Properties();
	if (read_Properties(gconf, conf) == 0) {
		fprintf(stderr, "failed to read configuration file(%s)\n", conf);
		exit(1);
	}

	db_user   = get_Properties(gconf, "db.user");
	db_passwd = get_Properties(gconf, "db.passwd");
	db_name   = get_Properties(gconf, "db.name");
	db_period = get_Properties(gconf, "db.period");
	db_dir    = get_Properties(gconf, "db.dir");

	if (db_user == NULL || db_passwd == NULL || db_name == NULL || db_period == NULL || db_dir == NULL) {
		fprintf(stderr, "failed to read db account info from %s\n", conf);
		exit(1);
	}

	period = atoi(db_period);

	if (period <= 0 || period > 300) {
		fprintf(stderr, "invalid period data=[%s]\n", db_period);
		exit(1);
	}

	time(&ctime);
	tp = localtime(&ctime);
	sprintf(today, "%04d%02d%02d", tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday);

	ctime = ctime - (3600 * 24 * period);

	if (fdate == NULL) {
		tp = localtime(&ctime);
		sprintf(fromdate, "%04d%02d%02d00", 
			tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday);
	} else {
		len = strlen(fdate);

		if (len != 8) {
			LOGG(L_ERROR, "invalid date=[%s] (ex: 20180821)\n", fdate);
			exit(1);
		}

		sprintf(fromdate, "%s00", fdate);
	}

	LOGG(L_DEBUG, "from=[%s]\n", fromdate);

	LOGG(L_DEBUG, "[000][%s]\n", gdat[0].coldate);
	LOGG(L_DEBUG, "[251][%s]\n", gdat[NUM_RAWDATA-1].coldate);

	err = loginOracle("GKeS", db_name, db_user, db_passwd);
	if (err) {
		LOGG(L_ERROR, "failed to login to database\n");
		exit(1);
	}

	if (strcmp(attr, "in") == 0) {
		dump_traffic(db_dir, 1, today, fromdate);		/* INPUT TRAFFIC */
	} else if (strcmp(attr, "out") == 0) {
		dump_traffic(db_dir, 2, today, fromdate);		/* OUTPUT TRAFFIC */
	} else if (strcmp(attr, "mem") == 0) {
		dump_resource_mem(db_dir, today, fromdate);
	} else if (strcmp(attr, "swap") == 0) {
		dump_resource_swap(db_dir, today, fromdate);
	} else if (strcmp(attr, "fs") == 0) {
		dump_resource_fsusage(db_dir, today, fromdate);
	} else if (strcmp(attr, "temp") == 0) {
		dump_environment_temp(db_dir, today, fromdate);
	} else if (strcmp(attr, "apccpu") == 0) {
		dump_apc_cpu(db_dir, today, fromdate);
	} else if (strcmp(attr, "apcmem") == 0) {
		dump_apc_mem(db_dir, today, fromdate);
	} else if (strcmp(attr, "const") == 0) {
		dump_apcluster_const(db_dir, today, fromdate);
	}

	return (1);
}


void init_STATDATA()
{
	int i = 0;

	for (i = 0; i < 24; i++) {
		gavg[i] = 0.0;
		gnum[i] = 0;
	}
}


void init_RAWDATA(char* fromdate)
{
	int i = 0;
	time_t now = 0;
	char date[12];

	memset(&gdat[0], 0x00, sizeof(gdat));
	memset(&gavg[0], 0x00, sizeof(gavg));
	memset(&gnum[0], 0x00, sizeof(gnum));

	memset(date, 0x00, sizeof(date));
	strncpy(date, fromdate, 10);		/* YYYYMMDDHH */

	now = str2time(date);

	for (i = 0; i < NUM_RAWDATA; i++) {
		strncpy(gdat[i].coldate, date, 10);
		gdat[i].value = 0.0;
		gdat[i].flag = INVALID_DATA;

		now += 7200;					/* add 2 hour */
		time2str(now, date);
	}
}


char* time2str(time_t ctime, char* buf)
{
	struct tm *tp = NULL;

	tp = localtime(&ctime);

	sprintf(buf, "%04d%02d%02d%02d", 
		tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour);

	return buf;
}


time_t str2time(char* strtime)
{
	struct tm stp;
	char year[5], month[3], day[3];
	char hour[3], minute[3], sec[3];

	memset(&stp,   0, sizeof(stp));

	memset(year,   0, sizeof(year));
	memset(month,  0, sizeof(month));
	memset(day,    0, sizeof(day));
	memset(hour,   0, sizeof(hour));
	memset(minute, 0, sizeof(minute));
	memset(sec,    0, sizeof(sec));

	strncpy(year,   &strtime[0], 4);
	strncpy(month,  &strtime[4], 2);
	strncpy(day,    &strtime[6], 2);
	strncpy(hour,   &strtime[8], 2);
	strncpy(minute, "00", 2);
	strncpy(sec,    "00", 2);

	stp.tm_year = atoi(year) - 1900;
	stp.tm_mon  = atoi(month) - 1;
	stp.tm_mday = atoi(day);
	stp.tm_hour = atoi(hour);
	stp.tm_min  = atoi(minute);
	stp.tm_sec  = atoi(sec);

	return (mktime(&stp));
}

