/*! \file WxsDataSourcePart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSDATASOURCEPART_H_
#define WXSDATASOURCEPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class Element;

/** Permite cargar una fuente wxs. */
/**
 * Se debe ingresar el url del servidor y el nombre de la capa.
 */
class WxsDataSourcePart : public Part {
public:
   /** Ctor */
	WxsDataSourcePart(bool Enable = true,
			bool Modified = false);
   /** Ctor */
   WxsDataSourcePart(Element* pElement, bool Enable = true, bool Modified = false);
   /** Dtor */
   virtual ~WxsDataSourcePart();

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

   /** Retorna el url construida con el nombre del servidor y la capa */
   std::string GetWxsElementUrl();
   /** Retorna la descripcion configurada por el usuario */
   std::string GetDescription();
   /* Modifica el contenido del control con string */
   void SetValue(std::string ControlName, const std::string Value);

protected:
   /** Configura los widgetParameters con datos en control */
   bool ReadParametersFromWidget();
   /** Actualiza el control con datos en los widgetParameters */
   bool UpdateParametersToWidget();

   Element* pElement_;  ///< Si no es NULL se le actualiza el url y la
                        ///< descripcion en cada commit.

   std::string url_;  ///< Servidor de la fuente wxs
   std::string layers_;  ///< Capas que se desea desplegar
   std::string description_;  ///< Descripcion de las capas

   std::string widgetUrl_;  ///< Servidor de la fuente wxs en control
   std::string widgetLayers_;  ///< Capas que se desea desplegar en control
   std::string widgetDescription_;  ///< Descripcion de las capas en control
};

}  //namespace

#endif /* WXSDATASOURCEPART_H_ */
