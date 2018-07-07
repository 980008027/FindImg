#include <stdio.h>
#include "com_jsdroid_findimg_FindImg.h"
#include "findimg/FindImg.h"
JNIEXPORT void JNICALL Java_com_jsdroid_findimg_FindImg_nativeFindImg
(JNIEnv *env, jclass clazz, jintArray imgBigArr, jint bigWidth, jint bigHeight, jintArray imgSmallArr, jint smallWidth, jint smallHeight, jint level, jint left, jint top, jint right, jint bottom, jint offset, jint distance, jfloat sim, jintArray outputArr) {
	jint *imgBig = env->GetIntArrayElements(imgBigArr,NULL);
	jint *imgSmall = env->GetIntArrayElements(imgSmallArr, NULL);
	jint *output = env->GetIntArrayElements(outputArr, NULL);

	Rect *rect = new Rect();
	rect->left = -1;
	rect->top = -1;
	rect->right = -1;
	rect->bottom = -1;
	Img *img_big = new Img(bigWidth,bigHeight);
	Img *img_small = new Img(smallWidth, smallHeight);
	
	img_big->SetColors(imgBig);
	img_small->SetColors(imgSmall);
	int ret = findImg(img_big, img_small, rect, level, left, top, right, bottom, offset, distance, sim);
	if (ret) {
		output[0] = rect->left;
		output[1] = rect->top;
		output[2] = rect->right;
		output[3] = rect->bottom;
	}
	else {
		output[0] = -1;
		output[1] = -1;
		output[2] = -1;
		output[3] = -1;
	}
	delete rect;
	delete img_big;
	delete img_small;
	env->ReleaseIntArrayElements(imgBigArr, imgBig,0);
	env->ReleaseIntArrayElements(imgSmallArr, imgSmall,0);
	env->ReleaseIntArrayElements(outputArr, output,0);

}
