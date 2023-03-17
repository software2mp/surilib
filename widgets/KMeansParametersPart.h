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

// Includes standard
// Includes Suri
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Includes App
// Defines

#ifndef KMEANSPARAMETERSPART_H_
#define KMEANSPARAMETERSPART_H_

#include "NonSupervisedParameterSelectionPart.h"

namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(KMeansParametersPartEvent);

class KMeansParametersPart : public suri::NonSupervisedParameterSelectionPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(KMeansParametersPartEvent);

   KMeansParametersPart();
   virtual ~KMeansParametersPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();

// -- Metodos de ParametersSelectionPart ---
   /** Elimina todas las clases cargadas. */
   virtual bool Reset();
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();
   /** Agrega clases que se usan para clasificar */
   virtual bool AddClass(int ClassId, Statistics Stats);
   /** Retorna un puntero al clasificador creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Retorna la cantidad de clases */
   virtual int GetClassCount();
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones van de 0 a */
// GetClassCount()-1.
   virtual bool GetClassProperties(ClassInformation &Properties);

   /** modifica el texto que informa la posicion */
   virtual void OnScroll(wxScrollEvent& Event);
protected:
   EVENT_OBJECT_PROPERTY(KMeansParametersPartEvent); /*! Objeto que conecta los */
   /* eventos */
};
}

#endif /* KMEANSPARAMETERSPART_H_ */
