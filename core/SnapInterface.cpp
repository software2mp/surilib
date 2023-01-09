/*! \file SnapInterface.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
