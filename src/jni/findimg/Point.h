#ifndef _Point_h_
#define _Point_h_
class Point
{
public:
	int x;
	int y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};
#endif

