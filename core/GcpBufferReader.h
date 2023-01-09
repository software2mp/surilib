/*! \file GcpBufferReader.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GCP_BUFFER_READER_H
#define GCP_BUFFER_READER_H

// Includes Estandar
#include <vector>
// Includes Suri
#include "ImageBuffer.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class Image;

/**
 * Contiene un Image con los candidatos a GCP calculados con el algoritmo
 * de Javier Urien. Lee el contenido de la imagen por bloques y maneja un buffer
 * que utiliza para extraer los candidatos a GCP.
 */
class GcpBufferReader {
public:
   /**
    * Constructor
    */
   explicit GcpBufferReader(Image* pImage,
                            const std::vector<int>& gcpBand);
   /**
    * Recorre el archivo de candidatos en busca de un candidato a GCP. Si encuentra
    * un punto, indica sus coordenadas por medio de los parametros. Si no hay
    * más candidatos, renueva el buffer y sigue buscando. Si el archivo se
    * termina y no encuentra mas puntos, devuelve false.
    * @param candidateX Coordenada X del candidato encontrado
    * @param candidateY Coordenada Y del candidato encontrado
    */
   bool GetNextCandidate(int &CandidateX, int& CandidateY);
   /**
    * Elimina los candidatos ubicados en la coordenada indicada.
    * Además, elimina los vecinos ubicados en una ventana de proximidad
    * del GCP generado.
    */
   void RemoveCandidates (unsigned int GcpX, unsigned int GcpY);
   /**
    * Obtiene radio de proximidad
    */
   unsigned int GetProximityWindowRadius() const;
   /**
    * Asigna radio de proximidad
    */
   void SetProximityWindowRadius(unsigned int ProxRadius);
   /**
    * Radio default de la ventana de proximidad
    */
   static const unsigned int kProximityRadius;
private:
   /**
    * Imagen a leer
    */
   ImageBuffer buffer_;
   /**
    * Ancho de la ventana de proximidad entre GCPs
    * Determina el radio de accion de RemoveCandidates
    */
   unsigned int proximityWindowRadius_;
   /**
    * Offset relativo al buffer
    */
   unsigned int offsetX_, offsetY_;
};

}

#endif // GCP_BUFFER_READER_H
