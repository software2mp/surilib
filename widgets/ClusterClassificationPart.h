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

#ifndef CLUSTERCLASSIFICATIONPART_H_
#define CLUSTERCLASSIFICATIONPART_H_

// Includes standard
// Includes Suri
#include "ClusterClassificationAlgorithm.h"
#include "ParametersSelectionPart.h"
#include "suri/VectorStyle.h"
#include "SupervisedParameterSelectionPart.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Clase intermedia con codigo comun a todos los parts que configuran
 * algoritmos de clasificacion que usan clusters
 */
class ClusterClassificationPart : public SupervisedParameterSelectionPart {
public:
   /** Ctor */
   ClusterClassificationPart(
         ClusterClassificationAlgorithm* pClusterClassificationAlgorithm);
   /** Dtor */
   virtual ~ClusterClassificationPart();

   // --- Metodos de Part ---
   /** Analiza si el part tiene datos validos. */
   virtual bool HasValidData();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();

   // -- Metodos de ParametersSelectionPart ---
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Retorna las propiedades de los pixels sin clase. */
   virtual bool GetNonClassifiedPixelInfo(ClassInformation &Properties);

protected:
   /**
    * Por falta de tiempo la interfaz de modifica usando herencia. 
    * Refactor pendiente: Componer un Widget que encapsule la interfaz
    * con la que se ingresa el threshold y color.
    */
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color) = 0;
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue() = 0;
   /** Cambia el valor del slider */
   virtual void SetThresholdCtrlValue(double Threshold) = 0;
   /** Retorna el valor del slider */
   virtual double GetThresholdCtrlValue() = 0;

   VectorStyle::Color color_; /*! variable con color de pixel no clasificado */
   ClusterClassificationAlgorithm* pAlgorithm_; /*! algoritmo a configurar */

   static const int NON_CLASSIFIED_PIXEL_VALUE;  /*! Valor de los pixels */
                                                 /* no clasificados */
};
}  // namespace suri


#endif /* CLUSTERCLASSIFICATIONPART_H_ */
