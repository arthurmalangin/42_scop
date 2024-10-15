#ifndef OBJLOADER_H
# define OBJLOADER_H

#include <iostream>
#include <vector>

class ObjLoader {
	public:
		ObjLoader(std::string fileName);
		std::vector <float>vertex;
		std::vector <unsigned int>indices;
};

#endif 
