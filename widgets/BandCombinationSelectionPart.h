/*! \file BandCombinationSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDCOMBINATIONSELECTIONPART_H_
#define BANDCOMBINATIONSELECTIONPART_H_

// Includes Suri
#include "BandSelectionPart.h"

// Include wx
#include "wx/checkbox.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(BandCombinationSelectionPartEvent);

/** Clase que permite cambiar la combinacion de bandas a renderizar de un elemento */
/**
 * Agrega dos listas al control de seleccion de bandas, una para seleccionar
 * combinacion de bandas RGB y una para escala de grises.
 * Modifica el nodo combinacion del elemento y actualiza el viewer.
 */
class BandCombinationSelectionPart : public BandSelectionPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(BandCombinationSelectionPartEvent);
   /** Constructor legacy*/
   explicit BandCombinationSelectionPart(RasterElement* pElement);
   /** Constructor */
   explicit BandCombinationSelectionPart(RasterLayer* pRasterLayer);
   /** Destructor */
   virtual ~BandCombinationSelectionPart();
   /** Crea la vantana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Configura las columnas de la lista de salida */
   void ConfigureOutputBandList();
   /** Carga los valores iniciales en las propiedades */
   virtual void SetInitialValues();
   /** Pasa los valores de las propiedades a los controles wx */
   virtual bool RollbackChanges();
   /** Devuelve si los controles tienen datos validos seleccionados */
   virtual bool HasValidData();
   /** Devuelve el estado de la propiedad modified_ */
   virtual bool HasChanged();
   /** Aplica la nueva seleccion de bandas al elemento. */
   virtual bool CommitChanges();
   /** Metodo que se ejecuta cuando se des/selecciona escala de grises */
   void OnGreyScaleChecked(wxCommandEvent &Event);
   /** Devuelve la posicion en la lista de salida en la que se inserta la proxima banda */
   virtual int GetNextInsertPosition();
   /** Inserta la banda indicada en la posicion indicada */
   virtual bool AddOutputBand(int BandIndex, int Position);
   /** Selecciona el item siguiente en el control wx */
   void SelectNextItem();
   /** Agrega el nodo combinacion de bandas al elemento */
   void SetBandCombinationNode();
   /** Devuelve una lista con las bandas seleccionadas */
   virtual std::vector<int> GetOutputBands();
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);
   /** Informa si esta en escala de grises */
   bool IsGreyScale();

protected:
   /** Selecciona como lista de salida la lista RGB */
   void SelectRgbBandList();
   /** Selecciona como lista de salida la lista en escala de grises */
   void SelectGreyScaleBandList();
   /** Cambia la lista activa. */
   std::vector<int> GetElementBandCombination();

private:
   EVENT_OBJECT_PROPERTY(BandCombinationSelectionPartEvent); /*! Objeto que */
   /* conecta los */
   /* eventos */
   RasterElement * pElement_; /*! Puntero al elemento (sobre el que se esta */
   RasterLayer* pRasterLayer_; /*! Puntero a la capa vectorial **/
   /* aplicando la herramienta) */
   wxListView * pOutputList_; /*! Puntero a la lista de bandas de salida */

   /** Elimina el tag de realce si existe. */
   void RemoveEnhancement(Element* pElement);
};
}

#endif /* BANDCOMBINATIONSELECTIONPART_H_ */
