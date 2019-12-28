#include "widget.h"
#include "ui_widget.h"

#include "QSquareMatrix.h" // Custom class
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

    // Initialize the tables

    fillMatrixTable(ui->matrixTable1, 1, 1);
    fillMatrixTable(ui->matrixTable2, 1, 1);
    fillMatrixTable(ui->matrixTable3, 1, 1);
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

QVector<QSquareMatrix<double>> Widget::Parse(QString fileName)
{
    QFile file(fileName);
    QVector<QSquareMatrix<double>> result;

        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QSquareMatrix<double> global, local;

            if(0 == file.size())
            {
                qDebug() << "Error - Empty file given.";
                return result;
            }

            while(!in.atEnd())
            {
                QString string = in.readLine();
                QStringList string_list = string.split(" ", QString::SkipEmptyParts);
                QVector<double> row;

                foreach (auto string, string_list)
                {
                    row.append(string.toDouble());
                }

                global.append(row);
            }

            int row_size = global[0].size();

            for (int i = 0; i < global.size(); i++)
            {
                if (local.size() == row_size)
                {
                    result.append(local);
                    local.clear();
                    row_size = global[i].size();
                }
                if ((global[i].size() == row_size) &&
                    (local.size() < row_size))
                {
                    local.append(global[i]);
                    row_size = global[i].size();
                }
            }

            if (local.size() == row_size)
                result.append(local);

            file.close();
        }

    return result;
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
    // When a cell has been changed, check if new value is a number
    // then if an imported matrix is present, change it's value
    // if value is NaN, change it to 0

    QRegularExpression regex("^-?[0-9]\\d*(\\.\\d+)?$");
    QString value = ui->matrixTable1->item(row, column)->text();

    if (regex.match(value).hasMatch())
    {
        if (ui->selectionBox1->currentIndex() >= 0 &&
            matrixList.size() > 0)
        {
            matrixList[ui->selectionBox1->currentIndex()][row][column] =
                    ui->matrixTable1->item(row, column)->text().toDouble();
        }
    }
    else
    {
        //ui->matrixTable1->item(row, column)->setText("0");
    }
}

void Widget::on_matrixTable2_cellChanged(int row, int column)
{
    // Value check, same as on_matrixTable1_cellChanged

    QRegularExpression regex("^-?[0-9]\\d*(\\.\\d+)?$");
    QString value = ui->matrixTable2->item(row, column)->text();

    if (regex.match(value).hasMatch())
    {
        if (ui->selectionBox2->currentIndex() >= 0 &&
            matrixList.size() > 0)
        {
            matrixList[ui->selectionBox2->currentIndex()][row][column] =
                    ui->matrixTable2->item(row, column)->text().toDouble();
        }
    }
    else
    {
        //ui->matrixTable2->item(row, column)->setText("0");
    }
}

void Widget::on_calculateButton_clicked()
{
    // Mathematical action selection

    // If matrices are of different size
    if (ui->matrixTable1->rowCount() != ui->matrixTable2->rowCount() ||
        ui->matrixTable1->columnCount() != ui->matrixTable2->columnCount())
    {
        qDebug() << "CalculationError: Matrices have different dimensions.";
        return;
    }

    ui->matrixTable3->clear();
    fillMatrixTable(ui->matrixTable3,
                    ui->matrixTable1->rowCount(),
                    ui->matrixTable1->columnCount());

    QSquareMatrix<double> matrix1, matrix2, result;

    matrix1.resize(ui->matrixTable1->rowCount());
    matrix2.resize(ui->matrixTable1->rowCount());
    result.resize(ui->matrixTable1->rowCount());

    for (int i = 0; i < ui->matrixTable1->rowCount(); i++)
    {
        for (int j = 0; j < ui->matrixTable1->columnCount(); j++)
        {
            if (ui->matrixTable1->item(i, j)->text().length() > 0)
                matrix1[i][j] = ui->matrixTable1->item(i, j)->text().toDouble();
            else
            {
                qDebug() << "CalculationError: One of the elements in matrix 1 was empty.";
                return;
            }
        }
    }

    for (int i = 0; i < ui->matrixTable2->rowCount(); i++)
    {
        for (int j = 0; j < ui->matrixTable2->columnCount(); j++)
        {
            if (ui->matrixTable2->item(i, j)->text().length() > 0)
                matrix2[i][j] = ui->matrixTable2->item(i, j)->text().toDouble();
            else
            {
                qDebug() << "CalculationError: One of the elements in matrix 2 was empty.";
                return;
            }
        }
    }

    switch (ui->mathBox->currentIndex())
    {
    case 0: // Addition (+)

        result = matrix1 + matrix2;
        qDebug() << "CalculationLog: Addition triggered.";

        break;
    case 1: // Subtraction (-)

        result = matrix1 - matrix2;
        qDebug() << "CalculationLog: Subtraction triggered.";

        break;
    case 2: // Multiplication (*)

        result = matrix1 * matrix2;
        qDebug() << "CalculationLog: Multiplication triggered.";

        break;
    case 3: // Inversion


        qDebug() << "CalculationLog: Inversion triggered.";

        break;
    case 4: // Transposition

        result = matrix1;
        result.transpose();
        qDebug() << "CalculationLog: Transposition triggered.";

        break;

    case 5: // Determinant

        result[0][0] = QSquareMatrix<double>::determinant(matrix1.getQVectorMatrix());
        qDebug() << "CalculationLog: Determinant triggered.";

        break;
    }

    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result.size(); j++)
        {
            ui->matrixTable3->item(i, j)->setText(QString::number(result[i][j]));
        }
    }

    qDebug() << "CalculationLog: Calculation finished with resulting matrix:";
    result.debug_print();
}

