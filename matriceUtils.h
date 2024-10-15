#ifndef MATRICEUTILS_H
#define MATRICEUTILS_H

#include <cmath>
#include <vector>

void calculateRotationMatrix(float angle, float matrix[16], int dir);
void multiplyMatrices(const float mat1[16], const float mat2[16], float result[16]);
std::vector<float> calculateModelCenter(const std::vector<float>& vertices);
void calculateTranslationMatrix(float tx, float ty, float tz, float matrix[16]);
void calculatePerspectiveMatrix(float fov, float aspectRatio, float near, float far, float matrix[16]);


#endif