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

// Includes App
#include "BandCombinationAndEnhancementPart.h"
#include "resources.h"
#include "RasterLayer.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/LibraryClient.h"

// Includes wx
#include "wx/xrc/xmlres.h"

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(BandCombinationAndEnhancementPartEvent,
      BandCombinationAndEnhancementPart)
      IMPLEMENT_EVENT_CALLBACK(OnGrayScaleCheck, OnGrayScaleCheck(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnEnhancementSelectionCheck,
                               OnEnhancementSelectionCheck(event), wxCommandEvent)
#ifdef __UNUSED_CODE__
      IMPLEMENT_EVENT_CALLBACK(OnColorTableSelectionCheck,
                               OnColorTableSelectionCheck(event), wxCommandEvent)
#endif
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 * Crea una instancia para la parte de combinacion de bandas y una para la parte
 * de realce.
 * @param[in] pElement: elemento sobre el que se aplica la herramienta.
 */
BandCombinationAndEnhancementPart::BandCombinationAndEnhancementPart(LibraryManager* pManager,
      RasterElement *pElement) :
      NEW_EVENT_OBJECT(BandCombinationAndEnhancementPartEvent), pColorTablePart_(NULL),
      pManager_(pManager), pLayer_(NULL), pElement_(pElement) {
   windowTitle_ = _(caption_BAND_COMBINATION_AND_ENHANCEMENT);
   pEnhancementPart_ = new EnhancementSelectionPart(pElement);
   pBandCombinationPart_ = new BandCombinationSelectionPart(pElement);
}

/** Ctor
 *  @param[in] pLayer capa vectorial
 */
BandCombinationAndEnhancementPart::BandCombinationAndEnhancementPart(LibraryManager* pManager,
                                                                     RasterLayer *pLayer) :
   NEW_EVENT_OBJECT(BandCombinationAndEnhancementPartEvent), pColorTablePart_(NULL),
   pManager_(pManager), pLayer_(pLayer), pElement_(NULL) {
   windowTitle_ = _(caption_BAND_COMBINATION_AND_ENHANCEMENT);
   pEnhancementPart_ = new EnhancementSelectionPart(pLayer);
   pBandCombinationPart_ = new BandCombinationSelectionPart(pLayer);
}

/**
 * Destructor
 */
BandCombinationAndEnhancementPart::~BandCombinationAndEnhancementPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea un panel con un sizer vertical y le agrega la ventana de la parte de
 * combinacion de bandas y la ventana de la parte de realce.
 * @return true si pudo agregar las ventanas
 * @return false si fallo agregar alguna ventana
 */
bool BandCombinationAndEnhancementPart::CreateToolWindow() {
   pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
   wxBoxSizer* pboxsizer = new wxBoxSizer(wxVERTICAL);
   pToolWindow_->SetSizer(pboxsizer);
   if (!pColorTablePart_) {
      LibraryClient client(pManager_, LibraryManager::READWRITE);
      SuriObject::UuidType densitysliceid =
      client.GetLibraryByCode(LibraryManagerFactory::
										DensitySliceCode)->GetId();
      if (pLayer_) {
         pColorTablePart_ = new ColorTableSelectionPart(this, pManager_, densitysliceid,
                                                        pLayer_);
      } else {
         pColorTablePart_ = new ColorTableSelectionPart(this, pManager_, densitysliceid,
                                                        pElement_);
      }
   }
   bool returnvalue = AddControl(pBandCombinationPart_, pToolWindow_,
                                 ControlArrangement(0))
         && AddControl(pEnhancementPart_, pToolWindow_, ControlArrangement(0))
         && AddControl(pColorTablePart_, pToolWindow_, ControlArrangement(0));
   if (returnvalue) {
      // Me conecto a los eventos
      GET_CONTROL(*pBandCombinationPart_->GetWindow(), wxT("ID_GREYSCALE_CHECKBOX"),
            wxCheckBox)->Connect(
               wxEVT_COMMAND_CHECKBOX_CLICKED,
               wxCommandEventHandler(
                     BandCombinationAndEnhancementPartEvent::OnGrayScaleCheck),
               NULL, pEventHandler_);

      GET_CONTROL(*pEnhancementPart_->GetWindow(), wxT("ID_CHOICE_CHECKBOX"),
            wxCheckBox)->Connect(
               wxEVT_COMMAND_CHECKBOX_CLICKED,
               wxCommandEventHandler(
                     BandCombinationAndEnhancementPartEvent::OnEnhancementSelectionCheck),
               NULL, pEventHandler_);
   }
   return returnvalue;
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void BandCombinationAndEnhancementPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
                                                      int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_BAND_COMBINATION_AND_ENHANCEMENT, ToolBitmap);
}

