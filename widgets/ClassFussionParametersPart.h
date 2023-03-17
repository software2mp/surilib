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

#ifndef CLASSFUSSIONPARAMETERSPART_H_
#define CLASSFUSSIONPARAMETERSPART_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>
#include <utility>

// Includes Suri
#include "ClassificationPart.h"
#include "ClassFussionAlgorithm.h"
#include "ClassMappingPart.h"
#include "wxGenericTableBase.h"

// Includes Wx
#include "wx/event.h"

// Defines

// forwards

namespace suri {

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ClassFusionEvent);

/**
 *Part para la fusion de clases
 */
class ClassFussionParametersPart : public suri::ClassificationPart {
   ClassFussionParametersPart(const ClassFussionParametersPart&);

public:
   /** Constructor **/
   explicit ClassFussionParametersPart(Element* pElement);
   /** Destructor **/
   virtual ~ClassFussionParametersPart();
   /** Retorna las propiedades de los pixels que no tienen clase */
   virtual bool GetNonClassifiedPixelInfo(ClassInformation &Properties);
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Metodos Part */
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Crea el Tool Window */
   bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Retorna la cantidad de clases */
   virtual int GetClassCount();
   /** Retorna las propiedades de la clase en la posicion indicada */
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties);
   /** Retorna el nodo que se debe agregar al elemento que se renderiza */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement);

private:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ClassFusionEvent);
   /** Algoritmo */
   ClassFussionAlgorithm algorithm_;
   /** Elemento al cual pertenecen los datos del part */
   Element* pElement_;
   /** Part que realiza el mapeo para la fusion */
   ClassMappingPart* pClassMappingPart_;
   /** Metodo que asocia el evento de agregar clase al boton del part */
   void ConfigureButtonEvent();
   /** Evento que se ejecuta al hacer click sobre boton de agregar clase */
   void OnAddClassButtonEventHandler(wxCommandEvent &Event);
   /** Objeto para conectar */
   EVENT_OBJECT_PROPERTY(ClassFusionEvent);
   /** Convierte las clases del elemento en cluster para el algoritmo de clasificacion */
   bool InsertClassesToTable(ClassMappingPart::MappingTable &pMappingTable,
                             std::vector<ClassMappingPart::ChoiceValues> &pClassesMap);
   /** Convierte el contenido de la tabla en un nodo xml
    * para la configuracion del algoritmo*/
   bool TableToClustersXmlNode(const ClassMappingPart::MappingTable& MappingTable);
   /*** Mapa para relacion id-nombre*/
   typedef std::map<std::string, ClassInformation> ClassMapping;
   typedef std::pair<std::string, ClassInformation> ClassMappingPair;
   /** Mapa para la relacion nombre */
   ClassMapping classmapping_;
   /** Clases que son el resultado de la fusion */
   std::vector<ClassInformation> resultclasses_;
   /** Indice para la creacion de clases */
   ClassInformation::IndexType lastindex_;
};

} /** namespace suri */

#endif /* CLASSFUSSIONPARAMETERSPART_H_ */
