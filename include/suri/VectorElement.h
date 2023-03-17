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

#ifndef VECTORELEMENT_H_
#define VECTORELEMENT_H_

// Includes Estandar
#include <map>
#include <list>
#include <vector>
#include <string>

// Includes suri
#include "Element.h"
#include "Part.h"
#include "suri/Vector.h"

// Define la decoracion de GetTypeAsString para poder utilizarla como un tipo
#define TYPE_DECORATOR(Type) wxString::Format(_(message_VECTOR_LAYER_OF_s), _(Type) )

// namespace suri
namespace suri {

// Forwards
class DataViewManager;

/**
 * Element que representa a un vector.
 * Guarda informacion del elemento como su SpatialReference y el estilo que
 * utiliza. Para crear el VectorElement se utiliza una funcion estatica a la
 * que se le pasa el nombre del arhivo.
 * \TODO Implementar metodo simetrico Destroy()
 */
class VectorElement : public Element {
private:
   /* Ctor. de Copia */
   VectorElement(const VectorElement &VectorElement);

public:
   /* ctor */
   VectorElement();
   /** dtor */
   virtual ~VectorElement();
   /** Retorna un string para el icono */
   virtual wxString DoGetIcon() const;
   /** Crea una previsualizacion del elemento */
   virtual void GetPreview(wxBitmap &Preview, int Width = 0, int Height = 0) const;
   /** Funcion estatica para crear un VectorElement */
   static VectorElement* Create(const std::string &FileName,
                                const Option& Metadata = Option());
   /** Retorna un identificador unico */
   virtual UidType GetUid() const;
   /** Obtiene la referencia espacial del elemento */
   virtual wxString GetSpatialReference() const;
   /** Obtiene la referencia espacial del elemento */
   virtual wxString GetSpatialReference(int Layer) const;
   /** Carga la referencia espacial del elemento */
   virtual bool SetSpatialReference(const std::string &Wkt);
   /** Carga la referencia espacial del elemento */
   virtual bool SetSpatialReference(int Layer, const std::string &Wkt);
   /** Devuelve los Part que se van a mostrar en las Propiedades */
   virtual std::vector<Part *> DoGetParts(DataViewManager* pDataViewManager = NULL,
                                          LayerInterface *pLayer = NULL,
                                          DatasourceInterface *pDatasource = NULL);
   /** Devuelve el tipo de Vector */
   virtual wxString GetTypeAsString();
   /** Obtiene el WKT del estilo del vector */
   virtual std::string GetStyle(int Layer) const;
   /** Permite setear el WKT del estilo del vector */
   virtual void SetStyle(const std::string &WktStyle, int Layer);
   /** Permite setear el WKT del estilo del vector */
   virtual void SetStyle(const std::string &WktStyle);
   /** Obtiene el subset con el extent del vector del nodo del elemento */
   virtual void GetElementExtent(Subset &ElementExtent);
   /** Obtiene el subset con el extent de una capa */
   virtual void GetLayerExtent(Subset &LayerExtent, int Layer);
   /** Actualiza extent y dependencias */
   virtual void UpdateData();
   /** Inicializa los nodos del elemento */
   virtual bool InitVector(Vector *pVector, const std::string &Filename);
      /** Inicializa los nodos del elemento */
   virtual bool InitVector(Vector *pVector, const std::string &Filename, Vector::VectorType type);
   /** Devuelve el nombre del campo que contiene los url de los hotlinks */
   virtual wxString GetHotLinkFieldName(const int Layer) const;
   /** Setea el nombre del campo que contiene los url de los hotlinks */
   void SetHotLinkFieldName(const int Layer, const wxString &FieldName);
   /** Setea el nombre del campo que contiene los url de los hotlinks */
   void SetHotLinkFieldName(const wxString &FieldName);
   /** Informa si el estado del elemento es valido */
   bool IsValid();
   /** Retorna la cantidad de capas en el nodo */
   int GetLayerCount();
   /** Obtiene la capa activa */
   const int GetActiveLayer() const;
   /** Setea la capa activa  (solo se puede setear una vez) */
   bool SetActiveLayer(int ActiveLayer);
   /** Devuelve el tipo de vector asociado al elemento. */
   Vector::VectorType GetType() const;
   /** Asigna el tipo de vector asociado al elemento. */
   void SetType(Vector::VectorType Type, bool recursive = true);
   /** Asigna el tipo de vector correspondiente a una capa del elemento. **/
   // TODO matiaslafroce: Esta sin implementar, hacerlo tomando ejemplo de SetType
   void SetLayerType(Vector::VectorType Type);
    /** Devuelve una lista con el contenido de los nodos hipervinculos */
   void GetHotlinkNodesList(std::list<std::string>& hotLinkNodesList) const;
   /** Obtiene el nombre de la capa activa **/
   std::string GetActiveLayerName() const;
   /** Metodo que devuelve el tipo de vectoren un string sin decorar. **/
   std::string GetStringType() const;
   std::vector<std::string> GetSelectedFeatures();
   /** Agrega un filtro a los atributos de la capa **/
   bool SetAttributeFilter(const std::string& filter);
   /** Devuelve atributos de la capa **/
   std::string GetAttributeFilter() const;
protected:
   /** Guarda el subset como el extent de la capa Layer en el nodo */
   virtual void SetLayerExtent(const Subset &LayerExtent, int Layer);
   /** Recorre las capas, calcula su extent y lo guarda en el nodo usando SetLayerExtent */
   virtual void SaveVectorExtent();
   /** Determina y agrega las dependencias */
   virtual void SetDependencies();
   /** Obtiene el nodo XML correspondiente a una capa */
   wxXmlNode *GetLayerNode(int Layer, wxXmlNode *pLayersNode) const;
   /** Carga un nodo XML correspondiente a una capa */
   wxXmlNode *SetLayerNode(int Layer, wxXmlNode *pLayersNode, const wxString &NodeContent);
   /** Configura el estilo con el que se dibujara la capa vectorial **/
   bool ConfigureStyle(Vector* pVector, Vector::VectorType type = Vector::Undefined);

private:
   /** Inicializa el elemento */
   virtual VectorElement* Initialize(const std::string &FileName,
                                     const Option& Metadata = Option());
   /** Capa activa que se utiliza */
   int activeLayer_;
   /** layerset */
   bool activeLayerSetted_;
   /** Asigna el tipo vectorial a cada una de las capas del elemento **/
   void SetChildrenType(wxXmlNode* pLayersNode, Vector::VectorType type);
   /** Devuelve vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> DoGetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);
};
}
#endif /*VECTORELEMENT_H_*/
