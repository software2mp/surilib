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

// Includes Suri
#include "SnapInterface.h"

namespace suri {

/** Destructor */
SnapInterface::~SnapInterface() {
}

/**
 * Configura el mundo que se utilizara
 * @param[in] pWorld puntero al mundo
 */
void SnapInterface::SetWorld(World* pWorld) {
   pWorld_ = pWorld;
}

/**
 * Configura el vector con las geometrias correspondientes a los segmentos
 * @param[in] SegmentGeometries vector con las geometrias correspondientes a los segmentos
 */
void SnapInterface::SetSegmentGeometries(std::vector<OGRGeometry*> SegmentGeometries) {
   segmentgeometries_.clear();
   segmentgeometries_ = SegmentGeometries;
}

/**
 * Configura el vector con las geometrias correspondientes a los vertices
 * @param[in] VertexGeometries vector con las geometrias correspondientes a los vertices
 */
void SnapInterface::SetVertexGeometries(std::vector<OGRGeometry*> VertexGeometries) {
   vertexgeometries_.clear();
   vertexgeometries_ = VertexGeometries;
}

/**
 * Configura el vector con las geometrias correspondientes a los buffers de los vertices
 * @param[in] VertexBuffers vector con las geoms correspondientes a los buffers de los vertices
 */
void SnapInterface::SetVertexBufferGeometries(std::vector<OGRGeometry*> VertexBuffers) {
   vertexbuffers_.clear();
   vertexbuffers_ = VertexBuffers;
}

/**
 * Configura el vector con las geometrias correspondientes a los buffers de los segmentos
 * @param[in] SegmentBuffers vector con las geoms correspondientes a los buffers de los segmentos
 */
void SnapInterface::SetSegmentBufferGeometries(std::vector<OGRGeometry*> SegmentBuffers) {
   segmentbuffers_.clear();
   segmentbuffers_ = SegmentBuffers;
}

} /** namespace suri */
