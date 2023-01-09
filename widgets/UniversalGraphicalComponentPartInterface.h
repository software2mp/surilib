/*! \file UniversalGraphicalComponentPartInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef UNIVERSALGRAPHICALCOMPONENTPARTINTERFACE_H_
#define UNIVERSALGRAPHICALCOMPONENTPARTINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/LibraryItem.h"

// Includes Wx
// Defines
/** En combo muestra: 'atributo principal - atributo extendido' */
#define SINGLE_ITEM 0x100
#define NO_DROP_DOWN_FLAG 0x80
#define EXTENDED_COMBO_FLAG 0x40
#define LABEL_FLAG 0x20
#define COMBO_READ_FLAG 0x10
#define FULLFLEDGED_FLAG 0x08
#define SAVE_FLAG 0x04
#define DELETE_FLAG 0x02
#define FAVORITE_FLAG 0x01

// forwards

namespace suri {

// forwards
class LibraryItemEditorNotificator;

/*!
 *
 */
class UniversalGraphicalComponentPartInterface : public Part {
public:
   /** Combination de features para el layout y funcionalidad del part
    * ReadOnlyWithFullFledged: Se muestra solamente el boton de llamar a la biblioteca
    * y el combobox se muestra en solo lectura.
    * EditionWithFavorites: Se muestra los botones de salvar, borrar
    * y el check de favoritos
    * EditionWithoutFavorites: Se muestra solamente los botones de salvar, borrar
    * OnlySave: Solamente se muestra el boton de save (sin borrar ni favoritos)
    * SaveFavoritesAndNoLabel: se muestra el boton de salvar, check de favoritos
    * y no se muestra el label de nombre**/
   /** Enum con flags:
    *  label,combo(1 read, 0 readwrite),fullfledged,save,delete,favorites**/
   typedef enum {
      ReadOnlyWithFullFledged = LABEL_FLAG | COMBO_READ_FLAG | FULLFLEDGED_FLAG,
      ExtendedNoDropDownWithFullFledged = EXTENDED_COMBO_FLAG | COMBO_READ_FLAG
            | FULLFLEDGED_FLAG | NO_DROP_DOWN_FLAG,
      ExtendedNoDropDown = EXTENDED_COMBO_FLAG | COMBO_READ_FLAG | NO_DROP_DOWN_FLAG
            | SINGLE_ITEM | FAVORITE_FLAG,
      ExtendedROWithFF = EXTENDED_COMBO_FLAG | COMBO_READ_FLAG | FULLFLEDGED_FLAG,
      RoWithFFAndNoLabel = COMBO_READ_FLAG | FULLFLEDGED_FLAG,
      ExtendedRoWithFFAndNoLabel = EXTENDED_COMBO_FLAG | COMBO_READ_FLAG | FULLFLEDGED_FLAG,
      ExtendedRwWithFFAndNoLabel = EXTENDED_COMBO_FLAG | FULLFLEDGED_FLAG,
      EditionWithFavorites = SAVE_FLAG | DELETE_FLAG | FAVORITE_FLAG ,
      EditionWithoutFavorites = SAVE_FLAG | DELETE_FLAG,
      Save = LABEL_FLAG | SAVE_FLAG,
      SaveWithoutLabel = SAVE_FLAG,
      SaveFavoritesAndNoLabel = SAVE_FLAG | COMBO_READ_FLAG | FAVORITE_FLAG,
      SingleFavoritesAndNoLabel = SINGLE_ITEM | COMBO_READ_FLAG | FAVORITE_FLAG
            | EXTENDED_COMBO_FLAG
   } FeatureCombinationEnum;
   UniversalGraphicalComponentPartInterface(): Part(true, false) {}
   virtual ~UniversalGraphicalComponentPartInterface() {}
   /** Obtiene el item seleccionado**/
   virtual const LibraryItem* GetSelectedItem() = 0;
   /** Indica que se ha actualizado la informacion del item desde afuera. Esto hace
    * que se habilite el boton de guardar */
   virtual void ItemInformationChanged(bool IsValidData) = 0;
   /** Actualiza la informacion de un item que fue actualizado desde otro lado*/
   virtual void UpdateItemCatalogInformation(const LibraryItem* pItem) = 0;
   /** Devuelve el ultimo estado del check*/
   virtual bool GetLastCheckState() const = 0;
   /** Metodo que fuerza la seleccion de un item. En caso de no existir
    *  un item con ese atributo principal, el CGU debera mostrar el atributo
    *  principal pasado por parametro e impedir la seleccion de items*/
   virtual void SelectItemByPrincipal(const std::string& PrincipalAttribute)=0;
   /** Indica la biblioteca con cual trabajar. Solo se puede setear una vez **/
   virtual bool SetLibrary(const SuriObject::UuidType& LibraryId)=0;
   /** Actualiza la configuracion de botones a desplegar en base a la combinacion de
    *  caracteristicas elegida **/
   virtual void UpdateFeaturesLayout(FeatureCombinationEnum FeatureCombination) = 0;
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged()=0;
   /** Metodo que fuerza la actualizacion de la lista de catalogos que
    *  muestra el cgu
    */
   virtual void UpdateCatalogList()=0;
};

} /** namespace suri */

#endif /* UNIVERSALGRAPHICALCOMPONENTPARTINTERFACE_H_ */
