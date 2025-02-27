// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#if !defined(IN_OV_COMPONENT) && !defined(NGRAPH_LEGACY_HEADER_INCLUDED)
#    define NGRAPH_LEGACY_HEADER_INCLUDED
#    ifdef _MSC_VER
#        pragma message( \
            "The nGraph API is deprecated and will be removed in the 2024.0 release. For instructions on transitioning to the new API, please refer to https://docs.openvino.ai/latest/openvino_2_0_transition_guide.html")
#    else
#        warning("The nGraph API is deprecated and will be removed in the 2024.0 release. For instructions on transitioning to the new API, please refer to https://docs.openvino.ai/latest/openvino_2_0_transition_guide.html")
#    endif
#endif

#include "ngraph/op/op.hpp"
#include "openvino/op/detection_output.hpp"

namespace ngraph {
namespace op {
using DetectionOutputAttrs = ov::op::v0::DetectionOutput::Attributes;

namespace v0 {
using ov::op::v0::DetectionOutput;
}  // namespace v0
using v0::DetectionOutput;

namespace v8 {
using ov::op::v8::DetectionOutput;
}  // namespace v8
}  // namespace op
}  // namespace ngraph
