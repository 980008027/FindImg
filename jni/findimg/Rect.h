#ifndef _Rect_h_
#define _Rect_h_
class Rect
{
public:
	int left;
	int top;
	int right;
	int bottom;

	Rect() {
		left = -1;
		top = -1;
		right = -1;
		bottom = -1;
	}
	Rect(int left, int top, int right, int bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	~Rect() {
		left = -1;
		top = -1;
		right = -1;
		bottom = -1;
	}
	int Width() {
		return right - left + 1;
	}
	int Height() {
		return bottom - top + 1;
	}
};

#endif