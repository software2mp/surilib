/*! \file LibraryItemEditorPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMEDITORPART_H_
#define LIBRARYITEMEDITORPART_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class LibraryItemEditorNotificator;

/*!
 * Clase base que representa un editor de items de biblioteca. Este brinda la interfaz
 *  y comportamiento comun a todos los editores.
 */
class LibraryItemEditorPart : public Part {
public:
   /** ctor **/
   explicit LibraryItemEditorPart(
         const SuriObject::UuidType& LibraryId,
         LibraryManager* pManager,
         const std::string& WxWindowdId,
         const std::string& WxCguPanelId,
         bool FilterFavorites = false,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites,
         LibraryManager::AccessType Access = LibraryManager::ALL);
   /** dtor **/
   virtual ~LibraryItemEditorPart();
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
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Configura el editor con el item que se pasa por parametro*/
   void SetActiveItem(const LibraryItem* pItem,
                      bool FirstSelected = false, bool Modifiable = true);
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields() {
      return NULL;
   }

   /**
    * Obtiene un vector con los atributos marcados para ser eliminados.
    */
   virtual std::vector<LibraryItemAttribute*>* GetAttributesToRemove() {
      return NULL;
   }

   /**
    * Metodo para hacer limpieza. Este codigo esta puesto para cubrir una
    * emergencia. Es para salir del paso. Cuando los parts dejen de existir
    * esto va a desaparecer y no hay problema.
    */
   virtual bool CleanUp();

   /** Indica la biblioteca con cual trabajar. Solo se puede setear una vez **/
   virtual bool SetLibrary(const SuriObject::UuidType& LibraryId);

   /** Obtiene el item activo en el editor **/
   const LibraryItem* GetActiveItem();
   /** Metodo estatico que se encarga de realizar la creacion del editor correspondiente
    *  al item que se pasa por parametro*/
   static LibraryItemEditorPart* Create(
         LibraryManager* pManager,
         const LibraryItem* pItem,
         const std::string& LibraryCode,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites);

protected:
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures() {
      return true;
   }

   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true) {
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
   virtual bool OnCommitChanges();
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el rollback involucra objetos adicionales a los items de biblioteca
    * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
    * los mismos)
    * @return true en caso de realizar correctamente el rollback "interno"
    * @return false en caso de que ocurra algun error durante el rollback
    */
   virtual bool OnRollback();
   LibraryClient editorClient_;
   SuriObject::UuidType libraryId_;
   LibraryItemEditorNotificator* pNotificator_;
   UniversalGraphicalComponentPartInterface* pCgu_;
   bool validData_;
   /** Id de la ventana que crea el part **/
   std::string wxWindowdId_;
   /** Id del panel en donde se ubica el cgu **/
   std::string wxCguPanelId_;
   /** Acceso que se desea configurar al editor (RO, RW) */
   LibraryManager::AccessType access_;
   /** bool que indica si el cgu se filtra por favoritos **/
   bool filterFavorites_;
   LibraryManager* pManager_;
};

} /** namespace suri */

#endif /* LIBRARYITEMEDITORPART_H_ */
