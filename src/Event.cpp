#include "Event.h"
#include "Observation.h"
#include <iostream>
#include <string>

Event::Event(Observation obs,Action act, int rw)
{
	reward = rw;
	action = act;
	observation = obs;
	counter = 0;
}

string Event::str(void)
{
//	string id = to_string(episode_id) + '\t' + to_string(event_id);
	string id = to_string(event_id);
	string a = to_string(action.linear_x) + '\t'
		+ to_string(action.angular_z);
	/*string s = to_string(observation.lf) + '\t'
		+ to_string(observation.ls) + '\t' 
		+ to_string(observation.rs) + '\t' 
		+ to_string(observation.rf);*/
	string s = to_string(observation.l1) + '\t'
		+ to_string(observation.l2) + '\t' 
		+ to_string(observation.l3) + '\t' 
		+ to_string(observation.l4) + '\t' 
		+ to_string(observation.l5) + '\t' 
		+ to_string(observation.l6) + '\t' 
		+ to_string(observation.l7) + '\t' 
		+ to_string(observation.l8) + '\t' 
		+ to_string(observation.l9) + '\t' 
		+ to_string(observation.l10) + '\t' 
		+ to_string(observation.r1) + '\t' 
		+ to_string(observation.r2) + '\t' 
		+ to_string(observation.r3) + '\t' 
		+ to_string(observation.r4) + '\t' 
		+ to_string(observation.r5) + '\t' 
		+ to_string(observation.r6) + '\t' 
		+ to_string(observation.r7) + '\t' 
		+ to_string(observation.r8) + '\t' 
		+ to_string(observation.r9) + '\t' 
		+ to_string(observation.r10);

	string t = to_string(time.toSec());

	return id + '\t' + t + '\t' + a + '\t' + s + " 0";
}

