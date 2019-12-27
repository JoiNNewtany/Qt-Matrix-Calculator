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

bool Widget::isValueCorrect(QString value)
{
    // Check if a value is a number

    QRegularExpression regex("^-?[0-9]\\d*(\\.\\d+)?$");
    return regex.match(value).hasMatch();
}

QVector<QSquareMatrix<double> > Widget::Parse(QString fileName)
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
    /* If a cell is changed, check if an index in
       the selectionBox is selected and if new data is correct.
       If so, overwrite that matrix in the global list of matrices. */
}

void Widget::on_calculateButton_clicked()
{
    // Mathematical action selection

    switch (ui->mathBox->currentIndex())
    {
    case 0: // Addition (+)

        break;
    case 1: // Subtraction (-)

        break;
    case 2: // Multiplication (*)

        break;
    case 3: // Inversion

        break;
    case 4: // Transposition

        break;
    }
}

void Widget::on_importButton_clicked()
{
    // Importing a matrix list from text file

    QString fileName = ui->importPathBox->text();

    if (fileName.length() > 0)
    {
        QFileInfo fileCheck(fileName);

        // Check if file exists and wether it's actually a file
        if (fileCheck.exists() && fileCheck.isFile())
        {
            matrixList = Parse(fileName);
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
            for (int i = 0; i < matrixList.size(); i++)
            {
                ui->selectionBox1->addItem("Matrix " + QString::number(i + 1));
                ui->selectionBox2->addItem("Matrix " + QString::number(i + 1));
            }
        }

    }
}
