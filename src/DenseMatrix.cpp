//
// Created by Ian Ferenc Diks on 20.12.22.
//
#include "DenseMatrix.h"

#include <cassert>
#include <iostream>
#include <limits>

namespace GeneTrail::Core {

    DenseMatrix::DenseMatrix(index_type rows, index_type cols) : AbstractMatrix(rows, cols), m_(rows, cols) {}

    DenseMatrix::DenseMatrix(std::vector<std::string> rows, std::vector<std::string> cols)
        : AbstractMatrix(std::move(rows), std::move(cols)), m_(this->rows(), this->cols()) {}

    DenseMatrix::DenseMatrix(DenseMatrix &&matrix) : AbstractMatrix(std::move(matrix)) { m_.swap(matrix.m_); }

    DenseMatrix &DenseMatrix::operator=(DenseMatrix &&matrix) {
        m_.swap(matrix.m_);
        AbstractMatrix::operator=(std::move(matrix));
        return *this;
    }

    DenseMatrix::DMatrix::ColXpr DenseMatrix::col(index_type j) { return m_.col(j); }

    DenseMatrix::DMatrix::ConstColXpr DenseMatrix::col(index_type j) const { return m_.col(j); }

    DenseMatrix::DMatrix &DenseMatrix::matrix() { return m_; }

    const DenseMatrix::DMatrix &DenseMatrix::matrix() const { return m_; }

    DenseMatrix::DMatrix::RowXpr DenseMatrix::row(index_type i) { return m_.row(i); }

    DenseMatrix::DMatrix::ConstRowXpr DenseMatrix::row(index_type i) const { return m_.row(i); }

    void DenseMatrix::rbind(const DenseMatrix &m) {
        assert(cols() == m.cols());
        auto r1 = rowNames();
        auto r2 = m.rowNames();
        DenseMatrix::DMatrix tmp(rows() + m.rows(), cols());
        tmp << m_, m.matrix();
        std::swap(tmp, m_);
        std::vector<std::string> new_row_names;
        new_row_names.reserve(r1.size() + r2.size());
        new_row_names.insert(new_row_names.end(), r1.begin(), r1.end());
        new_row_names.insert(new_row_names.end(), r2.begin(), r2.end());
        index_to_rowname_ = new_row_names;
        this->updateRowAndColNames_();
    }

    void DenseMatrix::cbind(const DenseMatrix &m) {
        assert(cols() == m.cols());
        auto c1 = colNames();
        auto c2 = m.colNames();
        DenseMatrix::DMatrix tmp(rows(), cols() + m.cols());
        tmp << m_, m.matrix();
        std::swap(tmp, m_);
        std::vector<std::string> new_col_names;
        new_col_names.reserve(c1.size() + c2.size());
        new_col_names.insert(new_col_names.end(), c1.begin(), c1.end());
        new_col_names.insert(new_col_names.end(), c2.begin(), c2.end());
        index_to_colname_ = new_col_names;
        this->updateRowAndColNames_();
    }

    void DenseMatrix::setCol(const std::string &name, const Vector &v) {
        auto res = colname_to_index_.find(name);
        if (res != colname_to_index_.end()) { m_.col(res->second) = v; }
    }

    void DenseMatrix::setCol(index_type j, const Vector &v) { m_.col(j) = v; }

    void DenseMatrix::remove_(const std::vector<index_type> &indices, std::map<std::string, index_type> &name_to_index,
                              std::vector<std::string> &index_to_name, std::function<void(index_type, index_type)> copy) {
        size_t next_idx = 1;
        size_t write_idx = indices[0];
        name_to_index.erase(index_to_name[write_idx]);
        for (size_t read_idx = indices[0] + 1; read_idx < index_to_name.size(); read_idx++) {
            if (next_idx < indices.size() && read_idx == indices[next_idx]) {
                assert(indices[next_idx - 1] < indices[next_idx]);
                name_to_index.erase(index_to_name[read_idx]);
                next_idx++;
            } else {
                auto it = name_to_index.find(index_to_name[read_idx]);
                if (it != name_to_index.end() && it->second == write_idx) { name_to_index.erase(it); }

                const std::string name = index_to_name[read_idx];
                index_to_name[write_idx] = name;
                name_to_index[name] = write_idx;
                copy(write_idx, read_idx);
                write_idx++;
            }
        }
        assert(name_to_index.size() == index_to_name.size() - indices.size());
    }

    void DenseMatrix::removeCols(const std::vector<index_type> &cols) {
        if (cols.empty()) return;
        remove_(cols, colname_to_index_, index_to_colname_, [this](index_type i, index_type j) { m_.row(i) = m_.row(j); });

        m_.conservativeResize(Eigen::NoChange, m_.cols() - cols.size());
        index_to_colname_.resize(m_.cols());
    }

    void DenseMatrix::removeRows(const std::vector<index_type> &rows) {
        if (rows.empty()) return;
        remove_(rows, rowname_to_index_, index_to_rowname_, [this](index_type i, index_type j) { m_.row(i) = m_.row(j); });

        m_.conservativeResize(Eigen::NoChange, m_.rows() - rows.size());
        index_to_rowname_.resize(m_.rows());
    }

    void DenseMatrix::setRow(const std::string &name, const Vector &v) {
        auto res = rowname_to_index_.find(name);
        if (res != rowname_to_index_.end()) { m_.row(res->second) = v.transpose(); }
    }

    void DenseMatrix::setRow(index_type i, const Vector &v) { m_.row(i) = v.transpose(); }

    void DenseMatrix::sortRows(std::vector<std::string> &m) {
        for (size_t r = 0; r < rows(); r++) {
            auto old_row = row(r);
            Eigen::Matrix<value_type, Eigen::Dynamic, 1> new_row(m.size());
            for (size_t o = 0; o < m.size(); o++) { new_row[o] = old_row[colIndex(m[o])]; }
            setRow(rowName(r), new_row);
        }
        setColNames(m);
    }

    void DenseMatrix::shuffle_(std::vector<index_type> perm, std::map<std::string, index_type> &name_to_index,
                               std::vector<std::string> &index_to_name, std::function<void(index_type, index_type)> swap) {
        for (size_t i = 0; i < perm.size(); i++) {
            index_type next = i;
            while (perm[next] > i) {
                swap(perm[next], next);
                std::swap(name_to_index[index_to_name[perm[next]]], name_to_index[index_to_name[next]]);
                std::swap(index_to_name[perm[next]], index_to_name[next]);
                std::swap(next, perm[next]);
            }
            perm[next] = next;
        }
    }

    void DenseMatrix::shuffleCols(const std::vector<index_type> &cols) {
        shuffle_(cols, colname_to_index_, index_to_colname_, [this](index_type i, index_type j) { m_.col(i).swap(m_.col(j)); });
    }

    void DenseMatrix::shuffleRows(const std::vector<index_type> &rows) {
        shuffle_(rows, rowname_to_index_, index_to_rowname_, [this](index_type i, index_type j) { m_.row(i).swap(m_.row(j)); });
    }

    void DenseMatrix::transpose() {
        AbstractMatrix::transpose();
        m_.transposeInPlace();
    }

}// namespace GeneTrail::Core