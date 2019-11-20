#ifndef TIMELINE_H
#define TIMELINE_H

#ifndef TLTIME_TYPE
#define TLTIME_TYPE unsigned long
#endif

typedef struct TLEvent TLEvent;
typedef struct Timeline Timeline;


typedef TLTIME_TYPE TLTime;
typedef void TLData;

typedef void(*TLEventHandler)(Timeline *,TLData *,TLTime);

struct TLEvent{
  TLEvent *next;
  TLTime startTime;
  TLEventHandler perform;
  TLData *data;
};

struct Timeline{
  TLEvent *firstEvent,*lastEvent,*firedEvent;
  TLTime startTime,finish;
  unsigned short repeat,repeatCount;
  char running;
};
Timeline *Timeline_create       (TLTime,TLEventHandler,TLData *);

TLTime    Timeline_getDuration  (Timeline *);
char      Timeline_update       (Timeline *,TLTime);
void      Timeline_addEvent     (Timeline *,TLTime,TLEventHandler,TLData *);
void      Timeline_removeEvent  (Timeline *,TLEventHandler,TLData *);
void      Timeline_begin        (Timeline *,TLTime);

/*

 last event determence the timeline duration

 */
#endif