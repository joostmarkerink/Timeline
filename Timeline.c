#include "Timeline.h"
#include <stdlib.h>

Timeline *Timeline_create(TLTime tm,TLEventHandler h,TLData *d){
  Timeline *tl=malloc(sizeof(Timeline));
  tl->firstEvent=tl->lastEvent=tl->firedEvent=0;
  tl->running=0;
  tl->repeat=1;
  Timeline_addEvent(tl,tm,h,d);
  return tl;
}

TLTime Timeline_getDuration(Timeline *tl){
  return tl->lastEvent->startTime;
}



char Timeline_update(Timeline *tl,TLTime time){

  if(tl->finish>0) return 0;
  
  char result=0;
  /*if(!tl->running) {
    tl->running=1;
    tl->startTime=time;
  }*/
  TLTime duration=Timeline_getDuration(tl);
  
  TLTime t=time-tl->startTime;
  TLTime rt=t-(duration*tl->repeatCount);

  TLEvent *e=tl->firedEvent;
  if(e){
    if(e->next && e->next->startTime<=rt){
      e=tl->firedEvent=e->next;
      (*e->perform)(tl,e->data,t);
      result=1;
      if(e==tl->lastEvent){ 
        tl->firedEvent=0;
        tl->repeatCount++;
        if(tl->repeatCount==tl->repeat){
          tl->finish=time;
        }
      }
    }
  }else{
    if(tl->firstEvent->startTime<=rt){
      e=tl->firedEvent=tl->firstEvent;
      (*e->perform)(tl,e->data,t);
      result=1;
    }
  }
  return result;
}

TLEvent *Timeline_findEventBefore(Timeline *tl,TLTime tm){
  TLEvent *p=0,*e=tl->firstEvent;
  while(e){
    if(e->startTime > tm) return p;
    p=e;
    e=e->next;
  }
  return 0;
}
/*
TLEvent *Timeline_findEventAfter(Timeline *tl,TLTime tm){
  TLEvent *e=tl->firstEvent;
  while(e){
    if(e->startTime>tm){
      break;
    }
      printf("%i %i\n",(unsigned)e->startTime,tm);

    e=e->next;
  }
  return e;
}*/



void Timeline_add(Timeline *tl,TLEvent *e){
  if(!tl->firstEvent && !tl->lastEvent){
    tl->firstEvent=tl->lastEvent=e;
  }else if(tl->firstEvent==tl->lastEvent){
    if(e->startTime<tl->firstEvent->startTime){
      e->next=tl->firstEvent;
      tl->firstEvent=e;
    }else{
      tl->firstEvent->next=e;
      tl->lastEvent=e;
    }
  }else{
    TLEvent *n=Timeline_findEventBefore(tl,e->startTime);
    if(n){
      e->next=n->next;
      n->next=e;
      if(n==tl->lastEvent){
        tl->lastEvent=n;
      }
    }else{
      e->next=tl->firstEvent;
      tl->firstEvent=e;
    }
  }
}

void Timeline_addEvent(Timeline *tl,TLTime tm,TLEventHandler h,TLData *d){
  TLEvent *e=malloc(sizeof(TLEvent));
  e->perform=h;
  e->data=d;
  e->startTime=tm;
  e->next=0;
  Timeline_add(tl,e);
}

void Timeline_begin(Timeline *tl,TLTime tm){
  tl->startTime=tm;
  tl->firedEvent=0;
  tl->repeatCount=0;
  tl->running=1;
  tl->finish=0;
  //Timeline_update(tl,tm);
}

TLEvent *Timeline_findPreviousEvent(Timeline *tl,TLEvent *n){
  TLEvent *e=tl->firstEvent;
  while(e){
    if(e->next==n) break;
    e=e->next;
  }
  return e;
}

void Timeline_remove(Timeline *tl,TLEvent *e){
 TLEvent *p=Timeline_findPreviousEvent(tl,e);
 if(p){
  p->next=e->next;
 }else{
  tl->firstEvent=e->next;
 }
 free(e);
}

TLEvent *Timeline_findEventWithData(Timeline *tl,TLData *d){
  TLEvent *e=tl->firstEvent;
  while(e){
    if(e->data==d) break;
    e=e->next;
  }
  return e;
}

TLEvent *Timeline_findEventWithDataAndHandler(Timeline *tl,TLData *d,TLEventHandler h){
  TLEvent *e=tl->firstEvent;
  while(e){
    if(e->data==d && e->perform==h) break;
    e=e->next;
  }
  return e;
}

void Timeline_removeEvent(Timeline *tl,TLEventHandler h,TLData *d){
  TLEvent *e=Timeline_findEventWithDataAndHandler(tl,h,d);
  if(e){
    Timeline_remove(tl,e);
  }
}