#include <stdio.h>
#include "Img.h"

#ifndef _ImgBlock_h_
#define _ImgBlock_h_

class ImgBlock
{
private:
	Img * img;
public:
	int border;
	int color;
	int s;
	int left;
	int top;
	int right;
	int bottom;
	ImgBlock() {
		left = -1;
		top = -1;
		right = -1;
		bottom = -1;
		color = 0;
		s = 0;
		border = 0;
	}
	~ImgBlock() {
		img = NULL;
		border = 0;
		color = 0;
		s= 0;
		left= -1;
		top= -1;
		right= -1;
		bottom= -1;
		
	}
	void Set(ImgBlock*other) {
		SetImg(other->img);
		border = other->border;
		color = other->color;
		s = other->s;
		left = other->left;
		top = other->top;
		right = other->right;
		bottom = other->bottom;
	}

	void SetImg(Img * img) {
		this->img = img;
	}
	Img * GetImg() {
		return img;
	}
	
	int Width() {
		return right - left + 1;
	}
	int Height() {
		return bottom - top + 1;
	}
};
class ImgBlockArray {
private:
	ImgBlock ** arr;
	int size;
	int len;
public:
	ImgBlockArray(int size) {
		len = 0;
		this->size = size;
		arr = new ImgBlock*[size];
	}
	~ImgBlockArray() {
		delete[] arr;
		arr = NULL;
	}
	int Length() {
		return len;
	}
	int Add(ImgBlock *i) {
		if (len > size - 1) {
			return -1;
		}
		arr[len++] = i;
		return len - 1;
	}
	
	ImgBlock *Get(int i) {
		if (i > len - 1) {
			return NULL;
		}
		return arr[i];
	}
};
#endif
