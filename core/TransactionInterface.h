/*! \file TransactionInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TRANSACTIONINTERFACE_H_
#define TRANSACTIONINTERFACE_H_

// Includes Estandar

// Includes Suri

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Modela una transaccion con las operaciones de Commit y Rollback
 */
class TransactionInterface {
public:
   /** Destructor */
   virtual ~TransactionInterface() {}
   /** Aplica los cambios y efectua la transaccion */
   virtual bool Commit()=0;
   /** Descarta los cambios (posteriores al ultimo commit si es que lo hubo) y no hay transac. */
   virtual bool Rollback()=0;
};

} /** namespace suri */

#endif /* TRANSACTIONINTERFACE_H_ */
