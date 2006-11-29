/*
 * $Id: krisp.c,v 1.4 2006-11-29 10:19:39 oops Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <krisp.h>

extern char dberr[1024];
char krerr[1024];

char * krisp_version_pl (void) {
	return krisp_version ();
}

char * krisp_uversion_pl (void) {
	return krisp_uversion ();
}

char * krisp_error_pl (void) {
	return krerr;
}

KR_API * krisp_open_pl (char *datafile, int city) {
	KR_API *db;

	struct stat f;
	int r;

	geocity = city;

	r = stat (datafile, &f);

	if ( r == -1 ) {
		sprintf (krerr, "datafile not found : %s", datafile);
		return NULL;
	}

	if ( f.st_size < 1 ) {
		sprintf (krerr, "datafile size is zero: %s", datafile);
		return NULL;
	}

	db = (KR_API *) malloc (sizeof (KR_API));

	if ( kr_open (db, datafile) ) {
		strcpy (krerr, dberr);
		free (db);
		return NULL;
	}

	return db;
}

char * krisp_search_pl (KR_API *db, char *host) {
	KRNET_API isp;
	static char ret[1024];

	if ( strlen (host) > 255 ) {
		strncpy (isp.ip , host, 255);
		isp.ip[255] = 0;
	} else {
		strcpy (isp.ip, host);
	}

	if ( kr_search (&isp, db) ) {
		return NULL;
	}

	sprintf (ret, "%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s",
			isp.key, isp.ip, isp.netmask, isp.network, isp.broadcast,
			isp.icode, isp.iname, isp.ccode, isp.cname, isp.city, isp.region);

	return ret;
}

void krisp_close_pl (KR_API *db) {
	if ( db != NULL ) {
		kr_close (db);
		free (db);
	}
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
