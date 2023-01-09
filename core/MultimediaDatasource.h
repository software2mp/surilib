/*! \file MultimediaDatasource.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
