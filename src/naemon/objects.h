#ifndef _OBJECTS_H
#define _OBJECTS_H

#if !defined (_NAEMON_H_INSIDE) && !defined (NAEMON_COMPILATION)
#error "Only <naemon/naemon.h> can be included directly."
#endif

#include "common.h"
#include "objects_common.h"
#include "objects_contact.h"
#include "objects_contactgroup.h"
#include "objects_command.h"
#include "objects_host.h"
#include "objects_hostdependency.h"
#include "objects_hostescalation.h"
#include "objects_hostgroup.h"
#include "objects_service.h"
#include "objects_servicedependency.h"
#include "objects_serviceescalation.h"
#include "objects_servicegroup.h"
#include "objects_timeperiod.h"
#include "objectlist.h"

NAGIOS_BEGIN_DECL


/*************** CURRENT OBJECT REVISION **************/
#define CURRENT_OBJECT_STRUCTURE_VERSION        402     /* increment when changes are made to data structures... */
/* Nagios 3 starts at 300, Nagios 4 at 400, etc. */


/***************** INDEXES ****************/
#define NUM_HASHED_OBJECT_TYPES      8
#define NUM_OBJECT_TYPES            14
#define NUM_INDEXED_OBJECT_TYPES    12

#define OBJTYPE_HOST                 0
#define OBJTYPE_SERVICE              1
#define OBJTYPE_COMMAND              2
#define OBJTYPE_TIMEPERIOD           3
#define OBJTYPE_CONTACT              4
#define OBJTYPE_CONTACTGROUP         5
#define OBJTYPE_HOSTGROUP            6
#define OBJTYPE_SERVICEGROUP         7
#define OBJTYPE_HOSTDEPENDENCY       8
#define OBJTYPE_SERVICEDEPENDENCY    9
#define OBJTYPE_HOSTESCALATION      10
#define OBJTYPE_SERVICEESCALATION   11
#define OBJTYPE_HOSTEXTINFO         12
#define OBJTYPE_SERVICEEXTINFO      13

/*** BACKWARD COMPAT JUNK. drop this on next major release ***/
#define NUM_OBJECT_SKIPLISTS        NUM_INDEXED_OBJECY_TYPES
#define HOST_SKIPLIST               OBJTYPE_HOST
#define SERVICE_SKIPLIST            OBJTYPE_SERVICE
#define COMMAND_SKIPLIST            OBJTYPE_COMMAND
#define TIMEPERIOD_SKIPLIST         OBJTYPE_TIMEPERIOD
#define CONTACT_SKIPLIST            OBJTYPE_CONTACT
#define CONTACTGROUP_SKIPLIST       OBJTYPE_CONTACTGROUP
#define HOSTGROUP_SKIPLIST          OBJTYPE_HOSTGROUP
#define SERVICEGROUP_SKIPLIST       OBJTYPE_SERVICEGROUP
#define HOSTDEPENDENCY_SKIPLIST     OBJTYPE_HOSTDEPENDENCY
#define SERVICEDEPENDENCY_SKIPLIST  OBJTYPE_SERVICEDEPENDENCY
/****************** DATA STRUCTURES *******************/

/* NOTIFY_LIST structure */
typedef struct notify_list {
	struct contact *contact;
	struct notify_list *next;
} notification;


/*
 * *name can be "Nagios Core", "Merlin", "mod_gearman" or "DNX", fe.
 * source_name gets passed the 'source' pointer from check_result
 * and must return a non-free()'able string useful for printing what
 * we need to determine exactly where the check was received from,
 * such as "mod_gearman worker@10.11.12.13", or "Nagios Core command
 * file worker" (for passive checks submitted locally), which will be
 * stashed with hosts and services and used as the "CHECKSOURCE" macro.
 */
struct check_engine {
	char *name;         /* "Nagios Core", "Merlin", "Mod Gearman" fe */
	const char *(*source_name)(void *);
	void (*clean_result)(void *);
};

struct check_output {
	char *short_output;
	char *long_output;
	char *perf_data;
};

/* CHECK_RESULT structure */
typedef struct check_result {
	int object_check_type;                          /* is this a service or a host check? */
	char *host_name;                                /* host name */
	char *service_description;                      /* service description */
	int check_type;					/* was this an active or passive service check? */
	int check_options;
	int scheduled_check;                            /* was this a scheduled or an on-demand check? */
	char *output_file;                              /* what file is the output stored in? */
	FILE *output_file_fp;
	double latency;
	struct timeval start_time;			/* time the service check was initiated */
	struct timeval finish_time;			/* time the service check was completed */
	int early_timeout;                              /* did the service check timeout? */
	int exited_ok;					/* did the plugin check return okay? */
	int return_code;				/* plugin return code */
	char *output;	                                /* plugin output */
	struct rusage rusage;			/* resource usage by this check */
	struct check_engine *engine;	/* where did we get this check from? */
	void *source;					/* engine handles this */
} check_result;


/* DBUF structure - dynamic string storage */
typedef struct dbuf {
	char *buf;
	unsigned long used_size;
	unsigned long allocated_size;
	unsigned long chunk_size;
} dbuf;


#define CHECK_STATS_BUCKETS                  15

/* used for tracking host and service check statistics */
typedef struct check_stats {
	int current_bucket;
	int bucket[CHECK_STATS_BUCKETS];
	int overflow_bucket;
	int minute_stats[3];
	time_t last_update;
} check_stats;


/********************* FUNCTIONS **********************/


/**** Top-level input functions ****/
int read_object_config_data(const char *, int);     /* reads all external configuration data of specific types */


int fcache_objects(char *cache_file);

NAGIOS_END_DECL
#endif
