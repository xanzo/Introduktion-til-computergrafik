#ifndef __FILEEXTRACTION_H__
#define __FILEEXTRACTION_H__

#include "DIKUgrafik/files.h"

/**
 * Utility template operator which makes it easy to read from a file.
 * \param file - the file to be read.
 * \param value - the value read.
 * \return a reference to the InputFile read from.
 */
template <typename Type>
InputFile& operator>>(InputFile& file, Type& value);


template <typename Type>
InputFile& operator>>(InputFile& file, Type& value)
{
    //std::cout << std::endl << "operator>> begin" << std::endl;

    static_cast<std::ifstream&>(file) >> value;
    file.CheckStatus();

    //std::cout << "operator>> end" << std::endl;

    return file;
}

#endif
