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
	void updateTable(int i, int j, double r);

private:
	void initializeTable(int depth, int width);
	bool saveToFile(std::string name);
	bool loadFromFile(std::string name, int depth);
	bool is_empty(std::ifstream& file);
	Eigen::MatrixXd m_matrix;
};
