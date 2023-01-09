/*! \file VectorQueryInformationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORQUERYINFORMATIONPART_H_
#define VECTORQUERYINFORMATIONPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Includes App

// Defines


namespace suri {

class DataViewManager;

class VectorQueryInformationPart : public Part {
public:
   /** Ctor */
   explicit VectorQueryInformationPart(DatasourceInterface *pDatasource);

   /** Dtor */
   virtual ~VectorQueryInformationPart();

   /** crea la ventana del Part */
   virtual bool CreateToolWindow();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado del Part */
   virtual void Update();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   DatasourceInterface *pDatasource_;  /**! Fuente de datos a mostrar */
   Part *pLayerTablePart_; /**! parte que contiene la tabla de la consulta */

   /**
    * Obtiene un dataviewmanager para el table part.
    */
   DataViewManager* GetDataView();

   /**
    * Obtiene el nombre del vector en memoria.
    */
   std::string GetMemoryUrl(const std::string& FileNameP);
};
}  // namespace suri
#endif /* VECTORQUERYINFORMATIONPART_H_ */
