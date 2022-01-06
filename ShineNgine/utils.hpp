#ifndef utility_hpp
#define utility_hpp

#include <fstream>
#include <iostream>
#include <ql/math/matrix.hpp>
#include <ql/models/model.hpp>
#include <ql/utilities/dataparsers.hpp>
#include <vector>


using namespace QuantLib;

class Reader
{
public:

//	std::vector<std::string> split(std::string str, char delimiter);

	inline Matrix readVolCSV(std::string file_name)
	{
		std::ifstream myfile;
		myfile.open(file_name);
		std::string myline, myentry, temp;
		std::vector<std::string> row;
		std::vector<std::vector<std::string>> matrix;
		if (myfile.good())
		{
			while (getline(myfile, myline))
			{
				std::stringstream ss(myline);
				row.clear();
				while (getline(ss, myentry, ','))
				{
					if (myentry == "\r")
					{
					}
					else
					{
						row.push_back(myentry);
					}
				}
				matrix.push_back(row);
			}
		}
		size_t n = matrix.size();
		size_t m = matrix[0].size();
		Matrix data(n, m);

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				data[i][j] = std::stold(matrix[i][j]);
			}
		}
		return(data);
	}


	inline std::vector<double> readRateCSV(std::string file_name)
	{
		std::vector<std::vector<std::string>> ln;

		std::ifstream openFile(file_name.data());
		if (openFile.is_open()) {
			std::string line;
			while (getline(openFile, line)) {
				std::vector<std::string> line_vector = split(line, ';');
				ln.push_back(line_vector);
			}
			openFile.close();
		}

		/*
		std::vector<double> Rate;
		for (int i = 1; i < ln.size(); i++)
		{
			Rate.push_back(std::stod(ln[i][2]));
		}*/

		std::vector<double> Rate;
		for (int i = 0; i < ln.size(); i++)
		{
			Rate.push_back(std::stod(ln[i][0]));
		}
		return Rate;
	}



	inline std::map<Date, double> readfixingCSV(std::string file_name)
	{
		std::vector<std::vector<std::string>> ln;
		std::map<Date, double> fixingDict;

		std::ifstream openFile(file_name.data());
		if (openFile.is_open()) {
			std::string line;
			while (getline(openFile, line)) {
				std::vector<std::string> line_vector = split(line, ';');
				ln.push_back(line_vector);
			}
			openFile.close();
		}

		std::vector<double> Rate;
		std::vector<Date> date;
		for (int i = 0; i < ln.size(); i++)
		{
			Rate.push_back(std::stod(ln[i][3]));
			date.push_back(DateParser::parseFormatted(ln[i][2], "%Y-%m-%d"));
			fixingDict.insert(std::make_pair(date[i], Rate[i]));
		}

		return fixingDict;
	}


	std::vector<std::string> split(std::string str, char delimiter) {
		std::vector<std::string> internal;
		std::stringstream ss(str);
		std::string temp;

		while (std::getline(ss, temp, delimiter)) {
			internal.push_back(temp);
		}

		return internal;
	}



	inline void writeCsv(const Matrix data, std::string file_name)
	{
		std::ofstream myfile;
		myfile.open(file_name);
		Size n = data.rows();
		Size m = data.columns();

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				myfile << data[i][j];
				if (j + 1 < m)
				{
					myfile << ", ";
				}
			}
			myfile << "\n";
		}
		myfile.close();
	}

	inline void newSection(std::string section_title)
	{
		std::string dash(section_title.length(), '-');
		std::cout << std::endl << "|-" << dash << "-|" << std::endl;
		std::cout << "| " << section_title << " |" << std::endl;
		std::cout << "|-" << dash << "-|" << std::endl << std::endl;
	}

	inline void printMatrix(const Matrix mat)
	{
		for (int i = 0; i < mat.rows(); i++)
		{
			for (int j = 0; j < mat.columns(); j++)
			{
				std::cout << mat[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

};


#endif