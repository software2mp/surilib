/*! \file ParserResult.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
#include <string>
// Includes Suri
#include "suri/ParserResult.h"
// Includes Wx
// Defines
// forwards

namespace suri {

ParserResult::ParserResult(bool Success, int LineNumber,
		const std::string& Content,
		const std::string& Description){
	this->content_ = Content;
	this->description_ = Description;
	this->lineNumber_ = LineNumber;
	this->success_ = Success;
}

const std::string& ParserResult::getContent() const {
	return content_;
}

const std::string& ParserResult::getDescription() const {
	return description_;
}

bool ParserResult::isSuccess() const {
	return success_;
}

int ParserResult::getLineNumber() const{
	return lineNumber_;
}

}
