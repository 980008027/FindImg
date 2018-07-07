#include "Point.h"
#include "Rect.h"
#include "Img.h"
#include "ImgBlock.h"
#include "PointScan.h"
#ifndef _FindImg_h_
#define _FindImg_h_
int _abs(int i) {
	if (i < 0)
		return -i;
	return i;
}

int _r(int color) {
	return (color >> 16) & 0xff;
}

int _g(int color) {
	return (color >> 8) & 0xff;
}

int _b(int color) {
	return color & 0xff;
}

int _rgb(int red, int green, int blue) {
	return (0xff << 24) | (red << 16) | (green << 8) | blue;
}
int compareColor(int color1, int color2, int offr, int offg, int offb) {
	int r1 = _r(color1);
	int r2 = _r(color2);
	if (_abs(r1 - r2) > offr) {
		return false;
	}
	int g1 = _g(color1);
	int g2 = _g(color2);
	if (_abs(g1 - g2) > offg) {
		return false;
	}
	int b1 = _b(color1);
	int b2 = _b(color2);
	if (_abs(b1 - b2) > offb) {
		return false;
	}
	return true;
}
//图片颜色分级，最低1级，最高256级，级别越高，耗时越长，精确度越高
void splitLevel(Img *img, int level) {
	if (level < 1) {
		level = 1;
	}
	if (level > 256) {
		level = 256;
	}
	if (level == 256) {
		return;
	}
	for (int i = 0; i < img->width; i++) {
		for (int j = 0; j < img->height; j++) {
			int color = img->GetColor(i,j);
			int r = _r(color);
			int g = _g(color);
			int b = _b(color);
			r = r / level * level;
			g = g / level * level;
			b = b / level * level;
			img->SetColor(i, j, _rgb(r, g, b));
		}
	}
}
//扫描单个特征
void scanImgBlock(ImgBlock *out,Img *img,int **hasScan,PointScan *pointScan,int posx,int posy, int offr, int offg, int offb, int distance, float sim) {
	
	int color = img->GetColor(posx, posy);
	out->SetImg(img);
	out->color = color;
	pointScan->Add(posx, posy);
	while (pointScan->HasPoint()) {
		Point point = pointScan->Remove();
		for (int i = -distance; i <= distance; i++) {
			for (int j = -distance; j <= distance; j++) {
				
				int x = point.x + i;
				int y = point.y + j;
				//如果越界，继续循环
				if (x < 0 || x > img->width - 1 || y < 0 || y > img->height - 1) {
					continue;
				}
				//判断是否可以添加
				int colorTemp = img->GetColor(x, y);
				if (colorTemp == color && hasScan[x][y] == 0) {
					hasScan[x][y] = 1;
					pointScan->Add(x, y);
					if (out->left == -1 || out->left > x) {
						out->left = x;
					}
					if (out->top == -1 || out->top > y) {
						out->top = y;
					}
					if (out->right == -1 || out->right < x) {
						out->right = x;
					}
					if (out->bottom == -1 || out->bottom < y) {
						out->bottom = y;
					}
					out->s++;
				}

			}
		}
	}
	out->border = (out->left == 0 || out->right == img->width - 1 || out->top == 0 || out->bottom == img->height - 1);	
}
int scanAllImgBlock(ImgBlock *limitColor,ImgBlockArray *out, Img *img, int left, int top, int right, int bottom,int offr,int offg,int offb, int distance, float sim) {
	int count = 0;
	if (left == 0 && top == 0 && right == 0 && bottom == 0) {
		right = img->width - 1;
		bottom = img->height - 1;
	}
	if (left < 0) {
		left = 0;
	}
	if (left > img->width - 1) {
		left = img->width - 1;
	}
	if (right < 0) {
		right = 0;
	}
	if (right > img->width - 1) {
		right = img->width - 1;
	}
	if (top < 0) {
		top = 0;
	}
	if (top > img->height - 1) {
		top = img->height - 1;
	}
	if (bottom < 0) {
		bottom = 0;
	}
	if (bottom > img->height - 1) {
		bottom = img->height - 1;
	}
	//创建hasScan数组
	int **hasScan = new int*[img->width];
	for (int i = 0; i < img->width; i++) {
		hasScan[i] = new int[img->height];
		for (int j = 0; j < img->height; j++) {
			hasScan[i][j] = 0;
		}
	}
	//创建pointScan统计类
	PointScan * pointScan = new PointScan(img->width*img->height);

	//扫描
	for (int i = left; i <= right; i++) {
		for (int j = top; j <= bottom; j++) {
			if (hasScan[i][j]) {
				continue;
			}
			if (limitColor) {
				int c = img->GetColor(i, j);
				if (c != limitColor->color) {
					hasScan[i][j] = true;
					continue;
				}
			}
			ImgBlock *scan = new ImgBlock();
			scanImgBlock(scan, img, hasScan, pointScan, i, j, offr,offg,offb,distance,sim);
			out->Add(scan);
			count++;
		}
	}

	//释放pointScan
	delete pointScan;
	//释放hasScan数组
	for (int i = 0; i < img->width; i++) {
		delete [] hasScan[i];
	}
	delete [] hasScan;
	return count;
}
ImgBlock *getMaxBlock(ImgBlockArray *blocks) {
	ImgBlock *ret = NULL;
	
	for (int i = 0; i < blocks->Length(); i++) {
		ImgBlock *temp = blocks->Get(i);
		if (temp->border == false) {
			if (ret == NULL || ret->s < temp->s) {
				ret = temp;
			}
		}
	}
	return ret;
}
//比较2个特征（的颜色以及颜色矩阵)是否相同
int compareImgBlock(ImgBlock *big, ImgBlock *small, Rect * out,int offr,int offg,int offb,float sim) {
	
	if (compareColor(big->color, small->color, offr, offg, offb)==false) {
		return false;
	}
	float scaleX = small->Width() * 1.0f / big->Width();
	float scaleY = small->Height() * 1.0f / big->Height();
	//最大错误数量
	int maxDisCount = (int)((1.0 - sim) * big->s + 0.5);
	int disCount = 0;
	for (int i = big->left; i <= big->right; i++) {
		for (int j = big->top; j < big->bottom; j++) {
			int bigx = i;
			int bigy = j;
			int bigColor = big->GetImg()->colors[bigx][bigy];
			if (bigColor != big->color) {
				continue;
			}
			int smallx = (int)(small->left + (bigx - big->left) * scaleX + 0.5f);
			int smally = (int)(small->top + (bigy - big->top) * scaleY + 0.5f);
			if (smallx < 0 || smallx > small->right || smally < 0
				|| smally > small->bottom) {
				continue;
			}
			//比较颜色
			int smallColor = small->GetImg()->colors[smallx][smally];
			if (compareColor(bigColor, smallColor, offr, offg, offb)==false) {
				disCount++;
				if (disCount > maxDisCount) {
					return false;
				}
			}
		}
	}

	float left = (big->left - small->left / scaleX);
	float top = (big->top - small->top / scaleY);
	float width_ = (small->GetImg()->width / scaleX);
	float height_ = (small->GetImg()->height / scaleY);
	out->left = left+0.5f ;
	out->top = top +0.5f;
	out->right = left + width_ - 1 ;
	out->bottom = top + height_ - 1;
	return true;
}

