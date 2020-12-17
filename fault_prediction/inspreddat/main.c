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

#include "inspred.h"

extern int loginOracle(char* site, char* dbname, char* user, char* passwd);
extern int insert_traffic_in(char* db_dir, char* today, char* fromdate);
extern int insert_traffic_out(char* db_dir, char* today, char* fromdate);
extern int insert_resource_mem(char* db_dir, char* today, char* fromdate);
extern int insert_resource_swap(char* db_dir, char* today, char* fromdate);
extern int insert_resource_fsusage(char* db_dir, char* today, char* fromdate);
extern int insert_environment_temp(char* db_dir, char* today, char* fromdate);

extern int insert_apc_cpu(char* db_dir, char* today, char* fromdate);
extern int insert_apc_mem(char* db_dir, char* today, char* fromdate);
extern int insert_apcluster_const(char* db_dir, char* today, char* fromdate);


Properties* gconf;


int main(int argc, char* argv[])
{
	int c = 0;
	int err = 0;
	int help = 0;

	char today[65];

	char* conf = NULL;
	char* attr = NULL;
	char* fname = NULL;
	char* date = NULL;
	char* db_user = NULL;
	char* db_passwd = NULL;
	char* db_name = NULL;
	char* db_dir = NULL;

	/* Usage */
	while((c = getopt(argc, argv, "hc:a:f:d:")) != EOF) {
		switch(c) {
		case 'c': conf = strdup(optarg); break; 
		case 'a': attr = strdup(optarg); break;
		case 'f': fname = strdup(optarg); break;
		case 'd': date = strdup(optarg); break;
		case 'h': help = 1; break;
		default : err = 1; break;
		}
	}

	if (err > 0 || help > 0 || conf == NULL || attr == NULL || fname == NULL || date == NULL) {
		fprintf(stderr, "\n");
		fprintf(stderr, "Usage: %s [-c config] [-f datafile] [-d date] [-a in|out|mem|swap|temp|apccpu|apcmem|const]\n", argv[0]);
		fprintf(stderr, "   -c config  : <config> config_file\n"); 
		fprintf(stderr, "   -f datafile: <datafile> forecast datafile\n"); 
		fprintf(stderr, "   -a in      : <in>     traffic in usage\n"); 
		fprintf(stderr, "   -a out     : <out>    traffic out usage\n"); 
		fprintf(stderr, "   -a mem     : <memory> memory usage\n"); 
		fprintf(stderr, "   -a swap    : <swap>   swap usage\n"); 
		fprintf(stderr, "   -a fs      : <filesystem> file system usage\n"); 
		fprintf(stderr, "   -a temp    : <temp>   temperature\n"); 
		fprintf(stderr, "   -a apccpu  : <apccpu> apc cpu usage\n"); 
		fprintf(stderr, "   -a apcmem  : <apcmem> apc memory usage\n"); 
		fprintf(stderr, "   -a const   : <const>  connected station\n"); 
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
	db_dir    = get_Properties(gconf, "db.dir");

	if (db_user == NULL || db_passwd == NULL || db_name == NULL || db_dir == NULL) {
		fprintf(stderr, "failed to read db account info from %s\n", conf);
		exit(1);
	}

	err = loginOracle("GKeS", db_name, db_user, db_passwd);
	if (err) {
		LOGG(L_ERROR, "failed to login to database\n");
		exit(1);
	}
/*
	time(&ctime);
	tp = localtime(&ctime);
	sprintf(today, "%04d%02d%02d", tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday);
*/
	strcpy(today, date);
	LOGG(L_DEBUG, "today=[%s]\n", today);


	if (strcmp(attr, "in") == 0) {
		insert_traffic_in(db_dir, today, fname);
	} else if (strcmp(attr, "out") == 0) {
		insert_traffic_out(db_dir, today, fname);
	} else if (strcmp(attr, "mem") == 0) {
		insert_resource_mem(db_dir, today, fname);
	} else if (strcmp(attr, "swap") == 0) {
		insert_resource_swap(db_dir, today, fname);
	} else if (strcmp(attr, "fs") == 0) {
		insert_resource_fsusage(db_dir, today, fname);
	} else if (strcmp(attr, "temp") == 0) {
		insert_environment_temp(db_dir, today, fname);
	} else if (strcmp(attr, "apccpu") == 0) {
		insert_apc_cpu(db_dir, today, fname);
	} else if (strcmp(attr, "apcmem") == 0) {
		insert_apc_mem(db_dir, today, fname);
	} else if (strcmp(attr, "const") == 0) {
		insert_apcluster_const(db_dir, today, fname);
	}

	return (1);
}

