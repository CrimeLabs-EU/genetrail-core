//
// Created by Ian Ferenc Diks on 20.12.22.
//

#ifndef GENETRAILCORE_DENSECOLUMNSUBSET_H
#define GENETRAILCORE_DENSECOLUMNSUBSET_H
#include "DenseMatrix.h"
#include <iostream>
#include "macros.h"

#include <boost/range/irange.hpp>

namespace GeneTrail::Core {
    /**
	 * This class represents a subset of a DenseMatrix.
	 *
	 * The reason we derive from DenseMatrix is that we
	 * want to be able to use DenseMatrix and DenseColumnSubset
	 * interchangably.
	 *
	 * Lets see if this works...
	 */
    class GENETRAILCORE_EXPORT DenseColumnSubset : public Matrix
    {
    public:
        using DMatrix = DenseMatrix::DMatrix;
        typedef std::vector<DenseMatrix::index_type> ISubset;
        typedef std::vector<std::string> SSubset;

        template<typename Iterator>
        DenseColumnSubset(DenseMatrix* mat, Iterator first, Iterator last);
        DenseColumnSubset(DenseMatrix* mat, ISubset cols);
        DenseColumnSubset(DenseMatrix* mat, const SSubset& cols);

        DenseColumnSubset(const DenseColumnSubset& subs) = default;
        DenseColumnSubset(DenseColumnSubset&& subs);

        DenseColumnSubset& operator=(const DenseColumnSubset& subs) = default;
        DenseColumnSubset& operator=(DenseColumnSubset&& subs);

        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last);

        value_type& operator()(index_type i, index_type j) override
        {
            // Directly access the underlying matrix to avoid a virtual function call
            return mat_->matrix()(i, col_subset_[j]);
        }

        value_type operator()(index_type i, index_type j) const override
        {
            // Directly access the underlying matrix to avoid a virtual function call
            return mat_->matrix()(i, col_subset_[j]);
        }

        /**
			 * Direct access to the Eigen expression template representing the
			 * respective column. This allows leveraging Eigen's vectorization
			 * features.
			 */
        DMatrix::ColXpr col(index_type j) {
            return mat_->matrix().col(col_subset_[j]);
        }

        /**
			 * Direct access to the Eigen expression template representing the
			 * respective column. This allows leveraging Eigen's vectorization
			 * features.
			 */
        DMatrix::ConstColXpr col(index_type j) const {
            return static_cast<const DenseMatrix*>(mat_)->matrix().col(col_subset_[j]);
        }

        const std::string& colName(index_type j) const override;
        const std::string& rowName(index_type i) const override;

        index_type colIndex(const std::string& col) const override;
        index_type rowIndex(const std::string& row) const override;

        index_type cols() const override;
        index_type rows() const override;

        bool hasCol(const std::string& name) const override;
        bool hasRow(const std::string& name) const override;

        void setColName(index_type j, const std::string& new_name) override;
        void setColName(const std::string& old_name, const std::string& new_name) override;
        void setColNames(const std::vector< std::string >& col_names) override;
        void setRowName(index_type i, const std::string& new_name) override;
        void setRowName(const std::string& old_name, const std::string& new_name) override;
        void setRowNames(const std::vector< std::string >& row_names) override;

        void removeCols(const std::vector< index_type >& indices) override;
        void removeRows(const std::vector< index_type >& indices) override;
        void shuffleCols(const std::vector< index_type >& perm) override;
        void shuffleRows(const std::vector< index_type >& perm) override;
        void transpose() override;

        const std::vector< std::string >& colNames() const override;
        const std::vector< std::string >& rowNames() const override;

    private:
        DenseMatrix* mat_;
        ISubset col_subset_;

        // TODO: Think of something smart to fix the hack below
        mutable SSubset row_names_cache_;
        mutable SSubset col_names_cache_;

        void remove_(const std::vector<Matrix::index_type>& indices, ISubset& subset);
    };

    template<typename Iterator>
    DenseColumnSubset::DenseColumnSubset(DenseMatrix* mat, Iterator begin, Iterator end)
        : mat_(mat),
          col_subset_(begin, end)
    {
    }

    template<typename InputIterator>
    void DenseColumnSubset::assign(InputIterator first, InputIterator last)
    {
        col_subset_.assign(first, last);
    }
}
#endif//GENETRAILCORE_DENSECOLUMNSUBSET_H
