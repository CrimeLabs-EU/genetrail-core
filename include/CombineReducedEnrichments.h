//
// Created by Ian Ferenc Diks on 20.12.22.
//

#ifndef GENETRAILCORE_COMBINEREDUCEDENRICHMENTS_H
#define GENETRAILCORE_COMBINEREDUCEDENRICHMENTS_H

#include "macros.h"

#include <initializer_list>
#include <istream>
#include <map>
#include <set>
#include <vector>

namespace GeneTrail::Core {
    class GENETRAILCORE_EXPORT CombineReducedEnrichments {
    public:
        CombineReducedEnrichments() = default;
        void writeFiles(const std::string &sampleOutDirs, const std::string &matrixOutFiles);

    private:
        using WritePerCategoryDB = std::vector<std::ofstream>;
        using CategoryDBs = std::vector<std::string>;
        using Samples = std::vector<std::string>;

        WritePerCategoryDB allWriters;
        CategoryDBs categoryDBs;
        std::vector<std::string> samples;
        std::vector<std::string> dirs;

        void parseMatrixOutFiles(const std::string &matrixOutFiles);
        void parseSampleOutDirs(const std::string &sampleOutDirs);

        void write(size_t idx);
        void readSample(std::map<std::string, std::vector<std::string>> &result, size_t idx_sample, const std::string &categoryDB);
        void writeMatrix(std::map<std::string, std::vector<std::string>> &result, std::ofstream& writer);
        void writeHeader(unsigned int idx);
    };
}// namespace genetrailcore

#endif//GENETRAILCORE_COMBINEREDUCEDENRICHMENTS_H
