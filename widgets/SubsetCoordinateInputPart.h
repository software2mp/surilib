/*! \file SubsetCoordinateInputPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SUBSETCOORDINATEINPUTPART_H_
#define SUBSETCOORDINATEINPUTPART_H_

// Includes standard
// Includes Suri
#include "suri/PartCollection.h"
#include "UniversalCoordinateInputPart.h"
#include "suri/Subset.h"
#include "LatitudeLongitudeInputPart.h"
#include "DecimalCoordinateInputPart.h"

// Includes Wx
// Defines

/** namespace suri */
namespace suri {
/** Permite definir un subset espacial a partir de las esquinas. */
/**
 * Permite definir un subset espacial a partir de las esquinas.
 * Para ello el usuario debera ingresar la esquina superior izquierda
 * e inferior derecha.
 * Estas se podran ingresar en cualquier sistema de referencia
 * manejado por la aplicacion, incluyendo pixel linea.
 *
 */
class SubsetCoordinateInputPart : public suri::PartCollection {
   /** Nombre del XRC que representa la ventana */
   static const std::string SUBSET_COORDINATE_XRC_NAME;

public:
   /** Ctor */
   SubsetCoordinateInputPart();
   /** Dtor */
   virtual ~SubsetCoordinateInputPart();

   bool SetInternalSubset(const suri::Subset &SubsetIn);
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();

protected:
   suri::LatitudeLongitudeInputPart *pULDegreeCorner_;
   suri::LatitudeLongitudeInputPart *pLRDegreeCorner_;
   suri::Part *pULDecimalCorner_;
   suri::Part *pLRDecimalCorner_;

private:
};
}

#endif /* SUBSETCOORDINATEINPUTPART_H_ */
