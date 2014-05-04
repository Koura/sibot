#pragma once
#include <fstream>
#include <Eigen\Dense>
#include <BWAPI.h>

class QTable
{
public:
	QTable();
	~QTable();
	void initialize(int depth, int width);
	void shutdown();
	double getValue(int i, int j);
	int maxAction(int i);
	double maxStateValue(int i);
	int getColumns();
	int getRows();
	void updateTable(int i, int j, int r);

private:
	void initializeTable();
	bool saveToFile(std::string name);
	bool loadFromFile(std::string name);
	bool is_empty(std::ifstream& file);
	Eigen::MatrixXd m_matrix;
};
