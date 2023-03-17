/** \file VectorEditionTool.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITIONTOOL_H_
#define VECTOREDITIONTOOL_H_

// Includes suri
#include "GenericTool.h"

namespace suri {

// fordwards
class FeatureSelection;
class VectorEditionTask;
class Vector;
class DatasourceManipulatorInterface;
class DatasourceValidatorInterface;
class SnapTool;

/**
 * Comandos que permiten editar vectores.
 */
class VectorEditionTool : public suri::GenericTool {
protected:
   /** Ctor */
   VectorEditionTool(DataViewManager* pDataViewManager,
                     DatasourceValidatorInterface* pValidator);
public:
   /** Ctor */
   VectorEditionTool(DataViewManager* pDataViewManager);
   /** Dtor */
   virtual ~VectorEditionTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

   /** Inicia la tarea de edicion vectorial con un nuevo vector */
   bool StartTask(Vector::VectorType NewVectorType,
            DatasourceManipulatorInterface* pManipulator = NULL);
   /** Inicia la tarea de edicion vectorial */
   bool StartTask(VectorDatasource* pDatasource);
   /** Tarea asociada a la edicion vectorial */
   VectorEditionTask* pVectorEditionTask_;
   /** Guarda referencia a vector en memoria mientras se edita. */
   Vector* pMemoryVector_;
   /** Fuente de datos que se esta creando*/
   VectorDatasource* pMemoryDatasource_;
   /** Validador que afecta al vector en edicion. NULL por default */
   DatasourceValidatorInterface* pValidator_;

private:
	/** Metodo auxiliar que persiste la seleccion dentro de la capa **/
	void SaveVectorSelectionToLayer(LayerInterface* pLayer) const;
   /** Crea un vector en memoria del tipo indicado. */
   Vector* CreateMemoryVector(Vector::VectorType NewVectorType, std::string& MemUrl) const;
   /** Inicia la seleccion de features */
   void ExecuteStartSelectVectorFeatures() const;
   /** Finaliza la seleccion de features **/
   void ExecuteEndSelection() const;
   /** Crea una capa vectorial en memoria de tipo correcto */
   void ExecuteCreateVectorDatasource();
   /** Crea una capa vectorial en memoria de tipo poligono */
   void ExecuteCreatePolygonDatasource();
   /** Crea una capa vectorial en memoria de tipo linea */
   void ExecuteCreateLineDatasource();
   /** Crea una capa vectorial en memoria de tipo punto */
   void ExecuteCreatePointDatasource();
   /** Inicializa la herramienta de snapping */
   void ExecuteStartSnapTool();
   /** Finaliza la herramienta de snapping */
   void ExecuteEndSnapTool();
   /** Inicia la tarea de edicion vectorial con vector seleccionado */
   void ExecuteStartTask();
   /** Finaliza la tarea de edicion vectorial */
   void ExecuteEndTask();
   /** Finaliza la edicion grafica de features, no finaliza la tarea. */
   void ExecuteEndEditFeatureDatasource() const;
   /** Finaliza DeletePoint, no finaliza la tarea. */
   void ExecuteEndDeletePointDatasource() const;
   /** Finaliza la creacion grafica de features, no finaliza la tarea. */
   void ExecuteEndCreateFeatureDatasource() const;
   /** Refresca el visualizador en edicion */
   void RefreshViewer() const;
   /** Permite seleccionar un nombre archivo para salvar el vector */
   bool GetOutputFileName(std::string &Filename);
   /** Copia el datasource en memoria a disco */
   bool SaveMemoryDatasourceToDisk(const std::string &Filename);
   /** Analiza si se puede eliminar el FD vectorial y sus archivos asociados */
   bool CanDeleteVectorDatasource(const std::string &Filename);
   /** Elimina la FD vectorial y sus archivos asociados */
   bool DeleteVectorDatasource(const std::string &Filename);
   /** Verifica, al momento de cambiar de herramienta, si se debe finalizar la edición vectorial*/
   bool EndEditFeatureIsNecessary();
   /** Comando asociado a la tarea de edicion vectorial */
   Command* pTaskCommand_;
   /** TaskConfig que permite configurar el Layout */
   suri::TaskConfig *pTaskConfig_;
   /** Herramienta de snapping que permite configurar las propiedades */
   suri::SnapTool *pSnapTool_;
};

} /* namespace suri */
#endif /* VECTOREDITIONTOOL_H_ */
