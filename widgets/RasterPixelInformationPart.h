/*! \file RasterPixelInformationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERPIXELINFORMATIONPART_H_
#define RASTERPIXELINFORMATIONPART_H_

#include "suri/Part.h"
#include "suri/Coordinates.h"

namespace suri {

class ParameterCollection;
class RasterDatasource;
class DataViewManager;
class World;

DECLARE_EVENT_OBJECT(RasterPixelInformationEvent);

/**
 * Muestra informacion del elemento raster, particularmente sobre el
 * pixel seleccionado.
 */
class RasterPixelInformationPart : public suri::Part {
public:
   FRIEND_EVENT_OBJECT(RasterPixelInformationEvent);

   /** Ctor */
   explicit RasterPixelInformationPart(ParameterCollection& Params);

   /** Dtor */
   virtual ~RasterPixelInformationPart();

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

   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();

   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   static const std::string kRasterPixelInformationXrcName;
   static const std::string kRasterPixelInformationTitle;

   EVENT_OBJECT_PROPERTY(RasterPixelInformationEvent); /*! Objeto para conectar eventos */

   Coordinates click_;
   Coordinates coords_;
   RasterDatasource* pRasterDs_;
   World* pViewerWorld_;

   /**
    * Obtiene un dataviewmanager para el table part.
    */
   DataViewManager* GetDataView();

   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);
};

}  // namespace suri

#endif  // RASTERPIXELINFORMATIONPART_H_
