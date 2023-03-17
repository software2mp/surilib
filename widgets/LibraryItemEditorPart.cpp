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

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "LibraryItemEditorPart.h"
#include "LibraryItemEditorNotificator.h"
#include "UniversalGraphicalComponentPart.h"
#include "LibraryItemEditorDebugPart.h"
#include "GdalSrsItemOrigin.h"
#include "SpatialReferenceEditionPart.h"
#include "SpatialReference.h"
#include "DatumEditionPart.h"
#include "DensitySliceEditionPart.h"
#include "WmsEditionPart.h"
#include "SpectralSignEditionPart.h"
#include "ConvolutionFiltersEditorPart.h"
#include "suri/LibraryManagerFactory.h"
#include "EllipsoidEditionPart.h"
#include "VectorPropertiesPart.h"
#include "EquationInSituEditorPart.h"

// Includes Wx

// Defines

// forwards

namespace suri {

class VectorLayer;

/**
 * Constructor
 * @param LibraryId Id de la biblioteca a editar
 * @param pManager Manager de Biblioteca
 * @param WxWindowdId String con el ID del panel principal del Part
 * @param WxCguPanelId String con el ID del panel en donde se ubica el CGU
 * @param FilterFavorites bool que indica si se filtra el CGU por favoritos
 * @param FeatureCombination combinacion de caracteristicas a mostrar en el CGU
 * @param Access acceso a la biblioteca requerido
 */
LibraryItemEditorPart::LibraryItemEditorPart(
		const SuriObject::UuidType& LibraryId,
		LibraryManager* pManager,
		const std::string& WxWindowdId,
		const std::string& WxCguPanelId,
		bool FilterFavorites,
		const UniversalGraphicalComponentPart::FeatureCombinationEnum& FeatureCombination,
		LibraryManager::AccessType Access) :
		Part(true, false), editorClient_(pManager, Access), libraryId_(
				LibraryId), pNotificator_(
				new LibraryItemEditorNotificator(this)),
				pCgu_(pManager ?
				      new UniversalGraphicalComponentPart(libraryId_, pManager,
						pNotificator_, FeatureCombination, FilterFavorites)
                  : NULL), validData_(
				true), wxWindowdId_(WxWindowdId), wxCguPanelId_(WxCguPanelId), access_(
				Access), filterFavorites_(FilterFavorites) {
}

/** dtor **/
LibraryItemEditorPart::~LibraryItemEditorPart() {
}

/** Indica si el Part tiene cambios para salvar */
bool LibraryItemEditorPart::HasChanged() {
	return modified_ || (pCgu_ && pCgu_->HasChanged());
}

/** Salva los cambios realizados en el Part */
bool LibraryItemEditorPart::CommitChanges() {
	/** Primero realiza un commit "interno" y luego sigue con la interaccion
	 *  con la biblioteca  */
	if (HasChanged() && HasValidData() && OnCommitChanges() && pCgu_) {
		// Se obtiene el item seleccionado en el CGU
		// TODO(TCK #2249): Notar que el CGU no agrega el item a la biblioteca si este
		// no existe y que ademas devuelve un const pointer, con lo que no lo
		// puedo agregar asi nomas.
		pCgu_->CommitChanges();
		const LibraryItem* pitem = pCgu_->GetSelectedItem();
		// Se obtienen los atributos modificados
		std::vector<LibraryItemAttribute*>* pattributes =
				CreateAttributesFromFields();
		if (pitem != NULL && pattributes != NULL) {
			// se replica el cambio en la biblioteca
			editorClient_.SetAttributesFromItem(libraryId_, pitem->GetId(), pattributes);
			delete pattributes;
		}

		// Atributos eliminados. En la version 2.6 esto se venia haciendo de otra forma
		// pero aca pinto hacerlo asi entonces adaptamos todas las modificaciones :)
		pattributes = GetAttributesToRemove();
      if (pitem != NULL && pattributes != NULL) {
         // se replica el cambio en la biblioteca
         editorClient_.RemoveAttributesFromItem(libraryId_, pitem->GetId(), pattributes);
         delete pattributes;
      }

      CleanUp();

		modified_ = false;
		return true;
	}
	return true;
}

/** Restaura los valor's originales del Part */
bool LibraryItemEditorPart::RollbackChanges() {
   if (!pCgu_)
      return true;
	pCgu_->RollbackChanges();
	UpdateFields(pCgu_->GetSelectedItem());
	modified_ = false;
	return OnRollback();
}

/** Crea el Tool Window */
bool LibraryItemEditorPart::CreateToolWindow() {
	pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
			wxT(wxWindowdId_.c_str()));
	if (!pToolWindow_)
		return false;
	if (pCgu_)
	   AddControl(pCgu_->GetWidget(), wxT(wxCguPanelId_.c_str()));
	ConfigureFeatures();
    modified_ = false;

