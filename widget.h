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

    // Parses a text file with matrices to a vector of QSquareMatrix
    QVector<QSquareMatrix<double>> Parse(QString fileName);

private slots:
    void on_sizeBox_valueChanged(int arg1);

    void on_matrixTable1_cellChanged(int row, int column);

    void on_calculateButton_clicked();

    void on_importButton_clicked();

    void on_selectionBox1_currentIndexChanged(int index);

    void on_selectionBox2_currentIndexChanged(int index);

private:
    Ui::Widget *ui;

    // Global vector of imported matrices.
    QVector<QSquareMatrix<double>> matrixList;
};
#endif // WIDGET_H
