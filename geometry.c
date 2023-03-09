#include "geometry.h"

bool vec2i_eq(vec2i a, vec2i b) { return a.x == b.x && a.y == b.y; }
bool size2i_eq(size2i a, size2i b) {
	return a.width == b.width && a.height == b.height;
}

bool rect2i_eq(rect2i a, rect2i b) {
	return vec2i_eq(a.pos, b.pos) && size2i_eq(a.size, b.size);
}

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
