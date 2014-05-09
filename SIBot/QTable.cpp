#include "QTable.h"

QTable::QTable()
{
}

QTable::~QTable()
{
}

void QTable::initialize(int depth, int width)
{
	initializeTable(depth, width);
	if(!loadFromFile("qtable.txt", depth))
	{
		BWAPI::Broodwar->sendText("initialized new table");
	}
	else
	{
		if(getRows()!=depth)
		{
			initializeTable(depth, width);
		}
		BWAPI::Broodwar->sendText("File found, not empty and opened");
	}
}

void QTable::shutdown()
{
	saveToFile("qtable.txt");
}

bool QTable::saveToFile(std::string name)
{
	std::ofstream file(name.c_str());
	if(file.is_open())
	{
		file << m_matrix;
		file.close();
		return true;
	}
	return false;
}

bool QTable::loadFromFile(std::string name, int depth)
{
	std::ifstream file(name.c_str());
	if(!is_empty(file))
	{
		if(file.is_open())
		{
			int i = 0;
			int j = 0;
			double d;
			while(file >> d)
			{
				if(!file)
				{
					return false;
				}
				if(j == 2)
				{
					j = 0;
					i++;
				}
				if(i<depth)
				{
					m_matrix(i, j) = d;
					j++;
				}
			}
			file.close();
			return true;
		}
		
	}
	return false;
}

bool QTable::is_empty(std::ifstream& file)
{
	return file.peek() == std::ifstream::traits_type::eof();
}

void QTable::initializeTable(int depth, int width)
{
	m_matrix = Eigen::MatrixXd::Zero(depth, width);
}

double QTable::getValue(int i, int j)
{
	return m_matrix(i,j);
}

int QTable::maxAction(int i, bool cd)
{
	int max = 0;
	int until = m_matrix.cols();
	if(cd) 
	{
		until--;
	}
	for(int j = 1; j < until; j++)
	{
		if(m_matrix(i,max) < m_matrix(i,j))
		{
			max = j;
		}
	}
	return max;
}

double QTable::maxStateValue(int i, bool cd)
{
	double max = m_matrix(i, 0);
	int until = m_matrix.cols();
	if(cd)
	{
		until--;
	}
	for(int j = 1; j < until; j++)
	{
		double currentAction = m_matrix(i, j);
		if(max < currentAction)
		{
			max = currentAction;
		}
	}
	return max;
}

int QTable::getColumns()
{
	return m_matrix.cols();
}

int QTable::getRows()
{
	return m_matrix.rows();
}

void QTable::updateTable(int i, int j, double r)
{
	m_matrix(i,j) = r;
}