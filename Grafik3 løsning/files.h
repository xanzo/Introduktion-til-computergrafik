#ifndef __FILES_H__
#define __FILES_H__

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>


/**
 * \class OutputFile
 * is a utility class which makes it easy to open, close, and write to a file.
 */
class OutputFile : public std::ofstream 
{
public:
    /**
     * Parameterized constructor, creates an instance of an OutputFile and opens an external file given by a filename
     * for writing.
     * \param Filename - a string containing the name of the file to be opened.
     */
    OutputFile(std::string Filename);

    /**
     * Destructor, closes the external file and destroys the instance of OutputFile.
     */
    virtual ~OutputFile();

    /**
     * Opens an external file given by the parameter. If there already exists an open file, it is closed before the 
     * new one is opened.
     * \param Filename - a string containing the name of the file to be opened.
     */
    void Open(std::string Filename);

    /**
     * Closes the external file if it is open.
     */
    void Close();

    /**
     * Returns the name of the external file.
     * \return the name of the currently open external file.
     */
    std::string Filename() const;

protected:

private:
    std::string filename;
    bool open;
};


/**
 * \class InputFile
 * is a utility class which makes it easy to open, close, and read from a file.
 */
class InputFile : public std::ifstream
{
public:
    /**
     * Parameterized constructor, creates an instance of an OInputFile and opens an external file given by a filename
     * for reading.
     * \param Filename - a string containing the name of the file to be opened.
     */
    InputFile(std::string Filename);

    /**
     * Destructor, closes the external file and destroys the instance of InputFile.
     */
    virtual ~InputFile();

    /**
     * Opens an external file given by the parameter. If there already exists an open file, it is closed before the 
     * new one is opened.
     * \param Filename - a string containing the name of the file to be opened.
     */
    void Open(std::string Filename);

    /**
     * Closes the external file if it is open.
     */
    void Close();

    /**
     * Returns the name of the external file.
     * \return the name of the currently open external file.
     */
    std::string Filename() const;

    /**
     * Checks the status of the currently open InputFile. If some error has occurred on the external file
     * a runtime_error exception is thrown.
     */
    void CheckStatus();

protected:

private:
    std::string filename;
    bool open;
};

#endif
