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

#ifndef MULTIMEDIADATASOURCE_H_
#define MULTIMEDIADATASOURCE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/Element.h"
#include "MultimediaElement.h"
#include "suri/Vector.h"
#include "suri/Image.h"
#include "suri/LayerInterface.h"
#include "suri/DatasourceValidatorInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase que representa una fuente de datos tipo Multimedia
 */
class MultimediaDatasource : public suri::DatasourceInterface {
public:
   /** Constructor */
   MultimediaDatasource();
   /** Destructor */
   virtual ~MultimediaDatasource();
   /** Funcion estatica para crear un MultimediaDatasource */
   static MultimediaDatasource* Create(const std::string &FileName);
   /** Funcion estatica para crear un MultimediaDatasource */
   static MultimediaDatasource* Create(const wxXmlNode* pRootNode);
   /** Funcion estatica para crear un MultimediaDatasource \deprecated */
   static MultimediaDatasource* Create(Element* pElement);
   /** Funcion estatica para crear un DatasourceInterface */
   static MultimediaDatasource* Create(DatasourceInterface *pDatasource);
   /** Obtiene el elemento \deprecated */
   virtual Element* GetElement();
   /** Devuelve la interpretacion de la fuente de datos como una imagen */
   virtual Image* GetDatasourceAsImage();
   /** Devuelve la interpretacion de la fuente de datos como un vector */
   virtual Vector* GetDatasourceAsVector();
   /** Compara dos tipos diferentes de Datasource */
   virtual bool Equals(DatasourceInterface &Datasource);
   /** Compara un Datasource contra un Layer*/
   virtual bool Equals(LayerInterface &Layer);
   /** Compara un Datasource contra un Element \deprecated */
   virtual bool Equals(Element &Element);
   /** Setea el nombre de la fuente de datos */
   virtual bool SetName(const std::string &Name);
   /** Setea el nombre de la fuente de datos */
   virtual std::string GetName() const;
   /** Cambia el nombre de la fuente de datos */
   virtual bool SetCopyRight(const std::string &CopyRight);
   /** Cambia la descripcion de una fuente de datos */
   virtual bool SetDescription(const std::string &Description);
   /** Cambia el url de la fuente de datos */
   virtual bool SetUrl(const std::string &Url);
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual bool SetIcon(const std::string &Icon);
   /** Cambia el nombre de la fuente de datos */
   virtual const std::string GetCopyRight()const;
   /** Cambia la descripcion de una fuente de datos */
   virtual const std::string GetDescription()const;
   /** Cambia el url de la fuente de datos */
   virtual const std::string GetUrl()const;
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual const std::string GetIcon()const;
   /** Devuelve una instancia del validador asociado a la fuente de datos**/
   virtual DatasourceValidatorInterface* GetAssociatedValidator()const;
   /** Obtiene el sistema de referencia que posee la fuente de datos*/
   virtual std::string GetSpatialReference() const;
   /** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual DatasourceInterface* GetNextDatasource();
   /** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual bool SetNextDatasource(DatasourceInterface* pDatasource);
   /** Devuelve true si un elemento esta listo para ser utilizado **/
   virtual bool IsValid() const;

private:
   /** Constructor de copia */
   MultimediaDatasource(const MultimediaDatasource &);
   /** Inicializa los atributos de Datasource a partir de un nombre de archivo */
   virtual MultimediaDatasource* Initialize(const std::string &FileName);
   /** Inicializa los atributos de Datasource a partir de un nodo xml */
   virtual MultimediaDatasource* Initialize(const wxXmlNode* pRootNode);
   /** Inicializa los atributos de Datasource a partir de un Element \deprecated */
   virtual MultimediaDatasource* Initialize(Element *pElement);
   /** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
   virtual MultimediaDatasource* Initialize(DatasourceInterface *pDatasource);
   Element *pElement_;                          /*! Puntero al elemento \deprecated */
};

} /** namespace suri */

#endif /* MULTIMEDIADATASOURCE_H_ */
