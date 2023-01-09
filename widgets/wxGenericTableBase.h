/** \file wxGenericTableBase.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXGENERICTABLEBASE_H_
#define WXGENERICTABLEBASE_H_

// Includes standard
#include <map>
#include <set>
#include <vector>
#include <string>

// Includes suri
#include "suri/Table.h"
#include "suri/ObserverInterface.h"
#include "suri/Option.h"
// Includes wx
#include "wx/wx.h"
#include "wx/grid.h"

// Forwards

namespace suri {

/**
 *  Clase que representa el modelo de un control grafico de grilla (wxGrid).
 *  A su vez es la vista del modelo de tablas de la aplicacion.
 */
class wxGenericTableBase : public ObserverInterface, public wxGridTableBase {
public:
   /** Constructor */
   explicit wxGenericTableBase(Table* pTable);
   /** Destructor */
   virtual ~wxGenericTableBase();
   /** Retrona el valor la celda(en formato string) en posicion pedida \deprecated */
   virtual wxString GetValue(int Row, int Column);
   /** Actualiza el valor de un campo con el valor que se pasa por parametro \deprecated */
   virtual void SetValue(int Row, int Column, const wxString& Value);
   /** Informa el numero de filas que tiene la tabla */
   virtual int GetNumberRows();
   /** Informa el numero de columnas que tiene la tabla */
   virtual int GetNumberCols();
   /** Informa si la celda esta vacia */
   virtual bool IsEmptyCell(int Row, int Column);
   /** Retorna el nombre de una columna */
   virtual wxString GetColLabelValue(int Column);
   /** Elimina todas las filas y su contenido de la tabla */
   virtual void Clear();
   /** Agrega filas a la tabla */
   virtual bool AppendRows(size_t NumRows = 1);
   /** Elimina filas de la tabla */
   virtual bool DeleteRows(size_t Pos = 0, size_t NumRows = 1);
   /** Elimina columnas de la tabla */
   virtual bool DeleteCols(size_t Pos = 0, size_t NumCols = 1);
   /** Determina si hubo cambios en la tabla */
   bool HasChanged() const;
   /** Resetea el flag de cambios de la tabla */
   void ResetChangesFlag();
   /** Informa si la tabla tiene alguna celda en edicion */
   bool IsEditing();
   /** Fuerza la finalizacion de la edicion */
   void EndEdition();
   /** Obtiene la tabla del modelo */
   Table* GetTable();
   /** Actualiza el Provider de Editores de Atributos */
   void UpdateProvider();
   /** Actualiza wxGrid para que coincida con los datos en TableInterface */
   void UpdateGrid();
   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);
   /** Oculta la columna correspondiente al indice que se pasa por parametro**/
   bool HiddeColumn(int ColumnIndex);
   /** Muestra la columna correspondiente al indice que se pasa por parametro**/
   bool ShowHiddenColumn(int ColumnIndex);
   /** Oculta la columna correspondiente al indice que se pasa por parametro**/
   bool IsColumnHidden(int ColumnId) const;
   /** Se agrega una nueva columna y se debe actualizar el control de las mismas **/
   void ColumnAdded(int ColumnId);
   /** Se notifica que fue eliminada la columna con id pasado por parametro **/
   void ColumnDeleted(int ColumnId, const std::string& ColumnName);
   /** Configura una columna como readonly */
   void SetReadOnlyColumnView(const std::string& ColumnName, bool ReadOnly = true);
   /** Consulta si una columna es readonly**/
   bool IsReadOnlyColumn(const std::string& ColumnName) const;
   /** Obtiene el indice de columna teniendo en cuenta las ocultas**/
   int GetFixedColumnIndex(int Column) const;

private:
   /** Actualiza el tamano de la grilla en pantalla */
   bool UpdateGridSize();
   /** Actualiza las filas de la grilla en pantalla */
   void UpdateRows();
   /** Actualiza las columnas de la grilla en pantalla */
   void UpdateColumns();

   Table* pTable_;   /*! Tabla con los datos/columnas que se representaran en wxGrid */
   bool hasChangedFlag_;      /*! Flag de cambios en la tabla */
   static const std::string ReadError;    /*! Valor para el error de lectura */
   static const std::string Hidden;       /*! Valor para ocultos */
   /** vector para el manejo de ids de columnas junto con las ocultas **/
   std::vector<long> colIndexHolder_;
   /** set para nombres de columna de solo lectura **/
   std::set<std::string> readOnlyColumns_;
};

} /** namespace suri */

#endif /* WXGENERICTABLEBASE_H_ */
