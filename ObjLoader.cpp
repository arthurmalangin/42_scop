#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));

    return tokens;
}

void debugPrint(std::vector <float>vertex) {
    std::cout << "Contenu de `this->vertex` :" << std::endl;
    for (const auto& vertex : vertex) {
        std::cout << vertex << std::endl;
    }
}

void debugPrintIndices(const std::vector<unsigned int>& indices) {
    std::cout << "Indices utilisés pour les faces après triangulation :" << std::endl;
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::cout << "Triangle: ";
        std::cout << indices[i] << ", " << indices[i + 1] << ", " << indices[i + 2] << std::endl;
    }
}


ObjLoader::ObjLoader(std::string fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier OBJ !" << std::endl;
        return ;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> splited = split(line, ' ');
        if (splited[0] == "v") {
            for (int i = 1; i < 4; i++)
                this->vertex.push_back(atof(splited[i].c_str()));
        }
        if (splited[0] == "f") {
            std::vector<unsigned int> faceIndices;

            // Parcours des indices dans la face
            for (size_t i = 1; i < splited.size(); ++i) {
                // Récupère les indices de sommet/normale/texture
                std::vector<std::string> indices = split(splited[i], '/');
                unsigned int vertexIndex = std::stoi(indices[0]) - 1; // Indice de sommet, conversion en 0-based
                faceIndices.push_back(vertexIndex);
            }

            // Si plus de 3 sommets, on doit trianguler
            if (faceIndices.size() > 3) {
                int pivot = faceIndices[0]; // Utilise le premier point comme pivot
                for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                    indices.push_back(pivot);
                    indices.push_back(faceIndices[i]);
                    indices.push_back(faceIndices[i + 1]);
                }
            }
            else if (faceIndices.size() == 3) {
                // Triangle, utiliser directement
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[1]);
                indices.push_back(faceIndices[2]);
            }
        }
    }
    //debugPrint(this->vertex);
    //debugPrintIndices(this->indices);
    file.close();
}