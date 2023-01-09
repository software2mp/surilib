/*! \file MetadataPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef METADATAPROPERTIESPART_H_
#define METADATAPROPERTIESPART_H_

#include "suri/Part.h"
#include "suri/SuriObject.h"
#include "suri/RasterElement.h"

#include "wx/bitmap.h"

namespace suri {

DECLARE_EVENT_OBJECT(MetadataPropertiesPartEvent);

class RasterDatasource;

class MetadataPropertiesPart : public suri::Part {
public:
   FRIEND_EVENT_OBJECT(MetadataPropertiesPartEvent);

   /**
    * Ctor.
    */
   MetadataPropertiesPart(RasterDatasource* pDatasource);

   /**
    * Dtor.
    */
   virtual ~MetadataPropertiesPart();

   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part.
    */
   virtual void SetInitialValues();

   /**
    * Actualiza el estado de la parte.
    */
   virtual void Update();

   /**
    * Crea la ventana de la parte.
    */
   virtual bool CreateToolWindow();

   /**
    * Retorna el icono de la herramienta.
    */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /**
    * Handler para el evento de click del boton guardar.
    */
   void OnSaveButtonClicked(wxCommandEvent &Event);

private:
   EVENT_OBJECT_PROPERTY(MetadataPropertiesPartEvent);

   RasterDatasource* pDatasource_;

   /**
    * Inicializa la lista.
    */
   void InitializeList();
};

}  // namespace suri

#endif  // METADATAPROPERTIESPART_H_