	return true;
}

/** Inicializa el part */
void LibraryItemEditorPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void LibraryItemEditorPart::Update() {
}

/** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
void LibraryItemEditorPart::SetActiveItem(const LibraryItem* pItem,
		bool FirstSelected, bool Modifiable) {
   if (pCgu_ && pItem != NULL && !FirstSelected) {
      pCgu_->SelectItemByPrincipal(pItem->GetPrincipalAttribute()->GetValue());
   } else if (!pItem) {
      pCgu_->UpdateCatalogList();
   }
	UpdateFields(pItem, Modifiable);
	modified_ = (!FirstSelected);
}

/**
 * Metodo para hacer limpieza. Este codigo esta puesto para cubrir una
 * emergencia. Es para salir del paso. Cuando los parts dejen de existir
 * esto va a desaparecer y no hay problema.
 */
bool LibraryItemEditorPart::CleanUp() {
   return true;
}

/** Devuelve si la parte tiene datos validos */
bool LibraryItemEditorPart::HasValidData() {
	return validData_;
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool LibraryItemEditorPart::OnCommitChanges() {
	return true;
}

/** Indica la biblioteca con cual trabajar. Solo se puede setear una vez **/
bool LibraryItemEditorPart::SetLibrary(const SuriObject::UuidType& LibraryId) {
	if (libraryId_.compare(SuriObject::NullUuid) != 0)
		return false;
	libraryId_ = LibraryId;
	return true;
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el rollback involucra objetos adicionales a los items de biblioteca
 * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
 * los mismos)
 * @return true en caso de realizar correctamente el rollback "interno"
 * @return false en caso de que ocurra algun error durante el rollback
 */
bool LibraryItemEditorPart::OnRollback() {
	return true;
}

/** Obtiene el item activo en el editor **/
const LibraryItem* LibraryItemEditorPart::GetActiveItem() {
	return pCgu_ ? pCgu_->GetSelectedItem() : NULL;
}

/** Metodo estatico que se encarga de realizar la creacion del editor correspondiente
 *  al item que se pasa por parametro
 * @param pManager
 * @param pItem
 * @pasram LibraryCode
 * @param Feature
 * @return
 */
LibraryItemEditorPart* LibraryItemEditorPart::Create(
		LibraryManager* pManager,
		const LibraryItem* pItem,
		const std::string& LibraryCode,
		const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature) {
	LibraryItemEditorPart* peditor = NULL;
	if (!pItem)
		return NULL;
	if (LibraryCode.compare(LibraryManagerFactory::SpatialReferenceLibraryCode)
			== 0) {
		peditor = new SpatialReferenceEditionPart(pItem->GetLibraryAssociated(),
				pManager, Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::DatumLibraryCode)
			== 0) {
		peditor = new DatumEditionPart(pItem->GetLibraryAssociated(), pManager,
				Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::EllipsoidLibraryCode)
			== 0) {
		peditor = new EllipsoidEditionPart(pItem->GetLibraryAssociated(),
				pManager, Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::DensitySliceCode)
			== 0) {
		peditor = new DensitySliceEditionPart(pItem->GetLibraryAssociated(),
				pManager, Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::WmsLibraryCode)
			== 0) {
		peditor = new WmsEditionPart(pItem->GetLibraryAssociated(), pManager,
				Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::VectorstyleLibraryCode)
			== 0) {
		VectorLayer* player = NULL;
		peditor = new ui::SimpleVectorStylePart(pItem->GetLibraryAssociated(),
				pManager, player, Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::SpectralSignLibraryCode)
         == 0) {
      peditor = new SpectralSignEditionPart(pItem->GetLibraryAssociated(),
            pManager, Feature);
	} else if (LibraryCode.compare(LibraryManagerFactory::ConvolutionFiltersCode)
         == 0) {
      peditor = new ConvolutionFiltersEditorPart(pItem->GetLibraryAssociated(),
            pManager, Feature);
   } else if (LibraryCode.compare(LibraryManagerFactory::EquationLibraryCode) == 0 ||
                           LibraryCode.compare(LibraryManagerFactory::IndexLibraryCode) == 0) {
      peditor = new EquationInSituEditorPart(pItem->GetLibraryAssociated(), pManager, Feature);
   } else {
		peditor = new LibraryItemEditorDebugPart(pItem->GetLibraryAssociated(),
				pManager, Feature);
	}
	return peditor;
}

} /** namespace suri */

