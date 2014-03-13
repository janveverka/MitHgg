#define G__DICTIONARY
#include "Rtypes.h"
#include "Math/SMatrix.h"

namespace MitHgg_BackgroundModel {
  struct dictionary {
    ROOT::Math::MatRepStd<double,3,3> rmmrsd33;
    ROOT::Math::SMatrix<double,3,3,ROOT::Math::MatRepStd<double,3,3> > rmsmd33;
  }; // dictionary
} // namespace
