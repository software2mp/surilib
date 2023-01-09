/*! \file RasterDatasource.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERDATASOURCE_H_
#define RASTERDATASOURCE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/Element.h"
#include "suri/RasterElement.h"
#include "suri/Vector.h"
#include "suri/Image.h"
#include "suri/LayerInterface.h"
#include "suri/DatasourceValidatorInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que representa una fuente de datos tipo Raster
 */
class RasterDatasource : public suri::DatasourceInterface {
public:
   /** Constructor */
   RasterDatasource();
   /** Destructor */
   virtual ~RasterDatasource();
   /** Funcion estatica para crear un RasterDatasource */
   static RasterDatasource* Create(const std::string &FileName,
                                   const Option& Metadata = Option());
   /** Funcion estatica para crear un RasterDatasource */
   static RasterDatasource* Create(const wxXmlNode* pRootNode);
   /** Funcion estatica para crear un RasterDatasource \deprecated */
   static RasterDatasource* Create(Element* pElement);
   /** Funcion estatica para crear un DatasourceInterface */
   static RasterDatasource* Create(DatasourceInterface *pDatasource);
   /** Obtiene el elemento \deprecated */
   virtual Element* GetElement();
   /** Devuelve la interpretacion de la fuente de datos como una imagen */
   virtual Image* GetDatasourceAsImage();
   /** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual bool SetNextDatasource(DatasourceInterface* pDatasource);
   /** Devuelve la interpretacion de la fuente de datos como un vector */
   virtual Vector* GetDatasourceAsVector();
   /** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual DatasourceInterface* GetNextDatasource();
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
   /** Retorna la cantidad de bandas */
   int GetBandCount() const;
   /** Nombre del tipo de dato */
   std::string GetDataType() const;
   /** Devuelve una instancia del validador asociado a la fuente de datos**/
   virtual DatasourceValidatorInterface* GetAssociatedValidator()const;
   /** Obtiene el sistema de referencia que posee la fuente de datos*/
   virtual std::string GetSpatialReference() const;
   /** Devuelve true si un elemento esta listo para ser utilizado **/
   virtual bool IsValid() const;

private:
   /** Constructor de copia */
   RasterDatasource(const RasterDatasource &);
   /** Inicializa los atributos de Datasource a partir de un nombre de archivo */
   virtual RasterDatasource* Initialize(const std::string &FileName,
                                        const Option& Metadata = Option());
   /** Inicializa los atributos de Datasource a partir de un nodo xml */
   virtual RasterDatasource* Initialize(const wxXmlNode* pRootNode);
   /** Inicializa los atributos de Datasource a partir de un Element \deprecated */
   virtual RasterDatasource* Initialize(Element *pElement);
   /** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
   virtual RasterDatasource* Initialize(DatasourceInterface *pDatasource);
   /** Metodo auxiliar que agregar el nodo multiplexor a la fuente de datos**/
   void AddMultiplexorRenderer();
   Element *pElement_;                          /*! Puntero al elemento \deprecated */
   RasterDatasource* pNextRasterDatasource_;    /*! Puntero al siguiente RasterDatasource */
};

} /** namespace suri */

#endif /* RASTERDATASOURCE_H_ */
