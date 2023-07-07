//===----------------------------------------------------------------------===//
//
// Copyright (C) 2022 Sophgo Technologies Inc.  All rights reserved.
//
// SOPHON-STREAM is licensed under the 2-Clause BSD License except for the
// third-party components.
//
//===----------------------------------------------------------------------===//

#ifndef SOPHON_STREAM_ELEMENT_TEMPLATE_H_
#define SOPHON_STREAM_ELEMENT_TEMPLATE_H_

#include <fstream>
#include <memory>
#include <mutex>

#include "common/profiler.h"
#include "element.h"
#include "template_context.h"
#include "template_inference.h"
#include "template_post_process.h"
#include "template_pre_process.h"

namespace sophon_stream {
namespace element {
namespace template {

  class Template : public ::sophon_stream::framework::Element {
   public:
    Template();
    ~Template() override;

    /**
     * @brief
     * 解析configure，初始化派生element的特有属性；调用initContext初始化算法相关参数
     * @param json json格式的配置文件
     * @return common::ErrorCode
     * 成功返回common::ErrorCode::SUCCESS，失败返回common::ErrorCode::PARSE_CONFIGURE_FAIL
     */
    common::ErrorCode initInternal(const std::string& json) override;
    void uninitInternal() override;

    /**
     * @brief
     * element的功能在这里实现。例如，算法模块需要实现组batch、调用算法、发送数据等功能
     * @param dataPipeId pop数据时对应的dataPipeId
     * @return common::ErrorCode 成功返回common::ErrorCode::SUCCESS
     */
    common::ErrorCode doWork(int dataPipeId) override;

    /**
     * @brief 从json文件读取的配置项
     */
    static constexpr const char* CONFIG_INTERNAL_STAGE_NAME_FIELD = "stage";
    static constexpr const char* CONFIG_INTERNAL_MODEL_PATH_FIELD =
        "model_path";
    static constexpr const char* CONFIG_INTERNAL_THRESHOLD_CONF_FIELD =
        "threshold_conf";
    static constexpr const char* CONFIG_INTERNAL_THRESHOLD_NMS_FIELD =
        "threshold_nms";
    static constexpr const char* CONFIG_INTERNAL_THRESHOLD_BGR2RGB_FIELD =
        "bgr2rgb";
    static constexpr const char* CONFIG_INTERNAL_THRESHOLD_MEAN_FIELD = "mean";
    static constexpr const char* CONFIG_INTERNAL_THRESHOLD_STD_FIELD = "std";
    static constexpr const char* CONFIG_INTERNAL_CLASS_NAMES_FILE_FIELD =
        "class_names_file";
    static constexpr const char* CONFIG_INTERNAL_ROI_FILED = "roi";
    static constexpr const char* CONFIG_INTERNAL_LEFT_FILED = "left";
    static constexpr const char* CONFIG_INTERNAL_TOP_FILED = "top";
    static constexpr const char* CONFIG_INTERNAL_WIDTH_FILED = "width";
    static constexpr const char* CONFIG_INTERNAL_HEIGHT_FILED = "height";

   private:
    std::shared_ptr<TemplateContext> mContext;          // context对象
    std::shared_ptr<TemplatePreProcess> mPreProcess;    // 预处理对象
    std::shared_ptr<TemplateInference> mInference;      // 推理对象
    std::shared_ptr<TemplatePostProcess> mPostProcess;  // 后处理对象

    bool use_pre = false;
    bool use_infer = false;
    bool use_post = false;
    int mBatch;

    std::string mFpsProfilerName;
    ::sophon_stream::common::FpsProfiler mFpsProfiler;

    common::ErrorCode initContext(const std::string& json);
    void process(common::ObjectMetadatas& objectMetadatas);
  };

}  // namespace template
}  // namespace element
}  // namespace sophon_stream

#endif  // SOPHON_STREAM_ELEMENT_TEMPLATE_H_