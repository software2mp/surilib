/*! \file VectorTableEditorPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORTABLEEDITORPART_H_
#define VECTORTABLEEDITORPART_H_

// Includes standard

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/Part.h"
// Includes Wx

// Includes App

// Defines

/** suri::VectorTableEditorPart: comment */
/*
 *
 */

namespace suri {
class VectorTableEditorPart : public suri::PartCollection {
public:
   /** Ctor */
   VectorTableEditorPart();
   /** Dtor */
   virtual ~VectorTableEditorPart();

   static const std::string VECTOR_TABLE_EDITOR_XRC_NAME; /*! Nombre de XRC */
};
}  // namespace suri
#endif /* VECTORTABLEEDITORPART_H_ */
