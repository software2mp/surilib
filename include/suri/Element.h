/*! \file Element.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

// Include Standard
#include <string>
#include <vector>
#include <map>

// Include suri
#include "suri/XmlElement.h"
#include "Model.h"
#include "suri/Part.h"
#include "suri/xmlnames.h"
#include "suri/Subset.h"
#include "suri/FactoryHelpers.h"
#include "suri/Option.h"

class wxWindow;
class wxBitmap;

/** namespace suri */
namespace suri {
// Forwards
class ElementViewer;
/*! Clase dummy para generar Model-View especificos */
class Widget;
class DataViewManager;
class DatasourceInterface;
class LayerInterface;

/** Model base de los elementos de contexto */
/**
 * Esta clase representa un elemento generico que puede asociarse a cualquier
 * contexto.
 * Provee la funcionalidad generica de los elementos de contexto.
 * La funcionalidad especifica debera implementarse en las clases concretas
 * derivadas de esta.
 * \note
 *    -  Herada de XmlElement para poder guardar sus propiedades en el xml
 *       de descripcion
 *    -  Hereda de Model para notificar a los views sus cambios.(por ej cambio
 *       de url)
 */
class Element : public Model, public XmlElement {

private:
   /** declara metodos privados y publicos de la clase Context. */
HAS_FACTORY(Element, std::string);

DECLARE_CREATE(Element, const std::string& Value1 ,
               const Option& Value2 = Option());

DECLARE_CREATE(Element, const wxXmlNode* Value1, const Option& Value2 = Option());

public:
   /** tipo para el identificador unico de elementos */
   typedef std::string UidType;

   static const std::string uidNull_; /*! variable global, representa uid nulo */
   /** constructor */
   /**
    * @param[in] ElementType es el nombre (tipo) del elemento.
    */
   Element(const wxString& ElementType = TYPE_PROPERTY_VALUE_GENERIC);
   /** destructor */
   virtual ~Element();
   /** asigna el nombre del elemento */
   virtual void SetName(const wxString& ElementName);
   /** asigna la descripcion del elemento */
   virtual void SetDescription(const wxString& ElementDescription);
   /** asigna el url del elemento */
   virtual void SetUrl(const wxString &ElementUrl, wxXmlNode *pParentNode = NULL,
                       bool Replace = true);
   /** asigna el url del icono */
   virtual wxString SetIcon(const wxString& IconUrl);
   /** asigna el copyright del elemento */
   virtual void SetCopyRight(const wxString &ElementCopyright);
   /** Genera el nodo Archivo */
   void SetFileNode(const wxString &FileType, const wxString &FormatDescription =
         wxEmptyString);
   /** retorna la descripcion del elemento */
   virtual wxString GetDescription();
   /** retorna el archivo asociado */
   virtual wxString GetName() const;
   /** retorna el url asociado */
   virtual wxString GetUrl() const;
   /** retorna un identificador unico */
   virtual UidType GetUid() const;
   /** retorna el copyright asociado */
   virtual wxString GetCopyRight() const;
   /** retorna un string para el icono */
   virtual wxString GetIcon();
   /** retorna un string para el icono */
   virtual wxString DoGetIcon() const;
   /** crea una previsualizacion del elemento */
   virtual void GetPreview(wxBitmap &Preview, int Width = 0, int Height = 0) const;
   /** muestra los detalles */
   virtual wxString GetDetails() const {
      wxXmlNode *pnode = GetNode(DETAILS_NODE);
      if (pnode) {
         return pnode->GetNodeContent();
      }
      return wxT("");
   }
   /** retorna las dependencias del elemento */
   virtual std::vector<wxString> GetDependency() const;
   /** retorna el tipo de elemento */
   virtual wxString GetPropertyType() const;
   /** obtiene la referencia espacial del elemento */
   virtual wxString GetSpatialReference() const;
   /** carga la referencia espacial */
   virtual bool SetSpatialReference(const std::string &Wkt);
   /** Muestra la herramienta asociada */
   virtual wxWindow* GetPropertyWindow(wxWindow *pParent);
   /** Accesor para el estado de activo */
   bool IsActive() const {
      return isActive_;
   }
   /** Accesor para el estado de activo */
   void Activate(bool SetActive = true);

