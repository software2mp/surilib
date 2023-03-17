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

#ifndef VECTORDATASOURCE_H_
#define VECTORDATASOURCE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/xmlnames.h"
#include "suri/VectorElement.h"
#include "suri/DatasourceValidatorInterface.h"

// Includes Wx
#include "wx/wx.h"
// Defines
// forwards

namespace suri {
   
// forwards
class Element;
class Image;
class Vector;

/**
 * Clase que representa una fuente de datos de tipo vectorial
 */
class VectorDatasource: public suri::DatasourceInterface {
public:
   /** Constructor */
   VectorDatasource();
   /** Destructor */
   virtual ~VectorDatasource();
   /** Funcion estatica para crear un VectorDatasource */
   static VectorDatasource* Create(const std::string &FileName,
                                   const Option& Metadata = Option());
   /** Funcion estatica para crear un VectorDatasource */
   static VectorDatasource* Create(const wxXmlNode* pRootNode);
   /** Funcion estatica para crear un RasterDatasource \depreacted */
   static VectorDatasource* Create(Element* pElement);
   /** Funcion estatica para crear un DatasourceInterface */
   static VectorDatasource* Create(DatasourceInterface *pDatasource);
   /** Obtiene el elemento \deprecated */
   virtual Element* GetElement();
   /** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual bool SetNextDatasource(DatasourceInterface* pDatasource);
   /** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual DatasourceInterface* GetNextDatasource();
   /** Compara dos tipos diferentes de Datasource */
   virtual bool Equals(DatasourceInterface &Datasource);
   /** Compara un Datasource contra un Layer */
   virtual bool Equals(LayerInterface &Layer);
   /** Compara un Datasource contra un Element */
   virtual bool Equals(Element &Element);
   /** Devuelve la interpretacion de la fuente de datos como una imagen */
   virtual Image* GetDatasourceAsImage();
   /** Devuelve la interpretacion de la fuente de datos como un vector */
   virtual Vector* GetDatasourceAsVector();
   /** Setea el nombre de la fuente de datos */
   virtual bool SetName(const std::string &Name);
   /** Setea el nombre de la fuente de datos */
   virtual std::string GetName() const;
   /** Setea la capa activa \deprecated */
   void SetActiveLayer(int ActiveLayer);
   /** Obtiene la capa activa */
   const int GetActiveLayer() const;
   /** Cambia el nombre de la fuente de datos */
   virtual bool SetCopyRight(const std::string &CopyRight);
   /** Cambia la descripcion de una fuente de datos */
   virtual bool SetDescription(const std::string &Description);
   /** Cambia el url de la fuente de datos */
   virtual bool SetUrl(const std::string &Url);
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual bool SetIcon(const std::string &Icon);
   /** Obtiene el nombre de la fuente de datos */
   virtual const std::string GetCopyRight()const;
   /** Obtiene la descripcion de una fuente de datos */
   virtual const std::string GetDescription()const;
   /** Obtiene el url de la fuente de datos */
   virtual const std::string GetUrl()const;
   /** Obtiene el icono asociado a la fuente de datos \deprecated */
   virtual const std::string GetIcon()const;
   /** Devuelve una instancia del validador asociado a la fuente de datos**/
   virtual DatasourceValidatorInterface* GetAssociatedValidator()const;
   /** Obtiene el hotlink de la fuente de datos */
   virtual bool HasHotlink()const;
   /** Obtiene el sistema de referencia que posee la fuente de datos*/
   virtual std::string GetSpatialReference() const;
   /** Obtiene el nombre de la capa activa **/
   std::string GetActiveLayerName() const;
   /** Devuelve true si un elemento esta listo para ser utilizado **/
   virtual bool IsValid() const;

private:
   /** Constructor de copia */
   VectorDatasource(const VectorDatasource &);
   /** Initialize a partir del nombre del archivo */
   virtual VectorDatasource* Initialize(const std::string &FileName,
                                        const Option& Metadata = Option());
   /** Initialize a partir del nodo xml */
   virtual VectorDatasource* Initialize(const wxXmlNode* pRootNode);
   /** Initialize a partir del elemento \deprecated */
   virtual VectorDatasource* Initialize(Element *pElement);
   /** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
   virtual VectorDatasource* Initialize(DatasourceInterface *pDatasource);
   /** Intenta inicializar subdatasets si existen varios tipos de geometrias **/
   virtual void InitializeSubdatasets (const std::string &FileName,
                                       VectorDatasource* pDatasource);
   /** Verifica que la capa activa del elemento sea la misma que el elem asociado */
   bool HasSameActiveLayer(Element *pElement);
   /**
    * Intenta crear un elemento vectoria a partir de la url y metadatos
    */
   VectorElement* CreateVectorElement(const std::string &FileName,
                                      const Option& Metadata,
                                      Vector::VectorType Type = Vector::Undefined);
   /*! Puntero al siguiente VectorDatasource */
   VectorDatasource* pNextVectorDatasource_;
   /*! Puntero al elemento \deprecated */
   VectorElement* pElement_;
   /*! Capa activa */
   int activeLayer_;
   std::map<Vector::VectorType, std::string> vectorTypeMap;
};

} /* namespace suri */

#endif /* VECTORDATASOURCE_H_ */