/** @mainpage
 *
 * @page "suri::LibraryItemEditorPart" Tutorial LibraryItemEditorPart: Uso y creacion de editores concretos.
 *
 * LibraryItemEditorPart es la clase base para los editores in-situ de items de biblioteca.
 *
 * Representa un editor de items de biblioteca. Este brinda la interfaz y comportamiento
 * comun a todos los editores. Tiene el comportamiento basico y comun a todos los editores
 * (interaccion con el sistema de bibliotecas y comunicacion con el componente CGU).
 *
 * Cada editor particular debe heredar de esta clase y reimplementar los metodos
 * que hacen a la particular de lo que se quiere modelar.
 *
 * Comnicaciones/Notificaciones entre Editor y CGU
 *
 * La clase encargada de la interaccion Editor-CGU es LibraryItemEditorNotificator.
 *
 * Se presentan los siguientes casos de uso para la comunicacion:
 *
 *    - Cambio de seleccion en CGU
 *
 *    - Informacion invalida o valida en editor
 *
 *    - Informacion modificada en el editor (involucra 3.)
 *
 *    - Se presiona boton Guardar en CGU
 *          -# Se obtiene informacion modificada en Editor. Si la informacion no es valida \n
 *       no se debe guardar el item.
 *          -# Se obtiene el texto del item del CGU (el atributo que lo describe) y se muestra \n
 *       en el pulldown. Si no existe ese item en la lista inicial, se crea.
 *          -# Se replica el cambio al sistema de biblioteca mediante el cliente de biblioteca \n
 *       de CGU.
 *
 *    - Se presiona boton Aceptar en editor (CommitChanges)
 *          -# Se obtiene el item seleccionado actualmente (si bien se notifica el cambio \n
 *          de seleccion de item, puede ocurrir que en vez de un cambio de seleccion \n
 *          ocurra una modificacion sobre el texto del pulldown generando un nuevo item).
 *          -# Se obtiene la informacion del item desde el editor (sus atributos).
 *          -# Se replica el cambio al sistema de biblioteca mediante el cliente de biblioteca \n
 *          de Editor.
 *
 * Como se dijo anteriormente, cada editor debe reimplementar los metodos que otorgan
 * particularidad al comportamiento del mismo y estos son:
 *
 * @code
 * virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields()=0;
 * virtual bool ConfigureFeatures() =0;
 * virtual void UpdateFields(const LibraryItem* pItem)=0;
 * @endcode
 *
 * CreateAttributesFromFields: Metodo que obtiene a partir de los datos del editor,
 * un vector con los atributos. Este metodo es el que hace el comportamiento particular
 * de cada editor a la hora de obtener la informacion que posee y transmitirla al
 * sistema de bibliotecas. Es utilizado a modo de Template Method (en CommitChanges
 * y en el notificador).
 *
 * Devuelve NULL si la informacion en el editor no es valida
 *
 * ConfigureFeatures: Metodo auxiliar privado que configura el part en si para cada
 * editor en particular. Este metodo es el que realmente tienen que implementar
 * las clases hijas para lograr la especializacion a la hora de iniciar el part
 * (metodo que se llama en CreateToolWindow, a modo de Template Method).
 * UpdateFields:   Metodo auxiliar privado que es necesario implementar en las clases hijas.
 *
 * Este metodo es el encargado de actualizar realmente la informacion en el part.
 *
 * Es llamado en el metodo SetActiveItem a modo de Template Method.
 *
 * Por lo que para crear un editor in-situ particular basta con implementar esos metodos.
 * No es necesario reimplementar todos los metodos pertenecientes a la logica
 * de Part (commit, rollback, createtoolwindow, etc.)
 *
 * Veamos una clase ejemplo:
 *
 * @code
 * class EditorEjemplo : public LibraryItemEditorPart {
 * public:
 *    virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields() {
 *       // A partir de los campos del editor creo un vector con los atributos
 *       // del item
 *    }
 *
 * private:
 *    virtual bool ConfigureFeatures() {
 *       // Configuro botones y diferentes componentes que tenga el part
 *       // este metodo se llama desde el CreateToolWindow de la clase padre
 *    }
 *    virtual void UpdateFields(const LibraryItem* pItem) {
 *       // Actualizo la informacion mostrada en el editor
 *    }
 * };
 * @endcode
 */

