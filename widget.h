#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>

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

    // Checks if a given value is correct for writing to table
    bool isValueCorrect(QString value);

private slots:
    void on_sizeBox_valueChanged(int arg1);

    void on_matrixTable1_cellChanged(int row, int column);

private:
    Ui::Widget *ui;
    // QVector<QSquareMatrix> matrixList, tableList;
    // MatrixList is the global vector of imported matrices.
    // TableList will consist of three matrices from matrix tables
    // where 0 is first matrix, 1 is second and 2 is result matrix.
};
#endif // WIDGET_H
