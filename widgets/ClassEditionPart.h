/*! \file ClassEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSEDITIONPART_H_
#define CLASSEDITIONPART_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "RasterDatasource.h"
#include "wxGenericTableBase.h"
#include "ClassInformation.h"
#include "ColorTable.h"
#include "suri/LayerInterface.h"
#include "TablePart.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Part que permite editar las propiedades de las clases de un raster
 * clasificada
 */
class ClassEditionPart : public suri::Part {
public:
   /** Constructor */
   ClassEditionPart(RasterDatasource* pRasterDatasource, LayerInterface* pLayer);
   /** Destructor */
   virtual ~ClassEditionPart();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part */
   virtual bool RollbackChanges();
   /** Inicializa los valores iniciales de la grilla del part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Crea la Tool Window */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   /** Metodo auxiliar para configurar la tabla de colores de la fuente de datos **/
   void ConfigureDatasourceColorTable(const ColorTable& ColorTable);
   /** Nombre de la primera columna */
   static const std::string FirstColumn;
   /** Nombre de la segunda columna */
   static const std::string SecondColumn;
   /** Puntero a la fuente de datos raster */
   RasterDatasource* pRasterDatasource_;
   /** Puntero a la capa activa a actualizar la tabla de colores **/
   LayerInterface* pLayer_;
   /** Tabla que se usa para asignar las bandas */
   Table* pVariablesTable_;
   /** Part que contiene la tabla **/
   TablePart* pTablePart_;
   /** Asociacion fila - indice */
   std::vector<ClassInformation::IndexType> indexes_;
};

} /** namespace suri */

#endif /* CLASSEDITIONPART_H_ */
