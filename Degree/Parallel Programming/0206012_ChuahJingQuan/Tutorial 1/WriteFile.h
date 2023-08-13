#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class WriteFile {

public:
	//Constructor and deconstructor 
	WriteFile();
	~WriteFile();

	void WriteFile::write_to_txtfile(std::string fileName, std::vector<float> temp_data, std::string type);

	//Method overload
	void WriteFile::write_to_txtfile(std::string fileName, std::vector<std::string> month);
	void WriteFile::write_to_txtfile(std::string fileName, std::string year);

};
