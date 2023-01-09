/*! \file DatumEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATUMEDITIONPART_H_
#define DATUMEDITIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "LibraryItemEditorPart.h"
#include "suri/SuriObject.h"

namespace suri {

// forwards
class LibraryManager;
class UniversalGraphicalComponentPart;
class Table;
class TablePart;
DECLARE_EVENT_OBJECT(DatumParametersTableEventHandler);

/**
 * Permite setear los parametros de un datum,
 * para definir datum propios.
 *
 * Los parametros que permite configurar son:
 * - Nombre
 * - Autoridad:Codigo
 * - Elipsoide
 * - Parametros Bursa-Wolf (para transformar a WGS84)
 *
 */
class DatumEditionPart : public LibraryItemEditorPart {
   /** Nombre del XRC que representa la ventana */
   static const std::string DATUM_EDITION_XRC_NAME;
   static const std::string DATUM_EDITION_CGU_XRC_NAME;
   /** XRC que contendra al cgu para el elipsoide asociado **/
   static const std::string ELLIPSOID_EDITION_CGU_XRC_NAME;

   /** ctor copia **/
   DatumEditionPart(const DatumEditionPart&);

public:
   /** Ctor. Carga el XRC de la pantalla */
   DatumEditionPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Dtor. */
   virtual ~DatumEditionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

private:
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
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();

   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /** Configura la tabla para mostrar los parametros bursa wolf para conversion
    *  del datum a wgs84
    *  @param[in] Wkt wkt del datum
    */
   void ConfigureBursaWolfParametersTable(const std::string& Wkt, bool Enable);
   /** Obtiene el wkt en funcion de los parametros definidos en la tabla **/
   std::string GetParametersWkt();
   /** Metodo auxiliar que se llama cuando se presiona el boton de agregar fila **/
   void OnAddRowClick(const wxCommandEvent& Event);
   /** Metodo auxiliar que se llama cuando se presiona el boton de eliminar fila **/
   void OnDelRowClick(const wxCommandEvent& Event);
   /** adminsitrador de la biblioteca */
   LibraryManager* pManager_;
   /** Cgu que mostrara el elipsoide asociado al datum*/
   UniversalGraphicalComponentPart* pSpheroidCGgu_;
   /** Tabla que se usa para asignar las bandas */
   Table* pTable_;
   /** Part que contiene la tabla **/
   TablePart* pTablePart_;
   FRIEND_EVENT_OBJECT(DatumParametersTableEventHandler);
   EVENT_OBJECT_PROPERTY(DatumParametersTableEventHandler);
   /** Puntero al ultimo item seleccionado. Esto evita que se actualice todo el tiempo la tabla */
   const LibraryItem* pItemCurrentSel_;
};
}

#endif /* DATUMEDITIONPART_H_ */
