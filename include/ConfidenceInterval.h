//
// Created by Ian Ferenc Diks on 20.12.22.
//

#ifndef GENETRAILCORE_CONFIDENCEINTERVAL_H
#define GENETRAILCORE_CONFIDENCEINTERVAL_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

#include "Exception.h"
#include "Statistic.h"

#include <boost/math/distributions/normal.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace GeneTrail::Core {

    template<typename value_type, typename InputIterator>
    class confidence_interval {
    public:
        /**
        * This methods calculates the bias-corrected and accelerated (BCa) bootstrap
        * confidence_interval, by Efron (1987, doi:10.2307/2289144).
        *
        * @param begin InputIterator
        * @param end InputIterator
        * @param theta Parameter of interest for the bootstrap distribution
        * @param alpha Significance level
        */
        static std::tuple<value_type, value_type> bca(InputIterator begin, InputIterator end, value_type theta, value_type alpha) {
            size_t B = boost::numeric_cast<value_type>(std::distance(begin, end));

            value_type z0_right = 0.0;
            value_type z0_left = 0.0;

            for (auto it = begin; it != end; ++it) {
                if (*it < theta) {
                    z0_right++;
                } else {
                    z0_left++;
                }
            }

            if (z0_left == 0) { z0_left = 1; }

            if (z0_right == 0) { z0_right = 1; }

            boost::math::normal dist(0, 1);
            z0_right = boost::math::quantile(dist, z0_right / B);
            z0_left = boost::math::quantile(dist, z0_left / B);

            value_type a = statistic::skewness<value_type>(begin, end) / boost::numeric_cast<value_type>(6.0);

            value_type z_alpha_left = boost::math::quantile(dist, alpha / boost::numeric_cast<value_type>(2.0));
            value_type z_alpha_right = boost::math::quantile(boost::math::complement(dist, alpha / boost::numeric_cast<value_type>(2.0)));

            value_type theta_bca_left = boost::math::cdf(dist, z0_left + (z0_left + z_alpha_left) / (1 - a * (z0_left + z_alpha_left)));
            value_type theta_bca_right = boost::math::cdf(dist, z0_right + (z0_right + z_alpha_right) / (1 - a * (z0_right + z_alpha_right)));

            std::vector<value_type> sorted(begin, end);
            std::sort(sorted.begin(), sorted.end());

            B = B - 1;
            return std::make_tuple(sorted[ceil(theta_bca_left * B)], sorted[floor(theta_bca_right * B)]);
        }

        /**
        * This methods calculates the percentiles of a given range.
        *
        * @param begin InputIterator
        * @param end InputIterator
        * @param alpha Significance level
        */
        static std::tuple<value_type, value_type> percentile(InputIterator begin, InputIterator end, value_type, value_type alpha) {
            std::vector<value_type> sorted(begin, end);
            std::sort(sorted.begin(), sorted.end());
            size_t left = (alpha / 2.0) * (std::distance(begin, end) - 1);
            size_t right = (1 - (alpha / 2.0)) * (std::distance(begin, end) - 1);
            return std::make_pair(sorted[left], sorted[right]);
        }

        using ci_method = std::tuple<value_type, value_type> (*)(InputIterator, InputIterator, value_type, value_type);
        static ci_method getCorrectionMethod(std::string method) {
            if (method == "percentile") { return percentile; }

            if (method == "bca") { return bca; }

            throw NotImplemented(__FILE__, __LINE__, "confidence_interval::" + method);
        }

        static std::tuple<value_type, value_type> compute(const std::string &method, InputIterator begin, InputIterator end, value_type theta,
                                                          value_type alpha) {
            return getCorrectionMethod(method)(begin, end, theta, alpha);
        }
    };

}// namespace genetrailcore

#endif//GENETRAILCORE_CONFIDENCEINTERVAL_H
