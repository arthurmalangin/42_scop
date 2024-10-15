#include "matriceUtils.h"

void calculateRotationMatrix(float angle, float matrix[16], int dir) {
    float _cos = cos(angle);
    float _sin = sin(angle);

    for (int i = 0; i < 16; ++i) {
        matrix[i] = 0.0f;
    }
    matrix[15] = 1.0f;

    if (dir == 1) {
        matrix[0] = _cos;
        matrix[2] = _sin;
        matrix[5] = 1.0f;
        matrix[8] = -_sin;
        matrix[10] = _cos;
    }
    else if (dir == 2) {
        matrix[0] = 1.0f;
        matrix[5] = _cos;
        matrix[6] = -_sin;
        matrix[9] = _sin;
        matrix[10] = _cos;
    }
}


void multiplyMatrices(const float mat1[16], const float mat2[16], float result[16]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[row * 4 + col] = mat1[row * 4] * mat2[col] +
                mat1[row * 4 + 1] * mat2[col + 4] +
                mat1[row * 4 + 2] * mat2[col + 8] +
                mat1[row * 4 + 3] * mat2[col + 12];
        }
    }
}

std::vector<float> calculateModelCenter(const std::vector<float>& vertices) {
    std::vector<float> center(3, 0.0f);
    size_t vertexCount = vertices.size() / 3;

    for (size_t i = 0; i < vertices.size(); i += 3) {
        center[0] += vertices[i];
        center[1] += vertices[i + 1];
        center[2] += vertices[i + 2];
    }
    center[0] /= vertexCount;
    center[1] /= vertexCount;
    center[2] /= vertexCount;

    return center;
}

void calculateTranslationMatrix(float tx, float ty, float tz, float matrix[16]) {
    matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = 0.0f;
    matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = 0.0f;
    matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
    matrix[12] = tx;   matrix[13] = ty;   matrix[14] = tz;   matrix[15] = 1.0f;
}


void calculatePerspectiveMatrix(float fov, float aspectRatio, float near, float far, float matrix[16]) {
    float tanHalfFovy = tan(fov / 2.0f);

    // Ligne 1 - Ajustement de l'aspect ratio et du champ de vision
    matrix[0] = 1.0f / (aspectRatio * tanHalfFovy);
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    // Ligne 2 - Ajustement du champ de vision pour l'axe Y
    matrix[4] = 0.0f;
    matrix[5] = 1.0f / tanHalfFovy;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    // Ligne 3 - Paramétrage de la profondeur avec les plans near et far
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = -(far + near) / (far - near);
    matrix[11] = -1.0f;

    // Ligne 4 - Calcul de l’échelle de la profondeur et paramètre final
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = -(2.0f * far * near) / (far - near);
    matrix[15] = 0.0f;
}