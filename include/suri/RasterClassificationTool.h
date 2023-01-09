/*! \file RasterClassificationTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERCLASSIFICATIONTOOL_H_
#define RASTERCLASSIFICATIONTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/VectorEditionTool.h"
#include "RasterElement.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Ejecuta los comandos correspondientes a la herramienta de clasificacion de rasters
 */
class RasterClassificationTool : public VectorEditionTool {
public:
   /** Constructor */
   explicit RasterClassificationTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~RasterClassificationTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   /** Despliega el widget para clasificacion no supervisada */
   void ExecuteNonSupervisedClassification() const;
   /** Despliega el widget para clasificacion no supervisada */
   void ExecuteSupervisedClassification() const;
   /** Despliega el widget para fusion de clases */
   void ExecuteClassFusion() const;
   /** Despliega el widget para matriz de confusion */
   void ExecuteConfusionMatrix() const;
   /** Despliega el widget para filtros de clasificacion */
   void ExecuteClassificationFilters() const;
   /** Metodo que ejecuta el proceso de clasificacion por umbrales */
   void ExecuteThresholdClassification() const;
   /** Ejecuta el proceso de clasificacion */
   void ExecuteClassificationProcess(const std::string& ClassificationType,
                                     const std::string& ClassificationName,
                                     bool RequiredContainsPolygons = false,
                                     bool ShowSpectralPart = true) const;
   /** Crea un resinto e inicia su edicion */
   void ExecuteCreateEnclosure();
   /** Ejecuta el proceso para mapear angulos espectrales */
   void ExecuteSpectralAngleMapper() const;
   /** Devuelve el elemento de la fuente de datos asociado a la capa seleccionada */
   RasterElement* GetRasterElement() const;
};

} /** namespace suri */

#endif /* RASTERCLASSIFICATIONTOOL_H_ */
