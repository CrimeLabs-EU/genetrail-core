//
// Created by Ian Ferenc Diks on 20.12.22.
//

#ifndef GENETRAILCORE_FILE_H
#define GENETRAILCORE_FILE_H

#include "macros.h"
#include <ios>
#include <memory>
#include <string>

namespace genetrailcore {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

    enum class GENETRAILCORE_EXPORT FileOpenMode {
        READ,
        WRITE,
        APPEND
    };


#pragma GCC diagnostic pop

    class GENETRAILCORE_EXPORT GenericFile {
    public:
        GenericFile(const std::string& path, FileOpenMode mode);
        GenericFile(const GenericFile&) = delete;
        GenericFile& operator=(const GenericFile&) = delete;

        GenericFile(GenericFile&&) = default;
        GenericFile& operator=(GenericFile&&) = default;

        /**
		 * Virtual destructor.
		 */
        virtual ~GenericFile();

        /**
		 * Bool conversion operator
		 *
		 * @returns true when the underlying stream is in a good state.
		 */
        operator bool() const;

        /**
		 * Logical not operator
		 *
		 * @returns true when the underlying stream is in a bad state.
		 */
        bool operator!() const;

        /**
		 * Check the current open mode.
		 *
		 * @returns true iff the file was opened using OpenMode::READ
		 */
        bool isReading() const;

        /**
		 * Check the current open mode.
		 *
		 * @returns true iff the file was opened using
		 *          OpenMode::WRITE || OpenMode::APPEND.
		 */
        bool isWriting() const;

        /**
		 * Get the OpenMode with which the file was opened.
		 *
		 * @returns The OpenMode with which the file was opened.
		 */
        FileOpenMode mode() const;

    protected:
        virtual bool isValid_() const;

        std::istream* in_strm_;
        std::ostream* out_strm_;

    private:
        enum class Compressor {
            BZIP2,
            GZIP,
            NONE,
            ZLIB
        };

        FileOpenMode mode_;

        bool destruct_;
        Compressor getCompressor_(const std::string& path) const;
        void openForReading_(const std::string& path);
        void openForWriting_(const std::string& path, std::ios_base::openmode m);
    };

}// namespace genetrailcore

#endif//GENETRAILCORE_FILE_H
