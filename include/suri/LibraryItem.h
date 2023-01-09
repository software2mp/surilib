/*! \file LibraryItem.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEM_H_
#define LIBRARYITEM_H_

// Includes Estandar
#include <string>
#include <map>
#include <utility>
#include <vector>

// Includes Suri
#include "suri/SuriObject.h"
#include "LibraryItemAttribute.h"
#include "suri/LibraryItemOrigin.h"

// Includes Wx

// Defines

namespace suri {

// forwards
class LibraryItemCatalog;

/*!
 * Clase que representa un item de biblioteca
 */
class LibraryItem : public suri::SuriObject {
   /** ctor copia **/
   LibraryItem(const LibraryItem&);

public:
   /** Constructor **/
   LibraryItem(const SuriObject::UuidType& LibraryId, const std::string& PrincipalValue);
   /** Destructor **/
   virtual ~LibraryItem();
   /**
    * Obtiene un atributo dado su nombre.
    * Precondicion: No existen dos atributos con el mismo nombre
    * @param[in] Name nombre del atributo que se quiere obtener
    * @return Attribute copia del atributo solicitado.
    * @return NULL en caso de no encontrar el atributo solicitado
    */
   const LibraryItemAttribute* GetAttribute(const std::string& Name) const;
   /**
    * Agrega un atributo con el nombre, tipo y valor pasado por paramtero
    * @param[in] Name nombre del atributo
    * @param[in] Type tipo del atributo
    * @param[in] Value valor del atributo
    * @return true si pudo agregar el atributo, false en caso contrario
    */
   bool AddAttribute(const std::string& Name,
                     const LibraryItemAttribute::AttributeTypeEnum& Type,
                     const std::string& Value);
   /**
    * Agrega el atributo pasado por parametro. Realiza una copia del atributo pasado por parametro.
    * @param[in] ItemAttribute atributo a agregar
    * @return true si pudo agregar el atributo, false en caso contrario
    */
   bool AddAttribute(LibraryItemAttribute* pItemAttribute);
   /**
    *
    * @param Attributes
    * @return
    */
   bool AddAttributes(std::vector<LibraryItemAttribute*>& Attributes);
   /**
    * Borra el atributo con nombre "Name"
    * @param[in] Name nombre del atributo a borrar
    * @return true en caso de poder borra el atributo, false en caso contrario
    */
   bool RemoveAttribute(const std::string& Name);
   /**
    * Reemplaza los atributos del item pasados por parametro
    * @param pAttributesVector atributos
    * @param Add boolean que indica si se agregan en caso de no encontrar
    * @return true en caso de reemplazar los atributos
    * @return false en caso contrario
    */
   bool ReplaceAttributes(std::vector<LibraryItemAttribute*>* pAttributesVector,
                          bool Add = true);


   /**
    * Elimina los atributos del item pasados por parametro.
    */
   bool RemoveAttributes(std::vector<LibraryItemAttribute*>* pAttributesVector);

   /**
    * Setea el valor del atributo con nombre "Name"
    * @param Name nombre del atributo a setear el valor
    * @param Value valor del atributo
    * @return true en caso de poder setear el valor del atributo
    * @return false en caso contrario (ej. nombre de atributo inexistente).
    */
   bool SetAttributeValue(const std::string& Name, const std::string& Value);
   /** Obtiene el atributo principal del item.*/
   const LibraryItemAttribute* GetPrincipalAttribute() const;
   /**
    * Devuelve la informacion necesaria para mostrar el item en el
    * catalogo de la biblioteca que lo contiene
    */
   LibraryItemCatalog* GetCatalogInformation() const;
   /** mapa de atributos **/
   typedef std::map<std::string, LibraryItemAttribute*> MapAttribute;
   /** Devuelve un mapa con todos los atributos que posee el item*/
   MapAttribute GetAttributes() const;
   /** Compara si dos items son iguales **/
   virtual bool Equals(const LibraryItem& Item) const;
   /** Metodo que devuelve el id de la biblioteca asociada **/
   SuriObject::UuidType GetLibraryAssociated() const;
   /** Obtiene el id del origen que creo el catalogo **/
   SuriObject::UuidType GetOriginId() const;
   /** Configura el id del origen al que pertenece el item **/
   void ConfigureOriginId(const SuriObject::UuidType& OriginId);
   /** Configura los atributos del item pisando los que tiene **/
   void SetAttributes(std::vector<LibraryItemAttribute*>& Attributes);

private:
   MapAttribute attributes_;
   /** Id de la biblioteca asociada **/
   SuriObject::UuidType libraryId_;
   /** Codigo que sirve para identificar de forma alternatia a UuidType**/
   LibraryItemAttribute* pPrincipal_;
   /** id del origen en el que se encuentra el item*/
   SuriObject::UuidType originId_;
};

} /** namespace suri */

#endif /* LIBRARYITEM_H_ */
