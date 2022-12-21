//
// Created by Ian Ferenc Diks on 20.12.22.
//

#ifndef GENETRAILCORE_DENSEMATRIX_H
#define GENETRAILCORE_DENSEMATRIX_H

#include "macros.h"

#include "AbstractMatrix.h"

#include <Eigen/Core>

#include <map>
#include <vector>

namespace GeneTrail::Core {
        class GENETRAILCORE_EXPORT DenseMatrix : public AbstractMatrix {
        public:
            typedef Eigen::Matrix<value_type, Eigen::Dynamic, Eigen::Dynamic> DMatrix;
            typedef Eigen::Matrix<value_type, Eigen::Dynamic, 1> Vector;

            /**
         * Default constructor
         * Reserves the storage for a rows * cols matrix
         * The matrix will not be initialised.
         *
         * @param rows
         * @param cols
         */
            DenseMatrix(index_type rows, index_type cols);

            /**
        * Row-name constructor
        *
        * This constructs a rows.size() x cols.size() matrix and sets the row and column names
        * to rows and cols respectively
        */
            DenseMatrix(std::vector<std::string> rows, std::vector<std::string> cols);

            /**
         * Default copy constructor
         */
            DenseMatrix(const DenseMatrix &) = default;

            /**
         * Move constructor
         * @param matrix
         */
            DenseMatrix(DenseMatrix &&matrix);

            /**
         * Return the i-th row
         * @param i
         * @return
         */
            DMatrix::RowXpr row(index_type i);

            /**
         * Return the i-th row, const version
         * @param i
         * @return
         */
            DMatrix::ConstRowXpr row(index_type i) const;

            /**
         * Set the row identified by "name" to v
         * This method is a noop if "name" is not present in the matrix
         * @param name
         * @param v
         */
            void setRow(const std::string &name, const Vector &v);

            /**
         * Set the i-th row to v
         * @param i
         * @param v
         */
            void setRow(index_type i, const Vector &v);

            void rbind(const DenseMatrix &m);

            void cbind(const DenseMatrix &m);

            /**
         * Return the j-th column
         * @param j
         * @return
         */
            DMatrix::ColXpr col(index_type j);

            /**
         * Return the j-th column
         * @param j
         * @return
         */
            DMatrix::ConstColXpr col(index_type j) const;

            /**
         * Set the column identified by "name" to v
         * @param name
         * @param v
         */
            void setCol(const std::string &name, const Vector &v);

            /**
         * Set the j-th column to v
         * @param j
         * @param v
         */
            void setCol(index_type j, const Vector &v);

            /**
         * Return a reference to the internal Eigen matrix
         * @return
         */
            DMatrix &matrix();

            /**
         * Return a reference to the internal Eigen matrix
         * @return
         */
            const DMatrix &matrix() const;

            /**
         * Return a reference to the matrix coefficient at position (i,j)
         * @param i
         * @param j
         * @return
         */
            value_type &operator()(index_type i, index_type j) override { return m_(i, j); }

            /**
         * Return the matrix coefficient at position (i,j)
         * @param i
         * @param j
         * @return
         */
            value_type operator()(index_type i, index_type j) const override { return m_(i, j); }

            /**
         * Move assignment operator
         * @param matrix
         * @return
         */
            DenseMatrix &operator=(DenseMatrix &&matrix);

            /**
         * Reorder the rows of the matrix according to a given permutation.
         *
         * @param perm A vector of matrix indices. The new matrix will consist of the rows at the indices contained in perm in the order they were specified.
         */
            void shuffleRows(const std::vector<index_type> &rows) override;

            /**
         * Reorder the rows of the matrix according to a given permutation.
         *
         * @param cols see shuffleRows
         */
            void shuffleCols(const std::vector<index_type> &cols) override;

            /**
         * Remove the rows identified by the passed indices
         * @param rows WARNING: The passed indices must be sorted in ascending order
         */
            void removeRows(const std::vector<index_type> &rows) override;

            /**
         * Remove the rows identified by the passed indices
         * @param cols WARNING: The passed indices must be sorted in ascending order
         */
            void removeCols(const std::vector<index_type> &cols) override;

            /**
         * see: AbstractMatrix::transpose for more information
         */
            void transpose() override;

            /**
         * Sorts the rows of the matrix according to the column names of matrix M
         * @param m
         */
            void sortRows(std::vector<std::string> &m);

        private:
            // Actual matrix payload
            DMatrix m_;

            // Helper for removing rows or columns
            void remove_(const std::vector<index_type> &indices, std::map<std::string, index_type> &name_to_index,
                         std::vector<std::string> &index_to_name, std::function<void(index_type, index_type)> copy);

            void shuffle_(std::vector<index_type> perm, std::map<std::string, index_type> &name_to_index, std::vector<std::string> &index_to_name,
                          std::function<void(index_type, index_type)> swap);
        };
    }// namespace GeneTrail

#endif//GENETRAILCORE_DENSEMATRIX_H
