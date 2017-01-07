#include <ACS_ZDoom.h>

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "utils.h"

[[extern("ACS")]]
struct Point rotate_point(struct Point coord, fixed angle)
{
	fixed new_angle = angle - get_angle(coord);
	fixed length = get_length(coord);
	
	struct Point new_point = {length * cos(new_angle),
							length * sin(new_angle)};
	return new_point;
}

[[extern("ACS")]]
fixed get_angle(struct Point coord)
{
	fixed angle = 0.0;
	if (coord.x) {
		angle = atan(coord.y / coord.x);
		
		if (coord.x < 0)
			angle += PI;
	} else {
		if (coord.y > 0) {
			angle = 0.5 * PI;
		} else if (coord.y < 0) {
			angle = 1.5 * PI;
		}
	}
	return angle;
}

[[extern("ACS")]]
fixed get_length(struct Point coord)
{	
	return sqrt(coord.x * coord.x + coord.y * coord.y);
}

[[extern("ACS")]]
int debug_msg(char* msg, ...)
{
	int result = 0;
	
	#ifdef XENOTOMB_DEBUG
	
    va_list args;
    va_start(args, msg);
    result = vprintf(msg, args);
    va_end(args);
	
	#endif // XENOTOMB_DEBUG
	
    return result;
	
}

[[extern("ACS")]]
accum unlerp(accum x, accum min, accum max)
{
	assert(min < max);
	return (x - min) / (max - min);
}

[[extern("ACS")]]
int safe_tid(void)
{
	static int last_tid = 0;
	
	debug_msg("safe_tid before: last_tid = %i\n", last_tid);
	last_tid = ACS_UniqueTID(++last_tid, 0);
	debug_msg("safe_tid after:  last_tid = %i\n", last_tid);
	
	if(last_tid == 0) { // can't find a free tid
		debug_msg("safe_tid: can't find free tid\n");
		last_tid = ACS_UniqueTID(1, 0); // so start again at 1
		debug_msg("safe_tid try again: last_tid = %i\n", last_tid);
		
		assert(last_tid != 0); // if last_tid is 0, no free tids on map
	}
	
	debug_msg("safe_tid returning: %i\n", last_tid);
	
	return last_tid;
}

[[extern("ACS")]]
fixed max(fixed x, fixed y)
{
	return (x > y) ? x : y;
}

[[extern("ACS")]]
fixed min(fixed x, fixed y)
{
	return (x < y) ? x : y;
}

// works even if clamp1 > clamp2
[[extern("ACS")]]
fixed clamp(fixed value, fixed clamp1, fixed clamp2)
{
	fixed low  = min(clamp1, clamp2);
	fixed high = max(clamp1, clamp2);
	
	if(value < low)
		value = low;
	if(value > high)
		value = high;
	
	return value;
}
