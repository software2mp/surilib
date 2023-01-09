/** \file VectorEditionTask.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITIONTASK_H_
#define VECTOREDITIONTASK_H_

// Includes standard
#include <set>
#include <string>

// Includes suri
#include "suri/TableEditionTask.h"
#include "suri/GuiGeometryEditorInterface.h"
#include "suri/GuiGeometryCreatorInterface.h"
#include "suri/World.h"

namespace suri {

// fordwards
class GuiGeometryEditor;
class GuiGeometryCreator;
class VectorDataLayer;
class ViewportManagerInterface;
class SnapTool;


/**
 * Tarea que administra la edicion/creacion de una capa vectorial.
 *
 * Guarda un cache con los cambios realizados en vector.
 * Justificacion: VectorRenderer trabaja con el datasource y no con
 * VectorDataLayer, para que se vean los cambios es necesario renderizarlos
 * OnPaint. Esto es ineficiente por lo que en el futuro se deberia mejorar.
 */
class VectorEditionTask : public TableEditionTask {
public:
   /** Nombre del campo que tiene la geometria */
   static const std::string GEOMETRY_COLUMN_NAME;

   /** Ctor */
   VectorEditionTask();
   /** Dtor */
   virtual ~VectorEditionTask();
   /** Configura el mundo que se usa para editar */
   bool SetWorld(World* pWorld);
   /** Retorna el mundo donde se realiza la edicion */
   World* GetWorld() const;
   /** Configura el srs y el tipo de vector editado */
   void ConfigureVectorData(VectorDatasource* pVectorDatasource);
   /** Comienza la tarea */
   virtual bool Start();
   /** Finaliza la tarea. Intenta guardar los cambios. */
   virtual bool End();
   /** Inicia la edicion de un feature existente */
   virtual bool StartFeatureEdition(FeatureIdType FeatureId);
   /** Finaliza la edicion de la geometria editada. */
   virtual bool EndFeatureEdition(bool SaveChanges = true);
   /** Retorna la geometria que se esta editando */
   virtual Geometry* GetEditedGeometry(std::string GeometryColumnName);
   /** Indica si el feature en edicion tiene una geometria. */
   bool EditedFeatureHasGeometry();
   /** Retorna editor que permite modificar la geometria. */
   GuiGeometryEditorInterface* GetFeatureEditor();
   /** Retorna editor que permite modificar la geometria. */
   GuiGeometryCreatorInterface* GetFeatureCreator();
   /** Elimina un feature de la capa en edicion  */
   bool DeleteFeature(FeatureIdType FeatureId);
   /** Retorna GeometryCollection con las geometrias modificadas por el usuario. */
   GeometryCollection* GetModifiedGeometries();
   /** Retorna GeometryCollection con las geometrias modificadas por el usuario. */
   GeometryCollection* GetDeletedGeometries();
   /** Retorna el ViewportManager */
   ViewportManagerInterface* GetViewportManager();
   /** Setea el ViewportManager */
   void SetViewportManager(ViewportManagerInterface* pViewportManager);
   /** Actualiza los visualizadores */
   void UpdateViewers();
   /** Nombre asignado a la tarea. */
   static const std::string TASKNAME;
   /** Agrega feature id a listado con las geometrias eliminadas */
   void AddDeletedFeature(FeatureIdType FeatureId);
   /** Busca el feature especificado en la lista de features modificados */
   bool IsModifiedFeature(FeatureIdType FeatureId);
   /** Elimina feature id de listado con las geometrias editadas/modificadas. */
   void RemoveModifiedFeature(FeatureIdType FeatureId);
   /** Setea un puntero a la herramienta de snapping existente */
   void SetSnapTool(SnapTool* Tool);
   /** Retorna un puntero a la herramienta de snapping */
   SnapTool* GetSnapTool();

private:
   /** Configura las columnas de tipo geometria que estan en la tabla. */
   virtual bool ConfigureGeometryColumnEditors();
   /** Funcion que indica si se puede iniciar la edicion */
   virtual bool CanStart();
   /** Crea el SelectionSource que hay que configurar al iniciar la tarea */
   virtual FeatureSelectionSource* CreateFeatureSelectionSource();
   /** Agrega feature id a listado con las geometrias editadas/modificadas. */
   void AddModifiedFeature(FeatureIdType FeatureId);
   /** Limpia listado con las geometrias editadas/modificadas. */
   void ClearModifiedFeatures();
   /** Limpia listado con las geometrias eliminadas */
   void ClearDeletedFeatures();
   /** Clase que permite trabajar con la seleccion */
   FeatureSelection* pFeatureSelection_;
   // --- Variables con datos de configuracion ---
   /** Fuente de datos que se esta editando */
   std::string vectorSrs_;
   /** Tipo de vector que se esta editando */
   Vector::VectorType vectorType_;
   /** Id del vector en edicion */
   SuriObject::UuidType datasourceId_;
   /** Mundo donde se realiza la edicion/creacion */
   World* pWorld_;
   // --- Clases que permiten editar graficamente el feature ---
   /** Se encarga de administrar la creacion de una geometria */
   GuiGeometryCreator* pGeometryCreator_;
   /** Se encarga de administrar la edicion de una geometria */
   GuiGeometryEditor* pGeometryEditor_;
   // --- CACHE CON FEATURES MODIFICADOS ---
   /** Vector con el id de los features modificados. */
   std::set<FeatureIdType> modifiedFeatures_;
   /** Cache con geometrias modificadas */
   GeometryCollection* pModifiedGeometries_;
   /** Cache con geometrias eliminadas */
   GeometryCollection* pDeletedGeometries_;
   ViewportManagerInterface* pViewportManager_;
   /** Herramienta de snapping que permite configurar las propiedades */
   suri::SnapTool *pSnapTool_;
};

} /* namespace suri */
#endif /* VECTOREDITIONTASK_H_ */
