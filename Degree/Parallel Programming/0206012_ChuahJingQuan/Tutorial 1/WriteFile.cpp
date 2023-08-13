#include "WriteFile.h"

std::ofstream fout;

//Constructor 
WriteFile::WriteFile() {}

//Deconstructor
WriteFile::~WriteFile() {}


void WriteFile::write_to_txtfile(std::string fileName, std::vector<float> temp_data, std::string type) {

	//Set path to save the file
	std::string path = "temp/";
	path.append(type.append("_"));

	//Generate file name with path
	std::string fileNameWithExtension = path.append(fileName.append(".txt"));

	//Open the file / Create the file if not found
	fout.open(fileNameWithExtension, std::ofstream::out);

	//Write to the file
	for (int i = 0; i < temp_data.size(); i++) {
		if (i == temp_data.size() - 1 && temp_data[temp_data.size() - 1] == 0) {
			printf("%c", '=');
		}
		else {
			fout << temp_data[i] << std::endl;
		}
	}

	//Close the file
	fout.close();
}

//Method overload
void WriteFile::write_to_txtfile(std::string fileName, std::vector<std::string> month) {
	//Set path to save the file
	std::string path = "temp/";

	//Generate file name with path
	std::string fileNameWithExtension = path.append(fileName.append(".txt"));

	//Open the file / Create the file if not found
	fout.open(fileNameWithExtension, std::ofstream::out);

	//Write to the file
	for (int i = 0; i < month.size(); i++) {
		fout << month[i] << std::endl;
	}

	//Close the file
	fout.close();
}

void WriteFile::write_to_txtfile(std::string fileName, std::string year) {
	//Set path to save the file
	std::string path = "temp/";

	//Generate file name with path
	std::string fileNameWithExtension = path.append(fileName.append(".txt"));

	//Open the file / Create the file if not found
	fout.open(fileNameWithExtension, std::ofstream::out);

	fout << year << std::endl;

	//Close the file
	fout.close();
}