   /** Accesor para el estado de activo */
   void ActivateGIS(bool SetActive = true);

   /** Accesor para el estado de activo */
   bool IsEdited() const {
      return edited_;
   }
   /** Asigna el estado de edicion del elemento */
   void SetEdited(bool SetEdited = true);
   /** Informa si el estado del elemento es valido */
   bool IsValid();
   /** Actualiza datos xml con el elemento. */
   /**
    * Metodo que actualiza datos xml con el elemento. Solo deberia actualizar
    * datos que dependan del elemento y no de modificaciones que no se
    * manifiestan en el elemento
    */
   virtual void UpdateData() {
   }
   ;
   /** Devuelve el tamanio de un elemento como string. */
   wxString GetSizeAsString();
   /** Devuelve el tamanio de un elemento */
   wxULongLong GetSize();
   /** Genera vector con part de elemento y los que generen sus descendientes */
   virtual std::vector<suri::Part*> GetParts(DataViewManager* pDataViewManager = NULL,
                                             LayerInterface *pLayer = NULL,
                                             DatasourceInterface *pDatasource = NULL);
   /** Genera un vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> GetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);
   wxXmlNode *GetFileNode();
   /** Indica si se debe renderizar el elemento */
   virtual bool ShouldRender();
   /** Asigna la propiedad que indica si se debe renderizar */
   virtual void SetShouldRender(const bool ShouldRender = true) {
      shouldRender_ = ShouldRender;
   }
   /** Devuelve el extent del elemento. */
   virtual void GetElementExtent(Subset &ElementExtent);
   /** Agrega un archivo a la lista de dependencias */
   virtual void AddDependency(const std::string &Filename);
   /** Elimina un archivo de la lista de dependencias */
   virtual void DelDependency(const std::string &Filename, wxXmlNode *pDepNode = NULL);
   /** Elimina todos los archivos de la lista de dependencias. */
   virtual void DelDependency();
   /** Modifica el url de un archivo de la lista de dependencia. */
   virtual void ModifyDependency(const std::string &FileNameOld,
                                 const std::string &FileNameNew);

   /** Verifica si el elemento actual tiene un elemento asociado */
   bool HasAssociatedElement() {
      return (pAssociatedElement_ != NULL);
   }

   /** Obtiene el elemento asociado */
   Element* GetAssociatedElement() {
      return pAssociatedElement_;
   }

   /** Establece el elemento asociado */
   void SetAssociatedElement(Element* pAssociatedElement) {
      pAssociatedElement_ = pAssociatedElement;
   }

   /** Obtiene el mapa de metadatos asociado al elemento **/
   Option GetMetadataMap() {
      return metadata_;
   }

   /**
    * Obtiene, si esta disponible, el valor no valido para el elemento.
    */
   void GetNoDataValue(bool& Available, double& NoDataValue);
   void SetNoDataValue(double NoDataValue);

protected:
   /** Guarda los datos de Subset en pParentNode */
   void SetSubset(const Subset &Subset, wxXmlNode *pParentNode);
   /** Devuelve un subset con datos guardados en pnode. */
   void GetSubset(Subset &Subset, wxXmlNode *pParentNode) const;
   /** Lee un double de pNode */
   bool GetContentAsDouble(double &Value, wxXmlNode *pNode) const;
   /** Devuelve vector con Parts especificos del elemento. */
   virtual std::vector<suri::Part*> DoGetParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL) {
      return std::vector<suri::Part*>();
   }
   /** Devuelve vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> DoGetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL) {
      return std::vector<suri::Part*>();
   }

   bool shouldRender_; /*! Flag que indica si se debe renderizar el elemento. */
   Option metadata_;

private:
   bool isActive_; /*! Flag despliegue */
   bool edited_; /*! Flag que informa si elemento esta en edicion */
   Element* pAssociatedElement_;  // Elemento asociado

private:
   /** Formatea un Url para ser almacenado de forma correcta */
   virtual wxString NormalizeUrl(const wxString& Url);
};
}

#endif /*ELEMENT_H_*/
