#include <iostream>
#include <windows.h>
#include <cmath>

#define PI 3.14159265

//define map size
const int width = 104, height = 54;//even only
char map[width][height];
char brightness[] = { " .:-=+*#%vnm@" };

bool horizontalcorrect = true;

float FocalLen = 1.0f;

float vertices[] = {
	//    x      y		z	  light
		-0.5f,	-0.5f,	1.0f, 1.0f, // left ball
		 0.5f,	-0.5f,	1.0f, 1.0f,
		 0.5f,	-0.5f,	2.0f, 1.0f,
		-0.5f,	-0.5f,	2.0f, 1.0f,
		-0.5f,	 0.5f,	1.0f, 1.0f,
		 0.5f,	 0.5f,	1.0f, 1.0f,
		 0.5f,	 0.5f,	2.0f, 1.0f,
		-0.5f,	 0.5f,	2.0f, 1.0f,

};

const int lines[] = {
	1, 2,
	2, 3,
	3, 4,
	4, 1,
	1, 5,
	2, 6,
	3, 7,
	4, 8,
	5, 6,
	6, 7,
	7, 8,
	8, 5,

};

float calculateCenterOfVertices(float varr[], int varrsize, int axis) {

	float maxval = varr[axis], minval = varr[axis];
	for (int i = axis; i < varrsize; i += 4) {
		if (varr[i] > maxval) maxval = varr[i];
		if (varr[i] < minval) minval = varr[i];
	}

	return (maxval + minval) / 2.0f;
}

void project(float varr[], int varrsize, const int larr[], int larrsize, float xtheta, float ytheta, float ztheta) {
	float projectedVertices[sizeof(varr) / sizeof(float)] = {};

	float midvalx = calculateCenterOfVertices(varr, varrsize, 0);
	float midvaly = calculateCenterOfVertices(varr, varrsize, 1);
	float midvalz = calculateCenterOfVertices(varr, varrsize, 2);

	for (int i = 0; i < varrsize; i += 4) { //vertex rotation using a 2D Rotation Matrix for each axis
		float x = varr[i] - midvalx, y = varr[i + 1] - midvaly, z = varr[i + 2] - midvalz;
		float xt = x, yt = y, zt = z;
		x = xt * cos(ztheta) - yt * sin(ztheta);
		y = xt * sin(ztheta) + yt * cos(ztheta);

		xt = x, yt = y, zt = z;
		y = yt * cos(xtheta) - zt * sin(xtheta);
		z = yt * sin(xtheta) + zt * cos(xtheta);

		xt = x, yt = y, zt = z;
		z = zt * cos(ytheta) - xt * sin(ytheta);
		x = zt * sin(ytheta) + xt * cos(ytheta);

		x += midvalx, y += midvaly, z += midvalz;

		if (horizontalcorrect) {
			x *= float(height) / float(width);
		}

		projectedVertices[i] = (FocalLen * x) / (z + FocalLen); //xproj
		projectedVertices[i + 1] = (FocalLen * y) / (z + FocalLen); //yproj
	}
	for (int i = 0; i < larrsize; i += 2) {
		int x0 = int(projectedVertices[(larr[i] - 1) * 4] * width) + width / 2;
		int y0 = int(projectedVertices[(larr[i] - 1) * 4 + 1] * height) + height / 2;
		int x1 = int(projectedVertices[(larr[i + 1] - 1) * 4] * width) + width / 2;
		int y1 = int(projectedVertices[(larr[i + 1] - 1) * 4 + 1] * height) + height / 2;

		int dx = abs(x1 - x0);
		int sx = (x0 < x1) ? 1 : -1;
		int dy = -abs(y1 - y0);
		int sy = (y0 < y1) ? 1 : -1;
		int error = dx + dy;
		int e2;

		while (true) { // Bresenham's Raster
			if (x0 >= 0 && y0 >= 0 && x0 < width && y0 < height) map[x0][y0] = '#';
			if (x0 == x1 && y0 == y1) { break; };
			e2 = 2 * error;
			if (e2 >= dy) {
				if (x0 == x1) { break; }
				error = error + dy;
				x0 = x0 + sx;
			}
			if (e2 <= dx) {
				if (y0 == y1) { break; }
				error = error + dx;
				y0 = y0 + sy;
			}
		}
	}
}

void displayMap() {
	system("cls");
	for (int i = 0; i < height; i += 1) {
		for (int j = 0; j < width; j += 1) {
			putchar(map[j][i]), putchar(map[j][i]);
		}
		putchar('\n');
	}
}
int main() {
	float alpha = 0.0f;
	while (true) {
		memset(map, ' ', sizeof(map));//clear projections
		project(vertices, sizeof(vertices) / sizeof(float), lines, sizeof(lines) / sizeof(int), 0, alpha, 0);
		displayMap();
		Sleep(100);
		alpha += PI / 60.0f;
	}

	return 0;
}