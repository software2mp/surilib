/** \file GdalSrsItemOrigin.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
