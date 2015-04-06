#include "RasterMan/files.h"
#include "RasterMan/iofiles.h"
#include "RasterMan/trace.h"


/*******************************************************************\
*                                                                   *
*                        O u t p u t F i l e                        *
*                                                                   *
\*******************************************************************/

OutputFile::OutputFile(std::string Filename) : filename(Filename), open(false)
{
    this->Open(this->filename);
}


OutputFile::~OutputFile()
{
    this->Close();
}


void OutputFile::Open(std::string Filename)
{
    Trace("OutputFile", "Open(std::string)");

    if (this->open == true) {
	if (this->filename != Filename) {
	    this->Close();
	    this->filename = "";
	}
    }

    if (this->open == false) {
	this->filename = Filename;
	this->std::ofstream::open(Filename.c_str());
	if (!*this) throw std::runtime_error(std::string("Cannot open file: ") + this->filename);
	this->open = true;
    }
}


void OutputFile::Close()
{
    Trace("OutputFile", "Close()");

    if (this->open == false) {
	this->std::ofstream::close();
	this->open = false;
    }
}


std::string OutputFile::Filename() const
{
    return this->filename;
}


/*******************************************************************\
*                                                                   *
*                         I n p u t F i l e                         *
*                                                                   *
\*******************************************************************/

InputFile::InputFile(std::string Filename) : filename(Filename), open(false)
{
    this->Open(this->filename);
}


InputFile::~InputFile()
{
    this->Close();
}


void InputFile::Open(std::string Filename)
{
    Trace("InputFile", "Open(std::string)");

    if (this->open == true) {
	if (this->filename != Filename) {
	    this->Close();
	    this->filename = "";
	}
    }

    if (this->open == false) {
	this->filename = Filename;
	this->std::ifstream::open(Filename.c_str());
	if (!*this) throw std::runtime_error(std::string("Cannot open file: ") + this->filename);
	this->open = true;
    }
}


void InputFile::Close()
{
    Trace("InputFile", "Close()");

    if (this->open == false) {
	this->std::ifstream::close();
	this->open = false;
    }
}


std::string InputFile::Filename() const
{
    return this->filename;
}


void InputFile::CheckStatus()
{
    Trace("InputFile", "CheckStatus()");

    if (!(*this)) {
	std::ostringstream errormessage;
	errormessage << "Error on reading file: \"" << this->Filename() << "\": ";
	if (this->eof()) {
	    errormessage << "End of File" << std::ends;
	}
	else {
	    errormessage << "Bad Input" << std::ends;
	}
	TraceMessage("InputFile::CheckStatus: " << errormessage.str() << std::endl;);
	throw std::runtime_error(errormessage.str());
    }
}

