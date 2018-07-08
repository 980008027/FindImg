
#include "Point.h"
#ifndef  _PointScan_h_
#define _PointScan_h_

class PointScan
{
private:
	int pos;
public:
	Point * points;
	PointScan(int size) {
		pos = 0;
		points = new Point[size];
	}
	~PointScan() {
		if(points){
			delete []points;
			points = NULL;
		}
	}
	Point Remove() {
		return points[--pos];
	}
	
	void Add(Point point) {
		points[pos++] = point;
	}
	void Add(int x, int y) {
		points[pos++] = Point(x, y);
	}
	int HasPoint() {
		return pos;
	}
	
};

#endif // ! _PointScan_h_
