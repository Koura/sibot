#include "QTable.h"

QTable::QTable()
{
}

QTable::~QTable()
{
}

void QTable::initialize(int depth, int width)
{
	if(!loadFromFile("qtable.txt"))
	{
		BWAPI::Broodwar->sendText("initialized new table");
		initializeTable();
	}
	else
	{
		BWAPI::Broodwar->sendText("File found not empty and opened");
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

bool QTable::loadFromFile(std::string name)
{
	std::ifstream file(name.c_str());
	if(!is_empty(file))
	{
		if(file.is_open())
		{
			initializeTable();
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
				m_matrix(i, j) = d;
				j++;
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

void QTable::initializeTable()
{
	m_matrix = Eigen::MatrixXd::Zero(96, 2);
}

double QTable::getValue(int i, int j)
{
	return m_matrix(i,j);
}

int QTable::maxAction(int i)
{
	int max = 0;
	for(int j = 1; j < m_matrix.cols(); j++)
	{
		if(m_matrix(i,max) < m_matrix(i,j))
		{
			max = j;
		}
	}
	return max;
}

double QTable::maxStateValue(int i)
{
	double max = m_matrix(i, 0);
	for(int j = 1; j < m_matrix.cols(); j++)
	{
		double currentAction; 
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