#ifndef _Img_h_
#define _Img_h_
class Img
{
public:
	int width;
	int height;
	int ** colors;

	Img(int width, int height) {
		this->width = width;
		this->height = height;
		colors = new int *[width];
		for (int i = 0; i < width; i++)
		{
			colors[i] = new int[height];
			for (int j = 0; j < height; j++) {
				colors[i][j] = 0;
			}
		}
	}
	~Img() {
		for (int i = 0; i < width; i++)
		{
			delete[] colors[i];
		}
		delete[] colors;
	}
	void SetColors(jint * pixels) {
		int pos = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++) {
				colors[j][i] = pixels[pos++];
			}
		}
	}
	int GetColor(int x, int y) {
		return colors[x][y];
	}
	void SetColor(int x, int y, int color) {
		colors[x][y] = color;
	}
	void Display(){
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++) {
				int color = colors[i][j];
				printf("\t%d",color);
			}
			printf("\n");
		}
	}
};
#endif
