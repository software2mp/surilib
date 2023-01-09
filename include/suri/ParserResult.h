/*! \file ParserResult.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARSERRESULT_H_
#define PARSERRESULT_H_

#include <string>

namespace suri {

/**
 * Permite obtener informaci�n m�s detallada de los errores ocurridos al
 * parsear un documento, como n�mero y contenido de la linea donde fall�.
 */

class ParserResult{
public:
	/**
	 * Construye un resultado a partir del n�mero de linea,
	 * contenido y descripci�n.
	 */
	ParserResult(bool Success, int LineNumber = 0,
				const std::string& Content = "",
				const std::string& Description = "");
	const std::string& getContent() const;
	const std::string& getDescription() const;
	int getLineNumber() const;
	bool isSuccess() const;

private:
	/* Flag para indicar si el parseo fue exitoso o no */
	bool success_;
	/* Linea que fall� al parsear (si corresponde) */
	std::string content_;
	/* Motivo por el que fall� el parseo (si corresponde)*/
	std::string description_;
	/* N�mero de linea en el que se produjo la falla (si corresponde) */
	int lineNumber_;
};

}

#endif /* PARSERRESULT_H_ */
