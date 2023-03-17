/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
