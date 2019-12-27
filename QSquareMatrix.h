#ifndef SQUAREMATRIX_H
#define SQUAREMATRIX_H

#include <iostream>
#include <math.h>

//Qt
#include "QVector"
#include "QFile"
#include "QTextStream"
#include "QDebug"

using namespace std;

template <class T>
class QSquareMatrix
{
    public:

        QSquareMatrix();
        QSquareMatrix(std::initializer_list<std::initializer_list<T>>);
        ~QSquareMatrix();

        void debug_print(){ qDebug() << matrix; }

        float determinant(); // https://codereview.stackexchange.com/a/200624

        int size(){ return matrix.size(); }

        void append(QVector<T> vector){ matrix.append(vector); }

        void clear(){ matrix.clear(); }

        QVector<QVector<T>> getQVectorMatrix(){ return matrix; }
        void getMatrixFromQVector(QVector<QVector<T>> _matrix){ *this->matrix = _matrix; }

        void transpose()
        {
            QSquareMatrix<T> temp;
            temp.resize(matrix.size());

            for(int i = 0; i < matrix.size(); i++)
                for(int j = 0; j < matrix.size(); j++)
                {
                    temp.matrix[j][i] = matrix[i][j];
                }
            matrix = temp.matrix;
        }

        void resize(const int &size)
        {
            matrix.clear();
            QVector<T> temp;

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    temp.append((T)0);
                }
                matrix.append(temp);
                temp.clear();
            }
        }

        QVector<T>& operator[](int nSubscript)
        {
            if( nSubscript >= 0 && nSubscript < matrix.size() )
            {
                return matrix[nSubscript];
            }
            else
            {
                throw std::out_of_range("Index out of range");
                QVector<T> err;
                return err;
            }
        }

        QSquareMatrix operator  = (const QSquareMatrix &value);
        QSquareMatrix operator += (const QSquareMatrix &value);
        QSquareMatrix operator += (const T &value);
        QSquareMatrix operator -= (const QSquareMatrix &value);
        QSquareMatrix operator -= (const T &value);
        QSquareMatrix operator /= (const T &value);
        QSquareMatrix operator *= (const QSquareMatrix &value);
        QSquareMatrix operator *= (const T &value);

        QSquareMatrix operator + (const QSquareMatrix &value)
        {
            QSquareMatrix temp;
            temp = *this += value;
            return temp;
        }

        QSquareMatrix operator + (const T &value)
        {
            QSquareMatrix temp;
            temp = *this += value;
            return temp;
        }

        QSquareMatrix operator - (const QSquareMatrix &value)
        {
            QSquareMatrix temp;
            temp = *this -= value;
            return temp;
        }

        QSquareMatrix operator - (const T &value)
        {
            QVector<QVector<T>> temp;
            temp = *this -= value;
            return temp;
        }

        QSquareMatrix operator / (const T &value)
        {
            QSquareMatrix temp;
            temp = *this /= value;
            return temp;
        }

        QSquareMatrix operator * (const QSquareMatrix &value)
        {
            QSquareMatrix temp;
            temp = *this *= value;
            return temp;
        }

        QSquareMatrix operator * (const T &value)
        {
            QSquareMatrix temp;
            temp = *this *= value;
            return temp;
        }

    private:
        QVector<QVector<T>> matrix;
};

template <class T>
QSquareMatrix<T>::QSquareMatrix()
{

}

template <typename T>
QSquareMatrix<T>::QSquareMatrix(std::initializer_list<std::initializer_list<T>> listlist)
{
    int rows = (int)(listlist.begin())->size();
    int cols = (int)listlist.size();
    QVector<T> temp;

    if (rows == cols)
    {
        int size = rows;

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                temp.append( ((listlist.begin()+i)->begin())[j] );
            }

            matrix.append(temp);
            temp.clear();
        }
    }
    else
    {
        throw std::invalid_argument("Non-square matrix given");
    }
}

template <class T>
QSquareMatrix<T>::~QSquareMatrix()
{

}

template<class T>
float QSquareMatrix<T>::determinant()
{
    float det = 0.; // the determinant value will be stored here

    if (matrix.size() == 1)
    {
        return matrix[0][0]; // no calculation needed
    }
    else if (matrix.size() == 2)
    {
        //in this case we calculate the determinant of a 2-dimensional matrix in a
        //default procedure
        det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
        return det;
    }
    else
    {
        //in this case we calculate the determinant of a squared matrix that have
        // for example 3x3 order greater than 2
        for (int p = 0; p < matrix[0].size(); p++)
        {
            //this loop iterate on each elements of the first row in the matrix.
            //at each element we cancel the row and column it exist in
            //and form a matrix from the rest of the elements in the matrix
            vector<vector<T>> TempMatrix; // to hold the shaped matrix;
            for (int i = 1; i < matrix.size(); i++)
            {
                // iteration will start from row one cancelling the first row values
                vector<T> TempRow;
                for (int j = 0; j < matrix[i].size(); j++)
                {
                    // iteration will pass all cells of the i row excluding the j
                    //value that match p column
                    if (j != p)
                    {
                       TempRow.push_back(matrix[i][j]);//add current cell to TempRow
                    }
                }
                if (TempRow.size() > 0)
                    TempMatrix.push_back(TempRow);
                //after adding each row of the new matrix to the vector tempx
                //we add it to the vector temp which is the vector where the new
                //matrix will be formed
            }
            det = det + matrix[0][p] * pow(-1, p) * CalcDeterminant(TempMatrix);
            //then we calculate the value of determinant by using a recursive way
            //where we re-call the function by passing to it the new formed matrix
            //we keep doing this until we get our determinant
        }
        return det;
    }
}

// Operators

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator = (const QSquareMatrix &value)
{
    matrix = value.matrix;

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator += (const QSquareMatrix &value)
{
    if (matrix.size() == value.matrix.size())
    {
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                matrix[i][j] += value.matrix[i][j];
            }
        }
    }

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator -= (const QSquareMatrix &value)
{
    if (matrix.size() == value.matrix.size())
    {
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                matrix[i][j] -= value.matrix[i][j];
            }
        }
    }

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator *= (const QSquareMatrix &value)
{
    QSquareMatrix<T> temp;
    temp.resize(matrix.size());

    for(int i = 0; i < matrix.size(); i++)
    {
        for(int j = 0; j < value.matrix.size(); j++)
        {
            for(int k = 0; k < matrix.size(); k++)
            {
                temp.matrix[i][j] += matrix[i][k] * value.matrix[k][j];
            }
        }
    }

    *this = temp;

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator += (const T &value)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            matrix[i][j] += value;
        }
    }

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator -= (const T &value)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            matrix[i][j] -= value;
        }
    }

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator /= (const T &value)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            matrix[i][j] /= value;
        }
    }

    return *this;
}

template <class T>
QSquareMatrix<T> QSquareMatrix<T>::operator *= (const T &value)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            matrix[i][j] *= value;
        }
    }

    return *this;
}

#endif
