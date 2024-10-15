#include <vector>
#include <string>

struct Vertex {
    float x, y, z;
};

struct Normal {
    float x, y, z;
};

struct TexCoord {
    float u, v;
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> texCoordIndices;
    std::vector<int> normalIndices;
};

class ObjLoader {
public:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<TexCoord> texCoords;
    std::vector<Face> faces;

    bool load(const std::string& filename);
};
