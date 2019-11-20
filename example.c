#include "Timeline.h"
#include <stdio.h>
#include <time.h>

void onTimelineEnd(Timeline *tl,TLData *data,TLTime t){
	const char *arg=(const char *)data;
	printf("onTimelineEnd %s\n",arg);
}

void onTimelineStart(Timeline *tl,TLData *data,TLTime t){
	const char *arg=(const char *)data;
	printf("onTimelineStart %s\n",arg);
}

void onTimelineCenter(Timeline *tl,TLData *data,TLTime t){
	const char *arg=(const char *)data;
	printf("onTimelineCenter %s\n",arg);
}

int main(){
	const char *argumentSample="my timeline";
	
	//construct a 5000 millisecond during timeline object.
	Timeline *tl=Timeline_create(5000,onTimelineEnd,(TLData *)argumentSample);
	
	//add an event at the start of the timeline.
	Timeline_addEvent(tl,0,onTimelineStart,(TLData *)argumentSample);

	//add an event half way the timeline.
	Timeline_addEvent(tl,2500,onTimelineCenter,(TLData *)argumentSample);

  //get milliseconds
  TLTime tm=clock()/1000;

  //start tl
  Timeline_begin(tl,tm);
  
  //stop timing at 6 seconds
  TLTime tm_end=tm+6000;
  
	while(tm<tm_end){
		//update current milliseconds
		tm=clock() / 1000;

		//perform possible timeline events
		Timeline_update(tl,tm);
	}
	
	return 0;
}