#include "obj.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool ObjLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier OBJ !" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (prefix == "vn") {
            Normal normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "vt") {
            TexCoord texCoord;
            iss >> texCoord.u >> texCoord.v;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "f") {
            Face face;
            std::string vertexData;
            while (iss >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string index;
                int vIdx, tIdx, nIdx;

                std::getline(vertexStream, index, '/');
                vIdx = std::stoi(index) - 1;
                face.vertexIndices.push_back(vIdx);

                if (std::getline(vertexStream, index, '/')) {
                    tIdx = !index.empty() ? std::stoi(index) - 1 : -1;
                    face.texCoordIndices.push_back(tIdx);
                }

                if (std::getline(vertexStream, index)) {
                    nIdx = !index.empty() ? std::stoi(index) - 1 : -1;
                    face.normalIndices.push_back(nIdx);
                }
            }
            faces.push_back(face);
        }
    }
    file.close();
    return true;
}
