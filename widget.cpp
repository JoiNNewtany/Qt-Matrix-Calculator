#include "widget.h"
#include "ui_widget.h"

#include "QFileDialog"
#include "QVector"
#include "QFile"
#include "QTextStream"
#include "QDebug"
#include "QTableWidget"
#include "QTableWidgetItem"
#include "QRegularExpression"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::fillMatrixTable(QTableWidget *matrixTable, int rows, int columns)
{
    // Set rows and columns for the table

    matrixTable->setRowCount(rows);
    matrixTable->setColumnCount(columns);

    // Fill them with empty items

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrixTable->setItem(i, j, new QTableWidgetItem);
        }
    }
}

bool Widget::isValueCorrect(QString value)
{
    // Check if a value is a number

    QRegularExpression regex("^-?[0-9]\\d*(\\.\\d+)?$");
    return regex.match(value).hasMatch();
}


// Slots //


void Widget::on_sizeBox_valueChanged(int arg1)
{
    // Change size of all matrices to arg1

    fillMatrixTable(ui->matrixTable1, arg1, arg1);
    fillMatrixTable(ui->matrixTable2, arg1, arg1);
    fillMatrixTable(ui->matrixTable3, arg1, arg1);
}

void Widget::on_matrixTable1_cellChanged(int row, int column)
{
    /* If a cell is changed, check if an index in
       the selectionBox is selected and if new data is correct.
       If so, overwrite that matrix in the global list of matrices. */
}
