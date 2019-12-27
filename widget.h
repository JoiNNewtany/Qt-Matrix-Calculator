#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "QSquareMatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // Changes size of a matrix table and Fills it with empty elements
    void fillMatrixTable(QTableWidget *matrixTable, int rows, int columns);

    // Checks if a given value is correct for writing to a table
    bool isValueCorrect(QString value);

    // Parses a text file with matrices to a vector of QSquareMatrix
    QVector<QSquareMatrix<double>> Parse(QString fileName);

private slots:
    void on_sizeBox_valueChanged(int arg1);

    void on_matrixTable1_cellChanged(int row, int column);

    void on_calculateButton_clicked();

    void on_importButton_clicked();

private:
    Ui::Widget *ui;

    // MatrixList is the global vector of imported matrices.
    // TableList will consist of three matrices from matrix tables
    // where 0 is first matrix, 1 is second and 2 is result matrix.
    QVector<QSquareMatrix<double>> matrixList, tableList;
};
#endif // WIDGET_H
