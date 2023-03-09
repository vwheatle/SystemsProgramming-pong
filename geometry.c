#include "geometry.h"

bool point_in_rect(vec2i point, rect2i rect) {
	return point.x >= rect.pos.x && point.y >= rect.pos.y &&
		point.x < (rect.pos.x + rect.size.width) &&
		point.y < (rect.pos.y + rect.size.height);
}

vec2i rect_bottom_right(rect2i rect) {
	return (vec2i) {
		rect.pos.x + rect.size.width - 1,
		rect.pos.y + rect.size.height - 1,
	};
}
