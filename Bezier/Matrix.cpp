#include "Matrix.h"

Matrix::Matrix() : row(4), col(4)
{
	data.clear();
	data.resize(row * col);
	data = identity(*this);
}

Matrix::Matrix(size_t rows, size_t cols)
{
	row = rows;
	col = cols;
	data.clear();
	data.resize(row * col);
	data = identity(*this);
}

Matrix::Matrix(vector<double>& const d, size_t rows, size_t cols)
{
	row = rows;
	col = cols;
	data.clear();
	data.resize(rows * cols);
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			data[i * col + j] = d[i * col + j];
		}
	}
}

Matrix::Matrix(Matrix& const m)
{
	row = m.getRow();
	col = m.getCol();
	data.clear();
	data.resize(getData(m).size());
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			data[i * col + j] = m(i, j);
		}
	}
}

Matrix::~Matrix()
{
	data.clear();
}

void Matrix::setRow(size_t const rows)
{
	row = rows;
	data.clear();
	data.resize(row * col);
	data = identity(*this);
}

void Matrix::setCol(size_t const cols)
{
	col = cols;
	data.clear();
	data.resize(row * col);
	data = identity(*this);
}

Matrix Matrix::Translate(float x, float y)
{
	Matrix t(2, 3);
	t(0, 2) = x;
	t(1, 2) = y;
	return t;
}

Matrix Matrix::Rotate(float angle)
{
	float DegToRad = 0.01745328888889;

	Matrix r(2, 3);
	r(0, 0) = cos(angle * DegToRad);
	r(0, 1) = -sin(angle * DegToRad);
	r(1, 0) = sin(angle * DegToRad);
	r(1, 1) = cos(angle * DegToRad);
	return r;
}

Matrix Matrix::Scale(float scale)
{
	Matrix s(2, 3);
	s(0, 0) = scale;
	s(1, 1) = scale;
	return s;
}

vector<double> Matrix::identity(Matrix& const m)
{
	for (int i = 0; i < m.getRow(); ++i)
	{
		for (int j = 0; j < m.getCol(); ++j)
		{
			m(i, j) = i == j ? 1 : 0;
		}
	}
	return getData(m);
}

void Matrix::swap(Matrix& m) {
	std::swap(data, m.data);
	std::swap(row, m.row);
	std::swap(col, m.col);
}

double& Matrix::operator()(size_t i, size_t j)
{
	return data[i * col + j];
}

double Matrix::operator()(size_t i, size_t j) const
{
	return data[i * col + j];
}

Matrix& Matrix::operator*=(Matrix& const m)
{
	if (col != m.getRow())
	{
		cout << "Can't operator * between these matrix : " << endl << m << endl << *this << endl << endl;
		return *this;
	}

	Matrix result = Matrix(data, row, col);
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			result(i, j) = 0;
			for (int k = 0; k < col; ++k)
			{
				result(i, j) += data[i * col + k] * m(k, j);
			}
		}
	}
	return result;
}

Matrix& Matrix::operator+=(Matrix& const m)
{
	if (col != m.getRow())
	{
		cout << "Can't operator + between these matrix : " << endl << m << endl << *this << endl << endl;
		return *this;
	}

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			data[i * col + j] += m(i, j);
		}
	}
	return *this;
}

Matrix Matrix::operator-=(Matrix& const m)
{
	if (col != m.getRow())
	{
		cout << "Can't operator - between these matrix : " << endl << m << endl << *this << endl << endl;
		return *this;
	}

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			data[i * col + j] -= m(i, j);
		}
	}
	return *this;
}

Matrix& Matrix::operator=(Matrix& const m)
{
	Matrix tmp = m;
	swap(tmp);
	return *this;
}

Vertex Matrix::operator*(Vertex v)
{
	Vertex newVertex(Color(v.r, v.g, v.b));
	newVertex.x = this->data[0] * v.x + this->data[1] * v.y + this->data[2] * 1;
	newVertex.y = this->data[3] * v.x + this->data[4] * v.y + this->data[5] * 1;
	return newVertex;
}

Matrix operator*(Matrix& const m, Matrix& const m2)
{
	Matrix result = m;
	return result *= m2;
}

Matrix operator+(Matrix& const m, Matrix& const m2)
{
	Matrix result = m;
	return result += m2;
}

Matrix operator-(Matrix& const m, Matrix& const m2)
{
	Matrix result = m;
	return result -= m2;
}

ostream& operator<<(ostream& os, Matrix& m)
{
	for (int i = 0; i < m.row; i++)
	{
		for (int j = 0; j < m.col; j++)
			cout << m(i, j) << "  ";
		cout << endl;
	}
	return os;
}
