#include "Matrix.h"
#define M_PI 3.14159265
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

Matrix Matrix::Translate(float x, float y, float z)
{
	Matrix t(4, 4);
	t(3, 0) = x;
	t(3, 1) = y;
	t(3, 2) = z;
	return t;
}

Matrix Matrix::RotateZ(float angle)
{
	float DegToRad = 0.01745328888889;

	Matrix r(4, 4);
	r(0, 0) = cos(angle * DegToRad);
	r(0, 1) = -sin(angle * DegToRad);
	r(1, 0) = sin(angle * DegToRad);
	r(1, 1) = cos(angle * DegToRad);
	return r;
}

Matrix Matrix::RotateX(float angle)
{
	float DegToRad = 0.01745328888889;

	Matrix r(4, 4);
	r(1, 1) = cos(angle * DegToRad);
	r(1, 2) = -sin(angle * DegToRad);
	r(2, 1) = sin(angle * DegToRad);
	r(2, 2) = cos(angle * DegToRad);
	return r;
}

Matrix Matrix::RotateY(float angle)
{
	float DegToRad = 0.01745328888889;

	Matrix r(4, 4);
	r(0, 0) = cos(angle * DegToRad);
	r(0, 2) = sin(angle * DegToRad);
	r(2, 0) = -sin(angle * DegToRad);
	r(2, 2) = cos(angle * DegToRad);
	return r;
}

Matrix Matrix::Scale(float scale)
{
	Matrix s(4, 4);
	s(0, 0) = scale;
	s(1, 1) = scale;
	s(2, 2) = scale;
	return s;
}

Matrix Matrix::Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	vector<double> data = { 2 / (right - left), 0, 0, -(right + left) / (right - left),
							0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
							0, 0, -2 / (zfar - znear), -(zfar + znear) / (zfar - znear),
							0, 0, 0, 1 };
	Matrix orthoMat(data, 4, 4);

	return orthoMat;
}

Matrix Matrix::Perspective(float FoV, float aspectRatio, float n, float f)
{
	float fovRad = tan((FoV * M_PI / 180) * 0.5);

	vector<double> data = { 1 / (fovRad * aspectRatio), 0, 0, 0,
							0, 1 / fovRad, 0, 0,
							0, 0, -(f + n) / (f - n), -(2 * f * n) / (f - n),
							0, 0, -1, 0 };

	Matrix mat(data, 4, 4);

	return mat;
}

Matrix Matrix::LookAt(Vec3 position, Vec3 target, Vec3 up)
{
	Vec3 forward = -(target - position).normalise();
	Vec3 right = (up ^ forward).normalise();
	up = right ^ forward;

	vector<double> data = { right.x, right.y, right.z, -right.dot(position),
							up.x, up.y, up.z, -up.dot(position),
							forward.x, forward.y, forward.z, -forward.dot(position),
							0, 0, 0, 1 };
	Matrix mat(data, 4, 4);

	return mat;
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
	newVertex.x = this->data[0] * v.x + this->data[4] * v.y + this->data[8] * v.z + this->data[12];
	newVertex.y = this->data[1] * v.x + this->data[5] * v.y + this->data[9] * v.z + this->data[13];
	newVertex.z = this->data[2] * v.x + this->data[6] * v.y + this->data[10] * v.z + this->data[14];
	return newVertex;
}

Matrix operator*(Matrix& const m, Matrix& const m2)
{
	Matrix res;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			res.data[j * 4 + i] = 0;

			for (int k = 0; k < 4; ++k)
			{
				res.data[j * 4 + i] += m.data[k * 4 + i] * m2.data[j * 4 + k];
			}
		}
	}

	return res;
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
