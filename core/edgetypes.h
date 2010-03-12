#ifndef _EDGETYPES_H_
#define _EDGETYPES_H_

#include <stdlib.h>
#include <string.h>
#include "hashtable_gs.h"
#include "hashtable_itr.h"

#include "graphserver.h"
#include "statetypes.h"

//---------------DECLARATIONS FOR EDGEPAYLOAD CLASS---------------------

struct EdgePayload {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, struct WalkOptions*);
  State* (*walkBack)(struct EdgePayload*, struct State*, struct WalkOptions*);
} ;

EdgePayload*
epNew( edgepayload_t type, void* payload );

void
epDestroy( EdgePayload* this );

edgepayload_t
epGetType( EdgePayload* this );

State*
epWalk( EdgePayload* this, State* param, WalkOptions* options );

State*
epWalkBack( EdgePayload* this, State* param, WalkOptions* options );

//---------------DECLARATIONS FOR ALIGHT CLASS---------------------------------------------

typedef struct Alight {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, struct WalkOptions*);
    State* (*walkBack)(struct EdgePayload*, struct State*, struct WalkOptions*);
    
    int n;
    int* arrivals;
    char** trip_ids;
    int* stop_sequences;
    
    ServiceCalendar* calendar;
    Timezone* timezone;
    int agency;
    ServiceId service_id;
    
    int overage; //number of seconds schedules past midnight of the last departure. If it's at 12:00:00, the overage is 0.
} Alight;

Alight*
alNew( ServiceId service_id, ServiceCalendar* calendar, Timezone* timezone, int agency );

void
alDestroy(Alight* this);

ServiceCalendar*
alGetCalendar( Alight* this );

Timezone*
alGetTimezone( Alight* this );

int
alGetAgency( Alight* this );

ServiceId
alGetServiceId( Alight* this );

int
alGetNumAlightings(Alight* this);

void
alAddAlighting(Alight* this, char* trip_id, int arrival, int stop_sequence);

char*
alGetAlightingTripId(Alight* this, int i);

int
alGetAlightingArrival(Alight* this, int i);

int
alGetAlightingStopSequence(Alight* this, int i);

int
alSearchAlightingsList(Alight* this, int time);

int
alGetLastAlightingIndex(Alight* this, int time);

int
alGetOverage(Alight* this);

int
alGetAlightingIndexByTripId(Alight* this, char* trip_id);

inline State*
alWalk(EdgePayload* this, State* state, WalkOptions* options);

inline State*
alWalkBack(EdgePayload* this, State* state, WalkOptions* options);

typedef struct PayloadMethods {
	void (*destroy)(void*);
	State* (*walk)(void*,State*,WalkOptions*);
	State* (*walkBack)(void*,State*,WalkOptions*);
	//char* (*to_str)(void*);
} PayloadMethods;

typedef struct CustomPayload {
  edgepayload_t type;
  void* soul;
  PayloadMethods* methods;
} CustomPayload;

PayloadMethods*
defineCustomPayloadType(void (*destroy)(void*),
						State* (*walk)(void*,State*,WalkOptions*),
						State* (*walkback)(void*,State*,WalkOptions*));


void
undefineCustomPayloadType( PayloadMethods* this );

CustomPayload*
cpNew( void* soul, PayloadMethods* methods );

void
cpDestroy( CustomPayload* this );

void*
cpSoul( CustomPayload* this );

PayloadMethods*
cpMethods( CustomPayload* this );

State*
cpWalk(CustomPayload* this, State* state, struct WalkOptions* walkoptions);

State*
cpWalkBack(CustomPayload* this, State* state, struct WalkOptions* walkoptions);

#endif
