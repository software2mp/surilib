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

#ifndef GDALSRSITEMORIGIN_H_
#define GDALSRSITEMORIGIN_H_

// Includes standar
#include <string>
#include <map>

// Includes suri
#include "suri/LibraryItemOrigin.h"

namespace suri {

/** Clase que representa el origen de datos de los sistemas de referencia
 *  que posee la biblioteca gdal
 */
class GdalSrsItemOrigin : public LibraryItemOrigin {
public:
   /** Tipos de bibliotecas soportadas */
   enum SrsItemType {
      UNKNOWN = -1, PROJECTED_SRS = 0, GEOGRAFIC_SRS, DATUM, ELLIPSOID
   };
   /** Constructor */
   GdalSrsItemOrigin(const SuriObject::UuidType& LibraryId, SrsItemType OriginItemType,
                     const std::string& FavoritesIndexFileName);
   /** Destructor */
   virtual ~GdalSrsItemOrigin();
   /** Esta funcion inicializa los srs, datums y elipsoides soportados por gdal. */
   static void InitializeGdalSrsIndexs();
   /** Nombre del atributo que guarda WKT en item */
   static const std::string WKT_ATTRIBUTE_NAME;
   /** nombre del atributo que indica si el sr es proyectado o geo **/
   static const std::string ProjectedAttr;

protected:
   /** Agrega el item pasado por parametro a la fuente de datos del origen. */
    virtual bool AddItemToSource(LibraryItem* pItem);
    /** Elimina un item de la fuente de datos  */
    virtual bool RemoveItemFromSourceByCode(const std::string& Code);
    /**
     * Elimina el item de la fuente de datos del origen correspondiente al id
     * que se pasa por parametro
     * Metodo que tiene un comportamiento particular para cada tipo de origen.
     * (una clase concreta de origen tiene que reimplementar este metodo)
     */
    virtual bool RemoveItemFromSource(const SuriObject::UuidType& ItemId);
    /** Obtiene el item pasado por parametro a la fuente de datos del origen */
    virtual const LibraryItem* GetItemByCodeFromSource(const std::string& Code);
    /** Reemplaza un item de la fuente de datos del origen  */
    virtual bool ReplaceItemFromSource(LibraryItem* pItem);
    /** Permite actualizar el item en memoria */
    virtual bool UpdateItem(LibraryItem* pItem) const;

private:
    /** Metodo auxiliar que se encarga de cargar los items del origen **/
    void LoadItems();
    /** Configura LibraryItem con datos en wkt */
    bool ConfigureLibraryItem(const std::string &Wkt, LibraryItem *pLibraryItem) const;
    /** Metodo que retorna el nombre del archivo de indice  **/
    static std::string GetIndexFileName(GdalSrsItemOrigin::SrsItemType Type);
    /** Tipo de items que administra esta instancia de origen */
    SrsItemType libraryItemType_;
};


} /* namespace suri */
#endif /* GDALSRSITEMORIGIN_H_ */