/**
 * Retorna si la parte esta habilitada.
 * @return true si alguna de las partes esta habilitada y la herramienta no fue
 *    desactivada con un SetEnabled(true)
 * @return false si ninguna de las partes esta habilitada o la herramienta fue
 *    desactivada con un SetEnabled(false)
 */
bool BandCombinationAndEnhancementPart::IsEnabled() {
   return Part::IsEnabled()
         && (pBandCombinationPart_->IsEnabled() || pEnhancementPart_->IsEnabled()
               || pColorTablePart_->IsEnabled());
}

/**
 * Retorna si la parte tiene cambios.
 * @return true si alguna de las partes tuvo cambios
 * @return false si ninguna de las partes tuvo cambios
 */
bool BandCombinationAndEnhancementPart::HasChanged() {
   return (pBandCombinationPart_->HasChanged() || pEnhancementPart_->HasChanged()
         || pColorTablePart_->HasChanged());
}

/**
 * Invoca al metodo commit de la parte de combinacion de bandas. Si el metodo
 * falla retorna, y no ejecuta el commit de la parte de realce.
 * Si la parte de seleccion de bandas tiene cambios, antes de ejecutar el
 * metodo commit de la parte de realce ejecuta el metodo refresh.
 * @return true si los metodos de las partes devolvieron true
 * @return false si fallo alguna de las partes
 */
bool BandCombinationAndEnhancementPart::CommitChanges() {
   bool returnvalue = false;
   bool refresh = pBandCombinationPart_->HasChanged();

   if (pBandCombinationPart_->CommitChanges()) {
      if (refresh) {
         pEnhancementPart_->Refresh();
      }
      returnvalue = pEnhancementPart_->CommitChanges();
      if (returnvalue) {
         returnvalue = pColorTablePart_->CommitChanges();
         if (pEnhancementPart_->IsChoiceActive()) {
            wxXmlNode* prenderzationnode = pLayer_->GetElement()->GetNode(
            RENDERIZATION_NODE);
            // es necesario agregar el nodo multiplexor para casos en raster de 1 banda
            if (!pLayer_->GetElement()->GetNode(
            RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_MULTIPLEXOR_NODE))
               pLayer_->GetElement()->AddNode(prenderzationnode,
               BAND_MULTIPLEXOR_NODE);
         }
      }
   }
   return returnvalue;
}

/**
 * Invoca a los metodos de las partes.
 * @return true si ambos metodos se ejecutaron con exito
 * @return false si fallo alguno de los metodos
 */
bool BandCombinationAndEnhancementPart::RollbackChanges() {
   bool returnvalue = pBandCombinationPart_->RollbackChanges()
         && pEnhancementPart_->RollbackChanges() && pColorTablePart_->RollbackChanges();

   ConfigureWindows();
   return returnvalue;
}

/**
 * Invoca a los metodos SetInitialValues de las partes.
 */
void BandCombinationAndEnhancementPart::SetInitialValues() {
   pBandCombinationPart_->SetInitialValues();
   pEnhancementPart_->SetInitialValues();
   pColorTablePart_->SetInitialValues();
   ConfigureWindows();
}

/**
 * Invoca a los metodos Update de las partes.
 */
void BandCombinationAndEnhancementPart::Update() {
   pBandCombinationPart_->Update();
   pEnhancementPart_->Update();
   pColorTablePart_->Update();
}

/** Habilita la herramienta de tabla de colores. */
void BandCombinationAndEnhancementPart::OnGrayScaleCheck(wxCommandEvent &Event) {
   pColorTablePart_->Activate(Event.IsChecked());
}

/** Deshabilita la herramienta de tabla de colores. */
void BandCombinationAndEnhancementPart::OnEnhancementSelectionCheck(
      wxCommandEvent &Event) {
   if (Event.IsChecked())
      pColorTablePart_->Check(false);
   Event.Skip();
}

#ifdef __UNUSED_CODE__
/** Deshabilita la herramienta de realces. */
void BandCombinationAndEnhancementPart::OnColorTableSelectionCheck(
      wxCommandEvent &Event) {
   if (Event.IsChecked()) {
      if (!pBandCombinationPart_->IsGreyScale()) {
         REPORT_DEBUG(
               "D: Error, la herramienta de tabla de colores esta activa sin escala de grises");
         return;
      }
      pEnhancementPart_->ActivateChoice(false);
   }

   Event.Skip();
}
#endif

/** Si encuentra una inconsistencia en estados de habilitacion lo soluciona */
void BandCombinationAndEnhancementPart::ConfigureWindows() {
   pColorTablePart_->Activate(pBandCombinationPart_->IsGreyScale());
}

/** Activa/desactiva el realce */
void BandCombinationAndEnhancementPart::ActivatEenhancement(bool Activate) {
   pEnhancementPart_->ActivateChoice(Activate);
}
} /** namespace suri **/
