#ifndef _NNDEPLOY_BASIC_WARPAFFINE_PREPROCESS_H_
#define _NNDEPLOY_BASIC_WARPAFFINE_PREPROCESS_H_

#include "nndeploy/base/common.h"
#include "nndeploy/base/glic_stl_include.h"
#include "nndeploy/base/log.h"
#include "nndeploy/base/macro.h"
#include "nndeploy/base/object.h"
#include "nndeploy/base/opencv_include.h"
#include "nndeploy/base/status.h"
#include "nndeploy/base/string.h"
#include "nndeploy/base/value.h"
#include "nndeploy/basic/opencv_convert.h"
#include "nndeploy/basic/params.h"
#include "nndeploy/dag/edge.h"
#include "nndeploy/dag/node.h"
#include "nndeploy/device/buffer.h"
#include "nndeploy/device/device.h"
#include "nndeploy/device/memory_pool.h"
#include "nndeploy/device/tensor.h"

namespace nndeploy {
namespace basic {

class NNDEPLOY_CC_API WarpaffinePreprocess : public dag::Node {
 public:
  WarpaffinePreprocess(const std::string &name, dag::Edge *input,
                       dag::Edge *output)
      : Node(name, input, output) {
    param_ = std::make_shared<WarpAffineParam>();
  }
  virtual ~WarpaffinePreprocess() {}

  virtual base::Status run();
};

}  // namespace basic
}  // namespace nndeploy

#endif /* _NNDEPLOY_BASIC_WARPAFFINE_PREPROCESS_H_ */