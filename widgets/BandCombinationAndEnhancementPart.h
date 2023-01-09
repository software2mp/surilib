/*! \file BandCombinationAndEnhancementPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDCOMBINATIONANDENHANCEMENTPART_H_
#define BANDCOMBINATIONANDENHANCEMENTPART_H_

// Includes suri
#include "suri/Part.h"
#include "BandCombinationSelectionPart.h"
#include "EnhancementSelectionPart.h"
#include "ColorTableSelectionPart.h"
#include "suri/LibraryManager.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(BandCombinationAndEnhancementPartEvent);

/** Clase que combina en un Part la seleccion de bandas y realce */
/**
 * Esta clase agrega en un mismo part, un part para la seleccion de bandas de
 * salida de la imagen, y un part para la seleccion del realce a aplicar a la
 * imagen. Cada uno de los metodos de esta clase encapsula y coordina a los
 * metodos de los part que incluye.
 */
class BandCombinationAndEnhancementPart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(BandCombinationAndEnhancementPartEvent);
   /** Ctor legacy*/
   explicit BandCombinationAndEnhancementPart(LibraryManager* pManager, RasterElement *pElement);
   /** Ctor */
   explicit BandCombinationAndEnhancementPart(LibraryManager* pManager, RasterLayer *pLayer);
   /** Dtor. */
   virtual ~BandCombinationAndEnhancementPart();
   /** Crea la ventana de la herramienta */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Retorna si la parte esta habilitada */
   virtual bool IsEnabled();
   /** Retorna si la combinacion de bandas seleccionadas es distinta a la del elemento */
   virtual bool HasChanged();
   /** Aplica la combinacion de bandas seleccionadas al elemento */
   virtual bool CommitChanges();
   /** Carga en los controles lo ultimos datos validos */
   virtual bool RollbackChanges();
   /** Carga en los controles la combinacion de bandas del elemento */
   void SetInitialValues();
   /** Metodo update para actualizar el estado de los Part */
   virtual void Update();
   /** Activa/desactiva el realce */
   virtual void ActivatEenhancement(bool Activate);

protected:
   /** Habilita la herramienta de tabla de colores. */
   void OnGrayScaleCheck(wxCommandEvent &Event);
   /** Deshabilita la herramienta de tabla de colores. */
   void OnEnhancementSelectionCheck(wxCommandEvent &Event);
#ifdef __UNUSED_CODE__
   /** Deshabilita la herramienta de realces. */
   void OnColorTableSelectionCheck(wxCommandEvent &Event);
#endif
   /** Si encuentra una inconsistencia en estados de habilitacion lo soluciona */
   void ConfigureWindows();

private:
   /*! Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(BandCombinationAndEnhancementPartEvent);
   /*! Parte de combinacion de bandas */
   BandCombinationSelectionPart * pBandCombinationPart_;
   /*! Parte de seleccion de realce */
   EnhancementSelectionPart * pEnhancementPart_;
   /*! Parte de seleccion de tabla de colores */
   ColorTableSelectionPart * pColorTablePart_;
   /*! Administrador de biblioteca **/
   LibraryManager* pManager_;
   /** Capa raster sobre la que se muesta la informacion de combinacion de bandas*/
   RasterLayer *pLayer_;
   /** Elemento que muestra la informacion de combinacion de bandas*/
   RasterElement* pElement_;
};
}

#endif /* BANDCOMBINATIONANDENHANCEMENTPART_H_ */