void Widget::on_importButton_clicked()
{
    // Importing a matrix list from text file

    QString fileName = ui->importPathBox->text();

    QFileInfo fileCheck(fileName);

    // Check if file exists and wether it's actually a file
    if (fileCheck.exists() && fileCheck.isFile())
    {
        //matrixList.clear();
        matrixList += Parse(fileName);
    }
    else
    {
        fileName = QFileDialog::getOpenFileName(this,"Open text file",
                                            QApplication::applicationDirPath(), "*.txt");
        ui->importPathBox->setText(fileName);
        matrixList = Parse(fileName);
    }

    // Fill selection boxes if matrix wasn't loaded empty
    if (matrixList.size())
    {
        ui->selectionBox1->clear();
        ui->selectionBox2->clear();

        for (int i = 0; i < matrixList.size(); i++)
        {
            ui->selectionBox1->addItem("Matrix " + QString::number(i + 1));
            ui->selectionBox2->addItem("Matrix " + QString::number(i + 1));
        }
    }
}

void Widget::on_selectionBox1_currentIndexChanged(int index)
{
    // Matrix loading for selection box 1

    if (index >= 0) // In case selection box is being cleared
    {
        ui->matrixTable1->clear();

        if(matrixList.size())
        {
            fillMatrixTable(ui->matrixTable1,
                            matrixList[index].size(),
                            matrixList[index].size());
            //ui->matrixTable1->setColumnCount(matrixList[index].size());
            //ui->matrixTable1->setRowCount(matrixList[index].size());

            for(int i = 0; i < matrixList[index].size(); i++)
            {
                for(int j = 0; j < matrixList[index][i].size(); j++)
                {
                    //QTableWidgetItem *item = new QTableWidgetItem;
                    //item->setText(QString::number(matrixList[index][i][j]));
                    //ui->matrixTable1->setItem(i, j, item);
                    ui->matrixTable1->item(i, j)->setText(QString::number(matrixList[index][i][j]));
                }
            }
        }
    }
}

void Widget::on_selectionBox2_currentIndexChanged(int index)
{
    // Matrix loading for selection box 2

    if (index >= 0) // In case selection box is being cleared
    {
        ui->matrixTable2->clear();

        if(matrixList.size())
        {
            fillMatrixTable(ui->matrixTable2,
                            matrixList[index].size(),
                            matrixList[index].size());
            //ui->matrixTable2->setColumnCount(matrixList[index].size());
            //ui->matrixTable2->setRowCount(matrixList[index].size());

            for(int i = 0; i < matrixList[index].size(); i++)
            {
                for(int j = 0; j < matrixList[index][i].size(); j++)
                {
                    //QTableWidgetItem *item = new QTableWidgetItem;
                    //item->setText(QString::number(matrixList[index][i][j]));
                    //ui->matrixTable2->setItem(i, j, item);
                    ui->matrixTable2->item(i, j)->setText(QString::number(matrixList[index][i][j]));
                }
            }
        }
    }
}

void Widget::on_exportButton_clicked()
{
    QString dir = QFileDialog::getSaveFileName(this,"Export Location",
                                               QApplication::applicationDirPath(), "*.txt");
    QFile file(dir);

    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        for (int i = 0; i < ui->matrixTable3->rowCount(); i++)
        {
            for (int j = 0; j < ui->matrixTable3->columnCount(); j++)
            {
                stream << ui->matrixTable3->item(i, j)->text() << " ";
            }
            stream << endl;
        }
    }
    else
    {
        qDebug() << "ExportError: Unable to write to file.";
    }
}

