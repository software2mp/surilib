/** \file ClusterClassificationPart.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef CLUSTERCLASSIFICATIONPART_H_
#define CLUSTERCLASSIFICATIONPART_H_

// Includes standard
// Includes Suri
#include "ClusterClassificationAlgorithm.h"
#include "ParametersSelectionPart.h"
#include "suri/VectorStyle.h"
#include "SupervisedParameterSelectionPart.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Clase intermedia con codigo comun a todos los parts que configuran
 * algoritmos de clasificacion que usan clusters
 */
class ClusterClassificationPart : public SupervisedParameterSelectionPart {
public:
   /** Ctor */
   ClusterClassificationPart(
         ClusterClassificationAlgorithm* pClusterClassificationAlgorithm);
   /** Dtor */
   virtual ~ClusterClassificationPart();

   // --- Metodos de Part ---
   /** Analiza si el part tiene datos validos. */
   virtual bool HasValidData();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();

   // -- Metodos de ParametersSelectionPart ---
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Retorna las propiedades de los pixels sin clase. */
   virtual bool GetNonClassifiedPixelInfo(ClassInformation &Properties);

protected:
   /**
    * Por falta de tiempo la interfaz de modifica usando herencia. 
    * Refactor pendiente: Componer un Widget que encapsule la interfaz
    * con la que se ingresa el threshold y color.
    */
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color) = 0;
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue() = 0;
   /** Cambia el valor del slider */
   virtual void SetThresholdCtrlValue(double Threshold) = 0;
   /** Retorna el valor del slider */
   virtual double GetThresholdCtrlValue() = 0;

   VectorStyle::Color color_; /*! variable con color de pixel no clasificado */
   ClusterClassificationAlgorithm* pAlgorithm_; /*! algoritmo a configurar */

   static const int NON_CLASSIFIED_PIXEL_VALUE;  /*! Valor de los pixels */
                                                 /* no clasificados */
};
}  // namespace suri


#endif /* CLUSTERCLASSIFICATIONPART_H_ */
