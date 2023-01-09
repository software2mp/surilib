/*! \file TableTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TABLETOOL_H_
#define TABLETOOL_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class TablePart;
class Table;

/**
 * Tool que implementa los comandos que interactuan sobre un TablePart
 */
class TableTool : public suri::GenericTool {
public:
   /** Constructor */
   explicit TableTool(DataViewManager* pDataViewManager, TablePart* pTablePart);
   /** Destructor */
   virtual ~TableTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);
   /** Setea el TablePart */
   void SetTablePart(TablePart* pTablePart);
   /** Getea el TablePart */
   TablePart* GetTablePart();
   /** Setea el puntero a funcion de cerrar vector */
   void SetCloseVectorFunction(void (*pCloseVectorFunction)() = NULL);
   /** Obtener indice de seleccion de Grid */
   std::vector<int> GetSelectedColumns();
   /** Obtener la tabla */
   Table* GetTable();

protected:
   /** Fabrica el comando si es uno de los predefinidos */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruye un comando previamente creado con CreateCommand */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creador del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);
   /** Inicializa el estado de Enable y Active */
   virtual void InitializeState(const std::string &CommandName);

private:
   TablePart* pTablePart_; /*! Puntero a Table Part */
   void (*pCloseVectorFunction_)(); /*! Puntero a funcion de cerrar vector */
   /** Accion de "Ejecutar" de AddColumn */
   void ExecuteAddColumn();
   /** Accion de "Ejecutar" de DeleteColumn */
   void ExecuteDeleteColumn();
   /** Accion de "Ejecutar" de AddRow */
   void ExecuteAddRow();
   /** Accion de "Ejecutar" de DeleteRow */
   void ExecuteDeleteRow();
   /** Accion de "Ejecutar" de Commit */
   void ExecuteCommit();
   /** Accion de "Ejecutar" de Rollback */
   void ExecuteRollback();
   /** Accion de "Ejecutar" de CloseVector */
   void ExecuteCloseVector();
   /** Accion de limpiar la seleccion de la tabla */
   void ExecuteClearSelection();
   /** Accion de seleccion de todos los registros de la tabla */
   void ExecuteSelectAll();
   /** Accion de invertir la seleccion de la tabla */
   void ExecuteInvertSelection();
   /** Accion de armado de consultas */
   void ExecuteQueryBuilder();
   /** Obtener indice de seleccion de Grid */
   std::vector<int> GetSelectedRows();
   /** Verifica si la tabla es de solo lectura */
   bool IsReadOnly();
};

} /** namespace suri */

#endif /* TABLETOOL_H_ */
