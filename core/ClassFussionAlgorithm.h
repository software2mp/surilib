/*! \file ClassFussionAlgorithm.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSFUSSIONALGORITHM_H_
#define CLASSFUSSIONALGORITHM_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>
#include <utility>

// Includes Suri
#include "ClassificationAlgorithmInterface.h"

// Includes Wx

// Defines

// forwards
class wxXmlNode;

namespace suri {

class ClassFussionAlgorithm : public suri::ClassificationAlgorithmInterface {
   ClassFussionAlgorithm(const ClassFussionAlgorithm&);

public:
   ClassFussionAlgorithm();
   /** dtor */
   virtual ~ClassFussionAlgorithm();
   /** Definicion de mapa de asignacion de clases */
   /** key = indice clase a fusionar , value = indice destino **/
   typedef long IndexValue;
   typedef std::map<IndexValue, IndexValue> FussionClasesTables;
   /** key = indice clase a fusionar , value = indice destino **/
   typedef std::pair<IndexValue, IndexValue> FussionClasesTablesPair;
   void SetFussionTable(const FussionClasesTables &FussionTable);
   /** Actualiza los datos internos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode);
   /** Genera nodo con los datos internos del algoritmo. */
   virtual wxXmlNode* GetNode();
   /** Clasifica los datos de entrada */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                         const std::string &DataType);
   /** Verifica que el algoritmo este bien configurado */
   virtual bool Check() const;
   /** */
   int GetClassCount() const;
   /** Indice para la noclase **/
   static const int NoClusterIndex;

private:
   /** Metodo para la iniciacion de la instancia **/
   virtual ClassFussionAlgorithm* Initialize(const std::string&);
   /** Valida el contenido de los nodos cluster y nocluster **/
   bool IsClusterNodeValid(const wxXmlNode* pNode, bool IsNoCluster = false);
   /** Metodo auxiliar para la validacion del xml*/
   bool IsXmlValid(const wxXmlNode* pNode);
   /** Tabla de asignacion de clases **/
   FussionClasesTables fussionTable_;
   /** Convierte el map seteado al xml **/
   bool FussionMapToXml(wxXmlNode* pParentNode);
   /** Convierte el xml a el mapea de asignacion**/
   bool XmlToMapFussion(const wxXmlNode* pNode);
   /* A partir del nodo cluster crea la informacion para insertar al mapa de fusion */
   bool InsertFussionMapFromXml(const wxXmlNode* pNode);
};

} /** namespace suri */

#endif /* CLASSFUSSIONALGORITHM_H_ */
