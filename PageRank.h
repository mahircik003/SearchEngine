#pragma once

#include <iostream>
#include <C:\Users\Mahir\Desktop\libraries\eigen-3.4.0\Eigen\Dense>

#define THRESHOLD 0.0000000000001
#define BETA 0.85

Eigen::MatrixXd readMatrixFromFile(const std::string &filename)
{
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return Eigen::MatrixXd();
    }

    // Read the matrix from the file
    std::vector<std::vector<int>> matrix;
    std::string line;

    while (std::getline(inputFile, line))
    {
        std::vector<int> row;
        std::istringstream iss(line);

        int value;
        while (iss >> value)
        {
            row.push_back(value);
        }

        matrix.push_back(row);
    }

    inputFile.close();

    // Determine the number of columns
    size_t numCols = matrix.empty() ? 0 : matrix[0].size();

    // Check if the matrix is empty or has inconsistent column counts
    if (matrix.empty() || std::any_of(matrix.begin(), matrix.end(),
                                      [numCols](const std::vector<int> &row)
                                      { return row.size() != numCols; }))
    {
        std::cerr << "Invalid matrix dimensions or inconsistent column counts." << std::endl;
        return Eigen::MatrixXd();
    }

    // Convert the vector of vectors to an Eigen matrix
    Eigen::MatrixXd eigenMatrix(matrix.size(), numCols);

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < numCols; ++j)
        {
            eigenMatrix(i, j) = matrix[i][j];
        }
    }

    return eigenMatrix;
}

void changeZerosToOnesInColumn(Eigen::MatrixXd &matrix)
{
    for (int j = 0; j < matrix.cols(); ++j)
    {
        bool allZeros = true;

        for (int i = 0; i < matrix.rows(); ++i)
        {
            if (matrix(i, j) != 0)
            {
                allZeros = false;
                break;
            }
        }

        if (allZeros)
        {
            // Change all zeros to ones in the column
            for (int i = 0; i < matrix.rows(); ++i)
            {
                matrix(i, j) = 1;
            }
        }
    }
}

Eigen::MatrixXd page_ranking(const std::string &filename)
{
    Eigen::MatrixXd A = readMatrixFromFile(filename);

    size_t m=A.cols();

    for (int j = 0; j < m; ++j)
    {
        bool allZeros = true;

        for (int i = 0; i < m; ++i)
        {
            if (A(i, j) != 0)
            {
                allZeros = false;
                break;
            }
        }

        if (allZeros)
        {
            // Change all zeros to ones in the column
            for (int i = 0; i < m; ++i)
            {
                if(i!=j)
                    A(i, j) = 1;
            }
        }
    }


    for (int i = 0; i < m; ++i)
    {
        if(A.col(i).sum()==0)
            continue;
        A.col(i) /= A.col(i).sum();
    }

    Eigen::MatrixXd r = Eigen::MatrixXd::Ones(m, 1);

    for (int i = 0; i < m; ++i)
    {
        r.row(i) /= m;
    }

    Eigen::MatrixXd uniformR(1, m);

    uniformR = (1.0 - BETA) * r;

    Eigen::MatrixXd r_prev = r;

    for (int i = 0; ; i++)
    {
        r = BETA * A * r_prev + uniformR;

        Eigen::MatrixXd diff = (r - r_prev);
        diff = diff.cwiseAbs();

        double total = diff.sum();

        if (total < THRESHOLD)
            break;

        r_prev = r;
    }

    return r;
}