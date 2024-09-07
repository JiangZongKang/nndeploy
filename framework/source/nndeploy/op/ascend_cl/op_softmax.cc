#include "nndeploy/op/op_softmax.h"

#include "aclnnop/aclnn_softmax.h"
#include "nndeploy/op/ascend_cl/acl_op_convert.h"
#include "nndeploy/op/ascend_cl/acl_op_include.h"
#include "nndeploy/op/ascend_cl/acl_op_util.h"
#include "nndeploy/op/op.h"

namespace nndeploy {
namespace op {

class AscendCLOpSoftmax : public OpSoftmax {
 public:
  AscendCLOpSoftmax() {}
  virtual ~AscendCLOpSoftmax() {}

  virtual base::Status init() {
    // 参数
    SoftmaxParam* param = (SoftmaxParam*)op_desc_.op_param_.get();
    dim_ = (int64_t)param->axis_;

    // 流
    device::Device* device = device::getDevice(device_type_);
    inner_stream_ = (aclrtStream)device->getCommandQueue();

    return base::kStatusCodeOk;
  }
  virtual base::Status deinit() { return base::kStatusCodeOk; }
  virtual base::Status preRun() {
    // 输入输出
    inner_input_ = AclOpConvert::convertFromTensor(inputs_[0]);
    inner_output_ = AclOpConvert::convertFromTensor(outputs_[0]);

    // 创建算子
    aclnnStatus aclnn_status = aclnnSoftmaxGetWorkspaceSize(
        inner_input_, dim_, inner_output_, &workspace_size_, &executor_);
    NNDEPLOY_RETURN_VALUE_ON_NEQ(aclnn_status, ACL_SUCCESS,
                                 base::kStatusCodeErrorOpAscendCL,
                                 "aclnnSoftmaxGetWorkspaceSize failed.");
    return base::kStatusCodeOk;
  }
  virtual base::Status run() {
    // 输入输出
    aclnnStatus aclnn_status =
        aclnnSoftmax(workspace_, workspace_size_, executor_, inner_stream_);
    NNDEPLOY_LOGE("dim_ is %d.\n", static_cast<int32_t>(dim_));
    NNDEPLOY_LOGE("Execute Operator failed. error code is %d.\n",
                  static_cast<int32_t>(aclnn_status));
    NNDEPLOY_RETURN_VALUE_ON_NEQ(aclnn_status, ACL_SUCCESS,
                                 base::kStatusCodeErrorOpAscendCL,
                                 "aclnnSoftmax failed.");

    return base::kStatusCodeOk;
  }
  virtual base::Status postRun() {
    aclDestroyTensor(inner_input_);
    aclDestroyTensor(inner_output_);
    // aclDestroyExecutor(executor_);
    return base::kStatusCodeOk;
  }

 private:
  std::string inner_op_type_ = "SoftmaxV2";

  aclTensor* inner_input_ = nullptr;
  int64_t dim_ = 0;
  aclTensor* inner_output_ = nullptr;
  aclOpExecutor* executor_;

  aclrtStream inner_stream_;
  aclopAttr* attr_{nullptr};
};

REGISTER_OP_IMPLEMENTION(base::DeviceTypeCode::kDeviceTypeCodeAscendCL,
                         kOpTypeSoftmax, AscendCLOpSoftmax)

}  // namespace op
}  // namespace nndeploy