//判断指定范围小图是否匹配
int compareImgRect(Img*bigImg, Img*smallImg,Rect *rect, int offr, int offg,
	int offb, int distance, float sim) {
	int left = rect->left;
	int top = rect->top;
	if (left < 0 || top < 0) {
		return false;
	}
	float scaleX = rect->Width()*1.0f/smallImg->width ;
	float scaleY = rect->Height()*1.0f/smallImg->height;
	int maxDisCount = (int)(smallImg->width * smallImg->height * (1 - sim) + 0.5f);
	
	int disCount = 0;
	for (int left_ = left - 1; left_ <= left + 1; left_++) {
		//Loop:
		for (int top_ = top - 1; top_ <= top + 1; top_++) {
			int continueLoop = false;
			for (int i = 0; i < smallImg->width; i++) {
				for (int j = 0; j < smallImg->height; j++) {
					int posx = (int)(i * scaleX + left_);
					int posy = (int)(j * scaleY + top_);
					if (posx < 0 || posx > bigImg->width - 1 || posy < 0
						|| posy > bigImg->height - 1) {
						//continue Loop
						continueLoop = true;
						i = smallImg->width;
						j = smallImg->height;
						break;
					}
					int color1 = smallImg->colors[i][j];
					int err = true;
					//Err:
					for (int dis = -distance; dis <= distance; dis++) {
						int tempx = posx + dis;
						if (tempx < 0 || tempx >= bigImg->width) {
							continue;
						}
						for (int dis2 = -distance; dis2 <= distance; dis2++) {
							int tempy = posy + dis2;
							if (tempy < 0 || tempy >= bigImg->height) {
								break;
							}
							int c = bigImg->colors[tempx][tempy];
							//比较颜色
							if (compareColor(c, color1, offr, offg, offb)) {
								err = false;
								//break Err;
								dis = distance + 1;
								dis2 = distance + 1;
								break;
							}
						}
					}
					if (err) {
						disCount++;
						if (disCount > maxDisCount) {
							//continue Loop;
							continueLoop = true;
							i = smallImg->width;
							j = smallImg->height;
							break;
						}
					}
				}
			}
			if (continueLoop) {
				continue;
			}
			int width_ = rect->Width();
			int height_ = rect->Height();
			rect->left = (int)left_;
			rect->top = (int)top_;
			rect->right = (int)(left_ + width_);
			rect->bottom = (int)(top_ + height_);
			
			return true;
		}
	}
	return false;
}

int findImg(Img *big, Img *small,Rect *out, int level, int left, int top, int right,
	int bottom, int offColor, int distance, float sim) {
	splitLevel(big, level);
	splitLevel(small, level);
	
	int offr = _r(offColor);
	int offg = _g(offColor);
	int offb = _b(offColor);
	ImgBlockArray *bigBlocks = new ImgBlockArray(big->width*big->height);
	ImgBlockArray *smallBlocks = new ImgBlockArray(small->width*small->height);
	
	ImgBlock *maxSmallBlock = NULL;
	scanAllImgBlock(NULL,smallBlocks,small,0,0,small->width-1,small->height-1,offr,offg,offb,distance, sim);
	maxSmallBlock = getMaxBlock(smallBlocks);
	if (maxSmallBlock == NULL) {
		delete smallBlocks;
		return false;
	}
	scanAllImgBlock(maxSmallBlock, bigBlocks, big, left, top, right, bottom, offr, offg, offb, distance,sim);
	printf("small blocks len:%d\n",smallBlocks->Length());
	printf("big blocks len:%d\n",bigBlocks->Length());
	int ret = false;
	int sucCount = 0;
	for (int i = 0; i < bigBlocks->Length(); i++) {
		ImgBlock * bigTemp = bigBlocks->Get(i);
		if (compareImgBlock(bigTemp, maxSmallBlock, out,offr,offg,offb,sim)) {
			sucCount++;
			if (compareImgRect(big, small, out, offr, offg, offb, distance, sim)) {
				ret = true;
				break;
			}
		}
	}
	delete bigBlocks;
	delete smallBlocks;
	return ret;
}


#endif 

