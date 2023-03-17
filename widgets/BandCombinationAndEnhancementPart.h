/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
