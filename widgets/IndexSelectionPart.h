/*! \file IndexSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INDEXSELECTIONPART_H_
#define INDEXSELECTIONPART_H_

// Includes Standard
#include <vector>

// Includes suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
// todo provisorio. Falta mover BandMathRenderer->suri?
#include "BandMathRenderer.h"
#include "suri/LibraryItemSelectionInterface.h"

// forwards
class wxChoice;
class wxPanel;

/** namespace suri */
namespace suri {

// forwards
class BasicLibraryItemSelector;
class LibraryManager;
class BandSelectorWidget;

/** Clase para crear la parte de seleccion de indice y bandas */
/**
 * Esta clase es un part que sirve para seleccionar un indice (para aplicar
 * sobre un elemento) y las bandas requeridas.
 */

class IndexSelectionPart : public Part, public ProcessAtributeProvider,
                           public LibraryItemSelectionInterface {
public:
   /** Constructor */
   explicit IndexSelectionPart(const RasterElement * pElement, LibraryManager* pManager);
   /** Destructor */
   virtual ~IndexSelectionPart();
   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve el estado del Part. */
   bool IsEnabled();
   /** Indica si la parte tiene datos validos */
   bool HasValidData();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Por defecto la lista de bandas esta vacia */
   void SetInitialValues();
   /** Retorna una ecuacion configurada con los parametros seleccionados por el usario */
   bool GetSelectedEquation(BandMathRenderer::Parameters::Equation* &pEquation) const;
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /**
    * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)
    */
   virtual void SetActiveItem(const LibraryItem* pItem);

private:
   std::vector< std::pair<std::string, std::string> > GetVariableDescription(const LibraryItem* pItem) const;
   wxArrayString bandNames_; /*! Nombres de las bandas */
   int selectedIndex_; /*! Pagina seleccionada(= a indice seleccionado) */
   std::vector<int> selectedBands_; /*! Bandas seleccionadas */
   wxString errorMessage_; /*! mensaje de error */
   /*! Contiene informacion de las ecuaciones que usa cada indice */
   std::vector<BandMathRenderer::Parameters::Equation*> indexDescriptions_;
   BasicLibraryItemSelector* pItemSelector_;
   BandSelectorWidget* pBandSelector_;
};
}  // namespace suri
#endif /* INDEXSELECTIONPART_H_ */
