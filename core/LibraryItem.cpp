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

#include <string>
#include <vector>

#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttribute.h"
#include "suri/LibraryItemCatalog.h"

namespace suri {

/** Constructor **/
LibraryItem::LibraryItem(const SuriObject::UuidType& LibraryId,
                         const std::string& PrincipalValue) :
      libraryId_(LibraryId),
      pPrincipal_(
            new LibraryItemAttribute(LibraryItemAttribute::PrincipalAttrName,
                                     PrincipalValue)), originId_(SuriObject::NullUuid) {
   AddAttribute(pPrincipal_);
}

/** Destructor. Elimina todos los atributos del item **/
LibraryItem::~LibraryItem() {
   delete_second_all(attributes_);
}

/**
 * Obtiene un atributo dado su nombre.
 * Precondicion: No existen dos atributos con el mismo nombre
 * @param[in] Name nombre del atributo que se quiere obtener
 * @return Attribute copia del atributo solicitado.
 * @return NULL en caso de no encontrar el atributo solicitado
 */
const LibraryItemAttribute* LibraryItem::GetAttribute(const std::string& Name) const {
   MapAttribute::const_iterator findit = attributes_.find(Name);
   const LibraryItemAttribute* pitem = NULL;
   if (findit != attributes_.end()) {
      pitem = findit->second;
   }
   return pitem;
}

/**
 * Agrega un atributo con el nombre, tipo y valor pasado por paramtero
 * @param[in] Name nombre del atributo
 * @param[in] Type tipo del atributo
 * @param[in] Value valor del atributo
 * @return true si pudo agregar el atributo, false en caso contrario
 */
bool LibraryItem::AddAttribute(const std::string& Name,
                               const LibraryItemAttribute::AttributeTypeEnum& Type,
                               const std::string& Value) {
   bool added = false;
   MapAttribute::iterator findit = attributes_.find(Name);
   if (findit == attributes_.end()) {
      LibraryItemAttribute* pitem = new LibraryItemAttribute(Name, Value, Type);
      attributes_.insert(std::make_pair(Name, pitem));
      added = true;
   }
   return added;
}

/**
 *
 * @param Name
 * @return
 */
bool LibraryItem::AddAttributes(std::vector<LibraryItemAttribute*>& Attributes) {
   std::vector<LibraryItemAttribute*>::iterator it = Attributes.begin();
   bool ok = true;
   for (; ok && it != Attributes.end(); ++it) {
      ok = AddAttribute(*it);
   }
   return ok;
}

/**
 * Agrega el atributo pasado por parametro. Realiza una copia del atributo pasado por
 * parametro.
 * @param[in] pItemAttribute atributo a agregar
 * @return true si pudo agregar el atributo, false en caso contrario
 */
bool LibraryItem::AddAttribute(LibraryItemAttribute* pItemAttribute) {
   bool added = false;
   std::string name = pItemAttribute->GetName();
   MapAttribute::const_iterator findit = attributes_.find(name);
   if (findit == attributes_.end()) {
      attributes_.insert(std::make_pair(name, pItemAttribute));
      added = true;
   }
   return added;
}

/**
 * Borra el atributo con nombre "Name"
 *
 * @param[in] Name nombre del atributo a borrar
 * @return true en caso de poder borra el atributo, false en caso contrario
 */
bool LibraryItem::RemoveAttribute(const std::string& Name) {
   bool removed = false;

   MapAttribute::iterator removeit = attributes_.find(Name);
   if  (removeit != attributes_.end()) {
      LibraryItemAttribute* pattr = removeit->second;
      delete pattr;

      attributes_.erase(removeit);
      removed = true;
   }

   return removed;
}

/**
 * Reemplaza los atributos del item pasados por parametro
 * @param pAttributesVector atributos
 * @param Add boolean que indica si se agregan en caso de no encontrar
 * @return true en caso de reemplazar los atributos
 * @return false en caso contrario
 */
bool LibraryItem::ReplaceAttributes(std::vector<LibraryItemAttribute*>* pAttributesVector,
                       bool Add) {
   std::vector<LibraryItemAttribute*>::iterator it = pAttributesVector->begin();
   for (; it != pAttributesVector->end(); ++it) {
      MapAttribute::iterator findit = attributes_.find((*it)->GetName());
      if (findit != attributes_.end()) {
         LibraryItemAttribute* poldattribute =  findit->second;
         attributes_.erase(findit);
         delete poldattribute;
         attributes_.insert(std::make_pair((*it)->GetName(), *it));
      } else if (findit == attributes_.end() && Add) {
         attributes_.insert(std::make_pair((*it)->GetName(), *it));
      }
   }
   return true;
}

/**
 * Elimina los atributos del item pasados por parametro.
 */
bool LibraryItem::RemoveAttributes(std::vector<LibraryItemAttribute*>* pAttributesVector) {
   std::vector<LibraryItemAttribute*>::iterator it = pAttributesVector->begin();
   for (; it != pAttributesVector->end(); ++it) {
      RemoveAttribute((*it)->GetName());
   }
   return true;
}

/**
 * Obtiene el atributo principal del item.
 * @return Atributo principal del item
 */
const LibraryItemAttribute* LibraryItem::GetPrincipalAttribute() const {
   return pPrincipal_;
}

/**
 * Devuelve la informacion necesaria para mostrar el item en el
 * catalogo de la biblioteca que lo contiene.
 * La informacion necesaria para el catalogo es el atributo "Principal"
 * y el atributo "Extended". Este ultimo es optativo
 * @return NULL en caso de no poseer informacion para mostrar en el catalog
 */
LibraryItemCatalog* LibraryItem::GetCatalogInformation() const {
   LibraryItemCatalog* plibraryitemcatalog = NULL;
   if (pPrincipal_) {
      const LibraryItemAttribute* pextendedattr = GetAttribute(
            LibraryItemAttribute::ExtendedAttrName);
      plibraryitemcatalog = new LibraryItemCatalog(GetId(), pPrincipal_,
                                                   pextendedattr, originId_);
   }
   return plibraryitemcatalog;
}

/** Devuelve un mapa con todos los atributos que posee el item*/
LibraryItem::MapAttribute LibraryItem::GetAttributes() const {
   return attributes_;
}

/** Compara si dos items son iguales **/
bool LibraryItem::Equals(const LibraryItem& Item) const {
   return pPrincipal_->Equals(*Item.GetPrincipalAttribute());
}

/** Metodo que devuelve el id de la biblioteca asociada **/
SuriObject::UuidType LibraryItem::GetLibraryAssociated() const {
   return libraryId_;
}

/** Obtiene la categoria del origen que creo el catalogo **/
SuriObject::UuidType LibraryItem::GetOriginId() const {
   return originId_;
}

/** Configura la categoria del origen al que pertenece el item **/
void LibraryItem::ConfigureOriginId(const SuriObject::UuidType& OriginId) {
   originId_ = OriginId;
}

/** 
 * Configura los atributos del item pisando los que tiene 
 * @param[in] Attributes vector de items
 **/
void LibraryItem::SetAttributes(std::vector<LibraryItemAttribute*>& Attributes) {
	/** Me quedo con el extended para luego agregarlo a la lista si no se pisa **/
	LibraryItemAttribute* pextended = NULL;
for (MapAttribute::iterator attit = attributes_.begin();
			attit != attributes_.end(); ++attit) {
		LibraryItemAttribute* pitemattr = attit->second;
		/** Si no es principal o extended lo elimino **/
		if (pitemattr->GetName().compare(
				LibraryItemAttribute::PrincipalAttrName) != 0
				&& pitemattr->GetName().compare(
						LibraryItemAttribute::ExtendedAttrName) != 0) {
			delete pitemattr;
		} else if (pitemattr->GetName().compare(
				LibraryItemAttribute::ExtendedAttrName) == 0) {
			pextended = pitemattr;
		}
	}
	attributes_.clear();
	bool hasextended = false;
	std::vector<LibraryItemAttribute*>::iterator it = Attributes.begin();
	for (; it != Attributes.end(); ++it) {
		if ((*it)->GetName().compare(LibraryItemAttribute::PrincipalAttrName)
				== 0) {
			/** sobreescribo el atributo principal **/
			delete pPrincipal_;
			pPrincipal_ = *it;
		} else if ((*it)->GetName().compare(
				LibraryItemAttribute::ExtendedAttrName) == 0) {
			hasextended = true;
			/** Como el atributo extended esta dentro de la lista de atributos
			 *  no es necesario guardar el anterior **/
			delete pextended;
			pextended = NULL;
		}
		AddAttribute(*it);
	}
	/** Vuelvo a agregar el atributo principal **/
	if (pPrincipal_)
		AddAttribute(pPrincipal_);
	/** Agrego el atributo extended a la lista **/
	if (!hasextended && pextended)
		AddAttribute(pextended);
}

}  // namespace suri
