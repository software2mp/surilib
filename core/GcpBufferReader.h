/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
