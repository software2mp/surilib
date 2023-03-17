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

#ifndef UNIVERSALGRAPHICALCOMPONENTPART_H_
#define UNIVERSALGRAPHICALCOMPONENTPART_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri

#include "suri/LibraryClient.h"
#include "UniversalGraphicalComponentPartInterface.h"

// Includes Wx
#include "wx/event.h"
#include "wx/combobox.h"

// Defines
// forwards

namespace suri {

// forwards
class LibraryItemEditorNotificator;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(SaveEvent);
DECLARE_EVENT_OBJECT(DeleteEvent);
DECLARE_EVENT_OBJECT(CallFullFledgedLibraryEvent);
DECLARE_EVENT_OBJECT(MarkFavoriteEvent);
DECLARE_EVENT_OBJECT(ComboBoxSelectionEvent);

/*!
 *  Componente grafico universal para seleccion de items de biblioteca
 */
class UniversalGraphicalComponentPart : public UniversalGraphicalComponentPartInterface {
public:
   /** constructor **/
   UniversalGraphicalComponentPart(const SuriObject::UuidType& LibraryId,
                                   LibraryManager* pManager,
                                   LibraryItemEditorNotificator* pNotificator = NULL,
                                   FeatureCombinationEnum FeatureCombination =
                                   ReadOnlyWithFullFledged,
                                   bool FavoriteFilter = false);
   /** destructor **/
   virtual ~UniversalGraphicalComponentPart();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part */
   virtual bool RollbackChanges();
   /** Crea el Tool Window */
   bool CreateToolWindow();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Obtiene el item seleccionado**/
   virtual const LibraryItem* GetSelectedItem();
   /** Actualiza la informacion de un item que fue actualizado desde otro lado*/
   virtual void UpdateItemCatalogInformation(const LibraryItem* pItem);
   /** Indica que se ha actualizado la informacion del item desde afuera. Esto hace
    * que se habilite el boton de guardar */
   virtual void ItemInformationChanged(bool IsValidData);
   /** Devuelve el ultimo estado del check*/
   virtual bool GetLastCheckState() const;
   /** Metodo que fuerza la seleccion de un item. En caso de no existir
    *  un item con ese atributo principal, el CGU debera mostrar el atributo
    *  principal pasado por parametro e impedir la seleccion de items*/
   virtual void SelectItemByPrincipal(const std::string& PrincipalAttribute);
   /** Indica la biblioteca con cual trabajar. Solo se puede setear una vez **/
   virtual bool SetLibrary(const SuriObject::UuidType& LibraryId);
   /** Actualiza la configuracion de botones a desplegar en base a la combinacion de
    *  caracteristicas elegida **/
   virtual void UpdateFeaturesLayout(FeatureCombinationEnum FeatureCombination);
   /** Metodo que fuerza la actualizacion de la lista de catalogos que
    *  muestra el cgu
    */
   void UpdateCatalogList();

private:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SaveEvent);
   FRIEND_EVENT_OBJECT(DeleteEvent);
   FRIEND_EVENT_OBJECT(CallFullFledgedLibraryEvent);
   FRIEND_EVENT_OBJECT(MarkFavoriteEvent);
   FRIEND_EVENT_OBJECT(ComboBoxSelectionEvent);
   /** Objetos para conectar los eventos */
   SaveEvent* pSaveEventHandler_;
   DeleteEvent* pDeleteEventHandler_;
   CallFullFledgedLibraryEvent* pCallFullFledgedLibraryEventHandler_;
   MarkFavoriteEvent* pMarkFavoriteEventHandler_;
   ComboBoxSelectionEvent* pComboBoxEventHandler_;
   /** Metodo auxiliar para contectar los eventos a los botones*/
   void ConfigureButtonsEvents();
   /** Evento que se ejecuta al hacer click sobre boton de agregar clase.
    *  En caso de no existir en la lista el valor del combo se agrega un nuevo item
    *  y se agrega el valor al combobox.
    *  En caso de que el valor exista, dependiendo del estado del check de favoritos
    *  se agrega o elimina de la lista de favoritos*/
   void OnSaveButtonEventHandler(wxCommandEvent &Event);
   /** Evento que se ejecuta al hacer click sobre boton de eliminar */
   void OnDeleteButtonEventHandler(wxCommandEvent &Event);
   /** Evento que se ejecuta al hacer click sobre boton de llamar a biblioteca */
   void OnCallFullFledgedLibraryButtonEventHandler(wxCommandEvent &Event);
   /** Evento que se ejecuta al hacer click sobre el checkbox de favoritos */
   void OnMarkFavoriteEventHandler(wxCommandEvent &Event);
   /** Evento que se ejecuta al seleccionar un item en el combobox cambiando el
    * tooltip asociado **/
   void OnComboBoxSelectionEventHandler(wxCommandEvent &Event);
   /** Evento asociado al cambio del texto en el combobox **/
   void OnComboBoxTextChanged(wxCommandEvent &Event);
   /** Metodo auxiliar para cargar las caracteristicas del CGU **/
   void ConfigureFeatures(FeatureCombinationEnum FeatureCombination);
   /** Metodo auxiliar para obtener el item de catalogo correspondiente al valor del
    * atributo Principal que se pasa por parametro*/
   LibraryItemCatalog* GetCatalogItemByPrincipal(const std::string& Principal);
   /** Metodo auxiliar para eliminar el item de catalogo correspondiente al valor del
     * atributo Principal que se pasa por parametro*/
   SuriObject::UuidType RemoveCatalogByPrincipal(const std::string& Principal);
   /** Metodo auxiliars para obtener un item de catalog mediante el id del item*/
   LibraryItemCatalog* GetCatalogItemById(const SuriObject::UuidType& ItemId);
   /** Metodo auxiliar para reemplazar un item de la lista de catalogo del CGU*/
   bool ReplaceCatalogItem(LibraryItemCatalog* pcatalog, std::string* pOldValue);
   /** Retorna el contenido del combo a partir del catalogo del item a representar */
   std::string GetItemComboValue(LibraryItemCatalog* pCatalog);
   /** Retorna la posicion en el combo del item */
   int GetItemComboPosition(LibraryItemCatalog* pCatalog);
   /** Metodo auxiliar que obtiene el item de catalogo de biblioteca seleccionado
    *  en el combobox **/
   LibraryItemCatalog* GetLibraryItemCatalogSelected();
   /** caracteristicas configuradas */
   FeatureCombinationEnum featureCombinationActive_;
   /** Id de la biblioteca **/
   SuriObject::UuidType libraryId_;
   /** Client de la biblioteca (Analogo al Cgu::Modelo)**/
   LibraryClient cguClient_;
   std::vector<LibraryItemCatalog*> catalog_;
   bool lastCheckState_;
   bool favoriteFilter_;
   bool comboTextChanged_;
   LibraryItemEditorNotificator* pNotificator_;
   LibraryItemCatalog* pItemSelected_;
   /** Referencia al combobox del part. Util ya que se necesita en varios lados **/
   wxComboBox* pCombo_;
   /** Posicion del combo seleccionada. Necesario para rollback **/
   int comboPositionSelected_;
   /** atributo que indica cual fue el item seleccionado por principal. Es necesario
    *  este atributo ya que la primera seleccion se realiza antes de crear la ventana
    */
   std::string lastPrincipalSelected_;
   /** Administrador de bibliotecas **/
   LibraryManager* pManager_;
   /** bool que guarda el ultimo estado del check **/
   bool itemFavoriteChecked_;
};

} /** namespace suri */

#endif /* UNIVERSALGRAPHICALCOMPONENTPART_H_ */
