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

#ifndef WARPPARAMETERSPART_H_
#define WARPPARAMETERSPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/ParserResult.h"
#include "suri/PartCollection.h"
#include "suri/Part.h"
#include "suri/GcpList.h"
// Includes Wx

// Includes App

// Defines

namespace suri {

// forwards

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(SelectionEvent);
/** Part que permite ingresar los parametros del algoritmo de Warp */
/**
 * Permite ingresar el tipo de algoritmo y sus parametros.
 */
class WarpParametersPart : public PartCollection {
public:
   /** Definicion de flags para permitir diferentes tipos de transformaciones **/
   static const int GcpFile = 0x01;
   static const int Polynomial = 0x02;
   static const int Exact = 0x04;
   static const int SrsSelector = 0x08;
   /** Ctor */
   explicit WarpParametersPart(const std::string& FileName = "", const int Flags = GcpFile);
   /** Dtor */
   virtual ~WarpParametersPart();

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
   /** Configura los widgets */
   bool ConfigureWidget();
   /** Devuelve un string con el algoritmo seleccionado **/
   std::string GetAlgorithmSelected() const;
   /** Metodo de validacion de datos */
   virtual bool HasValidData();
   /**
    * devuelve el orden seleccionado desde el Part por el usuario
    * @return -1 en caso de que sea un orden invalido (ej. triangulacion)
    * @return orden del polinomio deseado
    */
   int GetPolynomialOrderSelected() const;
   /** Metodo que devuelve el filename*/
   std::string GetFileName() const;
   /** Devuelve la lista de GCPs cargados */
   GcpList& GetGcpList();
   /** Nombre de algoritmos **/
   static const std::string ExactAlgorithm;
   static const std::string PolynomialAlgorithm;
   static const std::string TriangulationAlgorithm;
   static const std::string OrderLabel;

private:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SelectionEvent);
   int flags_;
   std::string algorithmSelected_;
   std::string lastAlgorithmSelected_;
   int orderSelected_;
   int lastOrderSelected_;
   int choicePositionSelected_;
   int lastChoicePositionSelected_;
   EVENT_OBJECT_PROPERTY(SelectionEvent);

   std::string fileName_;
   std::string lastFileName_;
   GcpList gcpList_;
   ParserResult parseResult_;
   /**
    *  Metodo auxiliar que dado un archivo crea una lista de puntos de control
    *  El archivo de GCPs se valida en este part antes de pasarse a otros.
    */
   ParserResult LoadGCPList();
   /**
    *  Muestro mensaje de errores producidos al cargar los GCPs
    */
   void DisplayError();
   /** Evento para el check de valid data **/
   void OnSelectionChanged(wxCommandEvent &Event);
   void OnFilePickerChanged(wxCommandEvent &Event);
};
}

#endif /* REPROJECTIONPARAMETERSPART_H_ */
