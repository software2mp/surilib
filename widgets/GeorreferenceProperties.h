/*! \file GeorreferenceProperties.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEORREFERENCEPROPERTIES_H_
#define GEORREFERENCEPROPERTIES_H_

#include "suri/Element.h"
#include "suri/PartCollection.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryItemSelectionInterface.h"

#include "wx/wx.h"

namespace suri {

class DatasourceManagerInterface;
class DatasourceInterface;
class RasterElement;

DECLARE_EVENT_OBJECT(GeorreferencePropertiesEvent);

/**
 * Clase que carga en una ventana la informacion de georreferencia de un elemento,
 * solo es informativa y no permite modificar.
 * Los metodos heredados de part que sirven para aplicar cambios no tiene efecto.
 */
class GeorreferenceProperties : public Part, public LibraryItemSelectionInterface {
public:
   FRIEND_EVENT_OBJECT(GeorreferencePropertiesEvent);

   /** Ctor. */
   GeorreferenceProperties(Element* pElement, LibraryManager* pLibraryManager);

   /** Ctor. */
   GeorreferenceProperties(DatasourceManagerInterface* pDatasourceManager,
                           DatasourceInterface* pDatasource,
                           LibraryManager* pLibraryManager);

   /** Dtor. */
   virtual ~GeorreferenceProperties();

   /** Inicializa los controles */
   virtual void SetInitialValues();

   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged() { return srsChanged_ || modelChanged_; }

   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Actualiza el estado de la parte */
   virtual void Update() { }

   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();

   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /** Carga controles de Sistema Geografico */
   void LoadGeograficData();

   /** Carga controles de Sistema Proyectado */
   void LoadProjectionData();

   /** Carga el arbol de parametros */
   void LoadParametersTree();

   /**
    * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)
    */
   virtual void SetActiveItem(const LibraryItem* pItem);

   /**
    * Maneja aviso de que alguno de los textboxes del modelo
    * cambio.
    */
   void OnModelTextChangedHandler(wxCommandEvent &Event);

private:
   EVENT_OBJECT_PROPERTY(GeorreferencePropertiesEvent);

   Element* pElement_;
   DatasourceManagerInterface* pDatasourceManager_;
   DatasourceInterface* pDatasource_;
   LibraryManager* pLibraryManager_;
   bool srsChanged_;
   bool modelChanged_;
   std::string selectedSrs_;
   double selectedPixelX_;
   double selectedPixelY_;
   double selectedNorth_;
   double selectedModelP_;
   double selectedModelL_;
   double selectedModelX_;
   double selectedModelY_;
   bool hasSrs_;

   /**
    * Carga datos de modelo de imagen.
    */
   void LoadImageModel();

   /**
    * Configura el manejador para el evento de actualizacion
    * del textbox.
    */
   void SetupModelTextboxHandler(const std::string& Id);

   /**
    * Obtiene el valor del textbox.
    */
   std::string GetTextboxValue(const std::string& Id);

   /**
    * Inicializa valores internos.
    */
   void InitInternals();

   /** */
   void SaveSrsToElement(RasterElement* pRaster);

   /** */
   void SaveSrsToFile(RasterElement* pRaster);

   /** */
   void SaveModelToElement(RasterElement* pRaster);

   /** */
   void SaveModelToFile(RasterElement* pRaster);
};

}  // namespace suri

#endif  // GEORREFERENCEPROPERTIES_H_
